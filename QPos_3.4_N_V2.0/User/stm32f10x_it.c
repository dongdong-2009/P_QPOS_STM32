/**
  ******************************************************************************
  * @file    USART/Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32f10x_it.h"
#include "BlueTooth.h"
#include "MSCard_PA1100.h"
#include "VectorTable.h"
#include "Roc_PersonalFont.h"
#include "Display.h"
#include "tamper.h"
#include "powercheck.h"
#include "UART_simulation.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Interrupt
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  HAL_DISP_SET();
  NVIC_SystemReset();
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

void TAMPER_IRQHandler(void)
{
  if(BKP_GetITStatus() != RESET)
  { /* Tamper detection event occurred */
	g_Tamper_Flag=1;
    /* Clear Tamper pin interrupt pending bit */
    BKP_ClearITPendingBit();

    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

    /* Disable Tamper pin */
    BKP_TamperPinCmd(DISABLE);
   
    /* Enable Tamper pin */
 //   BKP_TamperPinCmd(ENABLE);
      
  }
}
/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{//À¶ÑÀ 
	

}

/**
  * @brief  This function handles USARTz global interrupt request.
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)
{//À¶ÑÀ 
      if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
    {		
    		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    		memset(g_ucbBluetoothBuf,0,256);
		g_eBluetoothRecvStatus = eBluetooth_RecvAndAnalyze(g_ucbBluetoothBuf);	
		g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;
		g_BluetoothDateIN=1;
		BackLightCmd(ENABLE);
       	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    }
	else{}  
}
/**
  * @brief  This function handles USARTz global interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{//ÒôÆµ   
   
}
/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{	
	g_BluetoothState=eBluetooth_GetBluetoothState();
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		if(g_PWRKeyFlag)
		{
			if(PWR_Ctrl_Check())	
			{
				g_PWRKeyFlag++;//
				if(g_PWRKeyFlag==10)
				{
					Power_OFF(1);
				}
			}
			else
				g_PWRKeyFlag=0;
		}
		if (g_uiTime100ms_LCDOff_PowerOff)
		{
			g_uiTime100ms_LCDOff_PowerOff--;
		}
		
		if (g_uiTime100ms_LCDShow)
		{
			g_uiTime100ms_LCDShow--;
		}

		if (g_uiTime100ms_Timeout)
		{
			g_uiTime100ms_Timeout--;
		}

		if (MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal)
		{
			MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal--;
		}
		if (g_uiTime100ms_BTRecvTimeout)
		{
			g_uiTime100ms_BTRecvTimeout--;
		}
   	}
	else{}
}
void TIM2_IRQHandler(void)
{
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	 USB_Istr();
}

void EXTI3_IRQHandler(void)
{	
	EXTI_ClearITPendingBit(EXTI_Line3);
	g_PWRKeyFlag=1;
}

void EXTI15_10_IRQHandler()
{
	unsigned short len;
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
		//vLCD_ShowBluetoothIcon();
		BackLightCmd(ENABLE);
		g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;		
	}
	
	else if(EXTI_GetITStatus(EXTI_Line11) != RESET)//USB ´®¿ÚÖÐ¶Ï
	{
		if(HAL_GPUart_Rev(GP_UART_BUF,6))
			return;
		len =GP_UART_BUF[4]+GP_UART_BUF[5]*256;
		if(HAL_GPUart_Rev(GP_UART_BUF+6,len+3))
			return ;
		GP_UART_STATE=1;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}		
	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
