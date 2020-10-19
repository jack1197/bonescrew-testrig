/*
 * Status.cpp
 *
 *  Created on: 14 Oct 2020
 *      Author: Jack
 */

#include <Model/Status.hpp>


void Status::setMotorEncoder(int value)
{
    motorEncoder = value;
}

void Status::setSensorEncoder(int value)
{
    sensorEncoder = value;
}

void Status::setSensorTorque(int value)
{
    sensorTorque = value;
}

void Status::setMotorSpeed(int value)
{
    motorSpeed = value;
}

int Status::getMotorEncoder()
{
    return motorEncoder;
}

int Status::getSensorEncoder()
{
    return sensorEncoder;
}

int Status::getSensorTorque()
{
    return sensorTorque;
}

int Status::getMotorSpeed()
{
    return motorSpeed;
}
