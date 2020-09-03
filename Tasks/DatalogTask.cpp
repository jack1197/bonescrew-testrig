/*
 * DatalogTask.cpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#include <Tasks/DatalogTask.hpp>

DatalogTask::DatalogTask(Queue<DataFrame>* outputBuffer, Semaphore* dataShouldStart, Semaphore* dataHasEnded, bool* shouldStop, Queue<QueueableEvent>* usbEventQueue, bool* shouldSendData)
:outputBuffer(outputBuffer), dataShouldStart(dataShouldStart), dataHasEnded(dataHasEnded), shouldStop(shouldStop), usbEventQueue(usbEventQueue), shouldSendData(shouldSendData)
{
}

void DatalogTask::Setup()
{
    priority = 3;
    stackSize = 128;
    taskName = "DatalogTask";
}

void DatalogTask::TaskMethod()
{

    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        dataShouldStart->Take();
        RunDatalog();
        dataHasEnded->Give();
    }
}

void DatalogTask::RunDatalog()
{
    DataFrame frame;
    frame.count = 0;
    frame.raw = 1337;
    frame.torqueFP = 4269;
    frame.angVelocity = 9001;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        if(*shouldStop)
        {
            return;
        }
        outputBuffer->Enqueue(frame, false);
        if(!*shouldSendData)
        {
            usbEventQueue->Enqueue(QueueableEvent::DatAvail, false);
            //block DataAvail events to prevent it filling the queue
            *shouldSendData = true;
        }

        frame.count++;

        vTaskDelayUntil(&xLastWakeTime, 10 );
    }
}
