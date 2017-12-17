#include"Iwdg.h"
#include "stm32f10x.h"


void vIWDG_Init(void)
{
	RCC_LSICmd(ENABLE);//打开LSI
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //访问之前要首先使能寄存器写
	IWDG_SetPrescaler(IWDG_Prescaler_256);//4//4khz 256分频6.4ms
	IWDG_SetReload(4000);// 最大4095
	IWDG_ReloadCounter();
	IWDG_Enable();// Enable IWDG (the LSI oscillator will be enabled by hardware)
}

