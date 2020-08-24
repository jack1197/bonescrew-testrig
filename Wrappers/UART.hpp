/*
 * UART.h
 *
 *  Created on: 11/08/2019
 *      Author: jwi123
 */

#ifndef WRAPPERS_UART_HPP_
#define WRAPPERS_UART_HPP_

//UART 1A uses PC4/5, UART 1B uses PB0/1
typedef enum UartType_e {UART0, UART1A, UART1B, UART2, UART3, UART4, UART5, UART6, UART7} UartType;

class UART
{
    int UartTypeToRxPin(UartType uart);
    int UartTypeToTxPin(UartType uart);
    int UartTypeToGPIOPort(UartType uart);
    int UartTypeToGPIOPeriph(UartType uart);
    int UartTypeToBase(UartType uart);
    int UartTypeToPeriph(UartType uart);
    UartType uart;
public:
    UART(UartType uart, int baud);
    void Send(const char* toSend);
    bool Available();
    int Recv(char* buffer, int maxSize);
};

#endif /* WRAPPERS_UART_HPP_ */
