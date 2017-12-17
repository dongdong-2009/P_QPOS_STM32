/*
    this file is the source file for powerceck
*/

#include "powercheck.h"
#include "AN_ADC.h"
#include "stm32f10x.h"
#include "display.h"

unsigned char g_PWRKeyFlag=0;//关机键

void PWR_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    RCC_APB2PeriphClockCmd(PWR_Ctrl_CLK|PWR_SwitchCheck_CLK,ENABLE);      	
	
	GPIO_InitStructure.GPIO_Pin = PWR_Ctrl_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(PWR_Ctrl_Port, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = PWR_SwitchCheck_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(PWR_SwitchCheck_Port, &GPIO_InitStructure);  
}
void PWR_EXTI_Init()//关机终端
{
    EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	GPIO_EXTILineConfig(PWR_EXIT_Port, PWR_EXIT_Pin);
	
	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = PWR_EXIT_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = PWR_EXIT_Num;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void vPower_Init(void)
{	
	unsigned char i=0;
	PWR_GPIO_Init();
	PWR_Cmd(PowerON);
	
	while(1)//检测开机
	{
		if(PWR_Ctrl_Check())	
		{
			i++;
			Delay_ms(200);
			if(i==5)
			{
				iBuzzer_On(0);				
				break;			
			}
		}
		else
		{
			PWR_Cmd(PowerOFF);
		}
	}
	PWR_EXTI_Init();
}
void PWR_Cmd(POWERSTATE NewState)
{
	  if(NewState != PowerOFF)
	  {
	    GPIO_SetBits(PWR_Ctrl_Port, PWR_Ctrl_Pin);
	  }
	  else
	  {
	    GPIO_ResetBits(PWR_Ctrl_Port, PWR_Ctrl_Pin);
	  }
	
}
void Power_OFF(unsigned char flag)
{
	iBuzzer_On(0);
	HAL_DISP_CLS();
	BackLightCmd(DISABLE);
	PWR_Cmd(PowerOFF);
	
	if(flag)
		while(PWR_Ctrl_Check());//检测松手
	Reset_Device();

}
void PWR_Check()
{
	unsigned char i=0;
	while(1)
	{
		if(PWR_Ctrl_Check())	
		{
			i++;
			Delay_ms(200);

			if(i==5)
			{
				Power_OFF(1);
				break;
			}
		}
		else
		{
			break;
			i=0;
		}
	}
}



