/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"



void SwitchFets(uint8_t phaseA, uint8_t phaseB, uint8_t phaseC){
	static uint8_t lastPhaseStats[3] = {10,10,10};
	if(phaseA == 0 && lastPhaseStats[0] != 0){
		PHASE_A_LOW;
	}else if(phaseA == 1 && lastPhaseStats[0] != 1){
		PHASE_A_PWM;
	}else if(phaseA == 2 && lastPhaseStats[0] != 2){
		PHASE_A_FLOATING;
	}
	lastPhaseStats[0]  = phaseA;
		
	if(phaseB == 0 && lastPhaseStats[1] != 0){
		PHASE_B_LOW;
	}else if(phaseB == 1 && lastPhaseStats[1] != 1){
		PHASE_B_PWM;
	}else if(phaseB == 2 && lastPhaseStats[1] != 2){
		PHASE_B_FLOATING;
	}
	lastPhaseStats[1]  = phaseB;
	
	if(phaseC == 0 && lastPhaseStats[2] != 0){
		PHASE_C_LOW;
	}else if(phaseC == 1 && lastPhaseStats[2] != 1){
		PHASE_C_PWM;
	}else if(phaseC == 2 && lastPhaseStats[2] != 2){
		PHASE_C_FLOATING;
	}
	lastPhaseStats[2]  = phaseC;
}