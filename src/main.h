/*This file is part of desolateESC CC BY-NC-SA*/
#pragma once

//get the MCU librarys and default functions
#ifdef STM32F051
	#include "STM32F051_includes.h"
#endif
//get the TARGETs stuff
#include "TARGET.h"


#include "drive_fets.h"
#include "BEMF_sense.h"
#include "rotate_motor.h"


void ComparatorISR(uint16_t edgeTime);