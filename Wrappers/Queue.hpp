/*
 * Queue.hpp
 *
 *  Created on: 5/08/2019
 *      Author: sjh310
 */

#ifndef WRAPPERS_QUEUE_HPP_
#define WRAPPERS_QUEUE_HPP_

#include "FreeRTOS.h"
#include "queue.h"

template <class T>
class Queue
{
private:
    QueueHandle_t queue;
public:
    Queue(int queueLength);
    ~Queue();
    bool Enqueue(T queueItem, bool blocking = false);
    bool EnqueueFromISR(T queueItem, bool blocking = false);
    bool Dequeue(T* queueItem, bool blocking = false);
};

#include "Queue_impl.hpp"

#endif /* WRAPPERS_QUEUE_HPP_ */
