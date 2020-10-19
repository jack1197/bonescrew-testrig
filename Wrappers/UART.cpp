/*
 * UART.cpp
 *
 *  Created on: 11/08/2019
 *      Author: jwi123
 */

#include "UART.hpp"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

int UART::UartTypeToRxPin(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return GPIO_PIN_0;
    case UART1A:
        return GPIO_PIN_4;
    case UART1B:
        return GPIO_PIN_0;
    case UART2:
        return GPIO_PIN_6;
    case UART3:
        return GPIO_PIN_6;
    case UART4:
        return GPIO_PIN_4;
    case UART5:
        return GPIO_PIN_4;
    case UART6:
        return GPIO_PIN_4;
    case UART7:
        return GPIO_PIN_0;
    }
    return 0;
}
int UART::UartTypeToTxPin(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return GPIO_PIN_1;
    case UART1A:
        return GPIO_PIN_5;
    case UART1B:
        return GPIO_PIN_1;
    case UART2:
        return GPIO_PIN_7;
    case UART3:
        return GPIO_PIN_7;
    case UART4:
        return GPIO_PIN_5;
    case UART5:
        return GPIO_PIN_5;
    case UART6:
        return GPIO_PIN_5;
    case UART7:
        return GPIO_PIN_1;
    }
    return 0;
}

int UART::UartTypeToGPIOPort(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return GPIO_PORTA_BASE;
    case UART1A:
        return GPIO_PORTC_BASE;
    case UART1B:
        return GPIO_PORTB_BASE;
    case UART2:
        return GPIO_PORTD_BASE;
    case UART3:
        return GPIO_PORTC_BASE;
    case UART4:
        return GPIO_PORTC_BASE;
    case UART5:
        return GPIO_PORTE_BASE;
    case UART6:
        return GPIO_PORTD_BASE;
    case UART7:
        return GPIO_PORTE_BASE;
    }
    return 0;
}
int UART::UartTypeToGPIOPeriph(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return SYSCTL_PERIPH_GPIOA;
    case UART1A:
        return SYSCTL_PERIPH_GPIOC;
    case UART1B:
        return SYSCTL_PERIPH_GPIOB;
    case UART2:
        return SYSCTL_PERIPH_GPIOD;
    case UART3:
        return SYSCTL_PERIPH_GPIOC;
    case UART4:
        return SYSCTL_PERIPH_GPIOC;
    case UART5:
        return SYSCTL_PERIPH_GPIOE;
    case UART6:
        return SYSCTL_PERIPH_GPIOD;
    case UART7:
        return SYSCTL_PERIPH_GPIOE;
    }
    return 0;
}


int UART::UartTypeToBase(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return UART0_BASE;
    case UART1A:
    case UART1B:
        return UART1_BASE;
    case UART2:
        return UART2_BASE;
    case UART3:
        return UART3_BASE;
    case UART4:
        return UART4_BASE;
    case UART5:
        return UART5_BASE;
    case UART6:
        return UART6_BASE;
    case UART7:
        return UART7_BASE;
    }
    return 0;
}

int UART::UartTypeToPeriph(UartType uart)
{
    switch(uart)
    {
    case UART0:
        return SYSCTL_PERIPH_UART0;
    case UART1A:
    case UART1B:
        return SYSCTL_PERIPH_UART1;
    case UART2:
        return SYSCTL_PERIPH_UART2;
    case UART3:
        return SYSCTL_PERIPH_UART3;
    case UART4:
        return SYSCTL_PERIPH_UART4;
    case UART5:
        return SYSCTL_PERIPH_UART5;
    case UART6:
        return SYSCTL_PERIPH_UART6;
    case UART7:
        return SYSCTL_PERIPH_UART7;
    }
    return 0;
}

UART::UART(UartType uart, int baud)
    : uart(uart)
{
    SysCtlPeripheralEnable(UartTypeToPeriph(uart));
    SysCtlPeripheralEnable(UartTypeToGPIOPeriph(uart));

    GPIOPinTypeUART(UartTypeToGPIOPort(uart), UartTypeToRxPin(uart) | UartTypeToTxPin(uart));
    UARTConfigSetExpClk(UartTypeToBase(uart), configCPU_CLOCK_HZ, baud,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
            UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UartTypeToBase(uart));
    UARTEnable(UartTypeToBase(uart));
}

void UART::Send(const char* toSend)
{
    while(*toSend)
    {
        // Write the next character to the UART Tx FIFO.
        if(!UARTSpaceAvail(UartTypeToBase(uart)))
        {
            vTaskDelay(1);
        }
        else
        {
            UARTCharPut(UartTypeToBase(uart), *toSend);
            *toSend++;
        }
    }
}

bool UART::Available()
{
    return UARTCharsAvail(UartTypeToBase(uart));
}

int UART::Recv(char* buffer, int maxSize)
{
    int len = 0;
    if(maxSize <= 0) return len;

    char* end = buffer + maxSize - 1;

    while(buffer < end && Available())
    {
        char recv = UARTCharGet(UartTypeToBase(uart));
        *buffer = recv;
        buffer++;
        len++;
    }

    *buffer = '\0';
    return len;

}

int UART::RecvLn(char* buffer, int maxSize)
{
    int len = 0;
    if(maxSize <= 0) return len;

    char* end = buffer + maxSize - 1;

    while(buffer < (end-1) && *(buffer-1) != '\n' && *(buffer-1) != '\r')
    {
        if(!Available())
        {
            vTaskDelay(10);
            continue;
        }
        char recv = UARTCharGet(UartTypeToBase(uart));
        *buffer = recv;
        buffer++;
        len++;
    }

    *(buffer-1)= '\r';
    *(buffer)= '\n';
    *(buffer+1) = '\0';
    return len;

}
