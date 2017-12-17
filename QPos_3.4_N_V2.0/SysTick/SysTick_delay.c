/************************************************************************************************************
* 文件：	 SysTick_delay.C
* 功能描述： （1）使用SysTick的普通计数模式对延迟进行管理
*			 （2）包括delay_us,delay_ms
* 编写者：   老钟
************************************************************************************************************/
#include "SysTick_delay.h"
//static u8  Cali_us=0;//us延时倍乘数
//static u16 Cali_ms=0;//ms延时倍乘数
//extern volatile u32 TimingDelay;

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
///************************************************************************************
//*初始化延迟函数
//*SYSCLK:系统时钟(如：72MHz)
//*SYSTICK的时钟固定为HCLK时钟的1/8
//*demo  :  delay_init(72);	     //延时初始化	系统时钟为72MHz
//************************************************************************************/
//void Delay_Init(u8 SYSCLK)
//{
//	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8 = 9MHz
//	SysTick->CTRL |= CTRL_TICKINT_Set;
//	SysTick->VAL = SysTick_Counter_Clear;           //清空计数器
//
//	Cali_us=SYSCLK/8;		    
//	Cali_ms=(u16)Cali_us*1000;
//}
//
///************************************************************************************								    
//*延时nms(毫秒)
//*注意nms的范围
//*SysTick->LOAD为24位寄存器,所以,最大延时为:
//*nms<=0xffffff*8*1000/SYSCLK
//*SYSCLK单位为Hz,nms单位为ms
//
//*************************************************************************************/
//void Delay_ms(u16 nms)
//{	 		  	  
//
//	TimingDelay = nms;		   
//	SysTick->LOAD = Cali_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->CTRL |= SysTick_Counter_Enable ;          //开始倒数  
//	while(TimingDelay != 0);//等待时间到达   
//	SysTick->CTRL &= SysTick_Counter_Disable;       //关闭计数器
//	SysTick->VAL =SysTick_Counter_Clear;       //清空计数器
//
//} 
//
///************************************************************************************  
//*延时nus
//*nus为要延时的us数.
//*************************************************************************************/		    								   
//void Delay_us(u32 nus)
//{		
// 	TimingDelay = nus; 
//	SysTick->LOAD=Cali_us; //时间加载	  		 
//	SysTick->CTRL |= SysTick_Counter_Enable ;          //开始倒数  
//	while(TimingDelay != 0);//等待时间到达   
//	SysTick->CTRL &= SysTick_Counter_Disable;       //关闭计数器
//	SysTick->VAL =SysTick_Counter_Clear;       //清空计数器
//}

/************************************************************************************
*初始化延迟函数
*SYSCLK:系统时钟(如：72MHz)
*SYSTICK的时钟固定为HCLK时钟的1/8
*demo  :  delay_init(72);	     //延时初始化	系统时钟为72MHz
************************************************************************************/
void Delay_Init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}

/************************************************************************************								    
*延时nms(毫秒)
*注意nms的范围
*SysTick->LOAD为24位寄存器,所以,最大延时为:
*nms<=0xffffff*8*1000/SYSCLK
*SYSCLK单位为Hz,nms单位为ms
*对72M条件下,nms<=1864 
*************************************************************************************/
void Delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 

/************************************************************************************  
*延时nus
*nus为要延时的us数. 范围：< 1864135
*************************************************************************************/		    								   
void Delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}

/************************************************************************************  
*启动
*nus为要延时的us数. 范围：< 1864135
*************************************************************************************/

