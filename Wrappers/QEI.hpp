/*
 * QEI.hpp
 *
 *  Created on: 7 Sep 2020
 *      Author: Jack
 */

#ifndef WRAPPERS_QEI_HPP_
#define WRAPPERS_QEI_HPP_
#include "stdint.h"
#include "driverlib/qei.h"

typedef enum QeiType_e {QEI0A, QEI0B, QEI1,        QEI0=QEI0A} QeiType;

class QEI
{
    uint32_t QeiTypeToPhAPin(QeiType QeiType);
    uint32_t QeiTypeToPhBPin(QeiType QeiType);
    uint32_t QeiTypeToGPIOPort(QeiType QeiType);
    uint32_t QeiTypeToGPIOPeriph(QeiType QeiType);
    uint32_t QeiTypeToBase(QeiType QeiType);
    uint32_t QeiTypeToPeriph(QeiType QeiType);
    QeiType qeiType;
    uint32_t ui32MaxPosition;
    void init();
    bool inited = false;
public:
    QEI(QeiType qeiType, uint32_t ui32MaxPosition = 0xFFFFFFFF);
    uint32_t getPos(void);
    uint32_t getVel(void);
    void setPos(uint32_t);
};

#endif /* WRAPPERS_QEI_HPP_ */
