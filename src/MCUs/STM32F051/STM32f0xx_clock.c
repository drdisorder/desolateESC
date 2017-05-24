#include "main.h"
#include "STM32f0xx_clock.h"
void SystemClock_Config(void){
	SystemInit();
	/* Set FLASH latency */ 
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

	/* Enable HSI and wait for activation*/
	LL_RCC_HSI_Enable(); 
	while(LL_RCC_HSI_IsReady() != 1) {};

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);

	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1){};

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL){};

	/* Set APB1 prescaler */
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

	/* Set systick to 1ms in using frequency set to 48MHz */
	/* This frequency can be calculated through LL RCC macro */
	/* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_DIV2_VALUE, LL_RCC_PLL_MUL_12) */
	LL_Init1msTick(48000000);

	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	LL_SetSystemCoreClock(48000000);
}