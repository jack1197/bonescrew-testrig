/*
 * GPIOPort.h
 *
 *  Created on: Aug 5, 2019
 *      Author: Jack
 */

#ifndef WRAPPERS_GPIO_HPP_
#define WRAPPERS_GPIO_HPP_

typedef enum GPIOPort_e {
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTF
} GPIOPort;

class GPIO
{
    bool activeState;
    int pin;
    GPIOPort port;
    bool output;
    static int portToPeriph(GPIOPort port);
    static int portToGpioBase(GPIOPort port);
    static int pinToMask(int pin);
public:
    GPIO(GPIOPort port, int pin, bool output = false, bool activeHigh = true);
    void setActiveState(bool state);
    bool getActiveState();
};

#endif /* WRAPPERS_GPIO_HPP_ */
