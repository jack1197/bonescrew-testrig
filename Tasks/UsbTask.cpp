/*
 * UsbTask.cpp
 *
 *  Created on: 28 Aug 2020
 *      Author: Jack
 */

#include <cstring>
#include <Tasks/UsbTask.hpp>

extern "C"
{
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123fh6pm.h"

#include "driverlib/sysctl.h"
#include "driverlib/usb.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

#include "utils/uartstdio.h"
#include "usblib/usblibpriv.h"
}


//Descriptors modified from usbdbulk.c

#define DATA_IN_EP_MAX_SIZE     USBFIFOSizeToBytes(USB_FIFO_SZ_64)
#define DATA_OUT_EP_MAX_SIZE    USBFIFOSizeToBytes(USB_FIFO_SZ_64)

#define USB_RX_ERROR_FLAGS      (USBERR_DEV_RX_DATA_ERROR |                   \
                                 USBERR_DEV_RX_OVERRUN |                      \
                                 USBERR_DEV_RX_FIFO_FULL)

const uint8_t* UsbTask::g_pui8TorqueDeviceDescriptor =
(const uint8_t[])
{
    18,                         // Size of this structure.
    USB_DTYPE_DEVICE,           // Type of this structure.
    USBShort(0x110),            // USB version 1.1 (if we say 2.0, hosts assume
                                // high-speed - see USB 2.0 spec 9.2.6.6)
    USB_CLASS_VEND_SPECIFIC,    // USB Device Class
    0,                          // USB Device Sub-class
    1,                          // USB Device protocol
    64,                         // Maximum packet size for default pipe.
    USBShort(0x0483),           // Vendor ID (VID).
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
    USB_CONF_ATTR_BUS_PWR,      // Bus Powered, Self Powered, remote wake up.
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
    0,

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
    0,

    //
    // ResumeHandler
    //
    0,

    //
    // DisconnectHandler
    //
    0,

    //
    // EndpointHandler
    //
    HandleEndpoints,

    //
    // Device handler
    //
    0
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

UsbTask::UsbTask(GPIO* activeLed, Queue<UsbDataEvent>* usbEventQueue, Queue<DataFrame>* outputBuffer, Semaphore* dataShouldStart, Semaphore* dataHasEnded, bool* shouldStop, bool* shouldSendData)
    :activeLed(activeLed), usbEventQueue(usbEventQueue), outputBuffer(outputBuffer), dataShouldStart(dataShouldStart), dataHasEnded(dataHasEnded), shouldStop(shouldStop), shouldSendData(shouldSendData)
{
    priority = 13;
    stackSize = 1024;
    taskName = "UsbTask";
}

#define CONTROL_BUFFER_SIZE 512

static uint8_t cmdBufferArray[CONTROL_BUFFER_SIZE];
tUSBBuffer UsbTask::cmdBuffer =
{
    false, //USB->APP
    rxHandlerCmdEP,
    0,
    readCmdEP,
    availableCmdEP,
    (void *)&g_sTorqueDeviceInfo,
    cmdBufferArray,
    CONTROL_BUFFER_SIZE
};

static uint8_t rspBufferArray[CONTROL_BUFFER_SIZE];
tUSBBuffer UsbTask::rspBuffer =
{
    true,//APP->USB
    txHandlerRspEP,
    0,
    writeRspEP,
    availableRspEP,
    0,
    rspBufferArray,
    CONTROL_BUFFER_SIZE
};
#define DAT_BUFFER_SIZE 32

void UsbTask::Setup()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    USBStackModeSet(0, eUSBModeDevice, 0);

    InternalUSBTickInit();
    InternalUSBRegisterTickHandler(HandleUSBTick, (void*)this);

    IntPrioritySet(INT_USB0, (5 << 5) );

    USBDCDInit(0, &g_sTorqueDeviceInfo, (void*)this);

    cmdBuffer.pvCBData = (void*)this;
    rspBuffer.pvCBData = (void*)this;

    cmdBuffer.pvHandle = (void*)this;
    rspBuffer.pvHandle = (void*)this;

    USBBufferInit(&cmdBuffer);
    USBBufferInit(&rspBuffer);

}


void UsbTask::HandleReset(void *pvUsbTask)//first connect or reconnect
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
    usbTask->usbEventQueue->EnqueueFromISR(UsbDataEvent::Reset, false);
}


void UsbTask::HandleEndpoints(void *pvUsbTask, uint32_t ui32Status)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;

    if(ui32Status & USB_EP_DESC_IN | USBEPToIndex(USB_EP_1))
    {
        usbTask->processRspEP(ui32Status);
    }
    if(ui32Status & USB_EP_DESC_OUT | USBEPToIndex(USB_EP_2))
    {
        usbTask->processCmdEP(ui32Status);
    }
    if(ui32Status & USB_EP_DESC_IN | USBEPToIndex(USB_EP_3))
    {
        uint32_t EPStatus = USBEndpointStatus(USB0_BASE, USB_EP_3);
        USBDevEndpointStatusClear(USB0_BASE, USB_EP_3, EPStatus);
        usbTask->datActive = false;
        usbTask->usbEventQueue->EnqueueFromISR(UsbDataEvent::DatReady, false);
    }
}


void UsbTask::HandleUSBTick(void *pvUsbTask, uint32_t ui32TimemS)
{
    UsbTask* usbTask = (UsbTask*)pvUsbTask;
    if (usbTask->defferedRead)
    {
        uint32_t availSize = USBEndpointDataAvail(USB0_BASE, USB_EP_2);
        USBBufferEventCallback(&cmdBuffer, USB_EVENT_RX_AVAILABLE, availSize, (void*)0);
    }
}

void UsbTask::processCmdEP(uint32_t ui32Status)
{
    uint32_t EPStatus = USBEndpointStatus(USB0_BASE, USB_EP_2);
    USBDevEndpointStatusClear(USB0_BASE, USB_EP_2, EPStatus);
    if(EPStatus & USB_DEV_RX_PKT_RDY)
    {
        defferedRead = true;
        uint32_t availSize = USBEndpointDataAvail(USB0_BASE, USB_EP_2);
        USBBufferEventCallback(&cmdBuffer, USB_EVENT_RX_AVAILABLE, availSize, (void*)0);
    }
    else if(EPStatus & USB_RX_ERROR_FLAGS)
    {
        USBBufferEventCallback(&cmdBuffer, USB_EVENT_ERROR, (EPStatus & USB_RX_ERROR_FLAGS), (void*)0);
    }
}

void UsbTask::processRspEP(uint32_t ui32Status)
{
    uint32_t EPStatus = USBEndpointStatus(USB0_BASE, USB_EP_1);
    USBDevEndpointStatusClear(USB0_BASE, USB_EP_1, EPStatus);
    rspActive = false;
    USBBufferEventCallback(&rspBuffer, USB_EVENT_TX_COMPLETE, lastRspSize, (void*)0);
}


uint32_t UsbTask::availableCmdEP(void *pvHandle)
{
    //UsbTask* usbTask = (UsbTask*)pvHandle;
    uint32_t EPStatus = USBEndpointStatus(USB0_BASE, USB_EP_2);
    if(EPStatus & USB_DEV_RX_PKT_RDY)
    {
        uint32_t avail = USBEndpointDataAvail(USB0_BASE, USB_EP_2);
        return avail;
    }
    else
    {
        return 0;
    }
}

uint32_t UsbTask::availableRspEP(void *pvHandle)
{
    UsbTask* usbTask = (UsbTask*)pvHandle;
    if(usbTask->rspActive)
    {
        return 0;
    }
    else
    {
        return g_ui16MaxPacketSize;
    }

}


uint32_t UsbTask::readCmdEP(void *pvHandle, uint8_t *pi8Data, uint32_t ui32Length, bool bLast)
{
    UsbTask* usbTask = (UsbTask*)pvHandle;
    uint32_t EPStatus = USBEndpointStatus(USB0_BASE, USB_EP_2);
    if(EPStatus & USB_DEV_RX_PKT_RDY)
    {
        uint32_t avail = USBEndpointDataAvail(USB0_BASE, USB_EP_2);
        uint32_t count = ui32Length;
        int32_t status = USBEndpointDataGet(USB0_BASE, USB_EP_2, pi8Data, &count);
        if (count == avail)
        {
            USBDevEndpointStatusClear(USB0_BASE, USB_EP_2, EPStatus);
            USBDevEndpointDataAck(USB0_BASE, USB_EP_2, true);
            usbTask->defferedRead = false;
        }
        if (status != -1)
        {
            return count;
        }
    }

    return 0;
}

uint32_t UsbTask::writeRspEP(void *pvHandle, uint8_t *pi8Data, uint32_t ui32Length, bool bLast)
{
    UsbTask* usbTask = (UsbTask*)pvHandle;
    if(ui32Length > g_ui16MaxPacketSize || usbTask->rspActive) return 0;

    int32_t status = USBEndpointDataPut(USB0_BASE, USB_EP_1, pi8Data, ui32Length);

    if(status != -1)
    {
        usbTask->lastRspSize = ui32Length;
        if(bLast)
        {
            usbTask->rspActive = true;
            USBEndpointDataSend(USB0_BASE, USB_EP_1, USB_TRANS_IN);
        }
        return ui32Length;
    }
    else
    {
        return 0;
    }
}



static const char hw_typeCmd[] = "hw_type\r";
static const char hw_typeRsp[] = "hw_type 13\r\n";

static const char config_dumpCmd[] = "config_dump\r";
static const char config_dumpRsp[] =
"config_dump 45\r\n\
1 Test_1 0 -1 f 0x00000000\r\n\
2 Serial 0 16 s _big_fat_phnoy00\r\n\
3 SDADC_Timer_/us 0 10000 u 0\r\n\
4 Temp_ADC_Timer_/us 15 10000 u 625\r\n\
5 ADC1_3_Filter_Size 1 128 u 16\r\n\
6 SDADC_Filter_Size 1 10 u 2\r\n\
7 FG_analog_/Hz 20 10000 u 100\r\n\
8 DAC_Timer_/us 0 10000 u 100\r\n\
9 SIG_GAIN_mV/LSB16 -10 10 f 0x3E340A6C\r\n\
10 SIG_OFFSET_LSB16 -20000 20000 i 287\r\n\
11 Cal_Nm/mV -100 100 f 0x3BA3D70A\r\n\
12 Out_mV_at_0Nm 0 22000 u 5000\r\n\
13 Out_mV_at_Error 0 22000 u 10000\r\n\
14 DAC_GAIN_LSB14/mV -10 10 f 0x3FBF5AA7\r\n\
15 DAC_OFFSET_LSB14 -8000 8000 i 3\r\n\
16 DAC_Test_Mode_mV 0 22000 u 0\r\n\
17 DAC_Test_Mode_LSB14 0 16383 u 0\r\n\
18 DAC_5V_Output 0 1 u 0\r\n\
19 DAC_4-20mA_Output 0 1 u 0\r\n\
20 Temp_Int_Min -1000 1000 i 100\r\n\
21 Temp_Int_Max -1000 1000 i 0\r\n\
22 Temp_Ext_Min -1000 1000 i 100\r\n\
23 Temp_Ext_Max -1000 1000 i 0\r\n\
24 Temp_Ext_Test -100 300 i 0\r\n\
25 Temp_Sens_Ext 0 3 u 3\r\n\
26 Lifetime_/Min 0 65535 u 0\r\n\
27 CAN_ID_(BASE) 1 65535 u 6394\r\n\
28 CAN_ID_(EXTENSION) 0 65535 u 32818\r\n\
29 CAN_ID_FILTER 0 29 u 0\r\n\
30 CAN_Baudrate_/kBps 10 1000 u 250\r\n\
31 CAN_Auto_Send_1ms 0 60000 u 100\r\n\
32 CAN_Auto_Send_CMD 0 1000 u 8\r\n\
33 CAN_notAck_RT 0 1 u 0\r\n\
34 Use_Onewire_on_EE_Pin 0 1 u 0\r\n\
35 RPM_Source 0 3 u 2\r\n\
36 Use_Angle_Sensor_on_TIM19 0 1 u 1\r\n\
37 Angle_Cnt 1 65535 u 360\r\n\
38 SIG_Out_Nm_mult -4 3 i -2\r\n\
39 Use_UART 0 1 u 0\r\n\
40 UART_Baudrate_/100Bps 1 60000 u 1152\r\n\
41 Temp_Comp_Faktor -2 2 f 0x00000000\r\n\
42 Temp_Comp_Offset 0 100 u 25\r\n\
43 USB_enable 0 1 u 1\r\n\
44 Filter_Type 0 4 u 3\r\n\
45 Exponential_Faktor 0 10000 u 1\r\n";

static const char hw_idCmd[] = "hw_id_get\r";
static const char hw_idRsp[] = "hw_id_get 0123456789\r\n";

static const char measure_startCmd[] = "measure_start 10000\r";
static const char measure_startRsp[] = "measure_start 10000\r\n";

static const char measure_stopCmd[] = "measure_stop\r";
static const char measure_stopRsp[] = "measure_stop\r\n";

static const char angle_readCmd[] = "angle_read\r";
static const char angle_readRsp[] = "angle_read 0 0\r\n";

uint32_t UsbTask::rxHandlerCmdEP(void* pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData)
{
    UsbTask* usbTask = (UsbTask*)pvCBData;
    usbTask->usbEventQueue->EnqueueFromISR(UsbDataEvent::CmdAvail, false);
//    uint8_t buffer[65];
//    uint32_t read = USBBufferRead(&cmdBuffer,buffer, 64);
//    USBBufferWrite(&rspBuffer, buffer, read+1);
//    buffer[read] = 0;
//    UARTprintf("%s\n",buffer);
    return 0;
}

uint32_t UsbTask::txHandlerRspEP(void* pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData)
{
    UsbTask* usbTask = (UsbTask*)pvCBData;
    usbTask->usbEventQueue->EnqueueFromISR(UsbDataEvent::RspAvail, false);
    return 0;
}


void UsbTask::TaskMethod()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t countOffset = 0;

    volatile UBaseType_t uxHighWaterMark;
    while(1)
    {

        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        UsbDataEvent event = UsbDataEvent::NoEvent;
        usbEventQueue->Dequeue(&event, true);

        switch(event)
        {
        case UsbDataEvent::CmdAvail:
        {
            //assuming all commands fit in 1 packet
            char cmd[65];
            uint32_t read = USBBufferRead(&cmdBuffer,(uint8_t*)cmd, 64);
            if (read > 0)
            {
                if (strncmp(cmd, hw_typeCmd, sizeof(hw_typeCmd)-1) == 0)
                {
                    startRsp(hw_typeRsp, sizeof(hw_typeRsp)-1);
                }
                else if (strncmp(cmd, config_dumpCmd, sizeof(config_dumpCmd)-1) == 0)
                {
                    startRsp(config_dumpRsp, sizeof(config_dumpRsp)-1);
                }
                else if (strncmp(cmd, hw_idCmd, sizeof(hw_idCmd)-1) == 0)
                {
                    startRsp(hw_idRsp, sizeof(hw_idRsp)-1);
                }
                else if (strncmp(cmd, measure_startCmd, sizeof(measure_startCmd)-1) == 0)
                {
//                    logging = true;
                    DataFrame firstFrame;
                    DataFrame secondFrame;
                    while(outputBuffer->Dequeue(&firstFrame, false));//remove old frames
                    outputBuffer->Dequeue(&firstFrame, true);//wait for first 2 new frames, and put back into buffer for remaining code
                    outputBuffer->Dequeue(&secondFrame, true);//wait for first 2 new frames, and put back into buffer for remaining code
                    outputBuffer->Enqueue(firstFrame, false);
                    outputBuffer->Enqueue(secondFrame, false);
                    countOffset = firstFrame.count;

                    startRsp(measure_startRsp, sizeof(measure_startRsp)-1);
                }
                else if (strncmp(cmd, measure_stopCmd, sizeof(measure_stopCmd)-1) == 0)
                {
//                    logging = false;
                    startRsp(measure_stopRsp, sizeof(measure_stopRsp)-1);
                }
                else if (strncmp(cmd, angle_readCmd, sizeof(angle_readCmd)-1) == 0)
                {
                    startRsp(angle_readRsp, sizeof(angle_readRsp)-1);
                }
            }
        }
            break;
        case UsbDataEvent::RspAvail:
            if(rspRemaining && rspResume)
            {
                uint32_t written = 0;
                do
                {
                    written = USBBufferWrite(&rspBuffer, (uint8_t*)rspResume, rspRemaining);
                    rspRemaining -= written;
                    if(rspRemaining == 0)
                    {
                        rspResume = nullptr;
                    }
                    else
                    {
                        rspResume += written;
                    }
                } while (written > 0 && rspResume && rspRemaining);
            }
            break;
//        case UsbDataEvent::DatAvail:
//            //remove block from DataAvail events
//            if(*shouldSendData){
//                *shouldSendData = false;
//            }
//            //intended fallthrough
        case UsbDataEvent::DatReady:
        {
            if(!datActive)
            {
                uint8_t packetData[64];
                //can put up to 4 frames in 64 byte packet, send as many as possible
                //EDIT: Actually changing to only put max 1 in
                int i= 0;
                for(;i < 1;i++)
                {
                    DataFrame firstFrame;
                    bool success = outputBuffer->Dequeue(&firstFrame, true);
                    firstFrame.count -= countOffset;
                    *((DataFrame*)((&packetData)+(i*16))) = firstFrame;
                    if (!success) break;
                }
                uint32_t length = i*16;
                int32_t status = USBEndpointDataPut(USB0_BASE, USB_EP_3, packetData, length);
                if(status != -1)
                {
                    datActive = true;
                    USBEndpointDataSend(USB0_BASE, USB_EP_3, USB_TRANS_IN);
                }

            }
        }
            break;
        case UsbDataEvent::Reset:
            rspRemaining = 0;
            rspResume = nullptr;
            {
                bool success = false;
                do
                {
                    DataFrame frame;
                    success = outputBuffer->Dequeue(&frame, false);
                } while(success);
            }
//            if(logging)
//            {
//                *shouldStop = true;
//                dataHasEnded->Take();
//                *shouldStop = false;
//                logging = false;
//            }
            break;
        default:
            break;
        }
        //vTaskDelayUntil(&xLastWakeTime, 10);
    }
}

void UsbTask::startRsp(const char* toSend, size_t length)
{
    rspRemaining = length;
    rspResume = toSend;
    uint32_t written = 0;
    do
    {
        written = USBBufferWrite(&rspBuffer, (uint8_t*)rspResume, rspRemaining);
        rspRemaining -= written;
        if(rspRemaining == 0)
        {
            rspResume = nullptr;
        }
        else
        {
            rspResume += written;
        }
    } while (written > 0 && rspResume && rspRemaining);

}

