/*
 * MotionSegment.hpp
 *
 *  Created on: 12 Oct 2020
 *      Author: Jack
 */

#ifndef MODEL_MOTIONSEGMENT_HPP_
#define MODEL_MOTIONSEGMENT_HPP_

#include "FreeRTOS.h"

const static int PULSES_PER_REVOLUTION = 6400;
const static int TICKS_PER_SECOND = 1000;
const static int RTOSTICKS_PER_TICK = configTICK_RATE_HZ/TICKS_PER_SECOND; //use this to avoid inaccuracies from non-divisor tickratesl

//Units:
//Speed: pulses per second
//angular length: pulses

class MotionSegment
{
    int startCPS = 0;
    int endCPS = 0;
    int tickLength = 0;
    int pulsesCorrection = 0;
public:
    void setStartSpeed(int);
    void setEndSpeed(int);
    void setTickLength(int);
    void setWithAngularLength(int pulses, int startSpeed, int endSpeed);
    int getStartSpeed();
    int getEndSpeed();
    int getTickLength();
    int getPulsesCorrection();

    int pulsesForTick(int tickIndex);
    long long cumulativePulsesForTick(int tickIndex);
    int tickPPS(int tickIndex);
};

#endif /* MODEL_MOTIONSEGMENT_HPP_ */
