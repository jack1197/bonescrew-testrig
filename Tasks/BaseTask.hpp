/*
 * BaseTask.hpp
 *
 * Base class for all tasks
 *
 *  Created on: 3/08/2019
 *      Author: Samuel
 */

#ifndef BASETASK_HPP_
#define BASETASK_HPP_

#include "FreeRTOS.h"
#include "task.h"

class BaseTask
{
private:
    static void RunTask(void* params);
protected:
    int priority;
    int stackSize;
    char* taskName;
    TaskHandle_t handle;
    virtual void Setup() = 0;
    virtual void TaskMethod() = 0;
public:
    void Initialize();
};

#endif /* BASETASK_HPP_ */
