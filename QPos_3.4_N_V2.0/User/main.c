/**
  ******************************************************************************
  * @file    USART/Interrupt/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "string.h"
#include "SysTick_delay.h"
#include "AN_ADC.h"
#include "Display.h"
#include "Z8D256.h"
#include "Bluetooth.h"
#include "MSCard_PA1100.h"
#include "PBOC_Var.h"
#include "Roc_PersonalFont.h"
#include "Roc_LittleTool.h"
#include "VectorTable.h"
#include "UP_MPOS_CMD.h"
#include "Key.h"
#include "rtc.h"
#include "FontFonction.h"
#include "msd.h"
#include "Powercheck.h"
#include "Buzzer.h"
#include "usbhid.h"
#include"tamper.h"
#include "iwdg.h"
#include "IROM.h"
#include "UART_simulation.h"
#include "Nfc_main.h"
#include "APP_parameters.h"

void Set_System(void)
{
  ErrorStatus HSEStartUpStatus;
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK  : RCC->CFGR (AHB div) */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK RCC->CFGR (APB2 div) */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 RCC->CFGR (APB1 div) */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/6  RCC->CFGR (ADC div)*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    //PCLK1 = 36MHZ,PCLK2 = HCLK = 72MHZ.
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
#ifdef MPOS_UPDATE
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x6000);
#else
	 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 			// ??????????1?,1??????,3??????
}

void Reset_Device(void)
{
  	NVIC_SystemReset();
}

void vHardwareInit(void)
{
	unsigned char flag;
	Set_System();
	Delay_Init(72);
	iBUZZER_Init();
	vPower_Init();
#if 1	
	Display_Init();
	MSD_Init();         //字库FLASH 
	BackLightCmd(ENABLE);//打开背光
	vLCD_PowerON();//开机显示
#endif		
	vZ8D256_Init(); 
	AN_ADC_Init();     
	vBluetooth_Init(); 
	MSR_vInit();
	vPBOC_7816_Init();
 
	IROM_Read(FLASH_ADDR_FACTORY_TEST, &flag, 1);
	if((0x55!=flag)&&(0xaa!=flag))
	{
		//vFactory_Test(0);
	}
	else if(0xaa!=flag)//老化测试后
	{
		//vFactory_Test(1);
	}
	IROM_Read(FLASH_ADDR_FACTORY_INIT, &flag, 1);
	if(0x55==flag)//串口模式
	{
		HAL_GPUartInit();
		vFactory_Init(0);		
	}
	else if(0xaa==flag)//USB模式
	{	
		Set_USBClock(); //确定系统时钟是否为72MHZ，设置usb时钟为48MHZ
		USB_Interrupts_Config();
		usb_GPIO_init();
		USB_Init();//usb初始化
		vFactory_Init(1);
	}
	if(NFC_USE==Device_NFC_State())
	{
		vRC523_Init();
		NFC_FieldOff();
	}

	Device_Init();	
	vLCD_MainUI();   
//	vIWDG_Init();
}


int main(void)
{
	g_sCardReaderInfo.ucCardStatus = 0xff;
	g_sKeyboardInfo.ucTimeout = 0;
	g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;//关机时间 

	vHardwareInit(); 
	while(1)
	{	
		IWDG_ReloadCounter();
		vVectorTable_Device();		
		vVectorTable_CMD();		
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
