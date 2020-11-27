/*
 * DatalogTask.hpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#ifndef TASKS_DATALOGTASK_HPP_
#define TASKS_DATALOGTASK_HPP_

#include <Model/Enums.hpp>
#include <Tasks/BaseTask.hpp>
#include <Wrappers/Semaphore.hpp>
#include <Wrappers/Queue.hpp>
#include <Wrappers/QEI.hpp>
#include <Model/Status.hpp>
#include "driverlib/udma.h"

typedef struct  __attribute__ ((packed)) {
    uint32_t count;
    int16_t raw;
    int16_t reserved1 = 0;
    int16_t torqueFP;
    int16_t angVelocity;
    int32_t reserved2 = 0x23002300;
} DataFrame;

class DatalogTask: public BaseTask
{
    Queue<DataFrame>* outputBuffer;
    Queue<UsbDataEvent>* usbEventQueue;
    Semaphore* dataShouldStart;
    Semaphore* dataHasEnded;
    Status* status;
    QEI* motor_enc;
    QEI* sensor_enc;
    bool* shouldStop;
    bool* shouldSendData;
    static Semaphore gotReadingSemaphore;
    static void ISRHandler();
    uint16_t ADCbuffers[4][250];
    static tDMAControlTable DMATaskList[4];
    static unsigned char nextBuffer;

    uint32_t filteredSample();
protected:
    void Setup();
    void TaskMethod();
public:
    DatalogTask(Queue<DataFrame>* outputBuffer,
                Semaphore* dataShouldStart,
                Semaphore* dataHasEnded,
                bool* shouldStop,
                Queue<UsbDataEvent>* usbEventQueue,
                bool* shouldSendData,
                Status* status,
                QEI* motor_enc,
                QEI* sensor_enc);
};

#endif /* TASKS_DATALOGTASK_HPP_ */
