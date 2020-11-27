/*
 * MotionSegment.cpp
 *
 *  Created on: 12 Oct 2020
 *      Author: Jack
 */

#include <Model/MotionSegment.hpp>

void MotionSegment::setStartSpeed(int value)
{
    startCPS = value;
}

void MotionSegment::setEndSpeed(int value)
{
    endCPS = value;
}

void MotionSegment::setTickLength(int value)
{
    tickLength = value;
    pulsesCorrection = 0;
}

void MotionSegment::setWithAngularLength(int pulses, int startSpeed, int endSpeed)
{
    tickLength = (pulses*2*TICKS_PER_SECOND)/(startSpeed + endSpeed);
    if(pulses != 0 && tickLength == 0)
    {
        tickLength = 1;
    }
    startCPS = startSpeed;
    endCPS = endSpeed;
    pulsesCorrection = 0;
    int pulsesActual = cumulativePulsesForTick(tickLength-1);
    pulsesCorrection = pulses - pulsesActual;
}

int MotionSegment::getStartSpeed()
{
    return startCPS;
}

int MotionSegment::getEndSpeed()
{
    return endCPS;
}

int MotionSegment::getTickLength()
{
    return tickLength;
}

int MotionSegment::getPulsesCorrection()
{
    return pulsesCorrection;
}


int MotionSegment::pulsesForTick(int tickIndex)
{
    if(tickIndex > 0)
    {
        return cumulativePulsesForTick(tickIndex)-cumulativePulsesForTick(tickIndex-1);
    }
    else if(tickIndex == 0)
    {
        return cumulativePulsesForTick(0);
    }
    else
    {
        return 0;
    }
}

long long MotionSegment::cumulativePulsesForTick(int tickIndex)
{
    return
            //constant speed term
            ((long long)startCPS*(tickIndex+1))/TICKS_PER_SECOND
            //speed ramp term
            + (
                    (
                            ((long long)(endCPS-startCPS)*tickIndex*tickIndex)
                            /(tickLength-1)/TICKS_PER_SECOND
                    )+1
              )/2
            //correction term
            + ((long long)pulsesCorrection*tickIndex)/(tickLength-1);
            ;
}


int MotionSegment::tickPPS(int tickIndex)
{
    return startCPS + endCPS*tickIndex/(tickLength-1);
}
