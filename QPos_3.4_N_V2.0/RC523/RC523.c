#include "stm32f10x.h"
#include "string.h"
#include"RC523.h"
#include "SysTick_delay.h"

void RC523_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB , ENABLE);
#if 1
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);

	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Confugure MISO pin as Input Floating  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
   /* Confugure CS /RST pin as OUT_PP  */
  GPIO_InitStructure.GPIO_Pin = RC523_Pin_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(RC523_Port_CS, &GPIO_InitStructure); 

  GPIO_SetBits(RC523_Port_CS,RC523_Pin_CS);
  
}

void RC523_SPI_Config(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
	 SPI_Cmd(SPI1, DISABLE);  
	SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex;  //两线全双工
	SPI_InitStructure.SPI_Mode =SPI_Mode_Master;       //主
	SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b;      //8位
	SPI_InitStructure.SPI_CPOL =SPI_CPOL_High;        //CPOL=1时钟悬空高
	SPI_InitStructure.SPI_CPHA =SPI_CPHA_2Edge;       //CPHA=1 数据捕获第2个
	SPI_InitStructure.SPI_NSS =SPI_NSS_Soft;        //软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_4;  //2分频
	SPI_InitStructure.SPI_FirstBit =SPI_FirstBit_MSB;      //高位在前
	SPI_InitStructure.SPI_CRCPolynomial =7;        //CRC7
   
	SPI_Init(SPI1,&SPI_InitStructure);	
	SPI_Cmd(SPI1, ENABLE);

}
void vRC523_Init()
{
	RC523_GPIO_Config();
	/*
	GPIO_SetBits(RC523_Port_RST,RC523_Pin_RST);
	Delay_ms(1);
	GPIO_ResetBits(RC523_Port_RST,RC523_Pin_RST);
	Delay_ms(1);
	GPIO_SetBits(RC523_Port_RST,RC523_Pin_RST);
	Delay_ms(1);
	*/
	RC523_Init();	
		
}
unsigned char RC523_SPI_SendByte(unsigned char data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
 	{
 	}
 	SPI_I2S_SendData(SPI1, data);

 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
 	{
 	}
 	return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

unsigned char RC523_SPI_ReadByte()
{
	unsigned char data=0;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
 	{
 	}
 	SPI_I2S_SendData(SPI1, data);

 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
 	{
 	}
 	return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}
void RC_TEST(void)
{
	if(0x9000 == NFC_SendWUPA())
	{
		if(0==Activation_TypeA())
		{	
			iBuzzer_On(0);
			NFC_Deselect();
		}
	}
}

