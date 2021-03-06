/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

uint8_t sixBlockCommutationSteps[6] = {
	PHASE_LOW,
	PHASE_LOW,
	PHASE_FLOATING,
	PHASE_HIGH,
	PHASE_HIGH,
	PHASE_FLOATING
};

void SwitchPhaseStep(uint8_t rotationInverted){
	static int8_t PhaseStats[3] = {0,2,4};
	if(rotationInverted == 0){
		PhaseStats[PHASE_A] = Limit(PhaseStats[PHASE_A]+1, 6);
		PhaseStats[PHASE_B] = Limit(PhaseStats[PHASE_B]+1, 6);
		PhaseStats[PHASE_C] = Limit(PhaseStats[PHASE_C]+1, 6);
	}else{
		PhaseStats[PHASE_A] = Limit(PhaseStats[PHASE_A]-1, 6);
		PhaseStats[PHASE_B] = Limit(PhaseStats[PHASE_B]-1, 6);
		PhaseStats[PHASE_C] = Limit(PhaseStats[PHASE_C]-1, 6);		
	}
	SwitchFets(sixBlockCommutationSteps[PhaseStats[PHASE_A]], sixBlockCommutationSteps[PhaseStats[PHASE_B]], sixBlockCommutationSteps[PhaseStats[PHASE_C]]);
	setNextSensPhase(PhaseStats[PHASE_A], PhaseStats[PHASE_B], PhaseStats[PHASE_C]);
}