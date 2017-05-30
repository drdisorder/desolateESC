/*This file is part of desolateESC CC BY-NC-SA*/
#pragma once

//get the MCU librarys and default functions
#ifdef STM32F051
	#include "STM32F051_includes.h"
#endif
//TARGETs
#include "TARGET.h"

#include "config.h"

#include "drive_fets.h"
#include "BEMF_sense.h"
#include "rotate_motor.h"
#include "compute_dshot.h"
#include "sounds.h"
#include "telemetry.h"

extern uint16_t inputFailsave;
extern uint8_t RotationDirection;
extern uint32_t InputBuf[32];
extern uint32_t newInput;
extern uint32_t deg60Time;
void ComparatorISR(uint16_t edgeTime);