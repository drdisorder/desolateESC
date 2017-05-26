#include "config.h"

uint8_t RotationDirection;

uint16_t TimingFactor = 0;
uint16_t BlankingFactor = 0;


void getConfiguration(){
	float calcTiming = 30.0f-DEFAULT_TIMING;
	float calcBlanking = DEFAULT_BLANKING;
	RotationDirection = DEFAULT_ROTATION_DIRECTION;
	
	TimingFactor = (uint16_t)((float)calcTiming*17.1f);
	BlankingFactor = (uint16_t)((float)calcBlanking*17.1f);
}