/*
 * UartStatus.h
 *
 * Task to output status info over uart
 *
 *  Created on: 11/08/2019
 *      Author: jwi123
 */

#ifndef TASKS_UARTSTATUSTASK_HPP_
#define TASKS_UARTSTATUSTASK_HPP_

#include "BaseTask.hpp"
#include "Wrappers/UART.hpp"
#include "Model/Status.hpp"

class UartStatusTask: public BaseTask
{
    UART* uart;
    Status* status;
protected:
    void Setup();
    void TaskMethod();
public:
    UartStatusTask(Status* status, UART* uart);
};

#endif /* TASKS_UARTSTATUSTASK_HPP_ */
