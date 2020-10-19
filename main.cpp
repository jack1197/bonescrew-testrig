/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/**
 * main.cpp
 * Author: Jack Wilkie
 * Date Created: 24.8.2020
 */

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

extern "C"
{
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
}


#include "PeriphDefines.hpp"

//#include "Tasks/UsbTask.hpp"
#include "Tasks/HeartbeatTask.hpp"
#include "Tasks/DatalogTask.hpp"
#include "Tasks/MotorTask.hpp"
#include "Tasks/UartStatusTask.hpp"
#include "Tasks/UartControlTask.hpp"
#include "Tasks/MotionTask.hpp"
#include "Model/MotionSegment.hpp"
#include "Model/Status.hpp"
#include "Model/Settings.hpp"





extern "C"
{

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdbulk.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
}

int main(void)
{

	SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	                    SYSCTL_XTAL_16MHZ);


    static Queue<UsbDataEvent> usbEventQueue = Queue<UsbDataEvent>(16);
    static Queue<DataFrame> outputBuffer(16);
    static Queue<ControlEvent> controlQueue = Queue<ControlEvent>(16);
    static Queue<MotionSegment> motionQueue(2);
    static Semaphore dataShouldStart(1,0);
    static Semaphore dataHasEnded(1,0);
    static Settings settings;
    static Status status;
    static bool shouldStop = false;
    static bool shouldSendData = false;

    //static UsbTask usbTask(&led1, &usbEventQueue, &outputBuffer, &dataShouldStart, &dataHasEnded, &shouldStop, &shouldSendData);
    //usbTask.Initialize();
    static HeartbeatTask heartbeatTask(&led2);
    heartbeatTask.Initialize();
    static DatalogTask datalogTask(&outputBuffer, &dataShouldStart, &dataHasEnded, &shouldStop, &usbEventQueue, &shouldSendData, &status, &motor_enc, &sensor_enc);
    datalogTask.Initialize();
    static MotorTask motorTask(&motor_pul, &motor_dir, &motor_ena, &motor_alm, &motor_pend, &motionQueue, &status, &settings);
    motorTask.Initialize();
    static MotionTask motionTask(&motionQueue,&controlQueue,&settings );
    motionTask.Initialize();
    //static UartStatusTask uartStatusTask(&status, &usbUart);
    //uartStatusTask.Initialize();
    static UartControlTask uartControlTask(&usbUart, &controlQueue, &settings);
    uartControlTask.Initialize();



    vTaskStartScheduler();

    while(1);
}

extern "C"
{
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
    while(1);
}
}
