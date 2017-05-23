
enum{
	FALLING = 0,
	RISING = 1
};

void setNextSensPhase(uint8_t phaseA, uint8_t phaseB, uint8_t phaseC);

extern uint8_t nextSenseEdge;
extern uint8_t nextSensePhase;