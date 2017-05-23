/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

uint8_t nextSenseEdge = RISING;
uint8_t nextSensePhase = PHASE_B;

void setNextSensPhase(uint8_t phaseA, uint8_t phaseB, uint8_t phaseC){
	if(phaseA == 2 || phaseA == 5){
		nextSensePhase = PHASE_A;
		if(phaseA == 2) nextSenseEdge = RISING;
		else nextSenseEdge = FALLING;
	}else if(phaseB == 2 || phaseB == 5){
		nextSensePhase = PHASE_B;
		if(phaseB == 2) nextSenseEdge = RISING;
		else nextSenseEdge = FALLING;
	}else if(phaseC == 2 || phaseC == 5){
		nextSensePhase = PHASE_C;
		if(phaseC == 2) nextSenseEdge = RISING;
		else nextSenseEdge = FALLING;
	}
}