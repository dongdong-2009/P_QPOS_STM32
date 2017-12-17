/*
    this file is the soucer file for dispaly
*/
#include "ASCII.h"
#include "Display.h"
#include "stm32f10x.h"
#include "SysTick_delay.h"
#include "Roc_PersonalFont.h"
#include "Powercheck.h"
#include "VectorTable.h"

#define LCD_LIGTHCTR      GPIOB
#define LCD_LIGTHCTRPIN   GPIO_Pin_12 
#define LCD_RESBGPIO      GPIOB
#define LCD_RESBGPIOPIN   GPIO_Pin_14 
#define LCD_A0GPIO        GPIOA
#define LCD_A0GPIOPIN     GPIO_Pin_6
#define LCD_CSBGPIO       GPIOB
#define LCD_CSBGPIOPIN    GPIO_Pin_13

#define Flash_CSBGPIO       GPIOA
#define Flash_CSBGPIOPIN    GPIO_Pin_4

#define LCD_SPIMODE    Master
#define LCD_SPI        SPI1
#define LCD_SPI_CLK	RCC_APB2Periph_SPI1

#define LCD_SPICLKGPIO        GPIOA
#define LCD_SPICLKGPIOPIN     GPIO_Pin_5
#define LCD_SPIMOSIGPIO       GPIOA
#define LCD_SPIMOSIGPIOPIN    GPIO_Pin_7
#define LCD_SPIMISOGPIO       GPIOA
#define LCD_SPIMISOGPIOPIN    GPIO_Pin_6

#define LCD_COMMAND                       (0x01)
#define LCD_DATA                          (0x02)

#define LCD_LINE_NUM                      (0x41)
#define LCD_COLUMN_NUM				      (0x83)
#define LCD_PAGE_NUM                      (0x08)
//speicfize which SPI to selete
#define COMNUMBER                         20
#define DISPLAYON                         (0xAF)
#define DISPLAYOFF                        (0xAE)
#define SET_START_LINE(Line)              (0x40 | Line)
#define SET_PAGE_ADDR(Page)               (0xB0 | Page)
#define SET_COLUMN_ADDR_FISRT(Column)     (0x10|(column>>4))
#define SET_COLUMN_ADDR_SECOND(Column)    (Column & 0x0F)
#define READ_STATUE                       (0x00)
#define WRITE_DATA(data) 				  (data)
#define READ_DATA                         (0x00)
#define SEG_DIRECTION(MX)                 (0xA0 | (MX & 0x01))
#define INVERSE_DISPLAY(Inverse)          (0xA6 |(Inverse &0x01))
#define ALL_PIXEL_ON(AP)                  (0xA4 | (AP & 0x01))
#define BIAS_SELECT(BS)                   (0xA2 | (BS & 0x01))
#define READ_MODYE_WRITE                  (0xE0)
#define END_RMW                               (0xEE)
#define RESET_DISP                             (0xE2)
#define COM_DIRECTION(MY)                 (0xC0 | (MY & 0x08))
#define POWER_CONTROL(VB,VR,VF)           (0x28 | (VB << 0x02) | (VR << 0x01) | (VF << 0x00))
#define REGULATION_RATIO(RR)              (0x20 | RR)
#define SET_EV_FIRST                      (0x81)
#define SET_EV_SECOND(EV)                 (EV & 0x3F)
#define SET_BOOSTER(SB)                   (SB | 0x03)
#define ST7567_NOP                        (0xE3)
/*
显示屏和ST之间连接的GPIO说明
   DisPaly            ST
BACKLIGTH_CTR         PC9  
CS                    PC6
A0                    PC8
RST                   PC7
SPI_CLK               PB13
SPI_MOSI              PB15(SPI_MOSI)
SPI_MISO              PB14(SPI_MISO)
*/
extern unsigned char USB_Plugin;
void ST7567_GPIO_Init_QY()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA,ENABLE);
#if 1
	/*!< Configure ST7567 RESB pins: LCD_RESB ,and set this bit is "1"*/
	GPIO_InitStructure.GPIO_Pin = LCD_RESBGPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_RESBGPIO, &GPIO_InitStructure);    
    GPIO_SetBits(LCD_RESBGPIO,LCD_RESBGPIOPIN);	

	/*!< Configure ST7567 A0 pins: LCD_A0 ,and set this bit is "1"*/
	GPIO_InitStructure.GPIO_Pin = LCD_A0GPIOPIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LCD_A0GPIO, &GPIO_InitStructure); 	
//    GPIO_SetBits(LCD_A0GPIO,LCD_A0GPIOPIN);	

	/*!< Configure ST7567 CS pins: LCD_CS ,and set this bit is "1"*/
	GPIO_InitStructure.GPIO_Pin = LCD_CSBGPIOPIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_CSBGPIO, &GPIO_InitStructure);    
    GPIO_SetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);	 
    /*!< Configure ST7567 LIGTHCTR pins: LCD_LIGTHCTR ,and set this bit is "1"*/
	GPIO_InitStructure.GPIO_Pin = LCD_LIGTHCTRPIN;
	GPIO_Init(LCD_LIGTHCTR, &GPIO_InitStructure);    
    GPIO_SetBits(LCD_LIGTHCTR,LCD_LIGTHCTRPIN);    
#endif
//SPI GPIO 配置    
	GPIO_InitStructure.GPIO_Pin = LCD_SPICLKGPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    
	GPIO_Init(LCD_SPICLKGPIO, &GPIO_InitStructure); 
    
	GPIO_InitStructure.GPIO_Pin = LCD_SPIMOSIGPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    
	GPIO_Init(LCD_SPIMOSIGPIO, &GPIO_InitStructure);

	//LCD只发送不接收
  
//	GPIO_InitStructure.GPIO_Pin = LCD_SPIMISOGPIOPIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;    
//	GPIO_Init(LCD_SPIMISOGPIO, &GPIO_InitStructure); 

////FLASH 引脚
//	GPIO_InitStructure.GPIO_Pin = Flash_CSBGPIOPIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    
//	GPIO_Init(Flash_CSBGPIO, &GPIO_InitStructure);
//    GPIO_ResetBits(Flash_CSBGPIO,Flash_CSBGPIOPIN); 
}
void SPI_Init_QY(void)
{
    SPI_InitTypeDef   SPI_InitStructure;
    SPI_I2S_DeInit(LCD_SPI);	
	/*!< SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(LCD_SPI_CLK, ENABLE); 
	 SPI_Cmd(LCD_SPI, DISABLE);  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			
	/*!< SPI Config */	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(LCD_SPI, &SPI_InitStructure);   
    SPI_Cmd(LCD_SPI, ENABLE); /*!< SD_SPI enable */    
}
//背光灯控制
void BackLightCmd(unsigned char NewSta)
{
	if(NewSta!=DISABLE)
	{
		GPIO_ResetBits(LCD_LIGTHCTR,LCD_LIGTHCTRPIN); 
		g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;//关机时间 
	}
	else
	{
		GPIO_SetBits(LCD_LIGTHCTR,LCD_LIGTHCTRPIN);
	}
}

/**
  * @brief  Write a byte on the SD.
  * @param  Data: byte to send.
  * @retval None
  */
uint8_t SPI_SendData_QY(uint8_t Data)
{
	/*!< Wait until the transmit buffer is empty */
 	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET)
 	{
 	}

 	/*!< Send the byte */
 	SPI_I2S_SendData(LCD_SPI, Data);

	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET)
		/*!< Wait to receive a byte*/
//	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}

 	/*!< Return the byte read from the SPI bus */ 
	return (uint8_t)SPI_I2S_ReceiveData(LCD_SPI);
}

/**
  * @brief  Read a byte from the SD.
  * @param  None
  * @retval The received byte.
  */
uint8_t SPI_RecvData_QY(void)
{
	uint8_t Data = 0;

	/*!< Wait until the transmit buffer is empty */
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	/*!< Send the byte */
	SPI_I2S_SendData(LCD_SPI, 0x00);

	/*!< Wait until a data is received */
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	/*!< Get the received data */
	Data = (uint8_t)SPI_I2S_ReceiveData(LCD_SPI);

	/*!< Return the shifted data */
	return Data;
}
/**
 * @name HAL_DISP_WriteByte
 * @brief 该函数用来发送控制指令和数据给ST7567
 * @param uCmdOrData：指令和数据
 *        ucLCD_ISCommand：判断是不是指令
 * @author hanlei
 * @date 08-11-2012
**/
void HAL_DISP_WriteByte(uint8_t uCmdOrData,uint8_t ucLCD_ISCommand)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LCD_A0GPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_A0GPIO, &GPIO_InitStructure); 
		
	GPIO_SetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);
	GPIO_ResetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);	
	if(ucLCD_ISCommand == LCD_COMMAND)
	{
        GPIO_ResetBits(LCD_A0GPIO,LCD_A0GPIOPIN);	 
    }
    else
    {
		GPIO_SetBits(LCD_A0GPIO,LCD_A0GPIOPIN);  	
    }
	SPI_SendData_QY(uCmdOrData);
	GPIO_SetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);	

	GPIO_InitStructure.GPIO_Pin = LCD_A0GPIOPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(LCD_A0GPIO, &GPIO_InitStructure); 

}
/**
 * @name HAL_DISP_WriteString
 * @brief 该函数用来发送控制指令和数据给ST7567
 * @param uDisplayData：数据缓冲区地址
 *        uDataNum：发送的数量
 * @author hanlei
 * @date 08-11-2012
**/
void HAL_DISP_WriteString(const unsigned char *uDisplayData,unsigned char uDataNum)
{
	 uint8_t i = 0;
	 GPIO_ResetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);	
	 for( i = 0; i< uDataNum;i++)
	 {
		HAL_DISP_WriteByte(uDisplayData[i],LCD_DATA);
	 }
	 GPIO_SetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);	  	 
}
/**
 * @name HAL_DISP_SetXY
 * @brief 设置屏上的位置X坐标和Y坐标
 *        page ： X坐标
 *        column：Y坐标
 * @param none
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void HAL_DISP_SetXY(unsigned char page, unsigned char column)
{
	HAL_DISP_WriteByte(SET_PAGE_ADDR(page),LCD_COMMAND);			// Page or line
	HAL_DISP_WriteByte(SET_COLUMN_ADDR_FISRT(column) ,LCD_COMMAND);		// Column high			
	HAL_DISP_WriteByte(SET_COLUMN_ADDR_SECOND(column) ,LCD_COMMAND);	// Column low
}
/**
 * @name  HAL_DISP_CLS
 * @brief 清屏为白
 *        page ： X坐标
 *        column：Y坐标
 * @param none
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void HAL_DISP_CLS(void)
{
	unsigned char i,j;    
	for(i=0;i<LCD_LINE_NUM;i++)				
	{
		HAL_DISP_SetXY(i,0);	    
		for(j=0;j<LCD_COLUMN_NUM;j++)	 	
		{				
			HAL_DISP_WriteByte(0,LCD_DATA);
		}
	}
}


/**
 * @name  HAL_DISP_SET
 * @brief 清屏为黑
 * @param none
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void HAL_DISP_SET(void)
{
	unsigned char i,j;    
	for(i=0;i<LCD_LINE_NUM;i++)				
	{
		HAL_DISP_SetXY(i,0);	    
		for(j=0;j<LCD_COLUMN_NUM;j++)	 	
		{				
			HAL_DISP_WriteByte(0xFF,LCD_DATA);
		}
	}

}

//清楚某一个PAge的column到LCD_COLUMN_NUM
void ClearALLPage(unsigned char page,unsigned char column)
{
    unsigned char i = 0;
    HAL_DISP_SetXY(page,0);	    
    for(i=column;i<LCD_COLUMN_NUM;i++)	 	
    {				
        HAL_DISP_WriteByte(0x00,LCD_DATA);
    }      
}
//清楚某一个PAge的column到LCD_COLUMN_NUM
void ClearLines(unsigned char page1,unsigned char page2)
{
   unsigned char i,j;    
	for(i=page1;i<page2+1;i++)				
	{
		HAL_DISP_SetXY(i,0);	    
		for(j=0;j<LCD_COLUMN_NUM;j++)	 	
		{				
			HAL_DISP_WriteByte(0x00,LCD_DATA);
		}
	}     
}
//*******************************************************
// HAL_DISP_ResetLCD				                           
//
//
//********************************************************/
void HAL_DISP_ResetLCD(void)
{
											//reset,because the LCD need initialize the register,
											//so the reset need some times,before write,we set
											//1ms,for reset,and 5ms after reset.
	GPIO_ResetBits(LCD_RESBGPIO,LCD_RESBGPIOPIN);	
	Delay_ms(1);
	GPIO_SetBits(LCD_RESBGPIO,LCD_RESBGPIOPIN);
	Delay_ms(5);

}



/* export function prototypes -----------------------------------------------*/

/* export function ----------------------------------------------------------*/
/**
 * @name  ST7567_Into_Power_Save_QY
 * @brief 使显示屏进入低功耗状态
 * @param  page ： 行数
 *         column：列数
 *         order ：序号
 *         pASCIIchar:ACSII码表的起始地址。
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void ST7567_Into_Power_Save_QY(void)
{
	//Display　OFF
	HAL_DISP_WriteByte(DISPLAYOFF,LCD_COMMAND);	
    //ALL Pixel ON
	HAL_DISP_WriteByte(ALL_PIXEL_ON(0x01),LCD_COMMAND);		
}
/**
 * @name  ST7567_OUT_Power_Save_QY
 * @brief 使显示屏退出低功耗状态
 * @param  page ： 行数
 *         column：列数
 *         order ：序号
 *         pASCIIchar:ACSII码表的起始地址。
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void ST7567_OUT_Power_Save_QY(void)
{
	//ALL Pixel ON
	HAL_DISP_WriteByte(ALL_PIXEL_ON(0x00),LCD_COMMAND);		
	//Display ON
	HAL_DISP_WriteByte(DISPLAYON,LCD_COMMAND);   	
}
/**
 * @name  ST7567_Power_OFF_QY
 * @brief 使显示屏进入下电状态
 * @param  page ： 行数
 *         column：列数
 *         order ：序号
 *         pASCIIchar:ACSII码表的起始地址。
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void ST7567_Power_OFF_QY(void)
{
	//Display　OFF
	HAL_DISP_WriteByte(DISPLAYOFF,LCD_COMMAND);	
	//ALL Pixel ON
	HAL_DISP_WriteByte(ALL_PIXEL_ON(0x00),LCD_COMMAND);	
	//等待5ms
	Delay_ms(1);  
}
/**
 * @name ST7567_PowerOnConfig_QY
 * @brief ST7567的上电流程
 * @param none
 *        
 * @author hanlei
 * @date 08-11-2012
**/
void ST7567_Power_On_QY(void)
{
	GPIO_ResetBits(LCD_RESBGPIO,LCD_RESBGPIOPIN);	
	Delay_ms(1);
 	GPIO_SetBits(LCD_RESBGPIO,LCD_RESBGPIOPIN);	
  	Delay_ms(5);    
 	HAL_DISP_WriteByte(RESET_DISP,LCD_COMMAND);         //initialize interal function	,Software Reset
 	Delay_ms(1);
 	HAL_DISP_WriteByte(POWER_CONTROL(0x01,0x01,0x01),LCD_COMMAND);         //power control(VB,VR,VF=1,1,1)
 	Delay_ms(1);
 	HAL_DISP_WriteByte(REGULATION_RATIO(0x03),LCD_COMMAND);         //Regulator resistor select
 	Delay_ms(1);
 	HAL_DISP_WriteByte(BIAS_SELECT(0x01),LCD_COMMAND);         //set LCD bias=1/6
 	Delay_ms(1);
 	HAL_DISP_WriteByte(SET_EV_FIRST,LCD_COMMAND);         //set reference voltage
 	HAL_DISP_WriteByte(SET_EV_SECOND(0x24),LCD_COMMAND);	
	Delay_ms(1);
 	HAL_DISP_WriteByte(COM_DIRECTION(0x08),LCD_COMMAND);         //set SHL COM1 to COM64
 	Delay_ms(1);
 	HAL_DISP_WriteByte(SEG_DIRECTION(0x00),LCD_COMMAND);         //ADC select SEG1 to SEG132
 	Delay_ms(1);
 	HAL_DISP_WriteByte(SET_START_LINE(0x00),LCD_COMMAND);         //Initial Display Line
 	Delay_ms(1);
 	HAL_DISP_WriteByte(INVERSE_DISPLAY(0x00),LCD_COMMAND);         //set reverse display OFF
 	Delay_ms(1);
	HAL_DISP_WriteByte(DISPLAYON,LCD_COMMAND);         //turns the display ON
 	Delay_ms(1);
  	GPIO_ResetBits(LCD_CSBGPIO,LCD_CSBGPIOPIN);        
}
/**
 * @name  MID_DISP_ShowChar16X8
 * @brief 显示ACII吗表中的英文字符
 * @param  page ： 行数
 *         column：列数
 *         order ：序号
 *         pASCIIchar:ACSII码表的起始地址。
 *        
 * @author hanlei
 * @date 08-11-2012
**/

void MID_DISP_ShowChar16X6(unsigned char page,unsigned char column,unsigned char Order,const unsigned char *pASCIIchar)
{
	unsigned int  x;
	x = (Order - 0x20)*12; 
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(pASCIIchar+x,6);
	x = x+6;	     
	HAL_DISP_SetXY(page+1,column);
	HAL_DISP_WriteString(pASCIIchar+x,6);    
}
void MID_DISP_ShowChar16X8(unsigned char page,unsigned char column,unsigned char Order,const unsigned char *pASCIIchar)
{
	unsigned int  x;
	x = (Order - 0x20)*16; 
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(pASCIIchar+x,8);
    x = x+8;	     
	HAL_DISP_SetXY(page+1,column);
	HAL_DISP_WriteString(pASCIIchar+x,8);    
}
//{0x00,0x28,0x7C,0x54,0x28,0x00,0x00,0x00},/*"?????",0*/
void MID_DISP_Show8X8(unsigned char page,unsigned char column,const unsigned char *pASCIIchar)
{
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(pASCIIchar,8);    
}
void MID_DISP_Show8X20(unsigned char page,unsigned char column,const unsigned char *pASCIIchar)
{
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(pASCIIchar,20);    
}

void MID_DISP_ShowChar24X12(unsigned char page,unsigned char column,const unsigned char *pASCIIchar)
{
	unsigned int  x = 0;
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(pASCIIchar+x,12);  
    x = x + 12;    
	HAL_DISP_SetXY(page+1,column);
	HAL_DISP_WriteString(pASCIIchar+x,12);    
    x = x + 12;    
	HAL_DISP_SetXY(page+2,column);
	HAL_DISP_WriteString(pASCIIchar+x,12);  
}
/**
 * @name  MID_DISP_ShowCharStr
 * @brief 显示ACII吗表中的英文字符
 * @param  page ： 起始行数
 *         column：起始列数
 *         pStr:字符串缓冲区。
 *         len ：长度    
 * @author hanlei
 * @date 08-11-2012
**/
void MID_DISP_ShowCharStr(unsigned char page,unsigned char column,unsigned char *pStr,unsigned char len)
{
    unsigned char i = 0;
	for(i = 0; i< len ;i++)
	{
		if((column+8) > 128)
		{
			page = page +2;
			column = 0;
        }
		if((page + 1) > 7)
		{
			page = 0;
        }
		
        MID_DISP_ShowChar16X8(page,column,pStr[i],cuc16X8ASCII);
		column = column +8;
    }
}
void MID_DISP_ShowCharStr6(unsigned char page,unsigned char column,unsigned char *pStr,unsigned char len)
{
    unsigned char i = 0;
	for(i = 0; i< len ;i++)
	{
		if((column + 6) > 128)
		{
			page = page +2;
			column = 0;
        }
		if((page + 1) > 7)
		{
			page = 0;
        }
		
    MID_DISP_ShowChar16X8(page,column,pStr[i],cuc16X8ASCII);
		column = column +6;
    }
}
void MID_DISP_ShowChar24X8(unsigned char page,unsigned char column,const unsigned char buufer[])
{
	unsigned int  x = 0;
	HAL_DISP_SetXY(page,column);
	HAL_DISP_WriteString(buufer+x,8);  
    x = x + 8;    
	HAL_DISP_SetXY(page+1,column);
	HAL_DISP_WriteString(buufer+x,8);    
    x = x + 8;    
	HAL_DISP_SetXY(page+2,column);
	HAL_DISP_WriteString(buufer+x,8);      
}
#if 0
void MID_DISP_ShowCharStr24X6(unsigned char page,unsigned char column,unsigned char *pStr,unsigned char len)
{
    unsigned char i = 0;
    unsigned char bu = 0;
//    unsigned char bu[13] = {0};
//    unsigned char value = 0;
//    if(len == 1)
//    {
//        bu[0] = 0;
//        bu[1] = '.';       
//        bu[2] = 0;
//        bu[3] = pStr[0];
//    }
//    if(len == 2)
//    {
//        bu[0] = 0;
//        bu[1] = '.';       
//        bu[2] = pStr[0];
//        bu[3] = pStr[1];       
//    }
//    if(len >= 3)
//    {
//        for(i = 0;i < len -3;i++)
//        {
//            bu[i] = pStr[i];
//        }
//        bu[i] = '.';
//        for(;i < len;i++)
//        {
//            bu[i+1] = pStr[i];
//        }
//    }
	for(i = 0; i< len ;i++)
	{      
        if(pStr[i] == '.')//2014-5-20
        {
					MID_DISP_ShowChar24X8(page,column,shuzibuf+10*24);
//					MID_DISP_ShowChar16X8(page, column, 0x2a, shuzibuf);
          column = column + 8;            
        }  
        else
       {        
            bu = pStr[i] - 0x30;
            MID_DISP_ShowChar24X8(page,column,shuzibuf+bu*24);
//						MID_DISP_ShowChar16X8(page, column, bu, shuzibuf);
            column = column + 8;
       }
    }
}
#endif
/**
 * @name  MID_DISP_ShowHZ16X16
 * @brief 显示ACII吗表中的汉字
 * @param  page ： 行数
 *         column：列数
 *         pt:     该汉字缓冲区的起始地址        
 * @author hanlei
 * @date 08-11-2012
**/
void MID_DISP_ShowHZ16X16(unsigned char page,unsigned char column, const unsigned char *pt)
{
	unsigned char  i=0;	
	for(i=0;i<2;i++)						//?????
	{
		HAL_DISP_SetXY(page+i ,column);
		HAL_DISP_WriteString(pt+i*12,12);      
	}	
}
void MID_DISP_ShowHZ24X24(unsigned char page,unsigned char column, const unsigned char *pt)
{
	unsigned char  i=0;	
	for(i=0;i<3;i++)						//?????
	{
		HAL_DISP_SetXY(page+i ,column);
		HAL_DISP_WriteString(pt+i*24,24);      
	}	
}

/**
 * @name  MID_DISP_HexNumber
 * @brief 显示ACII吗表中的英文字符
 * @param  page ： 行数
 *         column：列数
 *         len : 长度
 *         pASCIIchar:ACSII码表的起始地址。
 *        
 * @author hanlei
 * @date 08-11-2012
**/
#if 0
void MID_DISP_HexNumber(unsigned char page,unsigned char column,unsigned char *pHexNumBuf,unsigned char len)
{
	unsigned char buffer[128];//用来表示十六进制数字转换后的字符缓冲区
	unsigned char i,j,HalfByteH,HalfByteL;//半个字节对应的ASCII码
    for(i = 0,j = 0;i < len; i ++)
	{
		HalfByteH = pHexNumBuf[i] >> 4;
        HalfByteL = pHexNumBuf[i] & 0x0F;
		if(HalfByteH <= 9)
		{
			buffer[j] = HalfByteH + '0';
        }
 		if(HalfByteH >= 10 && HalfByteH <= 15)
		{
			buffer[j] = HalfByteH - 10 + 'A';
        }  
		if(HalfByteL <= 9)
		{
			buffer[j+1] = HalfByteL + '0';
        }
 		if(HalfByteL >= 10 && HalfByteL <= 15)
		{
			buffer[j+1] = HalfByteL - 10  + 'A';
        }	
        j = j +2;		
    }
	MID_DISP_ShowCharStr(page,column,buffer,len <<1);
}
#endif

void MID_DISP_ShowHZStr(unsigned char page,unsigned char column,const unsigned char *pStr,unsigned char len)
{
    unsigned char i = 0;
	for(i = 0; i< len ;i++)
	{
		if((column+16) > 128)
		{
			page = page +2;
			column = 0;
    }
		if((page + 1) > 7)
		{
			page = 0;
    }
		
    MID_DISP_ShowHZ16X16(page,column,pStr+(i << 5));
		column = column +16;
  }    
}

void DispalyPower(unsigned char value)
{
//    unsigned short loop = 0x200;
    if(value >= 5)
        value = 5;
    vLCD_Draw20X8(0, 106, cucCell + (value * 20));          
}

void BuleToothState(void)
{
    
}
void PowerCurrentState(void)
{
    
}
void PowerChangeState(void)
{
    
}

void Display_Init(void)
{
    ST7567_GPIO_Init_QY();
	SPI_Init_QY();
	ST7567_Power_On_QY();	
	  HAL_DISP_CLS();  
}
//*****************************end of file***********************************************
