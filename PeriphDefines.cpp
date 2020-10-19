/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/*
 * GPIODefines.cpp
 *
 *  Created on: Aug 27, 2020
 *      Author: Jack
 */

#include "PeriphDefines.hpp"

GPIO led1(GPIO_PORTF, 1, true);
GPIO led2(GPIO_PORTF, 2, true);
GPIO led3(GPIO_PORTF, 3, true);

GPIO motor_pul(GPIO_PORTA, 2, true);
GPIO motor_dir(GPIO_PORTA, 3, true, false);
GPIO motor_ena(GPIO_PORTA, 4, true, false);
GPIO motor_alm(GPIO_PORTB, 6);
GPIO motor_pend(GPIO_PORTB, 7);

QEI motor_enc(QeiType::QEI0B);
QEI sensor_enc(QeiType::QEI1);

UART usbUart(UartType::UART0, 115200);
