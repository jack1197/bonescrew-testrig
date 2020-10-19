/*
 * MotionTask.hpp
 *
 *  Created on: 16 Oct 2020
 *      Author: Jack
 */

#ifndef TASKS_MOTIONTASK_HPP_
#define TASKS_MOTIONTASK_HPP_

#include <Tasks/BaseTask.hpp>
#include <Wrappers/Queue.hpp>
#include "Model/MotionSegment.hpp"
#include "Model/Enums.hpp"
#include "Model/Settings.hpp"

class MotionTask: public BaseTask
{
    Queue<MotionSegment>* motionQueue;
    Queue<ControlEvent>* eventQueue;
    Settings* settings;
    bool active = false;
    typedef enum States_e {Idle, MotionActive, MotionPause} State;
    State currState = Idle;
    bool motionIn = true;
    int totalDist = 0;
    void delayWhileActivating();
    bool makeLimitedMotion(int dist, int start, int end);
    void onMotionEndstop();
protected:
    void Setup();
    void TaskMethod();
public:
    MotionTask(Queue<MotionSegment>* motionQueue, Queue<ControlEvent>* eventQueue, Settings* settings);
};

#endif /* TASKS_MOTIONTASK_HPP_ */
