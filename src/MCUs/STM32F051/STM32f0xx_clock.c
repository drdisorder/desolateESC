#include "main.h"
#include "STM32f0xx_clock.h"
void SystemClock_Config(void){
	SystemInit();

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

	LL_RCC_HSI_Enable(); 
	while(LL_RCC_HSI_IsReady() != 1) {};

	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);

	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1){};

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL){};

	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

	LL_SetSystemCoreClock(48000000);
}