/*
 * RTOSMemWrap.cpp
 *
 *  Created on: 5/08/2019
 *      Author: jwi123
 */

#include "RTOSMemWrap.hpp"

void* operator new(size_t size)
{
    void *ptr = pvPortMalloc(size);
    return ptr;
}

void operator delete(void *ptr)
{
    vPortFree(ptr);
}

void* operator new[](size_t size)
{
    void *ptr = pvPortMalloc(size);
    return ptr;
}

void operator delete[](void *ptr)
{
    vPortFree(ptr);
}
