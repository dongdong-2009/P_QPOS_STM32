#include "stm32f10x.h"
#include <string.h>
#include "ZXB_type.h"
#include "HAL_7816.h"
#include "SysTick_delay.h"
#include "IROM.h"
#include "Nfc_main.h"
//#include "smartcard.h"
//#include "SystemTickDelay\SysTick_delay.h"

APDU	T_APDU;	
APDU	g_APDU;
unsigned char g_Flag_InsertCard;
static __IO uint8_t SCData = 0;

unsigned char G_CardStats = 0;	
unsigned char g_NFC_State=0;

#define SCC_ERR_SW1 0x6F//FE
#define SCC_ERR_SW2 0x01//00

unsigned char sc_t1_seqReader;//¶Á¿¨Æ÷¿éĞòÁĞºÅ
unsigned char sc_t1_seqCard;  //¿¨Æ¬µÄ¿éĞòÁĞºÅ
//unsigned char scc_direct_flag;
unsigned int sc_delay_time;
unsigned int sc_STIBWTR_bak;
unsigned int sc_t1IFSC; //t1ÖĞ£¬¿¨Æ¬¿É½ÓÊÕĞÅÏ¢¿éµÄ×î´ó³¤¶È	
unsigned int sc_t1_ifsc_reset;	// ¶Á¿¨Æ÷Ö»ÓĞÔÚ¸´Î»ºóµÚÒ»ÌõÖ¸Áî¿ÉÒÔ·¢FISC

int test_flag =0;
unsigned int sc_tim_us ;//usÑÓÊ±±¶³ËÊı
unsigned int sc_tim_passed ;//½ÓÊÕatrÊ±ÒÑÓÃµÄÊ±¼ä
unsigned int sc_t1;	// Îª0±íÊ¾t=0£»Îª1±íÊ¾t=1
unsigned int sc_one_etu=80;

unsigned int sc_bwt;
unsigned int sc_cwt;
unsigned int sc_bwt_s;//S¿éÔö¼ÓµÈ´ıÊ±¼ä


unsigned int sc_di =1;
unsigned int sc_fi =372;	
unsigned int sc_wi =10;	
unsigned int sc_egt=0;		

unsigned int sc_globaltimeflag;

typedef struct _atr
{
	unsigned char TS;
	unsigned char T0;
	unsigned char TA1;
	unsigned char TB1;
	unsigned char TC1;
	unsigned char TD1;
	unsigned char TA2;
	unsigned char TB2;
	unsigned char TC2;
	unsigned char TD2;
	unsigned char TA3;
	unsigned char TB3;
	unsigned char TC3;
	unsigned char TD3;
	unsigned char TA4;
	unsigned char TB4;
	unsigned char TC4;		
	unsigned char T[15];	
	unsigned char TCK;
} ATR_STRUCT; 


#if 0
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef;
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN)  || ((MODE) == GPIO_Mode_OUT) || \
                            ((MODE) == GPIO_Mode_AF)|| ((MODE) == GPIO_Mode_AN))


/** @defgroup Output_Maximum_frequency_enumeration 
  * @{
  */ 
typedef enum
{ 
  GPIO_Speed_400KHz = 0x00, /*!< Very Low Speed */
  GPIO_Speed_2MHz   = 0x01, /*!< Low Speed */
  GPIO_Speed_10MHz  = 0x02, /*!< Medium Speed */
  GPIO_Speed_40MHz  = 0x03  /*!< High Speed */
}GPIOSpeed_TypeDef;


/** @defgroup Output_type_enumeration
  * @{
  */ 
typedef enum
{ GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;
#define IS_GPIO_OTYPE(OTYPE) (((OTYPE) == GPIO_OType_PP) || ((OTYPE) == GPIO_OType_OD))

  
#endif







#if 0
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
{
  uint32_t temp = 0x00;
  uint32_t temp_2 = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  assert_param(IS_GPIO_AF(GPIO_AF));
  
  temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
  GPIOx->AFR[GPIO_PinSource >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
  temp_2 = GPIOx->AFR[GPIO_PinSource >> 0x03] | temp;
  GPIOx->AFR[GPIO_PinSource >> 0x03] = temp_2;
}
#endif

/** 
  * @brief  AF 0 selection  
  */ 
#define GPIO_AF_RTC_50Hz      ((uint8_t)0x00)  /*!< RTC 50/60 Hz Alternate Function mapping */
#define GPIO_AF_MCO           ((uint8_t)0x00)  /*!< MCO Alternate Function mapping */
#define GPIO_AF_RTC_AF1       ((uint8_t)0x00)  /*!< RTC_AF1 Alternate Function mapping */
#define GPIO_AF_WKUP          ((uint8_t)0x00)  /*!< Wakeup (WKUP1, WKUP2 and WKUP3) Alternate Function mapping */
#define GPIO_AF_SWJ           ((uint8_t)0x00)  /*!< SWJ (SW and JTAG) Alternate Function mapping */
#define GPIO_AF_TRACE         ((uint8_t)0x00)  /*!< TRACE Alternate Function mapping */

/** 
  * @brief  AF 1 selection  
  */ 
#define GPIO_AF_TIM2          ((uint8_t)0x01)  /*!< TIM2 Alternate Function mapping */
/** 
  * @brief  AF 2 selection  
  */ 
#define GPIO_AF_TIM3          ((uint8_t)0x02)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4          ((uint8_t)0x02)  /*!< TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5          ((uint8_t)0x02)  /*!< TIM5 Alternate Function mapping */
/** 
  * @brief  AF 3 selection  
  */ 
#define GPIO_AF_TIM9           ((uint8_t)0x03)  /*!< TIM9 Alternate Function mapping */
#define GPIO_AF_TIM10          ((uint8_t)0x03)  /*!< TIM10 Alternate Function mapping */
#define GPIO_AF_TIM11          ((uint8_t)0x03)  /*!< TIM11 Alternate Function mapping */
/** 
  * @brief  AF 4 selection  
  */ 
#define GPIO_AF_I2C1          ((uint8_t)0x04)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF_I2C2          ((uint8_t)0x04)  /*!< I2C2 Alternate Function mapping */
/** 
  * @brief  AF 5 selection  
  */ 
#define GPIO_AF_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
/** 
  * @brief  AF 6 selection  
  */ 
#define GPIO_AF_SPI3          ((uint8_t)0x06)  /*!< SPI3 Alternate Function mapping */
/** 
  * @brief  AF 7 selection  
  */ 
#define GPIO_AF_USART1        ((uint8_t)0x07)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF_USART2        ((uint8_t)0x07)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF_USART3        ((uint8_t)0x07)  /*!< USART3 Alternate Function mapping */
/** 
  * @brief  AF 8 selection  
  */ 
#define GPIO_AF_UART4         ((uint8_t)0x08)  /*!< UART4 Alternate Function mapping */
#define GPIO_AF_UART5         ((uint8_t)0x08)  /*!< UART5 Alternate Function mapping */
/** 
  * @brief  AF 10 selection  
  */ 
#define GPIO_AF_USB           ((uint8_t)0xA)  /*!< USB Full speed device  Alternate Function mapping */
/** 
  * @brief  AF 11 selection  
  */ 
#define GPIO_AF_LCD           ((uint8_t)0x0B)  /*!< LCD Alternate Function mapping */
/** 
  * @brief  AF 12 selection  
  */ 
#define GPIO_AF_FSMC           ((uint8_t)0x0C)  /*!< FSMC Alternate Function mapping */
#define GPIO_AF_SDIO           ((uint8_t)0x0C)  /*!< SDIO Alternate Function mapping */
/** 
  * @brief  AF 14 selection  
  */ 
#define GPIO_AF_RI            ((uint8_t)0x0E)  /*!< RI Alternate Function mapping */

/** 
  * @brief  AF 15 selection  
  */ 
#define GPIO_AF_EVENTOUT      ((uint8_t)0x0F)  /*!< EVENTOUT Alternate Function mapping */
#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF_RTC_50Hz) || ((AF) == GPIO_AF_MCO)    || \
                          ((AF) == GPIO_AF_RTC_AF1)  || ((AF) == GPIO_AF_WKUP)   || \
                          ((AF) == GPIO_AF_SWJ)      || ((AF) == GPIO_AF_TRACE)  || \
                          ((AF) == GPIO_AF_TIM2)     || ((AF)== GPIO_AF_TIM3)    || \
                          ((AF) == GPIO_AF_TIM4)     || ((AF)== GPIO_AF_TIM9)    || \
                          ((AF) == GPIO_AF_TIM10)    || ((AF)== GPIO_AF_TIM11)   || \
                          ((AF) == GPIO_AF_I2C1)     || ((AF) == GPIO_AF_I2C2)   || \
                          ((AF) == GPIO_AF_SPI1)     || ((AF) == GPIO_AF_SPI2)   || \
                          ((AF) == GPIO_AF_USART1)   || ((AF) == GPIO_AF_USART2) || \
                          ((AF) == GPIO_AF_USART3)   || ((AF) == GPIO_AF_USB)    || \
                          ((AF) == GPIO_AF_LCD)      || ((AF) == GPIO_AF_RI)     || \
                          ((AF) == GPIO_AF_TIM5)     || ((AF) == GPIO_AF_SPI3)   || \
                          ((AF) == GPIO_AF_UART4)    || ((AF) == GPIO_AF_UART5)  || \
                          ((AF) == GPIO_AF_FSMC)     || ((AF) == GPIO_AF_SDIO)   || \
                          ((AF) == GPIO_AF_EVENTOUT))
//*****************
//check_atr
//warm_cold: NULL /0 COLD;    1 WARM
// 0 pass  
// 1 fail
// 2 tck error
//*****************
unsigned char SCC_Check_atr(unsigned char* ATR, unsigned int length,unsigned int warm_cold)
{
	ATR_STRUCT atr_bak;
	unsigned int len_in=0;
	unsigned int len_s=0;	
	unsigned char K=0;	
	unsigned char Y=0;		
	unsigned char newY=0;
	int i;	


	if(length<3)		
		return 1;
	sc_bwt_s =1;
	memset((unsigned char*)&atr_bak,0,33);
	atr_bak.TS=ATR[0]; // 3b/3f
	atr_bak.T0=ATR[1]; // T0

	len_s =2;
	len_in =2;
	
	K =atr_bak.T0 & 0x0f;			//length of historical bytes
	Y =atr_bak.T0 >> 4; 				//TA1- TD1
	
	while(Y)
	{
		newY = 0;
		for( i = 0; i < 4; i++)		//TA - TD
		{
			if(Y & 0x01)			
			{
				*(((unsigned char*)&atr_bak)+len_s) =ATR[len_in];	
				if(i==3) 			//TD
				{
					Y = ATR[len_in] >> 4;
					newY = 1;
				}
				len_in++;
			}
			len_s++;
			if(!newY) Y >>= 1;
		}
	}
	memcpy((unsigned char*)&atr_bak.T,ATR+len_in,K);

	//TD1 for T=0/T=1
	if((atr_bak.T0&0x80)!=0x00)
		{//TD1 LOW HALF BYTE
		sc_t1=atr_bak.TD1&0x01;//20140421   td1!=td2  only td1		
		}
	else
		{//NO TD1
		sc_t1=0;
		}
	
	if(sc_t1)	
		{
//		STICR2 |=_T; //T=1
		}
	else
		{
//		STICR2 &=(~_T);	//T=0	
		}

	//check TCK
	if(sc_t1==1||(len_in+K+1==length))
		{
		if(len_in+K+1!=length)
			return 1;
		atr_bak.TCK=0;
		for(i=1; i<length; i++) //CHECK T0 -TCK
			atr_bak.TCK ^=ATR[i];
		if(atr_bak.TCK !=0)
			return 2;
		atr_bak.TCK = ATR[len_in+K];		
		}
	else //T=0
		{
		if(len_in+K!=length)
			return 1;		
		}

	//TA1 //±äÆµ
	sc_fi=372;
	sc_di =1;
//	etu_one_for11 =360;
//	etu_half_for11 =100;//180;	
//	etu_halfmore_for11 =etu_one_for11;
	//system clock =24MHz     SCK =3MHz
	if(((atr_bak.T0&0x10)!=0)&&((atr_bak.TD1&0x10)!=0)&&((atr_bak.TA2&0x10)==0)) //          TA2' bit5==0
	{
		if((atr_bak.TA1!=0x11)&&(atr_bak.TA1!=0x12)&&(atr_bak.TA1!=0x13))
			return 1;	
		if(atr_bak.TA1==0x11) //bit5==0
			{
//			STIECR =372; //STIECR=FI/DI
			sc_di =1;
//			etu_one_for11 =360;
//			etu_half_for11 =100;//180;
//			etu_halfmore_for11 =etu_one_for11;
			}	
		if(atr_bak.TA1==0x12)
			{
//			STIECR =(372/2); //STIECR=FI/DI
			sc_di =2;
//			etu_one_for11 =170;
//			etu_half_for11 =40;//85;
//			etu_halfmore_for11 =etu_one_for11;
			}	
		if(atr_bak.TA1==0x13)
			{
//			STIECR =(372/4); //STIECR=FI/DI
			sc_di =4;
//			etu_one_for11 =74;//80;
//			etu_half_for11 =25;//40;
//			etu_halfmore_for11 =40;
			}
	}	
	sc_one_etu=(sc_fi/sc_di);        //one_etu = ((FI*64)/(DI*3))-4;
	
	//TB1
	if(((atr_bak.T0&0x20)==0x20)&&(atr_bak.TB1!=0)&&(warm_cold==0) )
		return 1;
	if(((atr_bak.T0&0x20)!=0x20)&&(warm_cold==0) )
		return 1;	

	//TC1 encodes the extra guard time integer (N)
	if((atr_bak.T0&0x40)==0x40)
		{
		sc_egt =atr_bak.TC1;
		if(atr_bak.TC1==0xff)
			{
			if(sc_t1)
				USART_SetGuardTime(SC_USART, 11);
			else
				USART_SetGuardTime(SC_USART, 12);
			}
		else
			{
			USART_SetGuardTime(SC_USART, atr_bak.TC1+12);
			}
		}
	else
		{
		USART_SetGuardTime(SC_USART, 12);
		}
//	g_gtr_STIGTR_bak	=STIGTR;
	
	//TD1	
	if(((atr_bak.T0&0x80)!=0x00)&&((atr_bak.TD1&0x0E)!=0x00))
		return 1;

	//TA2 ??
	if(((atr_bak.TD1&0x10)==0x10)&&((atr_bak.TA2&0x10)!=0x00))
		return 1;
	
	//TB2 UNSPORT
	if((atr_bak.TD1&0x20)!=0x00)//MUST NO TB2
		return 1;
	
	//TC2 WI    WORK WAITING TIME = 960*D*WI   ; DEFAULT WI=10
	sc_wi=10;
	if((atr_bak.TD1&0x40)==0x40)
		{		
		//if((atr_bak.TC2>0)&&(atr_bak.TC2<=10))
		if(atr_bak.TC2==10)//ONLY SUPPORT 10 
			{
			sc_wi =atr_bak.TC2;
			}
		else	
			{
			return 1;
			}
		}
//	STICR2&=~_WTEN;
//	STICWTR =960*sc_di*bak_wi+480*sc_di+110;
//	STICR2|=_WTEN;
	sc_cwt =960*sc_di*sc_wi+480*sc_di+110;
	//TD2       TD2&0x0F =01/0E
	if(((atr_bak.TD1&0x80)!=0x00))
		{
		if(((atr_bak.TD2&0x0F)!=0x01)&&((atr_bak.TD2&0x0F)!=0x0e))//not t0 or sc_t1
			return 1;
		if(((atr_bak.TD1&0x0F)==0x01)&&((atr_bak.TD2&0x0F)==0x0e))//td1=0 td2=1
			return 1;		
		}
	
	//TA3
	if(sc_t1)
		{
		if((atr_bak.TD2&0x10)==0x00) 
			sc_t1IFSC =0x20; //È±Ê¡Öµ
		else
			{
			if(((atr_bak.TA3)==0xFF)||((atr_bak.TA3)<0x10))	//refuse 0-0f, ff
				return 1;
			else
				sc_t1IFSC =atr_bak.TA3; 
			}
		}
	
	//TB3
	if(sc_t1)
		{
		if((atr_bak.TD2&0x20)==0x00) 
			{
			return 1; //sc_t1 must have TB3
			}
		else
			{
			unsigned int temp=1;
			int i;
			unsigned char t1BWI;
			unsigned char t1CWI;
			t1BWI=(atr_bak.TB3>>4); //0-4
			t1CWI=(atr_bak.TB3&0x0f); //0-5
			
			if((t1BWI>4)||(t1CWI>5))
				return 1;
			
			//refuse 2expCWI <=N+1
			temp=1;
			for(i=0;i<t1CWI;i++)
				{
				temp*=2;
				}
			sc_cwt =temp+11+4+1;

			if(temp<=(sc_egt+1)&&(sc_egt!=0xff))
				return 1;	

			temp=1;
			for(i=0;i<t1BWI;i++)
				{
				temp*=2;
				}

			//STIBWTR =(temp*960*372*sc_di)/sc_fi+11+sc_di*970;
			 sc_bwt=(temp*960*372*sc_di)/sc_fi+11+sc_di*970;
			}
		}

	//TC3
	if((atr_bak.TC3!=0x00)&&((atr_bak.TD2&0x40)==0x40))
		{
		return 1;
		}

/*	
	// 10.8test
	g_108_test =0;
	if((atr_bak.TS==0x3b)&&(atr_bak.T0==0xf0)&&(atr_bak.TB1==0x00)&&(atr_bak.TD1==0x91)
		&&(atr_bak.TA2==0x81)&&(atr_bak.TD2==0x31)&&(atr_bak.TA3==0xfe)&&(atr_bak.TB3==0x45))
		{
		if(((atr_bak.TA1==0x11)&&(atr_bak.TC1==0x08)&&(atr_bak.TCK==0x73))
			||((atr_bak.TA1==0x12)&&(atr_bak.TC1==0x1e)&&(atr_bak.TCK==0x66))
			||((atr_bak.TA1==0x13)&&(atr_bak.TC1==0x1e)&&(atr_bak.TCK==0x67)))
			{
			g_108_test =1;  //ÔÚ´Ë3ÌõATRÊ¹ÓÃ¹Ü½ÅÄ£Äâ£¬Ê¹¶Á¿¨Æ÷ÄÜ½ÓÊÕ10.8etuµÄ×Ö½Ú
			}
		}
	//CWT
	g_1790_test=0;
	if((atr_bak.TS==0x3b)&&(atr_bak.T0==0xe0)&&(atr_bak.TB1==0x00)&&(atr_bak.TC1==0xff)
		&&(atr_bak.TD1==0x81)&&(atr_bak.TD2==0x31)&&(atr_bak.TB3==0x00))
		g_1790_test=1;  //ÔÚ´ËATRÏÂĞŞÕıIFSµÄCWT³¬Ê±Ê±¼ä
*/	
	return 0;
}
/**
  * @brief  Receives a new data while the time out not elapsed.
  * @param  None
  * @retval An ErrorStatus enumuration value:
  *          - SUCCESS: New data has been received
  *          - ERROR: time out was elapsed and no further data is received
  */
  /*
static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut)
{
  uint32_t Counter = 0;

  while((USART_GetFlagStatus(SC_USART, USART_FLAG_RXNE) == RESET) && (Counter != TimeOut))
  {
    Counter++;
  }

  if(Counter != TimeOut)
  {
    *Data = (uint8_t)USART_ReceiveData(SC_USART);
    return SUCCESS;    
  }
  else 
  {
    return ERROR;
  }
}
*/
#define USART_rTS 1
#define USART_rT0 2
#define USART_rBWT 3
#define USART_rCWT 4
// *Data ÊÕµ½µÄÊı¾İ
// para ¸ø³ö²ÎÊı
//return 0 ½ÓÊÕ³É¹¦
#if 1
unsigned int USART_ByteReceive(unsigned char *Data,unsigned char para)
{
	unsigned int counter=0x100000;
	if(USART_rBWT==para)
		counter=0x1000000;
	while ((SC_USART->SR & USART_FLAG_RXNE) == 0 )
	{
		counter--;
		if(counter==0)
			return 1;
		if(!SC_Detect())
			return 1;
	}  	
	*Data =    (uint16_t)(SC_USART->DR & (uint16_t)0x01FF);
	return 0;
	
}
#else
unsigned int USART_ByteReceive(unsigned char *Data,unsigned char para)
{
	volatile unsigned int temp32;
	if(para == USART_rTS) // TS  400~40000¸öÊ±ÖÓÖÜÆÚ
		{
		//42500 CLK = 42500/4.6M =9239 us
		SysTick->LOAD=9239*sc_tim_us; //Ê±¼ä¼ÓÔØ	  		
		SysTick->VAL=0x00;        //Çå¿Õ¼ÆÊıÆ÷
		SysTick->CTRL=0x01 ;      //¿ªÊ¼µ¹Êı 	
		sc_tim_passed =20160*80*sc_tim_us;		//1etu=80us
		}
	else if(para == USART_rT0) // ATR  global 20160 etus   ;between characters 14400etus; 
		{
		sc_tim_passed-=SysTick->VAL;
		//14400 etus = 14400*372/4.6M =114 435 us
		temp32 = 114435*sc_tim_us; //Ê±¼ä¼ÓÔØ	  
		if(sc_tim_passed<temp32)
			temp32 =sc_tim_passed;
			if(temp32==0)
				temp32=1;
		SysTick->LOAD=temp32; //Ê±¼ä¼ÓÔØ	  		 
		SysTick->VAL=0x00;        //Çå¿Õ¼ÆÊıÆ÷
		SysTick->CTRL=0x01 ;      //¿ªÊ¼µ¹Êı 	 
		}	
	else if(para == USART_rBWT)  //waiting time extension(wtx)   wtx=bwt*m, wtx+(m*D*960)
		{
		//
		SysTick->LOAD=((sc_bwt+sc_di*990)*sc_bwt_s*sc_one_etu) ;//114435*sc_tim_us; //Ê±¼ä¼ÓÔØ	  		 
		SysTick->VAL=0x00;        //Çå¿Õ¼ÆÊıÆ÷
		SysTick->CTRL=0x01 ;      //¿ªÊ¼µ¹Êı 	 
		}	
	else// (para == USART_rCWT)   //work waiting time //WT=960*D*WI   ; between characters  WT+D*480 etus
		{
			if(sc_t1)
				temp32= ((sc_cwt+4+1)*sc_one_etu);
			else
				temp32= ((sc_egt+(sc_di*480)+110)*sc_one_etu);
		
		SysTick->LOAD=temp32; //Ê±¼ä¼ÓÔØ	  		 
		SysTick->VAL=0x00;        //Çå¿Õ¼ÆÊıÆ÷
		SysTick->CTRL=0x01 ;      //¿ªÊ¼µ¹Êı 	 
		}	


	
	while ((SC_USART->SR & USART_FLAG_RXNE) == 0 )
		{
		
		temp32=SysTick->CTRL;
		if((temp32&0x01&&!(temp32&(1<<16))) == 0 )//³¬Ê±
			{
			SysTick->CTRL=0x00;       //¹Ø±Õ¼ÆÊıÆ÷
			SysTick->VAL =0X00;       //Çå¿Õ¼ÆÊıÆ÷	  
			return -2;
			}
		}
  	
	*Data =    (uint16_t)(SC_USART->DR & (uint16_t)0x01FF);
	return 0;
	
}

#endif
/**
  * @brief  Resends the byte that failed to be received (by the Smartcard) correctly.
  * @param  None
  * @retval None
  */
void SC_ParityErrorHandler(void)
{
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  } 
}

/**
  * @brief  This function handles SC_USART global interrupt request.
  * @param  None
  * @retval None
  */
void SC_USART_IRQHandler(void)
{
  /* If a Frame error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART);

    /* Resend the byte that failed to be received (by the Smartcard) correctly */
    SC_ParityErrorHandler();
  }
  
  /* If the SC_USART detects a parity error */
  if(USART_GetITStatus(SC_USART, USART_IT_PE) != RESET)
  {
    /* Enable SC_USART RXNE Interrupt (until receiving the corrupted byte) */
    USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);
    /* Flush the SC_USART DR register */
    USART_ReceiveData(SC_USART);
  }
  
  if(USART_GetITStatus(SC_USART, USART_IT_RXNE) != RESET)
  {
    /* Disable SC_USART RXNE Interrupt */
    USART_ITConfig(SC_USART, USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART);
  }
  
  /* If a Overrun error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_ORE) != RESET)
  {
    USART_ReceiveData(SC_USART);
  }
  /* If a Noise error is signaled by the card */
  if(USART_GetITStatus(SC_USART, USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART);
  }
}


void SC_DetectPinConfig(void)
{
  //EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 // NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable SC_PIN_OFF_GPIO clock */
  RCC_AHBPeriphClockCmd(SC_PIN_OFF_GPIO_CLK , ENABLE);
    
  /* Configure Smartcard OFF pin */
  GPIO_InitStructure.GPIO_Pin = SC_PIN_OFF;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(SC_PIN_OFF_GPIO, &GPIO_InitStructure);
  
  /* Configure EXTI line connected to Smartcard OFF Pin */
  //GPIO_EXTILineConfig(SC_DETECT_PIN, SC_DETECT_GPIO);

  //EXTI_ClearITPendingBit(SC_DETECT_EXTI);
  
 // EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 // EXTI_InitStructure.EXTI_Line = SC_DETECT_EXTI;
  //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);

  /* Clear the SC_DETECT_EXTI_IRQ Pending Bit */
 // NVIC_ClearPendingIRQ(SC_DETECT_IRQ);

  //NVIC_InitStructure.NVIC_IRQChannel = SC_DETECT_IRQ;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);    
}

void SC_Reset(BitAction ResetState)
{
  	GPIO_WriteBit(SC_PIN_RESET_GPIO, SC_PIN_RESET, ResetState);
}

static void SC_VoltageConfig(uint32_t SC_Voltage)
{
	switch(SC_Voltage)
	{
		case SC_VOLTAGE_3V:
			GPIO_ResetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V);
			//GPIO_SetBits(SC_PIN_1_8V_GPIO, SC_PIN_1_8V);
			break;
		case SC_VOLTAGE_1_8V:
			GPIO_SetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V);
			//GPIO_ResetBits(SC_PIN_1_8V_GPIO, SC_PIN_1_8V);
			break;
		case SC_VOLTAGE_5V:
		default:
			 /* Select Smartcard 5V */  
		    	GPIO_SetBits(SC_PIN_3_5V_GPIO, SC_PIN_3_5V);
		 	//GPIO_SetBits(SC_PIN_1_8V_GPIO, SC_PIN_1_8V);
			break;
	}
}

void SC_PowerCmd(unsigned char  NewState)
{
	  if(NewState)
	  {
		GPIO_ResetBits(SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC);
	  }
	  else
	  {
		GPIO_SetBits(SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC);
	  } 
}


void HAL_7816_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  USART_ClockInitTypeDef USART_ClockInitStructure;  
//	  NVIC_InitTypeDef NVIC_InitStructure;
	  
	  /* Enable GPIO clocks */
	  RCC_AHBPeriphClockCmd(RCC_APB2Periph_AFIO|SC_PIN_3_5V_GPIO_CLK | SC_PIN_RESET_GPIO_CLK, ENABLE);
	//   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//Enabled);
	   
	  /* Enable USART clock */
	  RCC_APB2PeriphClockCmd(SC_USART_CLK, ENABLE);

	                          
	 /* Configure USART CK pin as alternate function push-pull */
	  GPIO_InitStructure.GPIO_Pin = SC_USART_PIN_CK;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(SC_USART_GPIO, &GPIO_InitStructure);

	  /* Configure USART Tx pin as alternate function open-drain */
	  GPIO_InitStructure.GPIO_Pin = SC_USART_PIN_TX;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	  GPIO_Init(SC_USART_GPIO, &GPIO_InitStructure);

	  /* Configure Smartcard Reset pin */
	  GPIO_InitStructure.GPIO_Pin = SC_PIN_RESET;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(SC_PIN_RESET_GPIO, &GPIO_InitStructure);
		GPIO_SetBits(SC_PIN_RESET_GPIO,SC_PIN_RESET);
	  /* Configure Smartcard 3/5V pin */
	  GPIO_InitStructure.GPIO_Pin = SC_PIN_3_5V;
	  GPIO_Init(SC_PIN_3_5V_GPIO, &GPIO_InitStructure);

	  /* Configure Smartcard 1.8V pin */
	//  GPIO_InitStructure.GPIO_Pin = SC_PIN_1_8V;
	//  GPIO_Init(SC_PIN_1_8V_GPIO, &GPIO_InitStructure);

	  /* Configure Smartcard CMDVCC pin */
	  GPIO_InitStructure.GPIO_Pin = SC_PIN_CMDVCC;
	  GPIO_Init(SC_PIN_CMDVCC_GPIO, &GPIO_InitStructure);

//	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_USART1);
//	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
//RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);

	  SC_DetectPinConfig();
#if 0
	  /* Enable USART IRQ */
	  NVIC_InitStructure.NVIC_IRQChannel = SC_USART_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	  NVIC_Init(&NVIC_InitStructure);
#endif    
	/* SC_USART configuration ----------------------------------------------------*/
	  /* SC_USART configured as follow:
	        - Word Length = 9 Bits
	        - 0.5 Stop Bit
	        - Even parity
	        - BaudRate = 9677 baud
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Tx and Rx enabled
	        - USART Clock enabled
	  */

	  /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
	  //USART_SetPrescaler(SC_USART, 0x05);
	  //USART_SetPrescaler(SC_USART, 0x08); //64/(2*2)/4
	    USART_SetPrescaler(SC_USART, 0x0A);
	    
	  /* USART Guard Time set to 12 Bit */
	  USART_SetGuardTime(SC_USART, 12);
	  
	  USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
	  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
	  USART_ClockInit(SC_USART, &USART_ClockInitStructure);


	  USART_InitStructure.USART_BaudRate = 9600;//8600;//8585;//
	  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1_5;//USART_StopBits_0_5;//
	  USART_InitStructure.USART_Parity = USART_Parity_Even;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_Init(SC_USART, &USART_InitStructure); 

	  /* Enable the SC_USART Parity Error Interrupt */
	//  USART_ITConfig(SC_USART, USART_IT_PE, ENABLE);

	  /* Enable the SC_USART Framing Error Interrupt */
	//  USART_ITConfig(SC_USART, USART_IT_ERR, ENABLE);

	  /* Enable SC_USART */
	  USART_Cmd(SC_USART, ENABLE);

	  /* Enable the NACK Transmission */
	  USART_SmartCardNACKCmd(SC_USART, ENABLE);

	  /* Enable the Smartcard Interface */
	  USART_SmartCardCmd(SC_USART, ENABLE);
	  
	  /* Set RSTIN HIGH */  
	  SC_Reset(Bit_SET);
	  
	  /* Select 5V */ 
	  SC_VoltageConfig(SC_VOLTAGE_5V);
	  
	  /* Disable CMDVCC */
	  SC_PowerCmd(0);
	return;
}

unsigned char SC_Detect(void)
{
  return GPIO_ReadInputDataBit(SC_PIN_OFF_GPIO, SC_PIN_OFF);
}

int HAL_7816_PowerOn(unsigned char *pATR, unsigned char *pATRlen)
{
	unsigned char pATRBuf[40];
	unsigned char ATRBuflen;
	unsigned char i;
	
	sc_tim_us =4;
	g_Flag_InsertCard = 0;

	if(NULL == pATR)
	{
		pATR = pATRBuf;
	}

	if(NULL == pATRlen)
	{
		pATRlen = &ATRBuflen;
	}
	
	
	*pATRlen = 0;
	pATR[0] = 0;
	
	//SC_PowerCmd(1);	
	
 if (SC_Detect())
	{
		SC_PowerCmd(1);	
		GPIO_ResetBits(SC_PIN_RESET_GPIO,SC_PIN_RESET);
	// 1clk = 1/3.6M = 0.28us
	//   1etu =1/9600 =104us
	//vcc ÖÁrstÀ­¸ß¼ä¸ôÊ±¼äÎª40000~45000¸öÊ±ÖÓÖÜÆÚ
	Delay_ms(12);
		//SC_PowerCmd(1);
		GPIO_SetBits(SC_PIN_RESET_GPIO,SC_PIN_RESET);
		Delay_us(70);
//		Delay_ms(120);
		
		if((USART_ByteReceive(pATR, USART_rTS))!= 0)
			return 1;

		if(pATR[0]==3)
			{
			pATR[0] =0x3f;
			SC_USART->CR1 &=~USART_Parity_Even;
			SC_USART->CR1 |=USART_Parity_Odd;			
			}
		if((pATR[0] != 0x3b) && (pATR[0] != 0x3f))
		{
			return 1;	
		}

		for(i = 1; i< 40; i++)
		{
			if((USART_ByteReceive(pATR+i, USART_rT0)) != 0)
//				goto ATR_TIME;
				break;
		}
//ATR_TIME:		
		*pATRlen = i;
		i =SCC_Check_atr(pATR,i,0);

		return i;
	}

	return 1;

}

/**
  * @brief  Manages the Smartcard transport layer: send APDU commands and receives
  *   the APDU responce.
  * @param  SC_ADPU: pointer to a SC_ADPU_Commands structure which will be initialized.  
  * @param  SC_Response: pointer to a SC_ADPU_Responce structure which will be initialized.
  * @retval None
  */
static void SC_SendData0(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus)
{
  uint32_t i = 0;
  uint8_t locData = 0;
	
  /* Reset responce buffer ---------------------------------------------------*/
  for(i = 0; i < LC_MAX; i++)
  {
    SC_ResponceStatus->Data[i] = 0;
  }
  
  SC_ResponceStatus->SW1 = 0;
  SC_ResponceStatus->SW2 = 0;

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
     in case of a framing error FE */  
  USART_DMACmd(SC_USART, USART_DMAReq_Rx, ENABLE);

  /* Send header -------------------------------------------------------------*/
  SCData = SC_ADPU->Header.CLA;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }  
  
  SCData = SC_ADPU->Header.INS;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }
   
  SCData = SC_ADPU->Header.P1;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  } 
  
  SCData = SC_ADPU->Header.P2;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }   

  /* Send body length to/from SC ---------------------------------------------*/
  if(SC_ADPU->Body.LC)
  {
    SCData = SC_ADPU->Body.LC;
    USART_SendData(SC_USART, SCData);
    while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
    {
    }     
  }
 // else if(SC_ADPU->Body.LE)
  else
  { 
    SCData = SC_ADPU->Body.LE;
    USART_SendData(SC_USART, SCData);
    while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
    {
    }     
  }
  /* Flush the SC_USART DR */
 (void)USART_ReceiveData(SC_USART);
//    locData= (uint8_t)USART_ReceiveData(SC_USART);
  if(locData!=SCData)
  	{
  	locData =0;
  	}
USART_ClearFlag(SC_USART, USART_FLAG_RXNE);
  /* --------------------------------------------------------
    Wait Procedure byte from card:
    1 - ACK
    2 - NULL
    3 - SW1; SW2
   -------------------------------------------------------- */
  if((USART_ByteReceive(&locData, USART_rBWT)) == 0)
  {  
    if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
    {
      /* SW1 received */
      SC_ResponceStatus->SW1 = locData;

      if((USART_ByteReceive(&locData, USART_rCWT)) == 0)
      {
        /* SW2 received */
        SC_ResponceStatus->SW2 = locData;
      }
    }

	
    else if (((locData & (uint8_t)0xFE) == (((uint8_t)~(SC_ADPU->Header.INS)) & \
             (uint8_t)0xFE))||((locData & (uint8_t)0xFE) == (SC_ADPU->Header.INS & (uint8_t)0xFE)))
    {
      SC_ResponceStatus->Data[0] = locData;/* ACK received */
    }

	
  }

  /* If no status bytes received ---------------------------------------------*/
  if(SC_ResponceStatus->SW1 == 0x00)
  {
    /* Send body data to SC--------------------------------------------------*/
    if (SC_ADPU->Body.LC)
    {
      for(i = 0; i < SC_ADPU->Body.LC; i++)
      {
        SCData = SC_ADPU->Body.Data[i];
        
        USART_SendData(SC_USART, SCData);
        while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
        {
        } 
      }
      /* Flush the SC_USART DR */
      (void)USART_ReceiveData(SC_USART);
      /* Disable the DMA Receive (Reset DMAR bit only) */  
     USART_DMACmd(SC_USART, USART_DMAReq_Rx, DISABLE);
    }

    /* Or receive body data from SC ------------------------------------------*/
    else if (SC_ADPU->Body.LE)
    {
      for(i = 0; i < SC_ADPU->Body.LE; i++)
      {
        if(USART_ByteReceive(&locData, USART_rCWT) == 0)
        {
          SC_ResponceStatus->Data[i] = locData;
        }
      }
    }
    /* Wait SW1 --------------------------------------------------------------*/
      if(USART_ByteReceive(&locData, USART_rBWT) == 0)
      {
        SC_ResponceStatus->SW1 = locData;
      }
      else
      {
      }

    /* Wait SW2 ------------------------------------------------------------*/   
      if(USART_ByteReceive(&locData, USART_rCWT) == 0)
      {
        SC_ResponceStatus->SW2 = locData;
      }
      else
      {
      }

  }
}
void HAL_Card_APDU(APDU *pAPDU, unsigned short len)
{
	unsigned char len_61;


	
	SC_ADPU_Commands SC_ADPU;
	SC_ADPU_Responce SC_ResponceStatus;
	if(NFC_USE==Device_NFC_State())
	{
		if(g_NFC_State)
		{
			if(NFC_Card_APDU(pAPDU))
				g_NFC_State=0;
			return;
		}
	}
	
	Delay_us(100);//½â¾öÓĞĞ©¿¨¶Á¿¨ÎÊÌâ
	SC_ADPU.Header.CLA=pAPDU->CLA;
	SC_ADPU.Header.INS=pAPDU->INS;
	SC_ADPU.Header.P1=pAPDU->PP1;
	SC_ADPU.Header.P2=pAPDU->PP2;
	SC_ADPU.Body.LC=pAPDU->LC;
	SC_ADPU.Body.LE=pAPDU->LE;

	if(pAPDU->LC)
	{
		memcpy(SC_ADPU.Body.Data, pAPDU->pInData, pAPDU->LC);	
	}
	
	SC_SendData0(&SC_ADPU,&SC_ResponceStatus);

	if(pAPDU->LE)
	{
		memcpy(pAPDU->pOutData, SC_ResponceStatus.Data,SC_ADPU.Body.LE);	
	}

	pAPDU->SW1=SC_ResponceStatus.SW1;
	pAPDU->SW2=SC_ResponceStatus.SW2;
	pAPDU->RLE=SC_ADPU.Body.LE;	


	if((SC_ResponceStatus.SW1==0x90)&&(SC_ResponceStatus.SW2==0x00))
		{
		return;
		}

	else
		{	
		if(SC_ResponceStatus.SW1==0x6c)
			{
			SC_ADPU.Body.LC=0;	
			SC_ADPU.Body.LE= SC_ResponceStatus.SW2;
			SC_SendData0(&SC_ADPU,&SC_ResponceStatus);
			memcpy(pAPDU->pOutData, SC_ResponceStatus.Data,SC_ADPU.Body.LE);	
			pAPDU->SW1=SC_ResponceStatus.SW1;
			pAPDU->SW2=SC_ResponceStatus.SW2;
			pAPDU->RLE=SC_ADPU.Body.LE;			
			}
		len_61=0;
		while(SC_ResponceStatus.SW1==0x61)
			{
				
			if(pAPDU->LC==0x0e)
			{
				int ccc;
				ccc=0;
				test_flag=1;
			}				
				
			SC_ADPU.Header.CLA=0x00;
			SC_ADPU.Header.INS=0xc0;  //Get Response
			SC_ADPU.Header.P1=0x00;
			SC_ADPU.Header.P2=0x00;
			SC_ADPU.Body.LC= 0;	
			SC_ADPU.Body.LE= SC_ResponceStatus.SW2;
			SC_SendData0(&SC_ADPU,&SC_ResponceStatus);

			if(SC_ADPU.Body.LE )
				memcpy(pAPDU->pOutData+len_61, SC_ResponceStatus.Data,SC_ADPU.Body.LE);	
			len_61 =len_61+SC_ADPU.Body.LE;
			pAPDU->RLE=len_61;
			pAPDU->SW1=SC_ResponceStatus.SW1;
			pAPDU->SW2=SC_ResponceStatus.SW2;
			
			}

		}

	if(SC_ResponceStatus.SW1!=0x90)
		{
		if(SC_ResponceStatus.SW1 ==0x6a)
				return;
		else
				return;
		}


	

}


int AU9541_ICCARD_Reset(unsigned char *pucATR, unsigned char *puiATRLen)
{
	return HAL_7816_PowerOn(pucATR, puiATRLen);
}

void send_T0wait(unsigned char time)
{
//CCIDÖ¸ÁîÑÓÊ±Ğèµ÷ÓÃ´Ëº¯Êı£¬Ê¹ÓÃÒôÆµ¿ÚÎŞĞè´Ëº¯Êı
/*
	response_data[0] = RDR_to_PC_DataBlock;		
	response_data[1]=0;
	response_data[2]=0;
	response_data[3]=0;
	response_data[4]=0;
	
	response_data[7]=0x80; //bstatus TimeExtension
	response_data[8]=time;		//bError
	
	response_data[9]= 0;

	ep1_send(10);
	*/
}
/*
void init_delay0 (time)
{
	sc_delay_time =time;
}
void wait_delay()
{
	PIT0_PCSR = 0x0313;			//8·ÖÆµ£¬×Ô¶¯ÖØĞÂ¼ÓÔØ, PIT1Ê¹ÄÜ
	PIT0_PMR  = sc_delay_time;
	while((PIT0_PCSR&0x0004)==0);
	PIT0_PCSR = 0;  		//½ûÖ¹PITµÄÔËĞĞÎ»ºÍÖĞ¶ÏÊ¹ÄÜÎ»
	PIT0_PCSR |= 0x0004;  	
}
*/
unsigned char HAL_7816_LRC8( unsigned char * aData, unsigned long aSize )
{
	unsigned char lrc =0x00;
	int i;
	for(i=0; i<aSize; i++)
		lrc ^=aData[i];
	return lrc;
}
int HAL_7816_T1_ReBuild_R(unsigned char *data_out,unsigned short *data_out_len,unsigned char pcb)
{
	data_out[0] = 0;
	data_out[1] = pcb;
	data_out[2] = 0;//LEN;
	data_out[3] =HAL_7816_LRC8(data_out,3);
	*data_out_len=4;
	return 0;
}

int HAL_7816_T1_Build_R(unsigned char *data_out,unsigned short *data_out_len,unsigned char *bak_pcb,unsigned char pcb)
{
	if((sc_t1_seqCard&0x01)==0x00)
		pcb &=	(~0x10);
	else
		pcb |=	0x10;
	
	data_out[0] = 0;
	data_out[1] = pcb;
	data_out[2] = 0;//LEN;
	data_out[3] =HAL_7816_LRC8(data_out,3);
	*bak_pcb=data_out[1];
	*data_out_len=4;
	return 0;
}
//data_in ³¤¶ÈÎª5µÄs¿é
int HAL_7816_T1_Build_S(unsigned char *data_out,unsigned short *data_out_len,unsigned char *bak_pcb,unsigned char  *data_in)
{
	unsigned char sc_bwt_s;
	if((data_in[1]&0x1f)==3)//bwt
		{
		sc_bwt_s =data_in[3];
		if(sc_bwt_s==0)
			sc_bwt_s=1; //±¶ÊıÔÚ½ÓÊÕÖĞ´¦Àí£¬ÓÃÍêÒ»´ÎºóĞè»Ö¸´Îª1
		send_T0wait(sc_bwt_s); 
		data_out[0] = 0;
		data_out[1] = (data_in[1]|0x20);	
		data_out[2] = 1;				
		data_out[3] = data_in[3];				
		data_out[4] =HAL_7816_LRC8(data_out,1+3);			
		 *bak_pcb=data_out[1];
		}
	
	else if((data_in[1]&0x1f)==1)//ifsc
		{
		sc_t1IFSC =data_in[3];
		data_out[1]=0xe1;
		data_out[2]=1;
		data_out[3]=data_in[3];
		data_out[4]=HAL_7816_LRC8(data_out,4);
		 *bak_pcb=data_out[1];
		}
	*data_out_len=5;
	return 0;
}
//reader cmd len
//data_out Êä³ö¿éÊı¾İ
//data_out_len¿éÊı¾İ³¤¶È
//bak_pcb±¾¿épcb±¸·İ
//data_in Õû¸öcmd
//data_in_len cmd³¤¶È
//length_i ±¾¿éÊı¾İÇø³¤¶È
//position ËùÔÚcmdÖĞµÄÎ»ÖÃ
//chain ÊÇ·ñÎª×îºóÒ»¿é
int HAL_7816_T1_Build_I(unsigned char *data_out,unsigned short *data_out_len,unsigned char *bak_pcb,
	unsigned char *data_in,unsigned short data_in_len,unsigned char *length_i,unsigned char position,unsigned char *chain)
{
	unsigned char NAD =0;
	unsigned char PCB =0;
	unsigned char LEN =0;
	//IFSCÊÇ¿¨ÄÜ¹»½ÓÊÕµÄ¸÷¿éÖĞµÄĞÅÏ¢ÓòµÄ×î´ó³¤¶È ,
	if((data_in_len-position)<=sc_t1IFSC)
		{
		LEN =(data_in_len-position);
		PCB &=(~0x20); // END	
		*chain=0;
		}
	else
		{
		LEN =sc_t1IFSC;
		PCB |=0x20; //NOT END
		*chain=1;
		}

	if((sc_t1_seqReader&0x01)==0)
		PCB &=	(~0x40);
	else
		PCB |=	0x40;
		
	data_out[0] = NAD;
	data_out[1] = PCB;	
	data_out[2] = LEN;
	memcpy(data_out+3,data_in+position,LEN);
	data_out[3+LEN] =HAL_7816_LRC8(data_out,LEN+3);
	*bak_pcb=data_out[1];
	*data_out_len =LEN+4;
	*length_i =LEN;
	return 0;
}

int HAL_7816_Transmit1(unsigned char *cmd, unsigned int len, unsigned char *response, unsigned short* pOutlen)
{
	unsigned int i,para = 0;
	unsigned char parabyte;
	unsigned int err_para=0;

	*pOutlen =0;

	// ¿é¼ä¸ôÑÓÊ±20etu
	
	for(i=0;i<len;i++) 
		{
		USART_SendData(SC_USART, cmd[i] );
		}
	
	//nad=0
	//pcb //0XXX XXXX-I;     10XX XXXX  -R ;  11XX XXXX-S
	//head
	para =USART_rBWT;//BWT
	for(i=0;i<3;i++)
		{
		//response[i]=SCC_ReceiveByte(&para);
		if(USART_ByteReceive(response+i,USART_rBWT) !=0)
			{
			err_para=1;
			para=0;
			if(sc_globaltimeflag==2)
				{
				
				return -5;//power off
				}
			}
		para =USART_rCWT;	
		}

	//check len=ff
	//if((response[2]==0xff)&&(STIGTR ==11))
	if(response[2]==0xff)	
		{
		
		return -5;	//power off
		}
	//info
	for(i=0;i<response[2];i++) 
		{
		if(USART_ByteReceive(response+i+3,USART_rCWT) !=0)
			{
			err_para=1;
			para=0;
				if(sc_globaltimeflag==2)
				{
				return -5;//power off
				}
			}
		}
	//tck
	if(USART_ByteReceive(response+i+3,USART_rCWT) !=0)
		{
		err_para=1;
		para=0;
			if(sc_globaltimeflag==2)
			{//HAL_DISP_CLS();
			sc_globaltimeflag=2;
			//init_delay0 (one_etu); //tc1=n 
			//for(i=0;i<3;i++)
			//	wait_delay ();
			return -5;//power off
			}
		}

	// time out
	if(sc_globaltimeflag==2)
		{
		Delay_us(3*sc_one_etu);
		return -5;//power off
		}
	//check len=ff
	if(response[2]==0xff)
		{
		return -5;	//power off
		}	
	//parity err
	if(err_para==1) 
		return -3;
	//check nad=0
	if(response[0]!=0) 
		return -4;	
	//check pcb
	//i
	if((response[1]&0x80)==0x00)
		{		
		if((response[1]&0x1f)!=0) //i rfu!=0
			return -4;
		if(((sc_t1_seqCard&0x01)<<6)!=(response[1]&0x40))//i wrong sequence number
			return -4;	
		}
	//r
	if((response[1]&0xc0)==0x80)
		{
		if(response[2]!=0)//r len!=0
			return -4;	
		if((response[1]&0x20)==0x20)//r 100b
			return -4;	
		}
	//s
	if((response[1]&0xc0)==0xc0)
		{
		//ifs
		if((response[1]&0x1f)==1)
			{
			if(response[2]!=1)//s ifs len!=1
				return -4;	
			if(response[3]<0x10)//s ifs   IFSI:0x10~0xFE
				return -4;	
			if(response[3]==0xff)//s ifs  IFSI:0x10~0xFE
				return -4;	
			}
		//bwt
		if(((response[1]&0x1f)==3)&&(response[2]!=1))//s bwt len!=1
			return -4;	
		//request not after response
		if((response[1]&0x20)==0x20) //s response
			{
			if(sc_t1_ifsc_reset==1) //request not after response
				return -4;	
			}
		//s response   unknow
		if((response[1]&0x1f)>4) 
			return -4;	
		//abort
		if((response[1]&0x3f)==2)//s response   abort
			return -5;	//power off	
		}

	//edc error
	*pOutlen =response[2]+4;
	if(HAL_7816_LRC8(response,*pOutlen-1)!=response[*pOutlen-1]) 
		{
		*pOutlen =0;
		return -3;	
		}

	return *pOutlen;
}

//*********************************
// NAD (1 byte)  PCB (1 byte)  LEN (1 byte)  INF (0to 254 bytes)  LRC (1 byte)
// return 1 fail
// return 0 true
//*********************************

unsigned int HAL_Card_T1(APDU *pAPDU, unsigned short len)
{	
	#define T1_BUF_SIZE 272
	unsigned char cmd[T1_BUF_SIZE] = {0};	
	unsigned char block_reader[T1_BUF_SIZE] = {0};	//device  ÖÕ¶ËÊı¾İ
	unsigned char block_card[T1_BUF_SIZE] = {0};	//card ¿¨Êı¾İ
	unsigned char block_pcb;
	unsigned char block_pcb_pre =0;
	unsigned short block_len =0;	//¿é³¤¶È
	
	unsigned short res_len;//½ÓÊÕµ½µÄ³¤¶È
	 int retu;
	int tck_count=0 ; //³ö´í´ÎÊı£¬Á¬Ğø3´ÎÏàÍ¬µÄ´íÎóÔòÏÂµç
	unsigned int last_err=0;	//ÉÏ´Î´íÎó

	unsigned char block_position_reader=0;	//·Ö¿éÊ±£¬Ö¸ÁîËù·¢ËÍµ½µÄÎ»ÖÃ
	unsigned short block_position_card=0;	//·Ö¿éÊ±£¬Ö¸ÁîËù·¢ËÍµ½µÄÎ»ÖÃ	
	unsigned char block_i_len=0;	//±£´æi¿é³¤¶Èblock_len
	
	#define CHAIN_YES 1
	#define CHAIN_NO 0	
	unsigned char chain_flag=0;//0 no chain;   1 chain
	#define STATE_SENDING 0
	#define STATE_RECEIVING 1
	unsigned char state_flag; //send 0 ,rec 1

	//JUDGE LENGTH
	pAPDU->RLE = 0;	
	if(len<5)return 1;
	if(len>T1_BUF_SIZE)return 1;
	//DATA INPUT CMD
	cmd[0] = pAPDU->CLA;
	cmd[1] = pAPDU->INS;
	cmd[2] = pAPDU->PP1;
	cmd[3] = pAPDU->PP2;
	cmd[4] = pAPDU->PP3;	
	memcpy(cmd+5, pAPDU->pInData, len-5);	
	//WAIT TIME
	sc_bwt_s = 1;
	sc_STIBWTR_bak = sc_bwt;
	//PCBÄÚÈİ
	//PCB I-bit8=0b;   PCB R-bit8bit7bit6= 100b;   PCB S-bit8bit7= 11b;  
	//sequence I-bit7 R-bit5
	//I bit6 0-END  1-NOT END
	//R bit4-bit1: 0-true; 1-EDC/LRC ERROR; 2-OTHER ERROR
	//S bit6:  0-request ; 1-response;    bit5-bit1:0  RESYNCH;  1 IFS ;  2 ABORT; 3 WTX; 4 Vpp ERROR
	block_position_reader=0;	
	state_flag =STATE_SENDING;
	HAL_7816_T1_Build_I(block_reader,&block_len,&block_pcb_pre,cmd,len,&block_i_len,block_position_reader,&chain_flag);
	tck_count=0;
	//*********************************
	while(1)
		{
TRANSMIT1_CHECK:	

		retu = HAL_7816_Transmit1(block_reader,block_len,block_card,&res_len);

		sc_bwt_s = 1;
		sc_bwt=sc_STIBWTR_bak;
		tck_count++;	
	
		//0:power off error; 1:controlled error; 2:s block; 3:r block; 4:i block	
		// 0:  power off
		if(retu==-5)//time out or over err count
			{
			goto ERROR_DONE;
			}

		// 1:      parity error ||edc error,other error ||////////////// sequence number error
		else if((retu==-3)||(retu==-4))
			{
			if(retu!=last_err)
				{
				tck_count=0;
				}

			last_err=retu;

			if(tck_count>1)	//time out or over err count
				{
				goto ERROR_DONE;
				}
			
			if((block_pcb_pre&0xc0)!=0x80)//not r
				{
				if(retu==-3)
					{
					block_pcb=0x81;//R 1 parity or edc error
					}
				else
					{
					block_pcb =0x82;//R 2   other error
					}
				HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
				}
			else
				{
				HAL_7816_T1_ReBuild_R(block_reader,&block_len,block_pcb_pre);
				}		
			goto TRANSMIT1_CHECK;								
			}			

		//  2:        S BWT ,waiting      ||    S IFS
		else if((block_card[1]&0xC0)==0xC0) //block s
			{
			last_err=0;
			HAL_7816_T1_Build_S(block_reader,&block_len,&block_pcb_pre,block_card);	
			goto TRANSMIT1_CHECK;	
			}

		// 3:   R    edc||other
		else if((block_card[1]&0xc0)==0x80)
			{
			if(state_flag ==STATE_RECEIVING)
				{
				if((block_pcb_pre&0xc0)!=0x80)//not r
					{
					block_pcb =0x82;//R 2   other error
					HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
					tck_count=0;
					}
				else
					{
					HAL_7816_T1_ReBuild_R(block_reader,&block_len,block_pcb_pre);
					}		
				goto TRANSMIT1_CHECK;	
				}
			else//sending
				{
				if(sc_t1_seqReader==((block_card[1]&0x10)>>4))
					{
					//i again
					if(0==(block_card[1]&0x0f))//no error
						{
						tck_count=0;  //2014-08-18
						}
					else//81/82
						{
						if(tck_count>2)	//time out or over err count
							{
							goto ERROR_DONE;
							}
						}
					HAL_7816_T1_Build_I(block_reader,&block_len,&block_pcb_pre,cmd,len,&block_i_len,block_position_reader,&chain_flag);
					}
				else
					{
					if(chain_flag==CHAIN_NO)
						{
						if((block_pcb_pre&0xc0)!=0x80)//not r
							{
							block_pcb =0x82;//R 2   other error
							HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
							tck_count=0;
							}
						else
							{
							tck_count=0;
							HAL_7816_T1_ReBuild_R(block_reader,&block_len,block_pcb_pre);
							}		
						goto TRANSMIT1_CHECK;	
						}
					else
						{
						if(0==(block_card[1]&0x0f))//no error
							{
							//NEXT I BLOCK
							tck_count=0; //2014-08-18
							sc_t1_seqReader=(sc_t1_seqReader+1)&0x01;
							block_position_reader=block_position_reader+block_i_len;
							}
						else
							{
							//81/82
								if(tck_count>2)	//time out or over err count
								{
								goto ERROR_DONE;
								}
							}
						//i
						HAL_7816_T1_Build_I(block_reader, &block_len, &block_pcb_pre, cmd,len,&block_i_len,block_position_reader,&chain_flag);
						}
					}
				goto TRANSMIT1_CHECK;	
				
				}
			}
		
		// 4: i block
		else if((block_card[1]&0x80)==0)
			{
			if(state_flag==STATE_SENDING)
				{
				if(chain_flag==CHAIN_YES)//»¹Ã»·¢Íê
					{
					//????
					if(last_err!=-11)
						tck_count=0;
					last_err=-11;
					if(tck_count>1)	//time out or over err count
						{
						goto ERROR_DONE;
						}
			
					block_pcb =0x82;//R 2   other error
					HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
					goto	TRANSMIT1_CHECK;
					}
				else//·¢µ½×îºóÒ»¿é
					{
					sc_t1_seqReader=(sc_t1_seqReader+1)&0x01;
					}
				}
			//Ô¤ÆÚ¼´Îª½ÓÊÕµ½Êı¾İ
			state_flag=STATE_RECEIVING;
			if(sc_t1_seqCard!=((block_card[1]&0x40)>>6))//ĞòÁĞºÅ´íÎó
				{
				//R other error
				block_pcb =0x82;
				
				HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
				}
			else//ĞòÁĞºÅÕıÈ·
				{
				tck_count=0;
				sc_t1_seqCard=(sc_t1_seqCard+1)&0x01;
		
				memcpy(pAPDU->pOutData+block_position_card,block_card+3 ,res_len-4);	
				block_position_card+=res_len-4;
			
				if((block_card[1]&0x20)==0x20)//not last block
					{
					//R no err
					block_pcb =0x80;
					HAL_7816_T1_Build_R(block_reader,&block_len,&block_pcb_pre,block_pcb);	
					}
				else
					{
					pAPDU->SW1 = pAPDU->pOutData[block_position_card-2]; // block_card[res_len-2-1];
					pAPDU->SW2 = pAPDU->pOutData[block_position_card-1]; //block_card[res_len-1-1];
					pAPDU->RLE = block_position_card-2;						
					goto DONE;
					}
				}
			
			}
	tck_count=0;

	}//send
DONE:
	return 0;
ERROR_DONE:
	pAPDU->SW1 = SCC_ERR_SW1;
	pAPDU->SW2 = SCC_ERR_SW2;
	pAPDU->RLE = 0;	
	return 1;

}
//***********************************
//HAL_7816_IFSC_reader
//t1¿é³¤¶ÈĞ­Òé¡£
//´Ëº¯ÊıÊÇ¶Á¿¨Æ÷Ïò¿¨·¢ËÍ³¤¶È£¬
//±ØĞëÔÚÈ¡µ½ATRºóÁ¢¼´Ê¹ÓÃ£¬ÆäËûÊ±¼äÖ»ÄÜÊÇ¿¨Ïò¶Á¿¨Æ÷ÒªÇó¸ü¸Ä³¤¶È¡
//return 1 sucessful
//return 0 fail
//***********************************
unsigned int   HAL_7816_t1_IFSC_reader(void)
{
	int retu;
	unsigned char t1_s_s[5]= {0x00,0xc1,0x01,0xfe,0x3e};
	unsigned char t1_s_r[257]={0};
	unsigned short t1_s_r_len =0;
	unsigned char result=1;
	sc_t1_ifsc_reset=0;
	//¸´Î»Ó¦´ğºó£¬µÚÒ»¸öÊı¾İ¿éÊÇÖÕ¶Ë·¢Íùic¿¨µÄ£¬
	//¶øÇÒÖ»ÄÜÊÇÒ»¸öPCB=C1, IFSD=FE µÄs¿é
	//¿¨Æ¬²Ù×÷¹ı³ÌÖĞ£¬ÖÕ¶Ë²»ÄÜÔÙ·¢ËÍs¿é
	Delay_ms(5);
	sc_t1_seqReader=0;
	sc_t1_seqCard=0;

	retu =HAL_7816_Transmit1(t1_s_s,5,t1_s_r,&t1_s_r_len);

	if((retu==-5)||((sc_globaltimeflag==2))	)
		{
		result=0;
		sc_t1_seqReader=0;
		sc_t1_seqCard=0;
		sc_t1_ifsc_reset=1;	
		return result; //·µ»ØÇ°Ğèsc_t1_ifsc_reset=1;	
		}
	if((retu==-3)||(retu==-4)||((t1_s_r[1]!=0xe1))||((t1_s_r[1]==0xe1)&&(t1_s_r[3]!=0xfe)))
		{
		retu =HAL_7816_Transmit1(t1_s_s,5,t1_s_r,&t1_s_r_len);
		if(retu==-5)	
			{
			result=0;
			}
		else if((retu==-3)||(retu==-4)||(t1_s_r[1]!=0xe1))
			{
			retu =HAL_7816_Transmit1(t1_s_s,5,t1_s_r,&t1_s_r_len);
			if((retu==-5)||	(retu==-3)||(retu==-4)||(t1_s_r[1]!=0xe1))
				{
				result=0;
				}
			}
		}
	
	//ic¿¨±ØĞëÏòÖÕ¶Ë·µ»Øs¿é£¬
	//ÈôPCB=E1, Ê¹ÓÃATRÖĞµÄFISC
	//ÈôPCB=C1, Ê¹ÓÃĞÂµÄFISC :  0x10-FE
	if((t1_s_r[1]==0xc1)&&(result!=0))
		{
		if((t1_s_r[3]>=0x10)&&(t1_s_r[3]<=0xfe))
			{
			sc_t1IFSC =t1_s_r[3];
			t1_s_s[1]=0xe1;
			t1_s_s[3]=t1_s_r[3];
			t1_s_s[4]=HAL_7816_LRC8(t1_s_s,4);
			retu =HAL_7816_Transmit1(t1_s_s,5,t1_s_r,&t1_s_r_len);
			if(retu==-5)	
				{
				result=0;
				}							
			}
		}
	sc_t1_seqReader=0;
	sc_t1_seqCard=0;
	
	sc_t1_ifsc_reset=1;	
	return result; //·µ»ØÇ°Ğèsc_t1_ifsc_reset=1;	
}
void HAL_7816_DeepShutdown()
{
	if(!SC_Detect())
	{
		if(!GPIO_ReadInputDataBit(SC_PIN_CMDVCC_GPIO, SC_PIN_CMDVCC))//¶ÁÈ¡ÊÇ·ñÒÑ¾­ÏÂµç
		{
			SC_PowerCmd(0);
		}
	}
}
unsigned char HAL_7816_GetATR(unsigned char type, unsigned char *patr)
{
	unsigned char len=type;
	if(HAL_7816_PowerOn(patr, &len)!=0)
		return 0;
	return len;
}
int vPBOC_7816_Init(void)
{
	unsigned char atr[40]={0};
	unsigned char atr_len =0;
	
	HAL_7816_Init();
	HAL_7816_PowerOn(atr, &atr_len);	
	APP_POS_PAY_Init();
	
	return 0;
}

