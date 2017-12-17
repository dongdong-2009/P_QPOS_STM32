/**
  ******************************************************************************
  * @file    flash_if.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Header for flash_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_MAL_H
#define __FLASH_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define FLASH_MAX_SIZE			0x00040000
#define FLASH_START_ADDR			0x08000000
#define FLASH_END_ADDR			FLASH_START_ADDR + FLASH_MAX_SIZE -1

 #if defined (USE_STM3210B_EVAL)
#define FLASH_PAGE_SIZE               0x00000400   /* wTransferSize   = 1024 bytes */
 #elif defined (USE_STM3210E_EVAL)
  #define FLASH_PAGE_SIZE               0x00000800   /* wTransferSize   = 2048 bytes */
 #else
  #error "Missing define: USE_STM3210B_EVAL or USE_STM3210E_EVAL"
 #endif /* USE_STM3210B_EVAL */


#define FLASH_PAGE_ADDR_LO		(FLASH_PAGE_SIZE-1)
#define FLASH_PAGE_ADDR_HI		(FLASH_MAX_SIZE - FLASH_PAGE_ADDR_LO)
#define FLASH_ADDR_MASK			FLASH_MAX_SIZE -1

#define wTransferSize		FLASH_PAGE_SIZE
extern uint8_t  MAL_Buffer[wTransferSize]; /* RAM Buffer for Downloaded Data */

uint16_t FLASH_If_Init(void);
uint16_t FLASH_If_Erase (uint32_t SectorAddress);
uint16_t FLASH_If_Write(uint32_t SectorAddress, uint32_t DataLength);
uint8_t *FLASH_If_Read (uint32_t SectorAddress, uint32_t DataLength);

#endif /* __FLASH_IF_MAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
