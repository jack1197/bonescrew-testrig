/*
 * DatalogTask.cpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#include <Tasks/DatalogTask.hpp>
#include <Wrappers/UDMAArray.hpp>
#include <Model/FirCoeffs.hpp>
#include "inc/hw_memmap.h"
#include "inc/tm4c123fh6pm.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/udma.h"
#include "inc/hw_udma.h"


#define TORQUE_ADC_PERIPH     SYSCTL_PERIPH_ADC0
#define TORQUE_ADC            ADC0_BASE
#define TORQUE_ADC_CHANNEL    ADC_CTL_CH7
#define TORQUE_ADC_INT        INT_ADC0SS3

Semaphore DatalogTask::gotReadingSemaphore(1,0);

tDMAControlTable DatalogTask::DMATaskList[4];
unsigned char DatalogTask::nextBuffer = 2;

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
    SysCtlPeripheralEnable(TORQUE_ADC_PERIPH);
    while (!SysCtlPeripheralReady(TORQUE_ADC_PERIPH)) ;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UDMA)) ;

    uDMAEnable();
    uDMAControlBaseSet(udmaArray);
    uDMAChannelAttributeEnable(UDMA_CHANNEL_ADC0, UDMA_ATTR_USEBURST);

    uDMAChannelControlSet(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_4);
    uDMAChannelTransferSet(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)&ADC0_SSFIFO0_R, &ADCbuffers[0][0], 250);
    uDMAChannelControlSet(UDMA_CHANNEL_ADC0 | UDMA_ALT_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_4);
    uDMAChannelTransferSet(UDMA_CHANNEL_ADC0 | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)&ADC0_SSFIFO0_R, &ADCbuffers[1][0], 250);

    uDMAChannelEnable(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT);
    uDMAChannelEnable(UDMA_CHANNEL_ADC0 | UDMA_ALT_SELECT);
/*


    uDMAChannelScatterGatherSet(UDMA_CHANNEL_ADC0, 4, DMATaskList, true);


    //copy tasks for buffers
    DMATaskList[0] = uDMATaskStructEntry(
            250,
            UDMA_SIZE_16,
            UDMA_SRC_INC_NONE,
            &ADC0_SSFIFO0_R,
            UDMA_DST_INC_16,
            &ADCbuffers[0][0],
            UDMA_ARB_4,
            UDMA_MODE_PER_SCATTER_GATHER);
    DMATaskList[1] = uDMATaskStructEntry(
            250,
            UDMA_SIZE_16,
            UDMA_SRC_INC_NONE,
            &ADC0_SSFIFO0_R,
            UDMA_DST_INC_16,
            &ADCbuffers[1][0],
            UDMA_ARB_4,
            UDMA_MODE_PER_SCATTER_GATHER);
    DMATaskList[2] = uDMATaskStructEntry(
            250,
            UDMA_SIZE_16,
            UDMA_SRC_INC_NONE,
            &ADC0_SSFIFO0_R,
            UDMA_DST_INC_16,
            &ADCbuffers[2][0],
            UDMA_ARB_4,
            UDMA_MODE_PER_SCATTER_GATHER);
    DMATaskList[3] = uDMATaskStructEntry(
            250,
            UDMA_SIZE_16,
            UDMA_SRC_INC_NONE,
            &ADC0_SSFIFO0_R,
            UDMA_DST_INC_16,
            &ADCbuffers[3][0],
            UDMA_ARB_4,
            UDMA_MODE_PER_SCATTER_GATHER);

*/
    ADCSequenceConfigure(TORQUE_ADC, 0, ADC_TRIGGER_ALWAYS, 0);
    ADCHardwareOversampleConfigure(TORQUE_ADC, 4);


    ADCSequenceStepConfigure(TORQUE_ADC, 0, 0, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 1, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 2, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 3, TORQUE_ADC_CHANNEL | ADC_CTL_IE);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 4, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 5, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 6, TORQUE_ADC_CHANNEL);
    ADCSequenceStepConfigure(TORQUE_ADC, 0, 7, TORQUE_ADC_CHANNEL | ADC_CTL_IE | ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(TORQUE_ADC, 0);
    ADCSequenceDMAEnable(TORQUE_ADC, 0);
    //
    // Register the interrupt handler
    ADCIntRegister (TORQUE_ADC, 0, ISRHandler);


    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    IntPrioritySet(INT_ADC0SS0, (5 << 5) );
    ADCIntEnable(TORQUE_ADC, 0);
}

void DatalogTask::ISRHandler()
{
    uint32_t ints = ADCIntStatusEx(TORQUE_ADC, 0xffffffff);
    ADCIntClearEx(TORQUE_ADC, ints & ADC_INT_SS0);

//    uDMAChannelScatterGatherSet(UDMA_CHANNEL_ADC0, 4, DMATaskList, true);
//    uDMAChannelEnable(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT);

    ADCSequenceDMAEnable(TORQUE_ADC, 0);
    gotReadingSemaphore.GiveFromISR();
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


        //wait for samples
        gotReadingSemaphore.Take();

        //reset pingpong for next buffer
        uint32_t channel = UDMA_CHANNEL_ADC0 | ((nextBuffer % 2 == 0) ? UDMA_PRI_SELECT : UDMA_ALT_SELECT);
        uDMAChannelControlSet(channel, UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_4);
        uDMAChannelTransferSet(channel, UDMA_MODE_PINGPONG, (void *)&ADC0_SSFIFO0_R, &ADCbuffers[nextBuffer % 4][0], 250);

        uDMAChannelEnable(channel);
        nextBuffer++;

        uint32_t ulValue  = filteredSample();

        int32_t rawSignedValue = ulValue - 0x80000000;//underflow negative values, correct zero-offset, then reinterpret as signed.

        int64_t longlongSignedValue = rawSignedValue;

        //scale to data range (-4/5 to 4/5 of full signed range (or 1/10 to 9/10 of unsigned range))
        longlongSignedValue = (longlongSignedValue * 5)/4;

        //scale to torque range (with 2 dp decimal fixed-point type) (-20(-2000) to 20(2000) Nm over data range)
        int16_t torqueFPValue2dp = (longlongSignedValue*2000)/0x80000000/*2^31*/;

        //scale to torque range (with 3 dp decimal fixed-point type) (-20(-20000) to 20(20000) Nm over data range)
        int16_t torqueFPValue3dp = (longlongSignedValue*20000)/0x80000000/*2^31*/;

        frame.count++;
        frame.raw = rawSignedValue >> 16;
        frame.torqueFP = torqueFPValue2dp;
        status->setSensorEncoder(sensor_enc->getPos());
        status->setSensorTorque(torqueFPValue3dp);
        status->setMotorEncoder(motor_enc->getPos());

        outputBuffer->Enqueue(frame, false);

        vTaskDelayUntil(&xLastWakeTime, 100);

    }
}

uint32_t DatalogTask::filteredSample()
{
    uint64_t register accumulator = 0;

    unsigned char nextBuffer = this->nextBuffer;//copy locally to avoid unexpected changes

    //last part of first buffer with first part of forward filter
    uint16_t* bufferPtr = &ADCbuffers[(nextBuffer + 3) % 4][123];

    const register uint16_t* coeffsPtr = &firCoeffs[0];
    const register uint16_t* coeffsEnd = &firCoeffs[126];

    while(coeffsPtr <= coeffsEnd)
    {
        accumulator = accumulator + (((uint32_t) *coeffsPtr) * ((uint32_t) *bufferPtr));
        bufferPtr++;
        coeffsPtr++;
    }


    //first part of second buffer with second part of forward filter
    bufferPtr = &ADCbuffers[(nextBuffer) % 4][0];

    coeffsEnd = &firCoeffs[313];

    while(coeffsPtr <= coeffsEnd)
    {
        accumulator = accumulator + (((uint32_t) *coeffsPtr) * ((uint32_t) *bufferPtr));
        bufferPtr++;
        coeffsPtr++;
    }

    //second part of second buffer with first part of reversed filter

    coeffsPtr = &firCoeffs[312];
    coeffsEnd = &firCoeffs[250];


    while(coeffsPtr >= coeffsEnd)
    {
        accumulator = accumulator + (((uint32_t) *coeffsPtr) * ((uint32_t) *bufferPtr));
        bufferPtr++;
        coeffsPtr--;
    }
    //third buffer with second part of reversed filter
    bufferPtr = &ADCbuffers[(nextBuffer + 1) % 4][0];

    coeffsEnd = &firCoeffs[0];

    while(coeffsPtr >= coeffsEnd)
    {
        accumulator = accumulator + (((uint32_t) *coeffsPtr) * ((uint32_t) *bufferPtr));
        bufferPtr++;
        coeffsPtr--;
    }

    //divide accumulator to fit almost exactly in 32 bits.
    accumulator = (accumulator * 1000)/18401;
    //accumulator = (accumulator * 1000)/62;

    return accumulator;
}
