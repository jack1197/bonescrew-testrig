/*
 * DatalogTask.cpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#include <Tasks/DatalogTask.hpp>
#include "inc/hw_memmap.h"
#include "inc/tm4c123fh6pm.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"


#define ALTITUDE_ADC_PERIPH     SYSCTL_PERIPH_ADC0
#define ALTITUDE_ADC            ADC0_BASE
#define ALTITUDE_ADC_CHANNEL    ADC_CTL_CH7
#define ALTITUDE_ADC_INT        INT_ADC0SS3

Semaphore DatalogTask::gotReadingSemaphore(1,0);

DatalogTask::DatalogTask(Queue<DataFrame>* outputBuffer,
                         Semaphore* dataShouldStart,
                         Semaphore* dataHasEnded,
                         bool* shouldStop,
                         Queue<UsbDataEvent>* usbEventQueue,
                         bool* shouldSendData,
                         Status* status,
                         QEI* motor_enc,
                         QEI* sensor_enc)
:outputBuffer(outputBuffer),
 dataShouldStart(dataShouldStart),
 dataHasEnded(dataHasEnded),
 shouldStop(shouldStop),
 usbEventQueue(usbEventQueue),
 shouldSendData(shouldSendData),
 status(status),
 motor_enc(motor_enc),
 sensor_enc(sensor_enc)
{
    priority = 9;
    stackSize = 128;
    taskName = "DatalogTask";
}

void DatalogTask::Setup()
{
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(ALTITUDE_ADC_PERIPH);
    while (!SysCtlPeripheralReady(ALTITUDE_ADC_PERIPH)) ;

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ALTITUDE_ADC, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel (ALTITUDE_ADC_CHANNEL) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ALTITUDE_ADC, 3, 0, ALTITUDE_ADC_CHANNEL | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ALTITUDE_ADC, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ALTITUDE_ADC, 3, ISRHandler);

    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    IntPrioritySet(INT_ADC0SS3, (5 << 5) );
    ADCIntEnable(ALTITUDE_ADC, 3);
}

void DatalogTask::ISRHandler()
{
    gotReadingSemaphore.GiveFromISR();
    ADCIntClear(ALTITUDE_ADC, 3);
}

void DatalogTask::TaskMethod()
{
    DataFrame frame;
    frame.count = 0;
    frame.raw = 1337;
    frame.torqueFP = 1234;
    frame.angVelocity = 9001;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    volatile UBaseType_t uxHighWaterMark;

    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

        //read analog value
        //trigger sample on sequence 3
        ADCProcessorTrigger(ADC0_BASE, 3);

        //wait for sample
        gotReadingSemaphore.Take();

        //Get sample on sequence 3
        uint32_t ulValue;
        ADCSequenceDataGet(ALTITUDE_ADC, 3, &ulValue);
        frame.raw = ulValue;
        frame.torqueFP = ulValue;
        status->setSensorEncoder(sensor_enc->getPos());
        status->setSensorTorque(ulValue);
        status->setMotorEncoder(motor_enc->getPos());

        if(!*shouldSendData)
        {
            frame.count = 0;
        }
        else
        {
            outputBuffer->Enqueue(frame, false);
            frame.count++;
        }


        vTaskDelayUntil(&xLastWakeTime, 100);
    }
}
