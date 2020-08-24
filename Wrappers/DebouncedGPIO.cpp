/*
 * DebouncedGPIO.cpp
 *
 *  Created on: 15/08/2019
 *      Author: jwi123
 */

#include "DebouncedGPIO.hpp"

DebouncedGPIO::DebouncedGPIO(GPIO* gpio, int debouncePeriodMillis)
    : gpio(gpio),
      debouncePeriodMillis(debouncePeriodMillis),
      lastChangeDowncounter(0)
{
    lastActiveState = gpio->getActiveState();
}

bool DebouncedGPIO::getActiveState(int dtMillis)
{
    if(lastChangeDowncounter > dtMillis)
    {
        lastChangeDowncounter -= dtMillis;
        return lastActiveState;
    }

    lastChangeDowncounter = 0;

    bool currentActiveState = gpio->getActiveState();

    if(currentActiveState != lastActiveState)
    {
        lastActiveState = currentActiveState;
        lastChangeDowncounter = debouncePeriodMillis;
    }

    return lastActiveState;
}
