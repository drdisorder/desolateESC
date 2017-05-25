/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

uint8_t RotationDirection = 0;
static uint16_t BEMF_good_detects = 0;
static uint16_t Last_BEMF_input = 0;
uint8_t nextSenseEdge;
uint8_t nextSensePhase;
uint16_t inputFailsave = 0;
uint32_t InputBuf[32];
uint32_t newInput = 0;

int main(){
	SystemClock_Config();
	EnablePheriphClocks();
	
	FETControl_GPIO_init;
	ALL_PHASES_LOW;
	TimeTimerInit();
	PWMTimerInit();
	setPWMcompares(0);
	
	Comparator_GPIO_init;
	ComparatorInit();
	
	Input_GPIO_init;
	SignalInputInit();
	
	while(1){
		static uint8_t InputWasLow = 0;
		uint16_t loopstart = micros16();
		
		if(inputFailsave < 500) inputFailsave++;
		
		uint16_t inputValue = computeDshot();
		if(inputFailsave == 500) inputValue = 0;
		
		if(inputValue > 47){
			if(BEMF_good_detects < 100 && inputValue > 256) inputValue=256;
			if(InputWasLow == 1)setPWMcompares(inputValue>>1);
			if(Last_BEMF_input > 200){
				setComparatorInterruptStatus(1);
				BEMF_good_detects = 0;
				Last_BEMF_input = 0;
				SwitchPhaseStep(RotationDirection);
			}else if(Last_BEMF_input < 250)Last_BEMF_input++;
		}else{
			InputWasLow = 1;
			setComparatorInterruptStatus(0);
			ALL_PHASES_LOW;
		}
		
		
		while((uint16_t)((uint16_t)micros16()-(uint16_t)loopstart) < 125); // 8k loop
	}
}


void ComparatorISR(uint16_t edgeTime){
	static uint16_t timingTime = 0;
	static uint16_t blankingTime = 0;
	static uint32_t deg60Time = 0;
	static uint16_t lastInterruptTime = 0;
	uint32_t deg60intervalTime = (uint16_t)((uint16_t)edgeTime-(uint16_t)lastInterruptTime);
	
	if(getComparatorOut() != nextSenseEdge) return;
	if(BEMF_good_detects < 100) delay16(10);
	else delay16(3);
		
	if(getComparatorOut() != nextSenseEdge) return;
	
	
	Last_BEMF_input = 0;
	setComparatorInterruptStatus(0);
	if(BEMF_good_detects > 15){
		lastInterruptTime = edgeTime;
		
		while((uint16_t)((uint16_t)getTimeSource() - (uint16_t)lastInterruptTime) < timingTime);
		
		SwitchPhaseStep(RotationDirection);
		
		while((uint16_t)((uint16_t)getTimeSource() - (uint16_t)lastInterruptTime) < blankingTime);
	}
	deg60Time = ((deg60Time<<4)-deg60Time+deg60intervalTime)>>4;
	timingTime = deg60Time>>2; //15deg
	blankingTime = timingTime+(deg60Time>>3); //7.5deg
	
	setComparatorInterruptStatus(1);
	if(BEMF_good_detects < 1000)BEMF_good_detects++;
	
}













