/*
 * Queue.cpp
 *
 *  Created on: 5/08/2019
 *      Author: sjh310
 */

#include "Queue.hpp"

template <class T>
Queue<T>::Queue(int queueLength)
{
    queue = xQueueCreate(queueLength, sizeof(T));
}

template <class T>
Queue<T>::~Queue()
{
    vQueueDelete(queue);
}

template <class T>
bool Queue<T>::Enqueue(T queueItem, bool blocking)
{
    return xQueueSendToBack(
            queue,
            (void*) &queueItem,
            blocking ? portMAX_DELAY : (TickType_t) 0
                    ) == pdTRUE;
}

template <class T>
bool Queue<T>::EnqueueFromISR(T queueItem, bool blocking)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    bool returned = xQueueSendToBackFromISR(
            queue,
            (void*) &queueItem,
            &xHigherPriorityTaskWoken
                    ) == pdTRUE;
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return returned;
}

template <class T>
bool Queue<T>::Dequeue(T* queueItem, bool blocking)
{
    return xQueueReceive(
            queue,
            (void*) queueItem,
            blocking ? portMAX_DELAY : (TickType_t) 0
                    ) == pdTRUE;
}




