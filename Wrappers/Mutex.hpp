/*
 * Mutex.hpp
 *
 *  Created on: 3/08/2019
 *      Author: Samuel
 */

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include "FreeRTOS.h"
#include "semphr.h"

class Mutex
{
private:
    SemaphoreHandle_t MutexSemaphore;
public:
    Mutex();
    virtual ~Mutex();
    void Take();
    void Give();
};

#endif /* MUTEX_HPP_ */
