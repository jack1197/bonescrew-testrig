/*
 * UartControlTask.hpp
 *
 *  Created on: 16 Oct 2020
 *      Author: Jack
 */

#ifndef TASKS_UARTCONTROLTASK_HPP_
#define TASKS_UARTCONTROLTASK_HPP_

#include "BaseTask.hpp"
#include "Wrappers/UART.hpp"
#include "Wrappers/Queue.hpp"
#include "Model/Settings.hpp"
#include "Model/Enums.hpp"

class UartControlTask : public BaseTask
{
    UART* uart;
    Queue<ControlEvent>* eventQueue;
    Settings* settings;
    void setSetting(char* settingName, int32_t value);
    void getSetting(char* settingName);
protected:
    void Setup();
    void TaskMethod();
public:
    UartControlTask(UART* uart, Queue<ControlEvent>* eventQueue, Settings* settings);
};

#endif /* TASKS_UARTCONTROLTASK_HPP_ */
