/*
 * HeartbeatTask.cpp
 *
 *  Created on: 31 Aug 2020
 *      Author: Jack
 */

#include <Tasks/HeartbeatTask.hpp>

HeartbeatTask::HeartbeatTask(GPIO *blinkLed)
    :blinkLed(blinkLed)
{
    priority = 3;
    stackSize = 128;
    taskName = "HeartbeatTask";
}

void HeartbeatTask::Setup()
{

}


void HeartbeatTask::TaskMethod()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();



    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

        blinkLed->setActiveState(0);
        vTaskDelayUntil(&xLastWakeTime, 500 / portTICK_RATE_MS );
        blinkLed->setActiveState(1);
        vTaskDelayUntil(&xLastWakeTime, 50 / portTICK_RATE_MS );
    }
}
