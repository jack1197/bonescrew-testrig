/*
 * Semaphore.cpp
 *
 *  Created on: 4/08/2019
 *      Author: Samuel
 */

#include "Semaphore.hpp"

Semaphore::Semaphore(int maxCount, int initCount)
{
    if (maxCount <= 1)
    {
        SemaphoreHandle = xSemaphoreCreateBinary();
    }
    else
    {
        SemaphoreHandle = xSemaphoreCreateCounting(maxCount, initCount);
    }    
}

Semaphore::~Semaphore()
{
    vSemaphoreDelete(SemaphoreHandle);
}

void Semaphore::Take()
{
    xSemaphoreTake(SemaphoreHandle, portMAX_DELAY);
}

void Semaphore::TakeFromISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreTakeFromISR(SemaphoreHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Semaphore::Give()
{
    xSemaphoreGive(SemaphoreHandle);
}

void Semaphore::GiveFromISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(SemaphoreHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Does uxSemaphore not exist in the version of FreeRTOS we are using?
//      bcos uxSemaphore dosnt seem to exist in semphr.h
// int Semaphore::GetCount()
// {
//    return uxSemaphoreGetCount(SemaphoreHandle);
// }
