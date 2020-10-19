/*
 * MotorTask.hpp
 *
 *  Created on: 9 Oct 2020
 *      Author: Jack
 */

#ifndef TASKS_MOTORTASK_HPP_
#define TASKS_MOTORTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Wrappers/GPIO.hpp>
#include <Wrappers/Queue.hpp>
#include <Model/MotionSegment.hpp>
#include <Model/Status.hpp>
#include <Model/Settings.hpp>

class MotorTask: public BaseTask
{
    GPIO* motor_pul;
    GPIO* motor_dir;
    GPIO* motor_ena;
    GPIO* motor_alm;
    GPIO* motor_pend;
    Status* status;
    Queue<MotionSegment>* motionQueue;
    Settings* settings;
protected:
    void Setup();
    void TaskMethod();
public:
    MotorTask(GPIO* motor_pul, GPIO* motor_dir, GPIO* motor_ena, GPIO* motor_alm, GPIO* motor_pend, Queue<MotionSegment>* motionQueue, Status* status, Settings* settings);
};

#endif /* TASKS_MOTORTASK_HPP_ */
