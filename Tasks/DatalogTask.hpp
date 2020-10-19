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
