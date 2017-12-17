//***************************************************************************************
//*  File Name		Z8D256.h
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#ifndef __Z8D256_H__
#define __Z8D256_H__

#define Z8_UART USART3
#define Z8_UART_IRQ	USART3_IRQn
#define Z8_UART_CLK RCC_APB1Periph_USART3
#define Z8_UART_GPIO	GPIOB
#define Z8_UART_RxPIN	GPIO_Pin_11
#define Z8_UART_TxPIN	GPIO_Pin_10

typedef enum
{
	Z8D256_SUCEESCE = 0,
    Z8D256_NO_DATA,
    Z8D256_HEADER_ERROR,
    Z8D256_LENGTH_ERROR,
    Z8D256_XOR_ERROR,
    Z8D256_EXTIME,   
}Z8D256_RECV_STATUS;

extern Z8D256_RECV_STATUS g_eZ8D256_RecvStatus;

void vZ8D256_GPIO_Config(void);

void vZ8D256_USART_NVIC_Config(void);

void vZ8D256_USART_Config(void);

void vZ8D256_Init(void);

void vZ8D256_SendData(unsigned char* pucBuf, unsigned int uiLen);

int iZ8D256_RecvData(unsigned char* pucBuf, unsigned int uiLen);


#endif//_Z8D256_H_
/*******************************end of  file**********************************************/


