/*
 * MotionTask.cpp
 *
 *  Created on: 16 Oct 2020
 *      Author: Jack
 */

#include <Tasks/MotionTask.hpp>


MotionTask::MotionTask(Queue<MotionSegment>* motionQueue, Queue<ControlEvent>* eventQueue, Settings* settings)
:motionQueue(motionQueue), eventQueue(eventQueue), settings(settings)
{
    priority = 7;
    stackSize = 128;
    taskName = "MotionTask";
}

void MotionTask::Setup()
{

}


void MotionTask::TaskMethod()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    MotionSegment motion;



    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        //respond to events
        ControlEvent event = ControlEvent::NoEvent;
        bool eventFound = eventQueue->Dequeue(&event);

        if(!eventFound)
        {
            //pass
        }
        else if(event == ControlEvent::StartMotion)
        {
            //perform motion
            delayWhileActivating();
            if(currState == Idle || currState == MotionPause)
            {
                currState = MotionActive;
            }
        }
        else if(event == ControlEvent::PauseMotion)
        {
            if(currState == MotionActive)
            {
                currState = MotionPause;
            }
        }
        else if(event == ControlEvent::ResetMotion)
        {
            active = false;
            motionIn = true;
            totalDist = 0;
            currState = Idle;
        }
        else if(event == ControlEvent::JogCW)
        {
            delayWhileActivating();
            if(currState == Idle)
            {
//                motion.setWithAngularLength(-settings->getJogDist()/4, 0, -settings->getInsertSpeed());
//                motionQueue->Enqueue(motion, true);
                motion.setWithAngularLength(-settings->getJogDist(), -settings->getInsertSpeed(), -settings->getInsertSpeed());
                motionQueue->Enqueue(motion, true);
//                motion.setWithAngularLength(-settings->getJogDist()/4, -settings->getInsertSpeed(), 0);
//                motionQueue->Enqueue(motion, true);
            }
        }
        else if(event == ControlEvent::JogCCW)
        {
            delayWhileActivating();
            if(currState == Idle)
            {
//                motion.setWithAngularLength(settings->getJogDist()/4, 0, settings->getInsertSpeed());
//                motionQueue->Enqueue(motion, true);
                motion.setWithAngularLength(settings->getJogDist(), settings->getInsertSpeed(), settings->getInsertSpeed());
                motionQueue->Enqueue(motion, true);
//                motion.setWithAngularLength(settings->getJogDist()/4, settings->getInsertSpeed(), 0);
//                motionQueue->Enqueue(motion, true);
            }
        }

        if(settings->getEnableMotor() == false)
        {
            active = false;
            motionIn = true;
            totalDist = 0;
            currState = Idle;
        }

        int directionSign = motionIn ? -1 : 1;
        if(currState == MotionActive)
        {
            if(settings->getTrapezoidEnabled())
            {
                //calculate approx speeds
                int effective_time = settings->getRampTime() + settings->getHighDwellTime();
                int approx_speed = (settings->getDistPerTrapezoid()*1000)/effective_time;
                int approx_ramp_dist = (approx_speed*settings->getRampTime())/2000;
                int required_dwell_dist = settings->getDistPerTrapezoid() - approx_ramp_dist*2;

                if(!makeLimitedMotion(directionSign*approx_ramp_dist, 0, directionSign*approx_speed))
                if(!makeLimitedMotion(directionSign*required_dwell_dist, directionSign*approx_speed, directionSign*approx_speed))
                makeLimitedMotion(directionSign*approx_ramp_dist, directionSign*approx_speed, 0);

                motion.setStartSpeed(0);
                motion.setEndSpeed(0);
                motion.setTickLength(settings->getLowDwellTime());
                motionQueue->Enqueue(motion, true);
            }
            else
            {

                makeLimitedMotion(directionSign*settings->getDistPerTrapezoid(), directionSign*settings->getInsertSpeed(), directionSign*settings->getInsertSpeed());

            }

        }
        vTaskDelayUntil(&xLastWakeTime, 100);
    }
}

void MotionTask::delayWhileActivating()
{
    if(!active)
    {
        vTaskDelay(configTICK_RATE_HZ*2);
        active = true;
    }
}


bool MotionTask::makeLimitedMotion(int dist, int start, int end)
{
    MotionSegment motion;
    bool hitEnd = false;

    int targetDist = motionIn ? settings->getInsertDist() : -settings->getReverseDist();

    motion.setWithAngularLength(dist, start, end);

    int pulses = motion.cumulativePulsesForTick(motion.getTickLength()-1);
    if(abs(totalDist + pulses) == abs(targetDist)){
        //done motion - perfect length
        motionQueue->Enqueue(motion, true);
        onMotionEndstop();
        motion.setEndSpeed(0);
        motion.setStartSpeed(0);
        motion.setTickLength(settings->getFullInsertionPause());
        motionQueue->Enqueue(motion, true);
        hitEnd = true;
    } else if(abs(totalDist + pulses) > abs(targetDist))
    {
        //motion too long, adjust and then motion done
        motion.setWithAngularLength(targetDist - totalDist, start, end);
        motionQueue->Enqueue(motion, true);
        onMotionEndstop();
        motion.setEndSpeed(0);
        motion.setStartSpeed(0);
        motion.setTickLength(settings->getFullInsertionPause());
        motionQueue->Enqueue(motion, true);
        hitEnd = true;
    }
    else
    {
        totalDist += pulses;
        motionQueue->Enqueue(motion, true);
    }
    return hitEnd;
}

void MotionTask::onMotionEndstop()
{
    if(motionIn)
    {
        //switch direction#
        totalDist = 0;
        motionIn = false;
    }
    else
    {
        //finished
        motionIn = true;
        totalDist = 0;
        currState = Idle;
    }
}
