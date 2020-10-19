/*
 * QEI.cpp
 *
 *  Created on: 7 Sep 2020
 *      Author: Jack
 */

#include <Wrappers/QEI.hpp>
#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"

uint32_t QEI::QeiTypeToPhAPin(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
        return GPIO_PIN_0;
    case QEI0B:
        return GPIO_PIN_6;
    case QEI1:
        return GPIO_PIN_5;
    }
}

uint32_t QEI::QeiTypeToPhBPin(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
        return GPIO_PIN_1;
    case QEI0B:
        return GPIO_PIN_7;
    case QEI1:
        return GPIO_PIN_6;
    }
}

uint32_t QEI::QeiTypeToGPIOPort(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
        return GPIO_PORTF_BASE;
    case QEI0B:
        return GPIO_PORTD_BASE;
    case QEI1:
        return GPIO_PORTC_BASE;
    }
}

uint32_t QEI::QeiTypeToGPIOPeriph(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
        return SYSCTL_PERIPH_GPIOF;
    case QEI0B:
        return SYSCTL_PERIPH_GPIOD;
    case QEI1:
        return SYSCTL_PERIPH_GPIOC;
    }
}

uint32_t QEI::QeiTypeToBase(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
    case QEI0B:
        return QEI0_BASE;
    case QEI1:
        return QEI1_BASE;
    }
}

uint32_t QEI::QeiTypeToPeriph(QeiType qeiType)
{
    switch(qeiType)
    {
    case QEI0A:
    case QEI0B:
        return SYSCTL_PERIPH_QEI0;
    case QEI1:
        return SYSCTL_PERIPH_QEI1;
    }
}

void QEI::init()
{
    if(!inited)
    {
        SysCtlPeripheralEnable(QeiTypeToPeriph(qeiType));
        while(!SysCtlPeripheralReady(QeiTypeToPeriph(qeiType)));
        QEIConfigure(QeiTypeToBase(qeiType), QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP, ui32MaxPosition);

        SysCtlPeripheralEnable(QeiTypeToGPIOPeriph(qeiType));

        if(QeiTypeToGPIOPort(qeiType) == GPIO_PORTF_BASE)
        {
            GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
            GPIO_PORTF_CR_R |= GPIO_PIN_0; //PF0 unlocked
            GPIO_PORTF_LOCK_R = GPIO_LOCK_M;
        }
        if(QeiTypeToGPIOPort(qeiType) == GPIO_PORTD_BASE)
        {
            GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
            GPIO_PORTD_CR_R |= GPIO_PIN_7; //PD7 unlocked
            GPIO_PORTD_LOCK_R = GPIO_LOCK_M;
        }

        GPIOPinTypeQEI(QeiTypeToGPIOPort(qeiType), QeiTypeToPhAPin(qeiType));
        GPIOPinTypeQEI(QeiTypeToGPIOPort(qeiType), QeiTypeToPhBPin(qeiType));


        QEIEnable(QeiTypeToBase(qeiType));
        inited = true;
    }
}

QEI::QEI(QeiType qeiType, uint32_t ui32MaxPosition)
:qeiType(qeiType), ui32MaxPosition(ui32MaxPosition)
{
}

uint32_t QEI::getPos(void)
{
    init();
    return QEIPositionGet(QeiTypeToBase(qeiType));
}

uint32_t QEI::getVel(void)
{
    init();
    return QEIVelocityGet(QeiTypeToBase(qeiType));
}

void QEI::setPos(uint32_t value)
{
    init();
    QEIPositionSet(QeiTypeToBase(qeiType), value);
}
