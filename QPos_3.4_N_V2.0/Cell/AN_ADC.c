/**
  ******************************************************************************
  * @file    .\user\source\AN_ADC.c 
  * @author  hanlei
  * @version V1.0.0
  * @date    08-11-2012
  * @brief   the middle-layer program for SPI 
  ******************************************************************************
  * @attention
  *
  * new write by han lei is need to check its accuracy in pratice.  
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "AN_ADC.h"
#define POWERCAPACITY 10
#define DISPALYSEGMENT 10 
#define ADC_GET_COUNT	10
extern uint8_t AN_ADC_READDATA(void);
unsigned char PowerValue = 0x00;
/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_MSD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
#define ADCPeriph1 ADC1
#define ADCPeriph1PORT GPIOC
#define ADCPeriph1PIN  GPIO_Pin_0
#define ADCPeriph1GPIOCLK RCC_APB2Periph_GPIOC
#define ADCPeriph1CLK RCC_APB2Periph_ADC1

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* export function prototypes -----------------------------------------------*/

/* export function ----------------------------------------------------------*/
/**
 *@nanme AN_ADC_Init_QY
 *@brief ADCGPIO初始化函数
 *@param none
 *@author hanlei
 *@date 2012-11-15
**/
static void  AN_ADC1_GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ADCPeriph1GPIOCLK, ENABLE);
	/* Configure ADC peripher Channex in analog mode */
	GPIO_InitStructure.GPIO_Pin =  ADCPeriph1PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_Init(ADCPeriph1PORT, &GPIO_InitStructure);
}
/**
 *@nanme AN_ADC_Init_QY
 *@brief ADC配置初始化函数
 *@param none
 *@author hanlei
 *@date 2012-11-15
**/
static void AN_ADC1_Config(void)
{
	ADC_InitTypeDef   ADC_InitStructure;
	RCC_APB2PeriphClockCmd(ADCPeriph1CLK, ENABLE);
	  /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立工作模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//触发方式为软件触发	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数为1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//单通道模式
	ADC_Init(ADC1, &ADC_InitStructure);
	//使用ADC1，模拟通道为0，采样序列号为1，残阳时间为28.5
	ADC_RegularChannelConfig(ADCPeriph1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);
	/* Enable ADC1 */
    ADC_Cmd(ADCPeriph1, ENABLE);  
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADCPeriph1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADCPeriph1));	
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADCPeriph1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADCPeriph1));    	
}
/**
 *@nanme AN_ADC_Init_QY
 *@brief ADC初始化函数
 *@param none
 *@author hanlei
 *@date 2012-11-15
**/
void  AN_ADC_Init(void)
{
	AN_ADC1_GPIO_config();
	AN_ADC1_Config();
}
/**
 *@nanme AN_ADC_READDATA
 *@brief ADC初始化函数
 *@param 返回电量的五个等级
 *@author hanlei
 *@date 2012-11-15
**/
uint8_t AN_ADC_READDATA(void)
{
	uint32_t result = 0,id = 0;
	uint8_t i = 0;

	for(i = 0; i < 8; i++)
	{
		ADC_SoftwareStartConvCmd(ADCPeriph1, ENABLE);
		do
		{
			id = ADC_GetFlagStatus(ADCPeriph1,ADC_FLAG_EOC);
        }while(!id);
		result+=  ADC_GetConversionValue(ADCPeriph1);
		
    }
	id = 0;
	result = result >> 3;
	result = (unsigned int)(((unsigned long)result)*3300 >> 12);
	for(i = 0; i < POWERCAPACITY + 1; i++)
	{
		if(result >= 0x9F0 - 6 * i)
		{
			id = POWERCAPACITY - i;
			break;
		}			
    }
	result = 0;
	for(i = 0;i < DISPALYSEGMENT; i++)
	{
		if(id >= 60 - 6 * i)
		{
			result = DISPALYSEGMENT - i; 
		    break;
        }
    } 
    return result;	
}

unsigned char AN_ADC_GetCellVoltageAndElectric(void)
{
	int ADC_ValueSum = 0;
	int ADC_ValueMAX = 0;
	int ADC_ValueMIN = 4096;
	int ADC_Value;
	int ElectricLevel = 0;
	int iStatus;
	int iLoop = 0;

	while(iLoop < ADC_GET_COUNT)
	{
		iLoop++;
		ADC_SoftwareStartConvCmd(ADCPeriph1, ENABLE);
		do
		{
			iStatus = ADC_GetFlagStatus(ADCPeriph1,ADC_FLAG_EOC);
        }while(!iStatus);

		ADC_Value = ADC_GetConversionValue(ADCPeriph1);	

		if( ADC_Value > ADC_ValueMAX )	//发现有超过最大值的数据
		{
			ADC_ValueMAX = ADC_Value;	//替换最大值
		}
		else{}
		if( ADC_Value < ADC_ValueMIN )	//发现有小于最小值的数据
		{
			ADC_ValueMIN = ADC_Value;  	//替换最小值
		}
		else{}
		ADC_ValueSum += ADC_Value; 		
	}

	//电池电压滤波处理
	ADC_ValueSum -= ADC_ValueMAX;			//去掉一个最大值	
	ADC_ValueSum -= ADC_ValueMIN;			//去掉一个最小值
	ADC_Value = (ADC_ValueSum / (ADC_GET_COUNT - 2));	//求出电池电压的平均值
	/*2.1v - 1.8v*/
	ADC_ValueSum = (unsigned int)(((unsigned long)ADC_Value)*3300 >> 12);

	ElectricLevel = 0;
	for(iLoop = 1; iLoop < POWERCAPACITY + 1; iLoop++)
	{
		if(ADC_ValueSum >= 2100 - ((2100 - 1700) / POWERCAPACITY) * iLoop)
		{
			ElectricLevel = POWERCAPACITY - iLoop+1;
			break;
		}
		if(iLoop==10)//ADC_ValueSum 小于最小值
		{
			ElectricLevel=0;
		}					
    }

#if 0
	if(0 == ElectricLevel)
	{
		return ((ElectricLevel << 16) | ADC_ValueSum);	
	}
	else{}

	for(iLoop = 0; iLoop <= DISPALYSEGMENT; iLoop++)
	{
		if(ElectricLevel >= POWERCAPACITY - (POWERCAPACITY / DISPALYSEGMENT) * iLoop)
		{
			ElectricLevel = DISPALYSEGMENT - iLoop - 1; 
		    break;
        }
    }

	if(ElectricLevel < 0)
	{
		ElectricLevel = 0;	
	}
	else{}
#endif

	//return ((ElectricLevel << 16) | ADC_ValueSum);
    PowerValue = (ElectricLevel+1)/2;
    return PowerValue;
}



