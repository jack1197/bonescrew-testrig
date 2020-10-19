/*
 * Settings.cpp
 *
 *  Created on: 8 Oct 2020
 *      Author: Jack
 */

#include <Model/Settings.hpp>

void Settings::setInsertDist(int value)
{
    insertDist = value;
}

void Settings::setReverseDist(int value)
{
    reverseDist = value;
}

void Settings::setJogDist(int value)
{
    jogDist = value;
}

void Settings::setFullInsertionPause(int value)
{
    fullInsertionPause = value;
}

void Settings::setTrapezoidEnabled(bool value)
{
    enableTrapezoid = value;
}

void Settings::setInsertSpeed(int value)
{
    insertSpeed = value;
}

void Settings::setRampTime(int value)
{
    rampTime = value;
}

void Settings::setHighDwellTime(int value)
{
    highDwellTime = value;
}

void Settings::setLowDwellTime(int value)
{
    lowDwellTime = value;
}

void Settings::setDistPerTrapezoid(int value)
{
    distPerTrapezoid = value;
}


int Settings::getInsertDist()
{
    return insertDist;
}

int Settings::getReverseDist()
{
    return reverseDist;
}

int Settings::getJogDist()
{
    return jogDist;
}

int Settings::getFullInsertionPause()
{
    return fullInsertionPause;
}

bool Settings::getTrapezoidEnabled()
{
    return enableTrapezoid;
}

int Settings::getInsertSpeed()
{
    return insertSpeed;
}

int Settings::getRampTime()
{
    return rampTime;
}

int Settings::getHighDwellTime()
{
    return highDwellTime;
}

int Settings::getLowDwellTime()
{
    return lowDwellTime;
}

int Settings::getDistPerTrapezoid()
{
    return distPerTrapezoid;
}



void Settings::setEnableMotor(bool value)
{
    enableMotor = value;
}

bool Settings::getEnableMotor()
{
    return enableMotor;
}
