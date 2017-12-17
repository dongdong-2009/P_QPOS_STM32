/**
  ******************************************************************************
  * @file    AN_ADC.h 
  * @author  HKRT
  * @version V1.0.0
  * @date    01-Nov-2012
  * @brief   middle apply layer program 
  ******************************************************************************
  * @attention
  *
  * new write by han lei is need to check its accuracy in pratice. 
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _AN_ADC_H_
#define _AN_ADC_H_


#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#ifdef STM32L10X_MD
#include "stm32l1xx.h"	
#else
#include "STM32F10x.h"
#endif  
/* external variables-------------------------------------------------------------*/

	
/* Exported types ------------------------------------------------------------*/

/* external function-------------------------------------------------------------*/
void  AN_ADC_Init(void);
uint8_t AN_ADC_READDATA(void);
unsigned char AN_ADC_GetCellVoltageAndElectric(void);

#ifdef __cplusplus
}
#endif
#endif//_AN_ADC_H_
