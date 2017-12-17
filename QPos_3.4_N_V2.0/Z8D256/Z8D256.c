//***************************************************************************************
//*  File Name		Z8D256.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#include "stm32f10x.h"
#include "Z8D256.h"
#include "SysTick_delay.h"

Z8D256_RECV_STATUS g_eZ8D256_RecvStatus = Z8D256_NO_DATA;

unsigned char g_ucbZ8D256RecvBuf[256];

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vZ8D256_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);    
    GPIO_InitStructure.GPIO_Pin = Z8_UART_TxPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Z8_UART_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Z8_UART_RxPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Z8_UART_GPIO, &GPIO_InitStructure);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vZ8D256_USART_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;   
    // Enable the USARTX gloabal Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = Z8_UART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);	   
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vZ8D256_USART_Config(void)
{
  USART_InitTypeDef	 USART_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

  USART_DeInit(Z8_UART);
  /* ---------------------------------------------------------------
    USART Configuration: 
    FPCLK = 36 MHz, Baud rate =36000,MODE: RX & TX
  --------------------------------------------------------------- */
  USART_InitStructure.USART_BaudRate            = 115200;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  USART_InitStructure.USART_Parity              = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(Z8_UART, &USART_InitStructure);
//  USART_ClearFlag(USART1,USART_FLAG_RXNE); 
  /* Enable the USART Receive interrupt: this interrupt is generated when the 
   USART1 receive data register is not empty */
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	  //Ω” ’÷–∂œ
  
  /* Enable USART1 */
  USART_Cmd(Z8_UART, ENABLE);

}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vZ8D256_Init(void)
{    
    vZ8D256_GPIO_Config();
//    vZ8D256_USART_NVIC_Config();
    vZ8D256_USART_Config();
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vZ8D256_SendData(unsigned char* pucBuf, unsigned int uiLen)
{	 
	unsigned int uiLoop;
	for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
	{
		while(!(Z8_UART->SR & USART_FLAG_TXE)){};
		USART_SendData(Z8_UART, pucBuf[uiLoop]);		
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
int iZ8D256_RecvData(unsigned char* pucBuf, unsigned int uiLen)
{
   unsigned int uiTimeout = 0x400000;
	//unsigned int uiTimeout = 100000;
	unsigned int uiLoop = 0;	
    for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
    {
        while(!(Z8_UART->SR & USART_FLAG_RXNE))
        {
            uiTimeout--;
            if(uiTimeout == 0)
            {
                return (1);
            }
            else{}    
        }
        uiTimeout = 10000;	
        pucBuf[uiLoop] = (unsigned char)USART_ReceiveData(Z8_UART);			
    }
    return (0);
}
/*******************************end of  file**********************************************/


