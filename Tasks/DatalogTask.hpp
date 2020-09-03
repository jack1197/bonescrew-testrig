/*
 * DatalogTask.hpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#ifndef TASKS_DATALOGTASK_HPP_
#define TASKS_DATALOGTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Tasks/QueueableEvent.hpp>
#include <Wrappers/Semaphore.hpp>
#include <Wrappers/Queue.hpp>

typedef struct  __attribute__ ((packed)) {
    uint32_t count;
    int16_t raw;
    private: int16_t unused1 = 0;
    public: int16_t torqueFP;
    int16_t angVelocity;
    private: int32_t unused2 = 0;
} DataFrame;

class DatalogTask: public BaseTask
{
    Queue<DataFrame>* outputBuffer;
    Queue<QueueableEvent>* usbEventQueue;
    Semaphore* dataShouldStart;
    Semaphore* dataHasEnded;
    bool* shouldStop;
    bool* shouldSendData;
    void RunDatalog();
protected:
    void Setup();
    void TaskMethod();
public:
    DatalogTask(Queue<DataFrame>* outputBuffer, Semaphore* dataShouldStart, Semaphore* dataHasEnded, bool* shouldStop, Queue<QueueableEvent>* usbEventQueue, bool* shouldSendData);
};

#endif /* TASKS_DATALOGTASK_HPP_ */
