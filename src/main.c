/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

uint8_t RotationDirection = 0;
static uint16_t BEMF_good_detects = 0;
static uint16_t Last_BEMF_input = 0;
uint8_t nextSenseEdge;
uint8_t nextSensePhase;
uint16_t inputFailsave = 250;
uint32_t InputBuf[32];
uint32_t newInput = 0;
uint32_t deg60Time = 0;

int main(){
	
	//init MCU
	//________________________________________________________
	SystemClock_Config();
	EnablePheriphClocks();
	
	//read config
	//________________________________________________________
	getConfiguration();

	//init peripherals
	//________________________________________________________
	TimeTimerInit();
	PWMTimerInit();
	setPWMcompares(0);
	
	ComparatorInit();
	
	SignalInputInit();
	
	#ifdef UART_TLM
	TLMUARTInit();
	#endif
	
	ADCInit();
	
	//init GPIO's
	//________________________________________________________
	#ifdef UART_TLM
	UART_GPIO_init;
	#endif
	Input_GPIO_init;
	Comparator_GPIO_init;
	ADC_GPIO_init;
	FETControl_GPIO_init;
	ALL_PHASES_LOW;
	
	//startup sound
	//________________________________________________________
	noise(0,1,0);
	noise(0,0,0);
	noise(0,0,1);
	delay16(0xFFFF);
	
	while(1){
		static uint8_t InputWasLow = 0;
		static uint8_t running = 0;
		static int16_t TelemetryData[5] = {0,0,0,0,0};
		uint16_t loopstart = micros16();
		
		
		//read input
		//________________________________________________________
		if(inputFailsave < 1000) inputFailsave++;
		
		uint16_t inputValue = computeDshot();
		if(inputFailsave == 1000){
			inputValue = 0;
			InputWasLow = 0;
			setPWMcompares(0);
		}
		
		//startup and throttle handling
		//_______________________________________________________
		if(inputValue > 47){
			if(BEMF_good_detects < 100 && inputValue > 256) inputValue=256;
			if(InputWasLow == 1){
				setPWMcompares((inputValue>>1)+2);
				running = 1;
			}else running = 0;
			if(Last_BEMF_input > 200){
				setComparatorInterruptStatus(1);
				BEMF_good_detects = 0;
				deg60Time = 0;
				Last_BEMF_input = 0;
				SwitchPhaseStep(RotationDirection);
			}else if(Last_BEMF_input < 250)Last_BEMF_input++;
		}else{
			running = 0;
			if(inputFailsave <= 1){
				if(InputWasLow == 0){
					noise(8,1,1);
					delay16(0x8FFF);
					noise(8,1,1);
					InputWasLow = 1;
				}
			}
			setComparatorInterruptStatus(0);
			ALL_PHASES_LOW;
		}
		
		
		//read ADC's
		//_______________________________________________________	
		getADCvalues(TelemetryData);
		
		
		
		//compute telemetry
		//_______________________________________________________
		#ifdef VOLTAGE_CHANNEL
			TelemetryData[1] = TelemetryData[1]*10/VOLTAGE_SCALE_DEVIDER;
		#endif
		
		#ifdef CURRENT_CHANNEL
			static int16_t CalibCurrent = 0;
			static int16_t FilterCurrent = 0;
			if(running == 0){
				CalibCurrent = (CalibCurrent+(TelemetryData[2]<<1))/2;
			}
			TelemetryData[2] = ((TelemetryData[2]<<1)-CalibCurrent)*100/CURRENT_SCALE_DEVIDER;
			FilterCurrent = ((FilterCurrent<<3)-FilterCurrent+TelemetryData[2])>>3;
			TelemetryData[2] = FilterCurrent;
		
		#endif
		
		#ifdef UART_TLM
		if(telemetryRequest == 1){
			//calc rpm
			//_______________________________________________________
			TelemetryData[4] = 800000/deg60Time;
			if(running == 0)TelemetryData[4] = 0;
			
			//send telemetry
			//_______________________________________________________
			sendTelemetry(TelemetryData);
			telemetryRequest = 0;
		}
		#endif
		
		//looptime debug
		//TelemetryData[3] = (TelemetryData[3]*15+((uint16_t)((uint16_t)micros16()-(uint16_t)loopstart)))>>4;
		
		//complete loop
		//_______________________________________________________
		while((uint16_t)((uint16_t)micros16()-(uint16_t)loopstart) < 125); // 8k loop
	}
}


void ComparatorISR(uint16_t edgeTime){
	static uint32_t timingTime = 0;
	static uint32_t blankingTime = 0;
	static uint16_t lastInterruptTime = 0;
	uint32_t deg60intervalTime = (uint16_t)((uint16_t)edgeTime-(uint16_t)lastInterruptTime);
	
	//BEMF filters
	//_______________________________________________________
	if(getComparatorOut() != nextSenseEdge) return;
	if(deg60Time > 320){
		if(BEMF_good_detects < 100) delay16(10);
		else delay16(3);
		if(getComparatorOut() != nextSenseEdge) return;
	}
	
	Last_BEMF_input = 0;
	setComparatorInterruptStatus(0);
	
	//swich the phases
	//_______________________________________________________
	if(BEMF_good_detects > 15){
		lastInterruptTime = edgeTime;
		
		while((uint16_t)((uint16_t)getTimeSource() - (uint16_t)lastInterruptTime) < timingTime);
		
		SwitchPhaseStep(RotationDirection);
		
		while((uint16_t)((uint16_t)getTimeSource() - (uint16_t)lastInterruptTime) < blankingTime);
	}
	
	//calculate timing and blanking for the next step
	//_______________________________________________________
	deg60Time = ((deg60Time<<4)-deg60Time+deg60intervalTime)>>4;
	timingTime = (uint32_t)(deg60Time*TimingFactor)>>10;
	blankingTime = timingTime+((uint32_t)(deg60Time*BlankingFactor)>>10); 
	
	setComparatorInterruptStatus(1);
	if(BEMF_good_detects < 1000)BEMF_good_detects++;
	
}













