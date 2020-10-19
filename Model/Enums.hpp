/*
 * QueueableEvent.hpp
 *
 *  Created on: 2 Sep 2020
 *      Author: Jack
 */

#ifndef MODEL_ENUMS_HPP_
#define MODEL_ENUMS_HPP_


enum class UsbDataEvent {NoEvent, CmdAvail, RspAvail, DatReady, DatAvail, Reset};
enum class ControlEvent {NoEvent, StartMotion, PauseMotion, ResetMotion, JogCW, JogCCW};


#endif /* MODEL_ENUMS_HPP_ */
