/*
 * Mutex.cpp
 *
 *  Created on: 3/08/2019
 *      Author: Samuel
 */

#include "Mutex.hpp"

Mutex::Mutex()
{
    MutexSemaphore = xSemaphoreCreateMutex();
}

Mutex::~Mutex()
{
    vSemaphoreDelete(MutexSemaphore);
}

void Mutex::Take()
{
    xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
}

void Mutex::Give()
{
    xSemaphoreGive(MutexSemaphore);
}
