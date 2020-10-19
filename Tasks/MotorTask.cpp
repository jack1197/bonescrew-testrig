/*
 * MotorTask.cpp
 *
 *  Created on: 9 Oct 2020
 *      Author: Jack
 */

#include <Tasks/MotorTask.hpp>
#include "driverlib/sysctl.h"


void MotorTask::Setup()
{
}

void MotorTask::TaskMethod()
{

    TickType_t xLastWakeTime = xTaskGetTickCount();

    bool active = false;
    int tickIndex = 0;
    MotionSegment activeMotion;


    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

        //calculate required steps this for this ms

        int pulses = 0;

        if(!active)
        {
            tickIndex = 0;
            active = motionQueue->Dequeue(&activeMotion);
        }

        if(active)
        {
            pulses = activeMotion.pulsesForTick(tickIndex);
            status->setMotorSpeed(activeMotion.tickPPS(tickIndex));
            tickIndex++;
            if(tickIndex == activeMotion.getTickLength())
            {
                active = false;
            }
        }

        //check if motor enabled
        bool enabled = settings->getEnableMotor();
        motor_ena->setActiveState(enabled);
        if(!enabled)
        {
            pulses = 0;
            active = false;
            while(motionQueue->Dequeue(&activeMotion));//dequeue until empty
        }

        //send pulses
        if(pulses != 0)
        {
            motor_dir->setActiveState(pulses > 0);
            int absPulses = abs(pulses);
            for(int i = 0; i < absPulses ; i++)
            {
                SysCtlDelay(SysCtlClockGet() / 3 / 400000); //2.5µs delay
                motor_pul->setActiveState(1);
                SysCtlDelay(SysCtlClockGet() / 3 / 400000); //2.5µs delay
                motor_pul->setActiveState(0);
            }
        }

        vTaskDelayUntil(&xLastWakeTime,  10);
    }
}

MotorTask::MotorTask(GPIO* motor_pul, GPIO* motor_dir, GPIO* motor_ena, GPIO* motor_alm, GPIO* motor_pend,Queue<MotionSegment>* motionQueue, Status* status, Settings* settings)
:motor_pul(motor_pul),motor_dir(motor_dir),motor_ena(motor_ena),motor_alm(motor_alm),motor_pend(motor_pend),motionQueue(motionQueue), status(status), settings(settings)
{
    priority = 12;
    stackSize = 128;
    taskName = "MotorTask";
}
