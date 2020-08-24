/*
 * BaseTask.cpp
 *
 *  Created on: 3/08/2019
 *      Author: Samuel
 */

#include "BaseTask.hpp"
#include "FreeRTOSConfig.h"

void BaseTask::RunTask(void* params)
{
    BaseTask* object = (BaseTask*)params;
    object->TaskMethod();
}

void BaseTask::Initialize()
{
    Setup();
    if (pdTRUE != xTaskCreate(RunTask, taskName, stackSize, (void *)this , priority, &handle)) {
        while (1) ; // error creating task, out of memory?
    }
}
