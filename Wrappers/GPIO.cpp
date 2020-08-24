/*
 * GPIOPort.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: Jack
 */

#include "Wrappers/GPIO.hpp"

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"

int GPIO::portToPeriph(GPIOPort port)
{
    switch(port)
    {
    case GPIO_PORTA:
        return SYSCTL_PERIPH_GPIOA;
    case GPIO_PORTB:
        return SYSCTL_PERIPH_GPIOB;
    case GPIO_PORTC:
        return SYSCTL_PERIPH_GPIOC;
    case GPIO_PORTD:
        return SYSCTL_PERIPH_GPIOD;
    case GPIO_PORTE:
        return SYSCTL_PERIPH_GPIOE;
    case GPIO_PORTF:
        return SYSCTL_PERIPH_GPIOF;
    default:
        return 0;
    }
}

int GPIO::portToGpioBase(GPIOPort port)
{
    switch(port)
    {
    case GPIO_PORTA:
        return GPIO_PORTA_BASE;
    case GPIO_PORTB:
        return GPIO_PORTB_BASE;
    case GPIO_PORTC:
        return GPIO_PORTC_BASE;
    case GPIO_PORTD:
        return GPIO_PORTD_BASE;
    case GPIO_PORTE:
        return GPIO_PORTE_BASE;
    case GPIO_PORTF:
        return GPIO_PORTF_BASE;
    default:
        return 0;
    }
}

int GPIO::pinToMask(int pin)
{
    switch(pin)
    {
    case 0:
        return GPIO_PIN_0;
    case 1:
        return GPIO_PIN_1;
    case 2:
        return GPIO_PIN_2;
    case 3:
        return GPIO_PIN_3;
    case 4:
        return GPIO_PIN_4;
    case 5:
        return GPIO_PIN_5;
    case 6:
        return GPIO_PIN_6;
    case 7:
        return GPIO_PIN_7;
    default:
        return 0;
    }
}

GPIO::GPIO(GPIOPort port, int pin, bool output, bool activeHigh)
    : port(port),
      pin(pin),
      output(output),
      activeState(activeHigh)
{

    SysCtlPeripheralEnable(portToPeriph(port));

    if(port == GPIO_PORTF && pin == 0)
    {
        GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
        GPIO_PORTF_CR_R |= GPIO_PIN_0; //PF0 unlocked
        GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
    }
    if(port == GPIO_PORTD && pin == 7)
    {
        GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
        GPIO_PORTD_CR_R |= GPIO_PIN_7; //PD7 unlocked
        GPIO_PORTD_LOCK_R = GPIO_LOCK_M;
    }

    if(output)
    {
        GPIOPinTypeGPIOOutput(portToGpioBase(port), pinToMask(pin));
        GPIOPadConfigSet(portToGpioBase(port), pinToMask(pin), GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
        GPIOPinWrite(portToGpioBase(port), pinToMask(pin), !activeState);//set inactive
    }
    else
    {
        GPIOPinTypeGPIOInput (portToGpioBase(port), pinToMask(pin));
        GPIOPadConfigSet (portToGpioBase(port), pinToMask(pin), GPIO_STRENGTH_2MA,
                          activeHigh ? GPIO_PIN_TYPE_STD_WPD : GPIO_PIN_TYPE_STD_WPU);
    }
}

void GPIO::setActiveState(bool state)
{
    bool level = (state ? activeState : !activeState);
    GPIOPinWrite(portToGpioBase(port), pinToMask(pin), level ? pinToMask(pin) : 0);
}

bool GPIO::getActiveState()
{
    bool state = GPIOPinRead(portToGpioBase(port), pinToMask(pin)) == pinToMask(pin);
    return state == activeState;
}
