#include "main.h"
#include "TARGET.h"

void KISS_loop(){
	static uint8_t c_init = 1;
	if(c_init){
		LL_GPIO_InitTypeDef extra_GPIO; 
		extra_GPIO.Pin              = LL_GPIO_PIN_6; 
		extra_GPIO.Mode          = LL_GPIO_MODE_INPUT;  
		extra_GPIO.Speed         = LL_GPIO_SPEED_HIGH; 
		extra_GPIO.Pull             = LL_GPIO_PULL_UP; 
		extra_GPIO.Alternate     = LL_GPIO_AF_0; 
		LL_GPIO_Init(GPIOA, &extra_GPIO); 
		
		extra_GPIO.Pin              = LL_GPIO_PIN_12; 
		extra_GPIO.Mode          = LL_GPIO_MODE_OUTPUT;  
		extra_GPIO.Speed         = LL_GPIO_SPEED_HIGH; 
		extra_GPIO.Pull             = LL_GPIO_PULL_UP; 
		extra_GPIO.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		extra_GPIO.Alternate     = LL_GPIO_AF_0; 
		LL_GPIO_Init(GPIOA, &extra_GPIO); 
		
		GPIOA->BRR = LL_GPIO_PIN_12; 
		
		delay16(0xFFFF);
		if((GPIOA->IDR & LL_GPIO_PIN_6) == 0){//jumper closed
			if(RotationDirection) RotationDirection = 0;
			else RotationDirection = 1;
		}
		c_init = 0;
	}
	static uint8_t LEDactive = 0;
	if(inputFailsave < 900 && LEDactive == 0){
		GPIOA->BSRR = LL_GPIO_PIN_12; 
		LEDactive = 1;
	}
	if(inputFailsave > 900 && LEDactive == 1){
		GPIOA->BRR = LL_GPIO_PIN_12; 
		LEDactive = 0;
	}
	
	static uint32_t reseTimer = 0;
	if(inputFailsave == 1000 && (GPIOB->IDR & LL_GPIO_PIN_3) != 0) reseTimer++;
	else reseTimer = 0;
	
	if(reseTimer == 12000){
		NVIC_SystemReset();
	}
	
}