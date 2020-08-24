/*
 * RTOSMemWrap.hpp
 *
 *  Created on: 5/08/2019
 *      Author: jwi123
 */

#ifndef RTOSMEMWRAP_HPP_
#define RTOSMEMWRAP_HPP_

#include "FreeRTOS.h"

void* operator new(size_t size);

void operator delete(void *ptr);

#endif /* RTOSMEMWRAP_HPP_ */
