/*
 * Status.cpp
 *
 *  Created on: 14 Oct 2020
 *      Author: Jack
 */

#include <Model/Status.hpp>
#include <cstring>


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

const char * Status::getInputString()
{
    return inputString;
}

const char * Status::getOutputString()
{
    return outputString;
}

void Status::setInputString(const char * string)
{
    inputString[49] = '\0';
    strncpy(inputString, string, 49);
}

void Status::setOutputString(const char * string)
{
    inputString[49] = '\0';
    strncpy(outputString, string, 49);
}

