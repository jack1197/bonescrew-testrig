/*
 * DebouncedGPIO.h
 *
 *  Created on: 15/08/2019
 *      Author: jwi123
 */

#ifndef WRAPPERS_DEBOUNCEDGPIO_HPP_
#define WRAPPERS_DEBOUNCEDGPIO_HPP_

#include "GPIO.hpp"

class DebouncedGPIO
{
    GPIO* gpio;
    int lastChangeDowncounter;
    bool lastActiveState;
    int debouncePeriodMillis;
public:
    DebouncedGPIO(GPIO* gpio, int debouncePeriodMillis);
    bool getActiveState(int dtMillis);
};

#endif /* WRAPPERS_DEBOUNCEDGPIO_HPP_ */
