/*
 * Settings.hpp
 *
 *  Created on: 8 Oct 2020
 *      Author: Jack
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

class Settings
{
    bool enableMotor = false; //enable motor driver
    int insertDist = 15*6400;
    int reverseDist = 17*6400;
    int jogDist = 3200;
    int fullInsertionPause = 1000;
    bool enableTrapezoid = true;
    //for no trapezoid
    int insertSpeed = 3200;//CPS
    //for trapezoid
    int rampTime = 150;//ms to reach full speed
    int highDwellTime = 200;//ms at full speed
    int lowDwellTime = 250;//ms stopped
    int distPerTrapezoid = 3200;//distance of turning for each trapezoid
public:
    void setEnableMotor(bool value);
    void setInsertDist(int value);
    void setReverseDist(int value);
    void setJogDist(int value);
    void setFullInsertionPause(int value);
    void setTrapezoidEnabled(bool value);
    void setInsertSpeed(int value);
    void setRampTime(int value);
    void setHighDwellTime(int value);
    void setLowDwellTime(int value);
    void setDistPerTrapezoid(int value);

    bool getEnableMotor();
    int getInsertDist();
    int getReverseDist();
    int getJogDist();
    int getFullInsertionPause();
    bool getTrapezoidEnabled();
    int getInsertSpeed();
    int getRampTime();
    int getHighDwellTime();
    int getLowDwellTime();
    int getDistPerTrapezoid();
};

#endif /* SETTINGS_HPP_ */
