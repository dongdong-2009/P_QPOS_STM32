/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Mass Storage demo main file
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


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"
#include "flash_if.h"
#include "app_boot.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
typedef void (*pFunction)(void);
unsigned int JumpAddress;
pFunction Jump_To_Application;
#define MAX_LEN 0x800
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	unsigned int 	UpdateFileLen=0;
	unsigned char filebuf[MAX_LEN];
	
 	unsigned char BootFlag = 0;
	unsigned char BootLoader_Key[] = 
	{
		0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66
		//0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
	};

	Set_System();
	Set_USBClock();
		

	FLASH_If_Init();

 	/* Check if the Key push-button on STM3210x-EVAL Board is pressed */
	BOOT_NVMEM_Read(BOOT_FLAG_ADDR, &BootFlag, BOOT_FLAG_LEN);
#if 1
	if(BootFlag == 0xAA)
	{
			// Jump to user application
			JumpAddress = *(__IO uint32_t*) (APP_BASE_ADDR + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			// Initialize user application's Stack Pointer
			__set_MSP(*(__IO uint32_t*) APP_BASE_ADDR);
			Jump_To_Application();
	}
	//else
	//{
	//	BOOT_NVMEM_Write(BOOT_KEY_ADDR, BootLoader_Key, BOOT_KEY_LEN);
	//}

#endif
	
  Delay_Init(72);
  Led_Config();
  Display_Init();
//  Delay_ms(500);

  Led_Config();

  MID_DISP_ShowCharStr(0, 0, "BootVer", strlen("BootVer"));
  MID_DISP_ShowCharStr(2, 0, BOOT_VERSION, strlen(BOOT_VERSION));
  
  SPI_Flash_Config();
  
  Format_APDU();
	
  USB_Interrupts_Config();
  USB_Init();

	if(BootFlag==0x55)
	{	
		unsigned int totallen,offset,len,addr;
		unsigned char percent,tmp;
		unsigned char filelen[4];
		MID_DISP_ShowCharStr(0, 0, "UPDATING", strlen("UPDATING"));
		MID_DISP_ShowCharStr(4,24,"%",1);
		BOOT_NVMEM_Read(BOOTLOADER_LEN_OF_FILE_ADDR, filelen, BOOTLOADER_LEN_OF_FILE_LEN);
		UpdateFileLen=(filelen[0]<<24)+(filelen[1]<<16)+(filelen[2]<<8)+filelen[3];
		if(UpdateFileLen>FILE_MAX_LEN)
		{
			
		}
		else
		{
			offset=0;
			len=0;
			totallen=UpdateFileLen;
#if 1
			while(UpdateFileLen)
			{
				len=MAX_LEN<UpdateFileLen?MAX_LEN:UpdateFileLen;
				MSD_ReadBuffer(filebuf,SPI_FLASH_FIRMWARE_ADDR+offset,len);
				DES3_App((unsigned char *)BOOT_KEY_ADDR, 1, NULL, filebuf, &len, filebuf);
				BOOT_NVMEM_Write(APP_BASE_ADDR+offset,filebuf,len);
				UpdateFileLen-=len;
				offset+=len;
				percent=(offset*100)/totallen;
				tmp=percent/100+0x30;
				MID_DISP_ShowCharStr(4,0,&tmp,1);
				tmp=percent%100/10+0x30;
				MID_DISP_ShowCharStr(4,8,&tmp,1);
				tmp=percent%10+0x30;
				MID_DISP_ShowCharStr(4,16,&tmp,1);
			}
			BootFlag=0xaa;
			BOOT_NVMEM_Write(BOOT_FLAG_ADDR, &BootFlag, BOOT_FLAG_LEN);
			MID_DISP_ShowCharStr(6, 0, "please reboot", strlen("please reboot"));
#endif
		}

	}
	
  while (1)
  {
	PWR_Check();
  }
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
