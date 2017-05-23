
#define Limit( amt , max) ((amt) < (0) ? (max+amt) : ((amt) >= (max) ? (amt-max) : (amt)))

enum{
	PHASE_LOW = 0,
	PHASE_HIGH = 1,
	PHASE_FLOATING = 2
};
enum{
	PHASE_A = 0,
	PHASE_B = 1,
	PHASE_C = 2
};

void SwitchPhaseStep(uint8_t rotationInverted);

extern uint8_t sixBlockCommutationSteps[6];