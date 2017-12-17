//***************************************************************************************
//*  File Name		MSCard_PA1100.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#include "MSCard_PA1100.h"
#include "stm32f10x.h"
#include "SysTick_delay.h"
#include "string.h"
#include "Buzzer.h"

/******************************************************************************/
/* card data acquisition state */
/******************************************************************************/
#define MSR_CDAS_WAIT_FOR_CARD_PRESENT  0
#define MSR_CDAS_WAIT_FOR_BUFFER_READY  1
/******************************************************************************/
/* Mag Data */
/******************************************************************************/
#define MAG_DATA_PORT	GPIOB
#define MAG_DATA_PIN	GPIO_Pin_7

#define IO_PIN_MAG_DATA_LOW			((MAG_DATA_PORT->IDR & MAG_DATA_PIN) == 0)  
#define IO_PIN_MAG_DATA_HIGH		(!IO_PIN_MAG_DATA_LOW) 

#define IO_SET_MAG_DATA_LOW();		MAG_DATA_PORT->BRR = MAG_DATA_PIN;
#define IO_SET_MAG_DATA_HIGH();  	MAG_DATA_PORT->BSRR = MAG_DATA_PIN; 
/******************************************************************************/
/* Mag Strobe */
/******************************************************************************/
#define MAG_STROBE_PORT	GPIOB
#define MAG_STROBE_PIN	GPIO_Pin_6

#define IO_SET_MAG_STROBE_LOW();   MAG_STROBE_PORT->BRR = MAG_STROBE_PIN;
#define IO_SET_MAG_STROBE_HIGH();  MAG_STROBE_PORT->BSRR = MAG_STROBE_PIN;
/******************************************************************************/
/*add by Roc delay time */
/******************************************************************************/
#define TMR_vDelayms(a)	Delay_ms(a)	
#define TMR_vDelayus(a)	Delay_us(a)	
#define TMR_v250nsDelay()	__NOP();\
													__NOP();\
													__NOP();\
													__NOP();\
													__NOP();\
													__NOP();\
													__NOP();\
													__NOP();\
													__NOP();

static unsigned char MSR_ubCardDataAcquisitionState;
unsigned char MSR_fCardDataReady = FALSE;
/* MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal should be serviced every 100ms
   using a timer.
   When servicing MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal, it should be
   reduced by a count of 1 until it reaches zero at which time it should
   stay at zero.
   
   For example, the following code put into a timer interrupt routine that
   occurs every 100ms would do the job.
   
   if (MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal)
   {
      MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal--;
   }
   */
volatile unsigned int MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal;

/*add by roc*/
unsigned char MSR_ucTrack1Buf[MSR_TRACK_MAX_LENGTH];
unsigned char MSR_ucTrack1Len;
unsigned char MSR_ucTrack2Buf[MSR_TRACK_MAX_LENGTH];
unsigned char MSR_ucTrack2Len;
unsigned char MSR_ucTrack3Buf[MSR_TRACK_MAX_LENGTH];
unsigned char MSR_ucTrack3Len;
unsigned char MSR_ucCardNo[MSR_CARD_NO_MAX_LENGTH];
unsigned char MSR_ucCardNoLen;
/*if user swiping card, MSR_ucGetTrackData |= 0x80;
if user swiping card backward, MSR_ucGetTrackData |= 0x40;
if get track 1 data, MSR_ucGetTrackData |= 0x01;
if get track 2 data, MSR_ucGetTrackData |= 0x02;
if get track 3 data, MSR_ucGetTrackData |= 0x04.*/
unsigned char MSR_ucGetTrackData;
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static void MSR_vGPIO_Strobe_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
	GPIO_InitStructure.GPIO_Pin = MAG_STROBE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/*open drain*/
	GPIO_Init(MAG_STROBE_PORT, &GPIO_InitStructure);
	GPIO_SetBits(MAG_STROBE_PORT, MAG_STROBE_PIN);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc. mod  1--input, 0--output*/  
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static void MSR_vGPIO_Data_Config(unsigned char ucMod)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
	if(ucMod)
	{
	/*make data pin input*/
		GPIO_InitStructure.GPIO_Pin = MAG_DATA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		   /*oneshot strong pull-up*/
		GPIO_Init(MAG_DATA_PORT, &GPIO_InitStructure);
	}
	else
	{
	/*make data pin output*/
		GPIO_InitStructure.GPIO_Pin = MAG_DATA_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	 	/*open drain*/
		GPIO_Init(MAG_DATA_PORT, &GPIO_InitStructure);
		GPIO_SetBits(MAG_DATA_PORT, MAG_DATA_PIN);
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static void MSR_vTIM_NIVC(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;  			// 中断编号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 抢占优先级为1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		// 响应优先级为7级（子优先级）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// 
	NVIC_Init(&NVIC_InitStructure);		
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static void MSR_vTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);	
	/* TIM1 Peripheral Configuration ----------------------------------------*/
	/* Time Base configuration */
  //时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频7200+1) = 10kHz   
	TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);	
  //累计 TIM_Period个频率后产生一个更新或者中断	
	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//采样分频
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* TIM1 IT enable */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vInit(void)
{
   	/* this routine should be called once after power up during initialization */
	MSR_ubCardDataAcquisitionState = MSR_CDAS_WAIT_FOR_CARD_PRESENT;
	MSR_vTIM_Config();			 /*add by Roc*/
  	MSR_vTIM_NIVC();
	MSR_vGPIO_Strobe_Config();	 /*add by Roc*/
	MSR_vGPIO_Data_Config(0);	 /*add by Roc*/

	IO_SET_MAG_DATA_HIGH();
	IO_SET_MAG_STROBE_HIGH();

  /* Wait for 3 volt ASIC to complete power up reset. */
  /* need a minimum of 10ms (Trdy) delay here */
	TMR_vDelayms(10);

  /* Put 3 volt ASIC in new mode. */  
	MSR_vReset();
	MSR_vArm();
  	MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal = 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vCardDataAcquisitionManager(void)
{
   switch (MSR_ubCardDataAcquisitionState)//操作状态字节
   {
      case MSR_CDAS_WAIT_FOR_CARD_PRESENT://等待卡片的出现
         if (IO_PIN_MAG_DATA_LOW && !MSR_fCardDataReady)//DATA信号拉低表示卡片出现，MSR_fCardDataReady为零表示上一次数据已经由于复位被清除掉了。
         {
            MSR_vClearCardPresent();
            MSR_ubCardDataAcquisitionState = MSR_CDAS_WAIT_FOR_BUFFER_READY;
            MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal = 31; /* 3 seconds minimum */
         }
         break;

      case MSR_CDAS_WAIT_FOR_BUFFER_READY:
         if (IO_PIN_MAG_DATA_LOW)
         {
            MSR_fCardDataReady = TRUE;
            MSR_ubCardDataAcquisitionState = MSR_CDAS_WAIT_FOR_CARD_PRESENT;
         }
         else if (MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal == 0)
         {
            /* We only make it here if we timed out waiting for the buffer ready
            signal.  This time out is set at 3 seconds.  A user would have
            to swipe a card slower than 1 inch per second to trip this timeout.
            In this case, the card would not read anyway.  The most likely events
            that could cause this timeout would be an ESD strike that pulled 
            mag data line low or a power glitch that put the ASIC into its
            default mode (old mode). */
            MSR_vReset();
            MSR_vArm();
            MSR_ubCardDataAcquisitionState = MSR_CDAS_WAIT_FOR_CARD_PRESENT;
         }
         break;         
   }
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vClearCardPresent(void)
{  
   /* need a minimum of 20ns (Tsu_stbH) delay here */
   TMR_v250nsDelay();
   IO_SET_MAG_STROBE_HIGH();
   
   /* need a minimum of 1.1us (TstbH_CP) delay here */
   TMR_vDelayus(2);           
   IO_SET_MAG_STROBE_LOW();
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vGetData(unsigned char * pryubTkData, unsigned short uwCntInBits)
{
   unsigned char ubTkBitPos;
   
   ubTkBitPos = 1;
   *pryubTkData = 0; /* initialize */
   
   /* need a minimum of 250ns (TstbL) delay here */
   TMR_v250nsDelay();
   /* this delay is for the IO_SET_MAG_STROBE_HIGH(); statement in the while
      loop */
   
   while (uwCntInBits--)
   {
      /* need a minimum of 250ns (TstbL) delay here */
      /* This delay is already done when waiting for Tsd a few lines below
         this.  It is also done before the while loop the first time thru. */
      IO_SET_MAG_STROBE_HIGH();
      
      /* need a minimum of 250ns (TstbH) delay here */
      TMR_v250nsDelay();
      IO_SET_MAG_STROBE_LOW();;
      
      /* need a minimum of 170ns (Tsd) delay here*/
      TMR_v250nsDelay();
      if (IO_PIN_MAG_DATA_LOW) {
         *pryubTkData |= ubTkBitPos;
      }
                    
      ubTkBitPos <<= 1;
      if (ubTkBitPos == 0)
      {
//	  	 TMR_vDelay10us(); 
		 ubTkBitPos = 1;
         pryubTkData++;
         if (uwCntInBits)
         {
            *pryubTkData = 0; /* initialize */
         }
      }
   }
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vReset(void)
{
   	/* need a minimum of 250ns (TstbL) delay here */
   	TMR_v250nsDelay();
	MSR_vGPIO_Data_Config(0);	 /*add by Roc*/
	IO_SET_MAG_STROBE_HIGH();

	/* need a minimum of 170ns (Tsu1_rst) delay here */
	TMR_v250nsDelay();
	IO_SET_MAG_DATA_LOW();

	/* need a minimum of 4.5us (Tsu2_rst) delay here */
	TMR_vDelayus(6);
	IO_SET_MAG_STROBE_LOW();
   
	/* need a minimum of 250ns (TstbL) delay here */
	TMR_v250nsDelay();
	IO_SET_MAG_STROBE_HIGH();
   
	/* need a minimum of 250ns (TstbH) delay here */
	TMR_v250nsDelay(); 
	IO_SET_MAG_STROBE_LOW();
   
	/* need a minimum of 20ns (Tsu_mode) delay here */
	TMR_v250nsDelay();
//	IO_SET_MAG_DATA_HIGH();
	MSR_vGPIO_Data_Config(1);	 /*add by Roc*/

	/* need a minimum of 250ns (TstbL) delay here */
	/* need a minimum of 20ns (Th_mode) delay here */
	TMR_v250nsDelay();
	IO_SET_MAG_STROBE_HIGH();

	/* need a minimum of 12.75us (TstbH2) delay here */
	TMR_vDelayus(130);
//	TMR_vDelayus(10);
	IO_SET_MAG_STROBE_LOW();	
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vArm(void)
{
	/* need a minimum of 250ns (TstbL) delay here */
	TMR_v250nsDelay();
	IO_SET_MAG_STROBE_HIGH();
   
	/* need a minimum of 250ns (TstbH) delay here */
	TMR_v250nsDelay();
	IO_SET_MAG_STROBE_LOW();;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vClearCardData(void)
{
//    if (MSR_fCardDataReady)
//    {
      MSR_fCardDataReady = FALSE;
      MSR_vReset();
      MSR_vArm();      
//   }
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc. get MagTek--first 16 bit data*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iGetData_CheckMagTek(void)
{
	unsigned int uiTkBitPos; 
	unsigned int uiMagTek;
	unsigned char ucLoop;

	uiTkBitPos = 1;
	ucLoop = 16;
	uiMagTek = 0;
	while (ucLoop--)
   	{
      	/* need a minimum of 250ns (TstbL) delay here */
      	/* This delay is already done when waiting for Tsd a few lines below
         this.  It is also done before the while loop the first time thru. */
      	IO_SET_MAG_STROBE_HIGH();
      
      	/* need a minimum of 250ns (TstbH) delay here */
      	TMR_v250nsDelay();
      	IO_SET_MAG_STROBE_LOW();;
      
      	/* need a minimum of 170ns (Tsd) delay here*/
      	TMR_v250nsDelay();
      	if (IO_PIN_MAG_DATA_LOW) {
         	uiMagTek |= uiTkBitPos;
    }
		else{}
                    
      	uiTkBitPos <<= 1;
   	}
	/*get 16bit MagTek 0000 0000 0000 0011*/
	if(uiMagTek != 0xC000)
	{
		return (-1);
	}
	else{}

	return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc. check data odd*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static int MSR_iCheckData_OddAndLRC(unsigned char * pucData, int iDataLen, unsigned char ucTrackNo)
{
	int iDoLoop;
	unsigned char ucOdd;
	unsigned char ucLRC;
	unsigned char ucValidMask;
	unsigned char ucCheckBit;

	if((NULL == pucData) || (0 >= iDataLen))
	{
		return (0x01);
	}
	else{}

	switch(ucTrackNo)
	{
		case 1: 
			ucValidMask = 0x3f;
			ucCheckBit = 7;
			break;
		case 2:
		case 3:
			ucValidMask = 0x0f;
			ucCheckBit = 5;
			break;
		default:
			return (0x02);
	}

	iDataLen -= 1;		/*remove LRC byte*/
	ucLRC = 0;
	while(iDataLen)
	{
		iDataLen--;
		ucOdd = 0;
		iDoLoop = ucCheckBit;
		while(iDoLoop)
		{
			iDoLoop--;
			if((*pucData) & (0x01 << iDoLoop))
			{
				ucOdd++;  /*count "1" int a ucCheckBit*/
			}	
			else{}
		}
		if(!(ucOdd & 0x01))
		{
			return (0x03);		/*odd error*/
		}
		else{}

		ucLRC ^= (*pucData);
		pucData++;
	}	

	ucLRC &= ucValidMask;
	if(ucLRC != ((*pucData) & ucValidMask))
	{
		return (0x04);
	}	
	else{}

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by roc. */
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void MSR_vToASCII_Track2Or3(unsigned char * pucTrackData, unsigned char ucTrackLen)
{
	while(ucTrackLen)
	{
		ucTrackLen--;
		(*(pucTrackData + ucTrackLen)) &= 0x0f;
		(*(pucTrackData + ucTrackLen)) |= 0x30;
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by roc. */
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iMSCardTask(void)
{
	int iStatus;
	/* MSR_vCardDataAcquisitionManager() contains a state machine that
     services the ASIC.  This routine should be called on a periodic
     basis. */
//	MSR_ucGetTrackData = 0;
  MSR_vCardDataAcquisitionManager();
  	/* MSR_fCardDataReady gets set after a card has been swiped and the
     card data is ready to be extracted from the ASIC. */
  if(MSR_fCardDataReady)
  {
		/*someone has swipe card*/
		MSR_ucGetTrackData = 0x80;
		MSR_ucTrack1Len = 0;
		MSR_ucTrack2Len = 0;
		MSR_ucTrack3Len = 0;
		MSR_ucCardNoLen = 0;
			
    if(!MSR_iGetData_CheckMagTek()){}
		if(1)	
		{		
			iStatus = MSR_iGetData_Track1(MSR_ucTrack1Buf, &MSR_ucTrack1Len);	   //Track1
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack1Buf, &MSR_ucTrack1Len, 1);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x01;
				}
				else
				{
					MSR_ucTrack1Len = 0;
				}
			}
			else
			{
				MSR_ucTrack1Len = 0;
			}
			
			iStatus = MSR_iGetData_Track2Or3(MSR_ucTrack2Buf, &MSR_ucTrack2Len);	   //Track2
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack2Buf, &MSR_ucTrack2Len, 2);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x02;
				}
				else
				{
					MSR_ucTrack2Len = 0;
				}
			}
			else
			{
				MSR_ucTrack2Len = 0;
			}
			
			iStatus = MSR_iGetData_Track2Or3(MSR_ucTrack3Buf, &MSR_ucTrack3Len);	   //Track3
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack3Buf, &MSR_ucTrack3Len, 3);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x04;
				}
				else
				{
					MSR_ucTrack3Len = 0;	
				}
			}
			else
			{
				MSR_ucTrack3Len = 0;
			}
			
			if((MSR_ucGetTrackData & 0x02))
			{
				MSR_ucGetTrackData |= 0x40;
				MSR_vClearCardData();
				return (0);
			}
			else{}
			
			iStatus = MSR_iGetData_Track2Or3(MSR_ucTrack3Buf, &MSR_ucTrack3Len);	   //Track3
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack3Buf, &MSR_ucTrack3Len, 3);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x04;
				}
				else
				{
					MSR_ucTrack3Len = 0;
				}
			}
			else
			{
				MSR_ucTrack3Len = 0;
			}
			
			iStatus = MSR_iGetData_Track2Or3(MSR_ucTrack2Buf, &MSR_ucTrack2Len);	   //Track2
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack2Buf, &MSR_ucTrack2Len, 2);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x02;
				}
				else
				{
					MSR_ucTrack2Len = 0;
				}
			}
			else
			{
				MSR_ucTrack2Len = 0;
			}
			
 			iStatus = MSR_iGetData_Track1(MSR_ucTrack1Buf, &MSR_ucTrack1Len);	   //Track1
			if(0 == iStatus)
			{
				iStatus = MSR_iAnalyzingData_Track1Or2Or3(MSR_ucTrack1Buf, &MSR_ucTrack1Len, 1);
				if(0 == iStatus)
				{
					MSR_ucGetTrackData |= 0x01;
				}
				else
				{
					MSR_ucTrack1Len = 0;
				}
			}
			else
			{
				MSR_ucTrack1Len = 0;
			}	
		}
		else{}
		
		/* At this point the card data has been extracted from the ASIC into
			buffer ryubCardMsg. */
		if(MSR_ucGetTrackData & 0x02)
		{
			MSR_ucGetTrackData |= 0x40;
			MSR_vClearCardData();
			return (0);
		}
		else{}
		/* At this point the ASIC has been reset and armed to read the next 
			card. */
		iBuzzer_On(3);
		MSR_vClearCardData();
		return (2);
	}
	return (1);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by roc. */
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iGetCardNum(unsigned char * pucData, unsigned char * pucDataLen)
{
	unsigned char ucTrackLen;

	if((NULL == pucData) || (NULL == pucDataLen))
	{
		return (1);
	}
	else{}
	 
	MSR_vToASCII_Track2Or3(MSR_ucTrack2Buf, MSR_ucTrack2Len);
	MSR_vToASCII_Track2Or3(MSR_ucTrack3Buf, MSR_ucTrack3Len);
	ucTrackLen = 0;
	while(ucTrackLen <= MSR_ucTrack2Len)
	{
		if('=' == (*(MSR_ucTrack2Buf + ucTrackLen)))
		{
			/*remove ';' */
			ucTrackLen -= 1;
			break;	
		}
		else{}
		ucTrackLen++;	
	}	
	
	if(ucTrackLen > MSR_ucTrack2Len)
	{
		
		ucTrackLen = 0;
		while(ucTrackLen <= MSR_ucTrack3Len)
		{
			if('=' == (*(MSR_ucTrack3Buf + ucTrackLen)))
			{
				/*remove ';' '9' '9'*/
				ucTrackLen -= 3;
				break;	
			}
			else{}
			ucTrackLen++;	
		}

		if(ucTrackLen > MSR_ucTrack3Len)
		{
			return (1);
		}
		else
		{
			*pucDataLen	= ucTrackLen;
			memcpy(pucData, MSR_ucTrack3Buf + 3, ucTrackLen);
			MSR_ucCardNoLen	= ucTrackLen;
			memcpy(MSR_ucCardNo, MSR_ucTrack3Buf + 1, ucTrackLen);
//			memset(pucData + 6, '*', ucTrackLen - 10);
		}
	}
	else
	{
		*pucDataLen	= ucTrackLen;
		memcpy(pucData, MSR_ucTrack2Buf + 1, ucTrackLen);
		MSR_ucCardNoLen = ucTrackLen;
		memcpy(MSR_ucCardNo, MSR_ucTrack2Buf + 1, ucTrackLen);
//		memset(pucData + 6, '*', ucTrackLen - 10);
	}

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.get Track2 Or Track3 704 bit data. the buffer that pucTkData point must be 141 at least*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iGetData_Track2Or3(unsigned char * pucTkData, unsigned char *pucTkDataLen)
{
	int iStatus;
	unsigned char ubTkBitPos;
	unsigned char ubStartByte;
  int iCntInBits;
	iStatus = 0;
  ubTkBitPos = 1;
	ubStartByte = 0;
	iCntInBits = 704;

	if((NULL == pucTkData) || (NULL == pucTkDataLen))
	{
		return (0x01);
	}
	else{}
   
  /* need a minimum of 250ns (TstbL) delay here */
  TMR_v250nsDelay();
  /* this delay is for the IO_SET_MAG_STROBE_HIGH(); statement in the while loop */
	/* initialize */
	*pucTkData = 0; 
   	/*remove the noise,until get the ";"*/	
	while(iCntInBits)
  {
		iCntInBits--;
		ubStartByte >>= 1;
		ubStartByte &= 0x1f;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			/*store 1 bit "1" in bit4 of ubStartByte(bit0~bit7)*/
      ubStartByte |= 0x10;
    }
		else
		{
			/*get 1 bit "0" in bit4 of ubStartByte(bit0~bit7)*/
			ubStartByte &= 0x0f;
		}
		/*get the ';'*/
		if(0x0b == ubStartByte)
		{
			*pucTkData = ubStartByte;
			pucTkData++;
			*pucTkData = 0; /* initialize */
			break;	
		}
		else{}
  }

	if(0 >= iCntInBits)
	{
		*pucTkDataLen = 0;
		return (0x02);
	}
	else
	{}
		
	*pucTkDataLen = iCntInBits / 5 + 1;

	/*if next 5 bit is 11111, error*/
	ubTkBitPos = 1;
	ubStartByte = 0;
	while(iCntInBits)
  {
		iCntInBits--;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			ubStartByte |= ubTkBitPos;
		}
		else{}
                    
    ubTkBitPos <<= 1;
		/*get 5 bit and break*/
		if (ubTkBitPos == 0x20)
    {
		 	ubTkBitPos = 1;
			*pucTkData = ubStartByte;
			pucTkData++;
			*pucTkData = 0; /* initialize */
			break;
    }
		else{}
  }

	if(0x1f == ubStartByte)
	{
		iStatus |= 0x10;
		*pucTkDataLen = 0;
	}
	else{}

	if(0 >= iCntInBits)
	{
		*pucTkDataLen = 0;
		return (0x03);
	}
	else{}

	ubTkBitPos = 1;
	/*get other data (704 bit)*/
  while(iCntInBits)
  {
		iCntInBits--;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			*pucTkData |= ubTkBitPos;
		}
		else{}
                    
    ubTkBitPos <<= 1;
    if (ubTkBitPos == 0x20)
    {
			ubTkBitPos = 1;
      pucTkData++;
      *pucTkData = 0; /* initialize */
    }
		else{}
  }

	return iStatus;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.get Track2 Or Track3 704 bit data. the buffer that pucTkData point must be 141 at least*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iGetData_Track1(unsigned char * pucTkData, unsigned char *pucTkDataLen)
{
	int iStatus;
	unsigned char ubTkBitPos;
	unsigned char ubStartByte;
  int iCntInBits;
	iStatus = 0;
  ubTkBitPos = 1;
	ubStartByte = 0;
	iCntInBits = 704;

	if((NULL == pucTkData) || (NULL == pucTkDataLen))
	{
		return (0x01);
	}
	else{}
   
  /* need a minimum of 250ns (TstbL) delay here */
  TMR_v250nsDelay();
  /* this delay is for the IO_SET_MAG_STROBE_HIGH(); statement in the while loop */
	/* initialize */
	*pucTkData = 0; 
   	/*remove the noise,until get the ";"*/	
	while(iCntInBits)
  {
		iCntInBits--;
		ubStartByte >>= 1;
		ubStartByte &= 0x7f;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			/*store 1 bit "1" in bit6 of ubStartByte(bit0~bit7)*/
      ubStartByte |= 0x40;
    }
		else
		{
			/*get 1 bit "0" in bit6 of ubStartByte(bit0~bit7)*/
			ubStartByte &= 0x3f;
		}
		/*get the '%'*/
		if(0x45 == ubStartByte)
		{
			*pucTkData = ubStartByte;
			pucTkData++;
			*pucTkData = 0; /* initialize */
			break;	
		}
		else{}
  }

	if(0 >= iCntInBits)
	{
		*pucTkDataLen = 0;
		return (0x02);
	}
	else
	{}
		
	*pucTkDataLen = iCntInBits / 7 + 1;

	/*if next 7 bit is 11111, error*/
	ubTkBitPos = 1;
	ubStartByte = 0;
	while(iCntInBits)
  {
		iCntInBits--;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			ubStartByte |= ubTkBitPos;
		}
		else{}
                    
    ubTkBitPos <<= 1;
		/*get 7 bit and break*/
		if (ubTkBitPos == 0x80)
    {
		 	ubTkBitPos = 1;
			*pucTkData = ubStartByte;
			pucTkData++;
			*pucTkData = 0; /* initialize */
			break;
    }
		else{}
  }

	if(0x7f == ubStartByte)
	{
		iStatus |= 0x10;
		*pucTkDataLen = 0;
	}
	else{}

	if(0 >= iCntInBits)
	{
		*pucTkDataLen = 0;
		return (0x03);
	}
	else{}

	ubTkBitPos = 1;
	
	/*get other data (704 bit)*/
  while(iCntInBits)
  {
		iCntInBits--;
		/* need a minimum of 250ns (TstbL) delay here */
		/* This delay is already done when waiting for Tsd a few lines below
		 this.  It is also done before the while loop the first time thru. */
		IO_SET_MAG_STROBE_HIGH();
	
		/* need a minimum of 250ns (TstbH) delay here */
		TMR_v250nsDelay();
		IO_SET_MAG_STROBE_LOW();
	
		/* need a minimum of 170ns (Tsd) delay here*/
		TMR_v250nsDelay();
		if (IO_PIN_MAG_DATA_LOW) 
		{
			*pucTkData |= ubTkBitPos;
		}
		else{}
                    
    ubTkBitPos <<= 1;
    if (ubTkBitPos == 0x80)
    {
			ubTkBitPos = 1;
      pucTkData++;
      *pucTkData = 0; /* initialize */
    }
		else{}
  }

	return iStatus;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int MSR_iAnalyzingData_Track1Or2Or3(unsigned char * pucData, unsigned char * pucDataLen, unsigned char ucTrackNo)
{
	int iDoLoop;
	int iStatus;
	int iStartPos;
	unsigned char ucValidMask;
	unsigned char ucEndSentinel;
	unsigned char ucStartSentinel;
	if((NULL == pucData) || (NULL == pucDataLen))
	{
		return (0x01);
	}
	else{}

	switch(ucTrackNo)
	{
		case 1:
			ucValidMask = 0x3f;
			ucStartSentinel = 0x45;
			ucEndSentinel = 0x1f;
			break;
		case 2:		
		case 3:
			ucValidMask = 0x0f;
			ucStartSentinel = 0x0b;
			ucEndSentinel = 0x1f;
			break;
		default:
			*pucDataLen = 0;
			return (0x02); 
	}

	iDoLoop = 0;
	iStartPos = 0;
	ucStartSentinel &= ucValidMask;
	ucEndSentinel &= ucValidMask;
	
	if(ucStartSentinel != (*(pucData) & ucValidMask))
	{
		*pucDataLen = 0;
		return (0x03); 
	}
	else{}
	 
	while(iDoLoop < (*pucDataLen))
	{
		if(ucStartSentinel == (*(pucData + iDoLoop) & ucValidMask))
		{
			/*if data is Start Sentinel*/
			iStartPos = iDoLoop;
		} 	
		else{}
			
		if(ucEndSentinel == (*(pucData + iDoLoop) & ucValidMask))
		{
			/*if data is End Sentinel*/
			break;
		} 	
		else{}
		iDoLoop++;
	}

	if(iDoLoop >= (*pucDataLen))
	{
		*pucDataLen = 0;
		return (0x04);
	}
	else{}
		
	/*add End Sentinel*/	
	iDoLoop++;	
	/*the last data is End Sentinel, add the LRC*/
	iDoLoop++;	
	/* data:';' ~ '?' + LRC */	
	iDoLoop -= iStartPos;	 
	*pucDataLen = iDoLoop;
		
	/*the data has more than one Start Sentinel, copy data*/
	if(0 != iStartPos)
	{
		while(iDoLoop--)
		{
			*(pucData) = *(pucData + iStartPos);
			pucData++;
		}
		pucData -= *pucDataLen;
	}
	else{}

	iStatus = MSR_iCheckData_OddAndLRC(pucData, *pucDataLen, ucTrackNo);

	return iStatus << 8;
}
/*******************************end of  file**********************************************/
