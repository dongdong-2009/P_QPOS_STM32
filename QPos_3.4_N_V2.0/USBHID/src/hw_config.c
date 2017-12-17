/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_AINConfig
* Description    : Configures all IOs as AIN to reduce the power consumption.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void GPIO_AINConfig(void)
{

}
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void usb_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* PD.08 used as USB pull-up --> EK-STM32F */
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(USB_DISCONNECT_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{ 

  if (NewState != DISABLE)
  {
    GPIO_SetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);
  }
}


/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
//uint8_t JoyState(void)
//{
//  /* "right" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_RIGHT))
//  {
//    return RIGHT;
//  }
//  /* "left" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_LEFT))
//  {
//    return LEFT;
//  }
//  /* "up" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_UP))
//  {
//    return UP;
//  }
//  /* "down" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_DOWN))
//  {
//    return DOWN;
//  }
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_LEFT_BUTTON))
//  {
//    return LEFT_BUTTON;
//  }
//   if (!GPIO_ReadInputDataBit(GPIOC, JOY_RIGHT_BUTTON))
//  {
//    return RIGHT_BUTTON;
//  }
//  /* No key is pressed */
//  else
//  {
//    return 0;
//  }
//}

/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
//void Joystick_Send(uint8_t Keys)
//{
//  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
//  int8_t X = 0, Y = 0,MouseButton=0;
//
//  switch (Keys)
//  {
//    case LEFT:
//      X -= CURSOR_STEP;
//      break;
//
//    case RIGHT:
//      X += CURSOR_STEP;
//      break;
//	  
//    case UP:
//      Y -= CURSOR_STEP;
//      break;
//
//    case DOWN:
//      Y += CURSOR_STEP;
//	  break;
//	   
//	case LEFT_BUTTON:
//      MouseButton = MouseButton|0x01;
//      break;
//
// 	case RIGHT_BUTTON:
//      MouseButton = MouseButton|0x02;
//      break;
//
//    default:
//      return;
//  }
//
//  /* prepare buffer to send */
//  Mouse_Buffer[0] = MouseButton;
//  Mouse_Buffer[1] = X;
//  Mouse_Buffer[2] = Y;
//  
//  /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
//
//  if(Mouse_Buffer[0]!= 0)
//  {
//    Mouse_Buffer[0] = 0;
//    /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//    USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
//  }   
//
//#ifndef STM32F10X_CL
//  /* Enable endpoint for transmission */
//  SetEPTxValid(ENDP1);
//#endif /* STM32F10X_CL */
//  
//  
//}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);
	
  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &CustomHID_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &CustomHID_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}
#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_OTG_BSP_uDelay.
* Description    : provide delay (usec).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{}
#endif
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
