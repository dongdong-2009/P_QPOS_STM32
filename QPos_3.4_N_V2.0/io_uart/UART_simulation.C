//
#include <string.h>
#include "UART_simulation.h"
#include "stm32f10x.h"
#include "VectorTable.h"

unsigned int gpuart_halfBit;
							    
unsigned char  GP_UART_BUF[128];
unsigned char	 GP_UART_STATE=0;
void HAL_GPUart_WaitHalfBit()
{
	u32 temp;		   
	SysTick->LOAD=gpuart_halfBit;//Ê±¼ä¼ÓÔØ
	SysTick->VAL =0x00;           //Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL=0x01 ;          //¿ªÊ¼µ¹Êý  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//µÈ´ýÊ±¼äµ½´ï   
	SysTick->CTRL=0x00;       //¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL =0X00;       //Çå¿Õ¼ÆÊýÆ÷	 
}

unsigned short HAL_GPUartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	//Enable GPIO clocks 
	RCC_APB2PeriphClockCmd(TX_GPIO_PIN_EN, ENABLE);

	//°ë¸öbitµÄÊ±¼ä³¤¶È
	//ÏµÍ³ÆµÂÊ³ýÒÔ´®¿ÚÆµÂÊ³ýÒÔ2
	//gpuart_halfBit =8000000/115200/2-4; // 115200 ,1bit=8.68us
	gpuart_halfBit =(8000000/9600/2)+40;// 9600

	//RXÅäÖÃ
	if(1) 
{
	
	
	GPIO_InitStructure.GPIO_Pin = RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //?ÉÏÀ­å
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RX_GPIO_PORT, &GPIO_InitStructure);
//}
//	else
//{


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	
	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = UART_SIMULATION_IRQ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}	
	//TXÅäÖÃ
	GPIO_InitStructure.GPIO_Pin = TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TX_GPIO_PORT, &GPIO_InitStructure);

	TX_HIG;
	return 0;
}

static uint8_t HAL_GPUart_RevBit(void)
{
  return GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_PIN);
   //return EXTI_GetITStatus(EXTI_Line12);
}

//--------------------------------------------------------------------------------
// Send a byte of data 
//--------------------------------------------------------------------------------
unsigned char HAL_GPUart_SendByte (unsigned char x)
{
	int j; 
	unsigned int parity;  
	unsigned char data;		
	
	parity = 0; 
	
	//send start bit 
	TX_LOW;	
	HAL_GPUart_WaitHalfBit();
	HAL_GPUart_WaitHalfBit();


	//send now 8 bits
	for(j=0;j<8;j++)
	{
		if (!(x & 0x01))
		{
			TX_LOW;
		}
		else
		{
			TX_HIG;
			//parity++; 
		}
		x >>= 1; 
		HAL_GPUart_WaitHalfBit();
		HAL_GPUart_WaitHalfBit();
	}	

/*
	//send parity bit
	if (!(parity & 1))
	{
		//SPIPORT = 0; 
		HS32U2_GPDORE &= 0xF7; 
	}
	else 
	{
		//SPIPORT = 1;
		HS32U2_GPDORE |= 0x08; 
	}
	
	wait_delay (); 
*/
	//send stop bit 
	//SPIPORT = 1; 
	TX_HIG; 
	HAL_GPUart_WaitHalfBit();
	HAL_GPUart_WaitHalfBit();
//	close_pit();
	//  2stop bit
	//HAL_GPUart_WaitHalfBit();
	//HAL_GPUart_WaitHalfBit();
	//send_len++; 
    return 0;
}

//--------------------------------------------------------------------------------
// Receive a byte of data 
// 0 ok
// 1 fail
//--------------------------------------------------------------------------------
unsigned char HAL_GPUart_RevByte(unsigned char* x)
{
	//unsigned char the_bit; 
	unsigned char ret =0; 
	unsigned int i = 0; 
	unsigned char parity = 0; 

	// set flag

	
	//wait for start bit 
	//waiting 5bit 
//	if(1)
//{
	u32 temp;	//³¬Ê±Ê±¼äÎª500ms	   
	SysTick->LOAD=gpuart_halfBit*10;//Ê±¼ä¼ÓÔØ
	SysTick->VAL =0x00;           //Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL=0x01 ;          //¿ªÊ¼µ¹Êý  
	while(1)
	{
		temp=SysTick->CTRL;
		if(!(temp&0x01&&!(temp&(1<<16))))
			{
			SysTick->CTRL=0x00;       //¹Ø±Õ¼ÆÊýÆ÷
			SysTick->VAL =0X00;       //Çå¿Õ¼ÆÊýÆ÷	 			
			return 1;
			}
		if(!HAL_GPUart_RevBit())
			break;
		
	}
	SysTick->CTRL=0x00;       //¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL =0X00;       //Çå¿Õ¼ÆÊýÆ÷	 
//}	
//	else
//{
//	if(HAL_GPUart_RevBit())
//		return 1;
//}
	//--------------------------------------------------------------------------------

	//check the start bit
	//gpuart_halfBit=gpuart_halfBit-8;
	HAL_GPUart_WaitHalfBit();
	//gpuart_halfBit=gpuart_halfBit+8;
	if(HAL_GPUart_RevBit())
		return 1;

	
	HAL_GPUart_WaitHalfBit();

	//--------------------------------------------------------------------------------
	//receive data bits
	for (i=0; i<8; i++)
	{
		HAL_GPUart_WaitHalfBit();
		
		ret >>= 1;
		if(HAL_GPUart_RevBit())
			ret |= 0x80; 
		//parity ^= ret; 
		
		HAL_GPUart_WaitHalfBit();
	}

	//--------------------------------------------------------------------------------

	//Í£Ö¹Î»
	HAL_GPUart_WaitHalfBit();
	//HAL_GPUart_WaitHalfBit();
	//parity = (0x04 & HS32U2_EPPDR);
	
	//delay_time0(half_etu);

	//..wait_delay ();

	*x =ret;
	return 0;

}


unsigned char  HAL_GPUart_Send(unsigned char *SendBuf, unsigned short SendLen)
{
	short i = 0;
	for(i = 0; i<SendLen; i++)
	{
		HAL_GPUart_SendByte(SendBuf[i]);
	}
	return 0;

}

unsigned char  HAL_GPUart_Rev(unsigned char *RevBuf, unsigned short RevLen)
{
	unsigned short i = 0;
	unsigned char para;
	for(i = 0; i<RevLen; i++)
	{
		if(HAL_GPUart_RevByte(&para))
			return 1;
		else
			RevBuf[i]=para;
	}
	return 0;

}

//=================================
//StoreKey_Send 
//Í¨Ñ¶¸ñÊ½:
//Æ«ÒÆ Êý¾Ý		¸ñÊ½ ³¤¶È ËµÃ÷
//0x00	ÆðÊ¼·û	HEX	2	0x55 0xAA
//0x02	°üÀàÐÍ	HEX	1	0x51
//0x03	°üÐòºÅ	HEX	1	È¡ÖµÎª1~255£¬Ã¿´Î½»»¥ºóÔö¼Ó1£¬³¬¹ý255ºóÖØÐÂ´Ó1¿ªÊ¼¼ÆÊý
//0x04	°ü³¤¶È	HEX	2	±íÊ¾Ö¸ÁîºÍ²ÎÊýµÄ³¤¶È×ÜºÍ£¬µÍ×Ö½ÚÓÅÏÈ´«Êä
//0x06	Ö¸Áî		HEX	2	²Î¿¼Ö¸ÁîËµÃ÷
//0x08	²ÎÊý		HEX	n	²Î¿¼Ö¸ÁîËµÃ÷£¬³¤¶Èn²»³¬¹ý1024
//0x08+n	Ð£ÑéºÍ	HEX	1	´Ó¡°°üÀàÐÍ¡±µ½¡°²ÎÊý¡±µÄÊý¾ÝµÄÖð×Ö½ÚÒì»ò½á¹û
//0x09+n	ÖÕÖ¹·û	HEX	2	0xCC 0x33



//=================================
//StoreKey_PCtoM 
//Í¨Ñ¶¸ñÊ½:
//Æ«ÒÆÄÚÈÝ	¸ñÊ½ ³¤¶È 	ËµÃ÷
//0x00	STX		HEX	1		0x02 ÆðÊ¼Î»
//0x01	LEN		BCD	2		ÓÒ¶ÔÆë
//0x03	SEQ		HEX	4		ÐòÁÐºÅ,Ã¿´ÎÊÕ·¢ÎªÒ»×éÏàÍ¬µÄÊý¡£
//0x07	CON	HEX	LEN-4	Êý¾Ý
//L+3	ETX		HEX	1		0x03 ½áÊøÎ»
//L+4	LRC		HEX	1		Ð£ÑéÎ»
//=================================

unsigned char GP_UART_CMD(void)//´®¿Ú³õÊ¼»¯
{
	unsigned char iRET;
	unsigned short cmd,len;
	
	if(GP_UART_STATE)
	{
		GP_UART_STATE=0;
		g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;

		if(GP_UART_BUF[0]!=0x55||GP_UART_BUF[1]!=0xaa)
		return 1;
	
		len=GP_UART_BUF[5]<<8|GP_UART_BUF[4];
		cmd=GP_UART_BUF[6]<<8|GP_UART_BUF[7];
		switch(cmd)
		{
			case 0x7803:
				vLCD_GUI_ShowMid12X12("ÅäÖÃÖÐ...",4);
				iRET=iCMD_Factory_DeviceInit(GP_UART_BUF+8,len-2);	
			break;
			default:
				iRET=1;
			break;			
		}
			
		
		if(iRET)
		{	
			GP_UART_BUF[2]=0x45;
			GP_UART_BUF[4]=0x03;
			GP_UART_BUF[8]=iRET;
			GP_UART_BUF[10]=0xcc;
			GP_UART_BUF[11]=0x33;
			HAL_GPUart_Send(GP_UART_BUF,12);
		}
		else
		{
			GP_UART_BUF[2]=0x41;
			GP_UART_BUF[4]=0x02;
			GP_UART_BUF[9]=0xcc;
			GP_UART_BUF[10]=0x33;
			HAL_GPUart_Send(GP_UART_BUF,11);
		}
		return iRET;
	}
	return 0xff;
}

