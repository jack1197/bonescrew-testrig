/*
 * UartStatus.cpp
 *
 *  Created on: 11/08/2019
 *      Author: jwi123
 */

#include <Tasks/UartStatusTask.hpp>
#include "FreeRTOS.h"
#include "utils/ustdlib.h"

UartStatusTask::UartStatusTask(Status* status, UART* uart)
    : uart(uart),
      status(status)
{
    priority = 1;
    stackSize = 256;
    taskName = "UartStatusTask";
}


void UartStatusTask::Setup()
{

}

void UartStatusTask::TaskMethod()
{
    uart->Send("   \n\r\033[2JUart output started...\n\r");

    TickType_t xLastWakeTime = xTaskGetTickCount();


    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        char string[30];

        uart->Send("\n***Test Rig Status***\n\n\r");
        usnprintf (string, sizeof(string), "Motor Encoder:  %d\033[K\n\r",
                   status->getMotorEncoder());
        uart->Send(string);

        usnprintf (string, sizeof(string), "Motor Speed:    %d\033[K\n\r",
                   status->getMotorSpeed());
        uart->Send(string);

        usnprintf (string, sizeof(string), "Sensor Encoder: %d\033[K\n\r",
                   status->getSensorEncoder());
        uart->Send(string);

        usnprintf (string, sizeof(string), "Sensor Torque:  %d\033[K\r\033[6A",
                   status->getSensorTorque());
        uart->Send(string);

        vTaskDelayUntil(&xLastWakeTime, 1000);
    }

}
