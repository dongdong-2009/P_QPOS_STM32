#ifndef __RC523_H_
#define __RC523_H_

#include "stm32f10x.h"


#define RC523_Pin_CS GPIO_Pin_5
#define RC523_Port_CS GPIOB

#define RC523_Pin_RST GPIO_Pin_8
#define RC523_Port_RST GPIOB

#define NSS_HIGH   GPIO_SetBits(RC523_Port_CS,RC523_Pin_CS)
#define NSS_LOW    GPIO_ResetBits(RC523_Port_CS,RC523_Pin_CS)
#define NRESET_HIGH		 
#define NRESET_LOW	

void RC_TEST(void);
void vRC523_Init(void);
unsigned char RC523_SPI_SendByte(unsigned char data);
unsigned char RC523_SPI_ReadByte();
#endif
