/*
 * HeartbeatTask.hpp
 *
 *  Created on: 31 Aug 2020
 *      Author: Jack
 */

#ifndef TASKS_HEARTBEATTASK_HPP_
#define TASKS_HEARTBEATTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Wrappers/GPIO.hpp>

class HeartbeatTask: public BaseTask
{
    GPIO *blinkLed;
protected:
    void Setup();
    void TaskMethod();
public:
    HeartbeatTask(GPIO *blinkLed);
};

#endif /* TASKS_HEARTBEATTASK_HPP_ */
