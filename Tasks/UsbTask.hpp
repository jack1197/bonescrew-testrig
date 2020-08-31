/*
 * UsbTask.hpp
 *
 *  Created on: 28 Aug 2020
 *      Author: Jack
 */

#ifndef TASKS_USBTASK_HPP_
#define TASKS_USBTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Wrappers/GPIO.hpp>
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

    static const uint16_t g_ui16MaxPacketSize;
    static void HandleReset(void *pvUsbTask);
    static void HandleConfigChange(void *pvUsbTask, uint32_t ui32Info);
    static void HandleDisconnect(void *pvUsbTask);
    static void HandleEndpoints(void *pvUsbTask, uint32_t ui32Status);
    static void HandleSuspend(void *pvUsbTask);
    static void HandleResume(void *pvUsbTask);
    static void HandleDevice(void *pvUsbTask, uint32_t ui32Request,
                             void *pvRequestData);

    GPIO* activeLed;
protected:
    void Setup();
    void TaskMethod();

public:
    UsbTask(GPIO* activeLed);
};

#endif /* TASKS_USBTASK_HPP_ */
