/*
 * UartControlTask.cpp
 *
 *  Created on: 16 Oct 2020
 *      Author: Jack
 */

#include <Tasks/UartControlTask.hpp>
#include <string>

UartControlTask::UartControlTask(UART* uart, Queue<ControlEvent>* eventQueue, Settings* settings)
:uart(uart), eventQueue(eventQueue), settings(settings)
{
    // TODO Auto-generated constructor stub
    priority = 3;
    stackSize = 512;
    taskName = "UartControlTask";

}

void UartControlTask::Setup()
{

}

void UartControlTask::TaskMethod()
{

    TickType_t xLastWakeTime = xTaskGetTickCount();

    char string[50];
    char cmd[20];
    char param1[20];
    int32_t param2;

    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

        uart->RecvLn(string, 50);

        int numTokens = sscanf(string, "%19s %19s %i", &cmd, &param1, &param2);
        if(strcmp(cmd, "set") == 0)
        {

        }
        else if (strcmp(cmd, "get") == 0)
        {

        }
        else if (strcmp(cmd, "start") == 0)
        {
            settings->setEnableMotor(true);
            eventQueue->Enqueue(ControlEvent::StartMotion, false);
        }
        else if (strcmp(cmd, "pause") == 0)
        {
            eventQueue->Enqueue(ControlEvent::PauseMotion, false);
        }
        else if (strcmp(cmd, "stop") == 0)
        {
            eventQueue->Enqueue(ControlEvent::ResetMotion, false);
            settings->setEnableMotor(false);
        }
        else if (strcmp(cmd, "jog") == 0)
        {
            if(numTokens >= 2)
            {
                settings->setEnableMotor(true);
                int repeats = 1;
                if(numTokens == 3)
                {
                    if (param2 > 1)
                    {
                        repeats = param2;
                    }
                }
                if(strcmp(param1, "cw") == 0)
                {
                    uart->Send("jogging...\r\n");
                    for(int i = 0; i < repeats; i++)
                        eventQueue->Enqueue(ControlEvent::JogCW);
                }
                else if(strcmp(param1, "ccw") == 0)
                {
                    uart->Send("jogging...\r\n");
                    for(int i = 0; i < repeats; i++)
                        eventQueue->Enqueue(ControlEvent::JogCCW);
                }
                else
                {
                    uart->Send("jog: invalid direction\r\n");
                }
            }
            else
            {
                uart->Send("jog: not enough params\r\n");
            }

        }
        else
        {
            uart->Send(cmd);
            uart->Send(": command not found\r\n");
        }
        uart->Send(string);

        vTaskDelayUntil(&xLastWakeTime, 100);
    }

}
