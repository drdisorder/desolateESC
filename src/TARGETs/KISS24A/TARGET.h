
/* SIGNAL INPUT */
/*_______________________________________________________________________________________________*/
/*
 input pin is PB3 on TIM2 CH2
*/

#define INPUT_TIM TIM2
#define INPUT_TIM_CHAN LL_TIM_CHANNEL_CH2
#define INPUT_TIM_32BIT
#define INPUT_DMA_SOURCE TIM2->CCR2
#define INPUT_DMA_ENABLE_REQ LL_TIM_EnableDMAReq_CC2(INPUT_TIM);
#define INPUT_DMA_Channel LL_DMA_CHANNEL_3
#define INPUT_DMA_IRQn DMA1_Channel2_3_IRQn
#define INPUT_DMA_IRQHandler DMA1_Channel2_3_IRQHandler
#define INPUT_DMA_TC_ChannelFlag DMA_ISR_TCIF3

#define Input_GPIO_init \
	LL_GPIO_InitTypeDef input_GPIO; \
	input_GPIO.Pin              = LL_GPIO_PIN_3; \
	input_GPIO.Mode          = LL_GPIO_MODE_ALTERNATE;  \
	input_GPIO.Speed         = LL_GPIO_SPEED_HIGH; \
	input_GPIO.Pull             = LL_GPIO_PULL_DOWN; \
	input_GPIO.Alternate     = LL_GPIO_AF_2; \
	LL_GPIO_Init(GPIOB, &input_GPIO); 



/* PWM OUTPUT */
/*_______________________________________________________________________________________________*/
/*
FET Driver Connection: (A, B, C)
PWM: A8, A9, A10
EN : B0, B4, B5
*/

#define FETControl_GPIO_init \
	LL_GPIO_InitTypeDef FET_GPIOS; \
	FET_GPIOS.Pin              = LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10; \
	FET_GPIOS.Mode          = LL_GPIO_MODE_ALTERNATE; \
	FET_GPIOS.Speed         = LL_GPIO_SPEED_HIGH; \
	FET_GPIOS.OutputType = LL_GPIO_OUTPUT_PUSHPULL; \
	FET_GPIOS.Pull            = LL_GPIO_PULL_NO; \
	FET_GPIOS.Alternate     = LL_GPIO_AF_2; \
	LL_GPIO_Init(GPIOA, &FET_GPIOS); \
	FET_GPIOS.Mode          = LL_GPIO_MODE_OUTPUT; \
	FET_GPIOS.Pin              = LL_GPIO_PIN_0 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5; \
	LL_GPIO_Init(GPIOB, &FET_GPIOS); 
	
#define PWM_OUTPUT_POLARITY LL_TIM_OCPOLARITY_HIGH	
#define PWM_OUTPUT_IDLESTATE LL_TIM_OCIDLESTATE_LOW	

#define PWM_OUTPUT_N_POLARITY LL_TIM_OCPOLARITY_HIGH	
#define PWM_OUTPUT_N_IDLESTATE LL_TIM_OCIDLESTATE_LOW

#define PWM_DEADTIME 0 // 48M TIM


#define PHASE_A_PWM \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE); \
	GPIOB->BSRR = LL_GPIO_PIN_0; \
	
#define PHASE_A_FLOATING \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_8; \
	GPIOB->BRR = LL_GPIO_PIN_0; \
	
#define PHASE_A_LOW \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_8; \
	GPIOB->BSRR = LL_GPIO_PIN_0; \


#define PHASE_B_PWM \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE); \
	GPIOB->BSRR = LL_GPIO_PIN_4; \
	
#define PHASE_B_FLOATING \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_9; \
	GPIOB->BRR = LL_GPIO_PIN_4; \
	
#define PHASE_B_LOW \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_9; \
	GPIOB->BSRR = LL_GPIO_PIN_4; \
	
	
#define PHASE_C_PWM \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE); \
	GPIOB->BSRR = LL_GPIO_PIN_5; \
	
#define PHASE_C_FLOATING \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_10; \
	GPIOB->BRR = LL_GPIO_PIN_5; \
	
#define PHASE_C_LOW \
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT); GPIOA->BRR = LL_GPIO_PIN_10; \
	GPIOB->BSRR = LL_GPIO_PIN_5; \
	
	
	
#define ALL_PHASES_LOW PHASE_A_LOW; PHASE_B_LOW; PHASE_C_LOW;
	
	
	
/* BEMF Comparator INPUTS */
/*_______________________________________________________________________________________________*/


/*
Star: A3
A: A4
B: A2
C: A5
*/

#define Comparator_GPIO_init \
	LL_GPIO_InitTypeDef COMP_GPIOS; \
	COMP_GPIOS.Pin              = LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5; \
	COMP_GPIOS.Mode          = LL_GPIO_MODE_ANALOG; \
	COMP_GPIOS.Speed         = LL_GPIO_SPEED_HIGH; \
	COMP_GPIOS.Pull            = LL_GPIO_PULL_NO; \
	COMP_GPIOS.Alternate     = LL_GPIO_AF_0; \
	LL_GPIO_Init(GPIOA, &COMP_GPIOS); 

#define BEMF_Comparator COMP2
	
#define BEMF_STAR_CMP_IN LL_COMP_INPUT_PLUS_IO1

#define BEMF_A_CMP_IN LL_COMP_INPUT_MINUS_DAC1_CH1
#define BEMF_B_CMP_IN LL_COMP_INPUT_MINUS_IO1
#define BEMF_C_CMP_IN LL_COMP_INPUT_MINUS_DAC1_CH2	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	