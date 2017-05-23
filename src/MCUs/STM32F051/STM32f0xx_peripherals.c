#include "main.h"
#include "STM32f0xx_peripherals.h"
void EnablePheriphClocks(){
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_ADC);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2 | LL_APB1_GRP1_PERIPH_USART2 | LL_APB1_GRP1_PERIPH_TIM6 | LL_APB1_GRP1_PERIPH_TIM7);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG | LL_APB2_GRP1_PERIPH_USART1 | LL_APB2_GRP1_PERIPH_TIM1 | LL_APB2_GRP1_PERIPH_TIM15 | LL_APB2_GRP1_PERIPH_TIM16);
}