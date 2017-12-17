#ifndef __UART_SIMULATION_H
#define __UART_SIMULATION_H

#ifdef __cplusplus
 extern "C" {
#endif 
//=============================================
#define HS32U2_EPPDR 0
#define TX_GPIO_PIN_EN      RCC_APB2Periph_GPIOA 

//使用USB的数据线作为通讯
//PA11 和PA12
#define	TX_GPIO_PIN GPIO_Pin_12
#define	TX_GPIO_PORT GPIOA
#define	TX_LOW	GPIO_ResetBits(TX_GPIO_PORT,TX_GPIO_PIN)//GPIO_WriteBit(TX_GPIO_PORT, TX_GPIO_PIN, Bit_RESET)
#define 	TX_HIG		GPIO_SetBits(TX_GPIO_PORT,TX_GPIO_PIN)//GPIO_WriteBit(TX_GPIO_PORT, TX_GPIO_PIN, Bit_SET)

#define	UART_SIMULATION_IRQ EXTI_Line11
#define	RX_GPIO_PIN GPIO_Pin_11
#define	RX_GPIO_PORT GPIOA

extern unsigned char  GP_UART_BUF[128];
extern unsigned char	 GP_UART_STATE;

unsigned short HAL_GPUartInit(void);
//--------------------------------------------------------------------------------
// Send a byte of data 
//--------------------------------------------------------------------------------
unsigned char HAL_GPUart_SendByte (unsigned char x);
//--------------------------------------------------------------------------------
// Receive a byte of data 
//--------------------------------------------------------------------------------
unsigned char HAL_GPUart_RevByte(unsigned char* x);


unsigned char  HAL_GPUart_Send(unsigned char *SendBuf, unsigned short Sendlen);

unsigned char  HAL_GPUart_Rev(unsigned char *RevBuf, unsigned short RevLen);


//=============================================
#ifdef __cplusplus
}
#endif
#endif 

