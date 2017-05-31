#include "main.h"
#include "string.h"
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

/* Signal input */
/*_______________________________________________________________________________________________*/

__IO uint32_t inputDMAbuffer[32];
uint32_t InputBuf[32];
uint32_t newInput;

void SignalInputInit(){
	LL_TIM_InitTypeDef  InputTimerinit;
	InputTimerinit.Prescaler = 0;
	#ifdef INPUT_TIM_32BIT
		InputTimerinit.Autoreload = 0xFFFFFFFF;
	#else
		InputTimerinit.Autoreload = 0xFFFF;
	#endif
	InputTimerinit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	InputTimerinit.RepetitionCounter = 0;
	InputTimerinit.CounterMode = LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(INPUT_TIM, &InputTimerinit);
	LL_TIM_EnableCounter(INPUT_TIM);
	
	LL_TIM_IC_InitTypeDef InputConfig; 
	InputConfig.ICPolarity    = LL_TIM_IC_POLARITY_BOTHEDGE;
	InputConfig.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	InputConfig.ICPrescaler   = LL_TIM_ICPSC_DIV1;
	InputConfig.ICFilter      = 0x04;
	LL_TIM_IC_Init(INPUT_TIM, INPUT_TIM_CHAN, &InputConfig);
	
	LL_DMA_DeInit(DMA1, INPUT_DMA_Channel);
	LL_DMA_InitTypeDef DMA_InitStructure; 
	DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&INPUT_DMA_SOURCE; 
	DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)inputDMAbuffer; 
	DMA_InitStructure.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStructure.Mode                   = LL_DMA_MODE_CIRCULAR;
	DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
	#ifdef INPUT_TIM_32BIT
		DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
	#else
		DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	#endif
	DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
	DMA_InitStructure.NbData                 = 32;
	DMA_InitStructure.Priority               = LL_DMA_PRIORITY_VERYHIGH;
	LL_DMA_Init(DMA1, INPUT_DMA_Channel, &DMA_InitStructure);
	
	
	NVIC_SetPriority(INPUT_DMA_IRQn, 2);
	NVIC_EnableIRQ(INPUT_DMA_IRQn);
	
	LL_DMA_EnableIT_TC(DMA1, INPUT_DMA_Channel);

	INPUT_DMA_ENABLE_REQ;
	LL_DMA_EnableChannel(DMA1, INPUT_DMA_Channel);
}

void INPUT_DMA_IRQHandler(void){
	if((DMA1->ISR & INPUT_DMA_TC_ChannelFlag) != (uint32_t)RESET){
		DMA1->IFCR = INPUT_DMA_TC_ChannelFlag;
		if(((inputDMAbuffer[2]-inputDMAbuffer[0])*17) > inputDMAbuffer[31]-inputDMAbuffer[0]){
			memcpy(InputBuf, (uint32_t *)inputDMAbuffer, 128);
			//telemetry request should not get missed by the loop
			if(inputDMAbuffer[23]-inputDMAbuffer[22] > ((inputDMAbuffer[2]-inputDMAbuffer[0])>>1)) telemetryRequest = 1;
			newInput = 1;
		}else{
			LL_DMA_DisableChannel(DMA1, INPUT_DMA_Channel);
			LL_DMA_SetDataLength(DMA1, INPUT_DMA_Channel, 32);
			DMA1->IFCR = INPUT_DMA_TC_ChannelFlag;
			LL_DMA_EnableChannel(DMA1, INPUT_DMA_Channel);
		}
	}
}

/* PWM out */
/*_______________________________________________________________________________________________*/

void PWMTimerInit(){
	LL_TIM_InitTypeDef  PWMtimerinit;
	PWMtimerinit.Prescaler = 0;
	PWMtimerinit.Autoreload = 1024;
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
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH4, &PWMchannelinit);
		
	LL_TIM_OC_SetDeadTime(TIM1, PWM_DEADTIME);
	LL_TIM_EnableCounter(TIM1);
	LL_TIM_EnableAllOutputs(TIM1);
}

void setPWMcompares(uint32_t compare){
	TIM1->CCR1 = compare;
	TIM1->CCR2 = compare;
	TIM1->CCR3 = compare;	
	TIM1->CCR4 = compare;	
}

/* BEMF Comparator */
/*_______________________________________________________________________________________________*/

void ComparatorInit(){
	
	LL_COMP_InitTypeDef  Comperatorinit;
	Comperatorinit.PowerMode            = LL_COMP_POWERMODE_MEDIUMSPEED;
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


/* UARTs */
/*_______________________________________________________________________________________________*/



#ifdef UART_TLM
void TLMUARTInit(){
	LL_USART_DeInit(UART_TLM);
	
	LL_USART_EnableHalfDuplex(UART_TLM);
	
	LL_USART_InitTypeDef UART_Init;
	UART_Init.BaudRate                 = 115200U;
	UART_Init.DataWidth                = LL_USART_DATAWIDTH_8B;
	UART_Init.StopBits                   = LL_USART_STOPBITS_1;
	UART_Init.Parity                       = LL_USART_PARITY_NONE ;
	UART_Init.TransferDirection       = LL_USART_DIRECTION_TX_RX;
	UART_Init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_Init.OverSampling           = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(UART_TLM, &UART_Init);
	
	LL_USART_Enable(UART_TLM);
	
	NVIC_SetPriority(UART_TLM_IRQn, 10);
	NVIC_EnableIRQ(UART_TLM_IRQn);
	
	LL_USART_EnableIT_TXE(UART_TLM);
}

static uint8_t tlmuartbuflen = 0;
static uint8_t tlmuartleftbytes = 0;
static uint8_t tlmuartTXbuf[64];

void UART_TLM_IRQHandler(void){
	if(LL_USART_IsActiveFlag_TXE(UART_TLM)){
		if(tlmuartleftbytes > 0){
			LL_USART_TransmitData8(UART_TLM, tlmuartTXbuf[tlmuartbuflen-tlmuartleftbytes]);
			tlmuartleftbytes--;
		}else{
			LL_USART_DisableIT_TXE(UART_TLM);
		}
	}
}

void TLMUARTsend(uint8_t *buf, uint8_t length){
	tlmuartbuflen = length;
	tlmuartleftbytes = length;
	for(uint8_t i=0; i<length;i++) tlmuartTXbuf[i] = buf[i];
	LL_USART_EnableIT_TXE(UART_TLM);
}

#endif


/* ADCs */
/*_______________________________________________________________________________________________*/

__IO uint16_t ADC_DMAbuffer[ADC_CHANNEL_COUNT*20];

#ifndef MCU_VOLTAGE
#define MCU_VOLTAGE 3300
#endif

void ADCInit(){
	
	LL_SYSCFG_SetRemapDMA_ADC(LL_SYSCFG_ADC1_RMP_DMA1_CH2);
	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_InitTypeDef DMA_InitStructure; 
	DMA_InitStructure.PeriphOrM2MSrcAddress  = (uint32_t)&ADC1->DR;
	DMA_InitStructure.MemoryOrM2MDstAddress  = (uint32_t)ADC_DMAbuffer;
	DMA_InitStructure.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStructure.Mode                   = LL_DMA_MODE_CIRCULAR;
	DMA_InitStructure.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStructure.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStructure.NbData                 = ADC_CHANNEL_COUNT*20;
	DMA_InitStructure.Priority               = LL_DMA_PRIORITY_HIGH;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStructure);
	
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
	
	LL_ADC_CommonDeInit(ADC1_COMMON);
	LL_ADC_DeInit(ADC1);
	
	LL_ADC_InitTypeDef ADC_Init;
	ADC_Init.Clock                = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_Init.Resolution         = LL_ADC_RESOLUTION_12B;
	ADC_Init.DataAlignment  = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_Init.LowPowerMode  = LL_ADC_LP_MODE_NONE;
	LL_ADC_StructInit(&ADC_Init);
	
	LL_ADC_SetCommonPathInternalCh(ADC1_COMMON, LL_ADC_PATH_INTERNAL_TEMPSENSOR);
	
	
	LL_ADC_REG_InitTypeDef ADC_REG_Init;
	ADC_REG_Init.TriggerSource       = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_Init.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_Init.ContinuousMode   = LL_ADC_REG_CONV_CONTINUOUS;
	ADC_REG_Init.DMATransfer     	 = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	ADC_REG_Init.Overrun               = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
	LL_ADC_REG_Init(ADC1, &ADC_REG_Init);
	
	LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
	
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_13CYCLES_5);
	
	LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_TEMPSENSOR);
	#ifdef VOLTAGE_CHANNEL
	LL_ADC_REG_SetSequencerChAdd(ADC1, VOLTAGE_CHANNEL);
	#endif
	#ifdef CURRENT_CHANNEL
	LL_ADC_REG_SetSequencerChAdd(ADC1, CURRENT_CHANNEL);
	#endif
	
	LL_ADC_StartCalibration(ADC1);
	delay16(5000);
	while(LL_ADC_IsCalibrationOnGoing(ADC1));
	delay16(5000);
	LL_ADC_ClearFlag_ADRDY(ADC1);
	delay16(5000);
	LL_ADC_Enable(ADC1);
	delay16(5000);
	while (!LL_ADC_IsEnabled(ADC1));
	delay16(5000);
	LL_ADC_REG_StartConversion(ADC1);
}	

void getADCvalues(int16_t *ADCdatas){
	static int16_t calibCurrent = 0;
	uint8_t ArrMax = ADC_CHANNEL_COUNT*20;
	uint8_t ChannelOrder[ADC_CHANNEL_COUNT] = ADC_CHANNEL_ORDER;
	ADCdatas[0] = __LL_ADC_CALC_TEMPERATURE(MCU_VOLTAGE,ADC_DMAbuffer[ChannelOrder[0]],LL_ADC_RESOLUTION_12B); 
	#ifdef VOLTAGE_CHANNEL
	ADCdatas[1] = ADC_DMAbuffer[ChannelOrder[1]];
	#else
	ADCdatas[1] = 0;
	#endif
	#ifdef CURRENT_CHANNEL
	//get Current avg
	uint32_t CurrentAVG = 0;
	for(uint8_t i=ChannelOrder[2];i<ArrMax; i+=ADC_CHANNEL_COUNT){
		CurrentAVG += ADC_DMAbuffer[i];
	}
	ADCdatas[2] = (CurrentAVG/ArrMax);
	#else
	ADCdatas[2] = 0;
	#endif
}	


/* watchdog */
/*_______________________________________________________________________________________________*/


void WDTinit(){
	LL_IWDG_Enable(IWDG);
	
	LL_IWDG_EnableWriteAccess(IWDG);
	
	LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_4);
	while(LL_IWDG_IsActiveFlag_PVU(IWDG));
	LL_IWDG_SetReloadCounter(IWDG, 1500);
	while(LL_IWDG_IsActiveFlag_RVU(IWDG));
	LL_IWDG_Enable(IWDG);
	LL_IWDG_ReloadCounter(IWDG);	
}

void resetWDT(){
	LL_IWDG_ReloadCounter(IWDG);
}
















