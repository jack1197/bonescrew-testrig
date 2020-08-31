/*
 * UsbTask.cpp
 *
 *  Created on: 28 Aug 2020
 *      Author: Jack
 */

#include <Tasks/UsbTask.hpp>

extern "C"
{
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/usb.h"
#include "driverlib/gpio.h"

#include "utils/uartstdio.h"
}


//Descriptors modified from usbdbulk.c

#define DATA_IN_EP_MAX_SIZE     USBFIFOSizeToBytes(USB_FIFO_SZ_64)
#define DATA_OUT_EP_MAX_SIZE    USBFIFOSizeToBytes(USB_FIFO_SZ_64)

const uint8_t* UsbTask::g_pui8TorqueDeviceDescriptor =
(const uint8_t[])
{
    18,                         // Size of this structure.
    USB_DTYPE_DEVICE,           // Type of this structure.
    USBShort(0x110),            // USB version 1.1 (if we say 2.0, hosts assume
                                // high-speed - see USB 2.0 spec 9.2.6.6)
    USB_CLASS_VEND_SPECIFIC,    // USB Device Class
    0,                          // USB Device Sub-class
    0,                          // USB Device protocol
    64,                         // Maximum packet size for default pipe.
    USBShort(0x1483),           // Vendor ID (VID).
    USBShort(0xa104),           // Product ID (PID).
    USBShort(0x100),            // Device Version BCD.
    1,                          // Manufacturer string identifier.
    2,                          // Product string identifier.
    3,                          // Product serial number.
    1                           // Number of configurations.
};

const uint8_t* UsbTask::g_pui8TorqueConfigurationDescriptor =
(const uint8_t[])
{
    //
    // Configuration descriptor header.
    //
    9,                          // Size of the configuration descriptor.
    USB_DTYPE_CONFIGURATION,    // Type of this descriptor.
    USBShort(39),                // The total size of this full structure.
    1,                          // The number of interfaces in this
                                // configuration.
    1,                          // The unique value for this configuration.
    4,                          // The string identifier that describes this
                                // configuration.
    USB_CONF_ATTR_SELF_PWR,      // Bus Powered, Self Powered, remote wake up.
    50,                        // The maximum power in 2mA increments.
};

const uint8_t* UsbTask::g_pui8TorqueInterfaceDescriptor =
(const uint8_t[])
{
    //
    // Vendor-specific Interface Descriptor.
    //
    9,                              // Size of the interface descriptor.
    USB_DTYPE_INTERFACE,            // Type of this descriptor.
    0,                              // The index for this interface.
    0,                              // The alternate setting for this
                                    // interface.
    3,                              // The number of endpoints used by this
                                    // interface.
    USB_CLASS_VEND_SPECIFIC,        // The interface class
    0,                              // The interface sub-class.
    0,                              // The interface protocol for the sub-class
                                    // specified above.
    5,                              // The string index for this interface.

    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USBEPToIndex(USB_EP_1),
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | USBEPToIndex(USB_EP_2),
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),  // The maximum packet size.
    0,

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USBEPToIndex(USB_EP_3),
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
};

tConfigSection UsbTask::g_sTorqueConfigSection =
{
    9,
    g_pui8TorqueConfigurationDescriptor
};

tConfigSection UsbTask::g_sTorqueInterfaceSection =
{
    30,
    g_pui8TorqueInterfaceDescriptor
};

const tConfigSection*const * UsbTask::g_psTorqueSections =
(const tConfigSection*const [])
{
    &g_sTorqueConfigSection,
    &g_sTorqueInterfaceSection
};

const tConfigHeader UsbTask::g_sTorqueConfigHeader =
{
    2,
    g_psTorqueSections
};

const tConfigHeader*const * UsbTask::g_sTorqueConfigHeaders =
(const tConfigHeader*const [])
{
    &g_sTorqueConfigHeader
};

const uint16_t UsbTask::g_ui16MaxPacketSize = USBFIFOSizeToBytes(USB_FIFO_SZ_64);

const tCustomHandlers UsbTask::g_sTorqueHandlers =
{
    //
    // GetDescriptor
    //
    0,

    //
    // RequestHandler
    //
    0,

    //
    // InterfaceChange
    //
    0,

    //
    // ConfigChange
    //
    HandleConfigChange,

    //
    // DataReceived
    //
    0,

    //
    // DataSentCallback
    //
    0,

    //
    // ResetHandler
    //
    HandleReset,

    //
    // SuspendHandler
    //
    HandleSuspend,

    //
    // ResumeHandler
    //
    HandleResume,

    //
    // DisconnectHandler
    //
    HandleDisconnect,

    //
    // EndpointHandler
    //
    HandleEndpoints,

    //
    // Device handler
    //
    HandleDevice
};

const uint8_t* UsbTask::g_pui8LangDescriptor =
(const uint8_t [])
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

const uint8_t* UsbTask::g_pui8ManufacturerString =
(const uint8_t [])
{
    (17 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'e', 0, 's', 0, 't', 0, ' ', 0, 'M', 0, 'a', 0, 'n', 0, 'u', 0,
    'f', 0, 'a', 0, 'c', 0, 't', 0, 'u', 0, 'r', 0, 'e', 0, 'r', 0,
};

const uint8_t* UsbTask::g_pui8ProductString =
(const uint8_t [])
{
    (18 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'o', 0, 'r', 0, 'q', 0, 'u', 0, 'e', 0, ' ', 0, 's', 0, 'e', 0,
    'n', 0, 's', 0, 'o', 0, 'r', 0, ' ', 0, 'T', 0, 'i', 0, 'v', 0, 'a', 0,
};

const uint8_t* UsbTask::g_pui8SerialNumberString =
(const uint8_t [])
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

const uint8_t* UsbTask::g_pui8ConfigString =
(const uint8_t [])
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

const uint8_t* UsbTask::g_pui8InterfaceString =
(const uint8_t [])
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

const uint8_t*const * UsbTask::g_ppui8StringDescriptors =
(const uint8_t*const [])
{
    g_pui8LangDescriptor,
    g_pui8ManufacturerString,
    g_pui8ProductString,
    g_pui8SerialNumberString,
    g_pui8ConfigString,
    g_pui8InterfaceString
};

tDeviceInfo UsbTask::g_sTorqueDeviceInfo =
{
    &g_sTorqueHandlers,
    g_pui8TorqueDeviceDescriptor,
    g_sTorqueConfigHeaders,
    g_ppui8StringDescriptors,
    6

};

UsbTask::UsbTask(GPIO* activeLed)
    :activeLed(activeLed)
{
    priority = 3;
    stackSize = 1024;
    taskName = "UsbTask";
}


void UsbTask::Setup()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    USBStackModeSet(0, eUSBModeDevice, 0);

    USBDCDInit(0, &g_sTorqueDeviceInfo, this);
}


void UsbTask::TaskMethod()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();



    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

        vTaskDelayUntil(&xLastWakeTime, 5 / portTICK_RATE_MS );
    }
}


void UsbTask::HandleReset(void *pvUsbTask)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
    usbTask->activeLed->setActiveState(1);
}



void UsbTask::HandleConfigChange(void *pvUsbTask, uint32_t ui32Info)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
}


void UsbTask::HandleDisconnect(void *pvUsbTask)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
    usbTask->activeLed->setActiveState(1);
}


void UsbTask::HandleEndpoints(void *pvUsbTask, uint32_t ui32Status)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
}


void UsbTask::HandleSuspend(void *pvUsbTask)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
}


void UsbTask::HandleResume(void *pvUsbTask)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
}


void UsbTask::HandleDevice(void *pvUsbTask, uint32_t ui32Request,
                         void *pvRequestData)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
}
