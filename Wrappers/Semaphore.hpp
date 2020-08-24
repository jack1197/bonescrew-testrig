/*
 * Semaphore.hpp
 *
 *  Created on: 4/08/2019
 *      Author: Samuel
 */

#ifndef SEMAPHORE_HPP_
#define SEMAPHORE_HPP_

#include "FreeRTOS.h"
#include "semphr.h"

class Semaphore
{
private:
    SemaphoreHandle_t SemaphoreHandle;
public:
    Semaphore(int maxCount, int initCount);
    virtual ~Semaphore();
    void Take();
    void TakeFromISR();
    void Give();
    void GiveFromISR();
    // int GetCount();
};

#endif /* SEMAPHORE_HPP_ */
