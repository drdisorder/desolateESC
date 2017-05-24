#include "main.h"
#include "STM32f0xx_peripherals.h"


/* clocks */
/*_______________________________________________________________________________________________*/

void EnablePheriphClocks(){
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1 | LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2 | LL_APB1_GRP1_PERIPH_USART2 | LL_APB1_GRP1_PERIPH_TIM3 | LL_APB1_GRP1_PERIPH_TIM6);
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1 | LL_APB1_GRP2_PERIPH_SYSCFG | LL_APB1_GRP2_PERIPH_USART1 | LL_APB1_GRP2_PERIPH_TIM1 | LL_APB1_GRP2_PERIPH_TIM16 | LL_APB1_GRP2_PERIPH_TIM17);
}

/* Time */
/*_______________________________________________________________________________________________*/

void TimeTimerInit(){
	LL_TIM_InitTypeDef  TIMEtimerinit;
	TIMEtimerinit.Prescaler = 47;
	TIMEtimerinit.Autoreload = 0xFFFF;
	TIMEtimerinit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIMEtimerinit.RepetitionCounter = 0;
	TIMEtimerinit.CounterMode = LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(TIM6, &TIMEtimerinit);
	LL_TIM_EnableCounter(TIM6);
}

uint16_t micros16(){
	return TIM6->CNT;
}

void delay16(uint16_t delay){
	uint16_t startTime = TIM6->CNT;
	while((uint16_t)((uint16_t)TIM6->CNT - (uint16_t)startTime) < delay);
}

/* PWM out */
/*_______________________________________________________________________________________________*/

void PWMTimerInit(){
	LL_TIM_InitTypeDef  PWMtimerinit;
	PWMtimerinit.Prescaler = 0;
	PWMtimerinit.Autoreload = 1000;
	PWMtimerinit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	PWMtimerinit.RepetitionCounter = 0;
	PWMtimerinit.CounterMode = LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(TIM1, &PWMtimerinit);

	LL_TIM_OC_InitTypeDef  PWMchannelinit;
	PWMchannelinit.OCMode           = LL_TIM_OCMODE_PWM1;
	PWMchannelinit.OCState           = LL_TIM_OCSTATE_ENABLE;
	PWMchannelinit.OCNState         = LL_TIM_OCSTATE_ENABLE;
	PWMchannelinit.CompareValue  = 0;
	PWMchannelinit.OCPolarity        = PWM_OUTPUT_POLARITY;
	PWMchannelinit.OCIdleState      = PWM_OUTPUT_IDLESTATE;
	PWMchannelinit.OCNPolarity       = PWM_OUTPUT_N_POLARITY;
	PWMchannelinit.OCNIdleState    = PWM_OUTPUT_N_IDLESTATE;
	
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &PWMchannelinit);
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &PWMchannelinit);
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH3, &PWMchannelinit);
		
	LL_TIM_OC_SetDeadTime(TIM1, PWM_DEADTIME);
	LL_TIM_EnableCounter(TIM1);
	LL_TIM_EnableAllOutputs(TIM1);
}

void setPWMduty(uint32_t duty){
	TIM1->CCR1 = duty;
	TIM1->CCR2 = duty;
	TIM1->CCR3 = duty;	
}

/* BEMF Comparator */
/*_______________________________________________________________________________________________*/

void ComparatorInit(){
	
	LL_COMP_InitTypeDef  Comperatorinit;
	Comperatorinit.PowerMode            = LL_COMP_POWERMODE_HIGHSPEED;
	Comperatorinit.InputPlus              = BEMF_STAR_CMP_IN;
	Comperatorinit.InputMinus           = LL_COMP_INPUT_MINUS_VREFINT;
	Comperatorinit.InputHysteresis      = LL_COMP_HYSTERESIS_NONE;
	Comperatorinit.OutputSelection      = LL_COMP_OUTPUT_TIM3_IC1;
	Comperatorinit.OutputPolarity       = LL_COMP_OUTPUTPOL_NONINVERTED;
	LL_COMP_Init(BEMF_Comparator, &Comperatorinit);
	LL_COMP_Enable(BEMF_Comparator);
	
	LL_TIM_InitTypeDef  BEMFtimerinit;
	BEMFtimerinit.Prescaler = 5; // 8M
	BEMFtimerinit.Autoreload = 0xFFFF;
	BEMFtimerinit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	BEMFtimerinit.RepetitionCounter = 0;
	BEMFtimerinit.CounterMode = LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(TIM3, &BEMFtimerinit);
	LL_TIM_EnableCounter(TIM3);
	
	LL_TIM_IC_InitTypeDef BEMFinputConfig; 
	BEMFinputConfig.ICPolarity    = LL_TIM_IC_POLARITY_BOTHEDGE;
	BEMFinputConfig.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	BEMFinputConfig.ICPrescaler   = LL_TIM_ICPSC_DIV1;
	BEMFinputConfig.ICFilter      = 0x00;
	LL_TIM_IC_Init(TIM3, LL_TIM_CHANNEL_CH1, &BEMFinputConfig);
	
	LL_TIM_EnableIT_CC1(TIM3);
	NVIC_SetPriority(TIM3_IRQn, 1);
}

void setComparatorInput(uint8_t phase){
	if(phase == 0){
		LL_COMP_SetInputMinus(BEMF_Comparator, BEMF_A_CMP_IN);
	}else if(phase == 1){
		LL_COMP_SetInputMinus(BEMF_Comparator, BEMF_B_CMP_IN);
	}else if(phase == 2){
		LL_COMP_SetInputMinus(BEMF_Comparator, BEMF_C_CMP_IN);
	}
	TIM3->SR = (uint16_t)~LL_TIM_SR_CC1IF;
}

void setComparatorEdge(uint8_t edge){
	if(!edge) LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
	else LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);	
	TIM3->SR = (uint16_t)~LL_TIM_SR_CC1IF;
}

uint8_t getComparatorOut(){
	return (!(LL_COMP_ReadOutputLevel(BEMF_Comparator)));
}

void setComparatorInterruptStatus(uint8_t Enable){
	if(Enable == 1){
		TIM3->SR = (uint16_t)~LL_TIM_SR_CC1IF;
		NVIC_EnableIRQ(TIM3_IRQn);
	}else{
		NVIC_DisableIRQ(TIM3_IRQn);
	}
}

uint16_t getTimeSource(){
	return TIM3->CNT;
}

void TIM3_IRQHandler(void){
	if (TIM3->SR & LL_TIM_SR_CC1IF){
		TIM3->SR = (uint16_t)~LL_TIM_SR_CC1IF;
		ComparatorISR(TIM3->CCR1);
	}
}



























