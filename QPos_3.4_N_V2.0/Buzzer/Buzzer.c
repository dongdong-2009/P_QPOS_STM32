/*
    this file is source file for buzzer,
        
*/
#include "Buzzer.h"
//#include "atudio.h"
#include "stm32f10x.h"

BUZZER_RECV_STATUS g_eBuzzerRecvStatus = BUZZER_NO_DATA;



void iBUZZER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	
    RCC_APB2PeriphClockCmd(BUZZER_CLK|RCC_APB2Periph_AFIO,ENABLE);      	

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //失能JTAG调试功能，使能SWJ调试
	  
	GPIO_InitStructure.GPIO_Pin = BUZZER_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(BUZZER_Port, &GPIO_InitStructure);  
	GPIO_ResetBits(BUZZER_Port,BUZZER_Pin);
}

void buzzer_beep()
{
	unsigned short i;
	for(i=0;i<500;i++)
	{
		GPIO_SetBits(BUZZER_Port,BUZZER_Pin);
		Delay_us(100);
		GPIO_ResetBits(BUZZER_Port,BUZZER_Pin);
		Delay_us(100);
	}
	
}

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iBuzzer_On(unsigned char ucTime)
{
    buzzer_beep();
    return (0);
}