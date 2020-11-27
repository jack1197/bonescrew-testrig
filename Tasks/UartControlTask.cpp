/*
 * UartControlTask.cpp
 *
 *  Created on: 16 Oct 2020
 *      Author: Jack
 */

#include <Tasks/UartControlTask.hpp>
#include <string>
#include "utils/ustdlib.h"

UartControlTask::UartControlTask(UART* uart, Queue<ControlEvent>* eventQueue, Settings* settings, Status* status)
:uart(uart), eventQueue(eventQueue), settings(settings), status(status)
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

        {
            char* buffer = string;
            int maxSize = 50;
            int len = 0;

            char* end = buffer + maxSize - 1;

            while(buffer < (end-1) && *(buffer-1) != '\n' && *(buffer-1) != '\r')
            {
                if(!uart->Available())
                {
                    vTaskDelay(10);
                    continue;
                }
                uart->Recv(buffer, 2);
                if(*buffer != '\n' && *buffer != '\r')
                {
                    status->setInputString(string);
                }
                buffer++;
                len++;

            }

            *(buffer-1)= '\0';
        }
        status->setInputString("");

        int numTokens = sscanf(string, "%19s %19s %i", &cmd, &param1, &param2);
        if(strcmp(cmd, "set") == 0)
        {
            if(numTokens == 3)
            {
                setSetting(param1, param2);
                int value = getSetting(param1);
                usnprintf(string, sizeof(string), "set: %s %d", param1, value);
                status->setOutputString(string);
            }
            else
            {
                status->setOutputString("set: not enough params");
            }
        }
        else if (strcmp(cmd, "get") == 0)
        {
            if(numTokens == 2)
            {
                int value = getSetting(param1);
                usnprintf(string, sizeof(string), "get: %s %d", param1, value);
                status->setOutputString(string);
            }
            else
            {
                status->setOutputString("get: not enough params");
            }
        }
        else if (strcmp(cmd, "start") == 0)
        {
            settings->setEnableMotor(true);
            eventQueue->Enqueue(ControlEvent::StartMotion, false);
            status->setOutputString("starting motion...");
        }
        else if (strcmp(cmd, "pause") == 0)
        {
            eventQueue->Enqueue(ControlEvent::PauseMotion, false);
            status->setOutputString("pausing motion...");
        }
        else if (strcmp(cmd, "stop") == 0)
        {
            eventQueue->Enqueue(ControlEvent::ResetMotion, false);
            settings->setEnableMotor(false);
            status->setOutputString("stopping motion...");
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
                    status->setOutputString("jogging cw...");
                    for(int i = 0; i < repeats; i++)
                        eventQueue->Enqueue(ControlEvent::JogCW);
                }
                else if(strcmp(param1, "ccw") == 0)
                {
                    status->setOutputString("jogging ccw...");
                    for(int i = 0; i < repeats; i++)
                        eventQueue->Enqueue(ControlEvent::JogCCW);
                }
                else
                {
                    status->setOutputString("jog: invalid direction");
                }
            }
            else
            {
                status->setOutputString("jog: not enough params");
            }

        }
        else if (strcmp(cmd, "zero") == 0)
        {
        }
        else
        {
            usnprintf(string, sizeof(string), "%s: command not found", cmd);
            status->setOutputString(string);
        }
        //uart->Send(string);

        vTaskDelayUntil(&xLastWakeTime, 100);
    }

}

void UartControlTask::setSetting(char* settingName, int32_t value)
{
    if(strcmp(settingName, "ena") == 0) settings->setEnableMotor( value);
    else if(strcmp(settingName, "ind") == 0) settings->setInsertDist( value);
    else if(strcmp(settingName, "rvd") == 0) settings->setReverseDist( value);
    else if(strcmp(settingName, "jgd") == 0) settings->setJogDist( value);
    else if(strcmp(settingName, "pau") == 0) settings->setFullInsertionPause( value);
    else if(strcmp(settingName, "trp") == 0) settings->setTrapezoidEnabled( value);
    else if(strcmp(settingName, "ins") == 0) settings->setInsertSpeed( value);
    else if(strcmp(settingName, "rmp") == 0) settings->setRampTime( value);
    else if(strcmp(settingName, "hdt") == 0) settings->setHighDwellTime( value);
    else if(strcmp(settingName, "ldt") == 0) settings->setLowDwellTime( value);
    else if(strcmp(settingName, "dpt") == 0) settings->setDistPerTrapezoid( value);

}

int UartControlTask::getSetting(char* settingName)
{

    if(strcmp(settingName, "ena") == 0) return settings->getEnableMotor();
    else if(strcmp(settingName, "ind") == 0)return settings->getInsertDist();
    else if(strcmp(settingName, "rvd") == 0)return settings->getReverseDist();
    else if(strcmp(settingName, "jgd") == 0)return settings->getJogDist();
    else if(strcmp(settingName, "pau") == 0)return settings->getFullInsertionPause();
    else if(strcmp(settingName, "trp") == 0)return settings->getTrapezoidEnabled();
    else if(strcmp(settingName, "ins") == 0)return settings->getInsertSpeed();
    else if(strcmp(settingName, "rmp") == 0)return settings->getRampTime();
    else if(strcmp(settingName, "hdt") == 0)return settings->getHighDwellTime();
    else if(strcmp(settingName, "ldt") == 0)return settings->getLowDwellTime();
    else if(strcmp(settingName, "dpt") == 0)return settings->getDistPerTrapezoid();
    else return -1111111111;
}
