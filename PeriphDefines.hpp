/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
/*
 * GPIODefines.hpp
 *
 *  Created on: Aug 27, 2020
 *      Author: Jack
 */

#ifndef PERIPHDEFINES_HPP_
#define PERIPHDEFINES_HPP_

#include "Wrappers/GPIO.hpp"
#include "Wrappers/UART.hpp"
#include "Wrappers/QEI.hpp"

extern GPIO led1;
extern GPIO led2;
extern GPIO led3;

extern GPIO motor_pul;
extern GPIO motor_dir;
extern GPIO motor_ena;
extern GPIO motor_alm;
extern GPIO motor_pend;

extern QEI motor_enc;
extern QEI sensor_enc;

extern UART usbUart;

#endif /* PERIPHDEFINES_HPP_ */
