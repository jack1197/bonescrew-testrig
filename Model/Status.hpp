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
public:
    void setMotorEncoder(int);
    void setSensorEncoder(int);
    void setSensorTorque(int);
    void setMotorSpeed(int);
    int getMotorEncoder();
    int getSensorEncoder();
    int getSensorTorque();
    int getMotorSpeed();
};

#endif /* MODEL_STATUS_HPP_ */
