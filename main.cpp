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


int main(void)
{

	SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	                    SYSCTL_XTAL_16MHZ);



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
