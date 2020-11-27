/*
 * Status.hpp
 *
 *  Created on: 14 Oct 2020
 *      Author: Jack
 */

#ifndef MODEL_STATUS_HPP_
#define MODEL_STATUS_HPP_

class Status
{
    int motorEncoder = 1;
    int sensorEncoder = 2;
    int sensorTorque = 3;
    int motorSpeed = 4;
    char inputString[50] = "";
    char outputString[50] = "";
public:
    void setMotorEncoder(int);
    void setSensorEncoder(int);
    void setSensorTorque(int);
    void setMotorSpeed(int);
    void setInputString(const char *);
    void setOutputString(const char *);
    int getMotorEncoder();
    int getSensorEncoder();
    int getSensorTorque();
    int getMotorSpeed();
    const char * getInputString();
    const char * getOutputString();
};

#endif /* MODEL_STATUS_HPP_ */
