/*
 * UsbTask.hpp
 *
 *  Created on: 28 Aug 2020
 *      Author: Jack
 */

#ifndef TASKS_USBTASK_HPP_
#define TASKS_USBTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Tasks/DatalogTask.hpp>
#include <Wrappers/GPIO.hpp>
#include <Wrappers/Queue.hpp>
#include <stdint.h>
#include <usblib/usblib.h>
#include <usblib/device/usbdevice.h>

class UsbTask: public BaseTask
{
    static const uint8_t* g_pui8TorqueDeviceDescriptor;
    static const uint8_t* g_pui8TorqueConfigurationDescriptor;
    static const uint8_t* g_pui8TorqueInterfaceDescriptor;

    static tConfigSection g_sTorqueConfigSection;
    static tConfigSection g_sTorqueInterfaceSection;
    static const tConfigSection*const * g_psTorqueSections;
    static const tConfigHeader g_sTorqueConfigHeader;
    static const tConfigHeader*const * g_sTorqueConfigHeaders;

    static const tCustomHandlers g_sTorqueHandlers;

    static const uint8_t* g_pui8LangDescriptor;
    static const uint8_t* g_pui8ManufacturerString;
    static const uint8_t* g_pui8ProductString;
    static const uint8_t* g_pui8SerialNumberString;
    static const uint8_t* g_pui8ConfigString;
    static const uint8_t* g_pui8InterfaceString;
    static const uint8_t*const * g_ppui8StringDescriptors;

    static tDeviceInfo g_sTorqueDeviceInfo;

    static tUSBBuffer cmdBuffer;
    static tUSBBuffer rspBuffer;
    static const uint16_t g_ui16MaxPacketSize;
    static void HandleReset(void *pvUsbTask);
    static void HandleEndpoints(void *pvUsbTask, uint32_t ui32Status);
    static void HandleUSBTick(void *pvUsbTask, uint32_t ui32TimemS);

    void processCmdEP(uint32_t ui32Status);
    void processRspEP(uint32_t ui32Status);

    static uint32_t rxHandlerCmdEP(void* pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);
    static uint32_t txHandlerRspEP(void* pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);

    static uint32_t availableCmdEP(void *pvHandle);
    static uint32_t availableRspEP(void *pvHandle);

    static uint32_t readCmdEP(void *pvHandle, uint8_t *pi8Data, uint32_t ui32Length, bool bLast);
    static uint32_t writeRspEP(void *pvHandle, uint8_t *pi8Data, uint32_t ui32Length, bool bLast);

    GPIO* activeLed;

    bool defferedRead = false;
    uint32_t lastRspSize = 0;
    bool rspActive = false;
    bool datActive = false;
    bool logging = false;


    const char* rspResume = nullptr;
    uint32_t rspRemaining = 0;

    Queue<UsbDataEvent>* usbEventQueue;

    void startRsp(const char* toSend, size_t length);
    Queue<DataFrame>* outputBuffer;
    Semaphore* dataShouldStart;
    Semaphore* dataHasEnded;
    bool* shouldStop;
    bool* shouldSendData;

protected:
    void Setup();
    void TaskMethod();

public:

    UsbTask(GPIO* activeLed, Queue<UsbDataEvent>* usbEventQueue, Queue<DataFrame>* outputBuffer, Semaphore* dataShouldStart, Semaphore* dataHasEnded, bool* shouldStop, bool* shouldSendData);
};

#endif /* TASKS_USBTASK_HPP_ */
