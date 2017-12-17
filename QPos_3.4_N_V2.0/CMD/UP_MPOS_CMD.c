//***************************************************************************************
//*  File Name		UP_MPOS_CMD.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			05/20/2014 
//*  Description	
//*
//***************************************************************************************
#include "stm32f10x.h"
#include "UP_MPOS_CMD.h"
#include "string.h"
#include "Roc_LittleTool.h"	
#include "Roc_PersonalFont.h"
#include "IROM.h"
#include "Bluetooth.h"
#include "Roc_ErrorCode.h"
#include "SysTick_delay.h"
#include "Sercrity.h"
#include "MID_PBOC.h"
#include "APP_PBOC.h"
#include "PBOC_Var.h"
#include "VectorTable.h"
#include "Key.h"
#include "display.h"
#include "MSCard_PA1100.h"
#include "Buzzer.h"
#include "sercrity.h"
#include "rtc.h"
#include "FontFonction.h"
#include "ASCII.h"
#include "msd.h"
#include "Nfc_main.h"
#include	"IC_interface.h"
#include	"lcd_gui.h"
#include	"APP_parameters.h"

#include "des.h"

CACHE_DATA g_sCacheData;

CARD_READER_INFO g_sCardReaderInfo;

KEYBOARD_INFO g_sKeyboardInfo;

LCD_INFO g_sLCDInfo;

unsigned char mPOS_FrameAnalyse(void)
{
	if((g_eBluetoothRecvStatus == BLUETOOTH_RECV_SUCEESCE)||(g_eBluetoothRecvStatus == BLUETOOTH_RECV_FRAME_SUCCESS))
	{
		mPOS_ACKorNAK(mPOS_ACK);
		if(g_sBluetoothMessage.ucbFrameIndex==2)
		{
			g_eBluetoothRecvStatus = BLUETOOTH_RECV_SUCEESCE;
		}
		/*****************************/
		//IOS系统必须加延时，可能是数据发送太快IOS没收到
		/*****************************/
		Delay_ms(60);//调试时发现
		if(g_eBluetoothRecvStatus == BLUETOOTH_RECV_SUCEESCE)
		{
			g_sBluetoothMessage.ucbCmd[0]=g_sBluetoothMessage.ucallbagdata[0];
			g_sBluetoothMessage.ucbCmd[1]=g_sBluetoothMessage.ucallbagdata[1];
			g_eBluetoothRecvStatus=BLUETOOTH_CMD_OK;
		}
		else
		{
			g_eBluetoothRecvStatus=BLUETOOTH_RECV_NO_DATA;
		}
	}
	return 0;
}
void mPOS_ACKorNAK(unsigned char ANS)
{	
	unsigned char pucBuf[12];
	unsigned char ucLRC = 0;

	g_sBluetoothMessage.ucbFrameLen[0] = 0;
	g_sBluetoothMessage.ucbFrameLen[1] = 4;	
	g_sBluetoothMessage.ucbLen[0] = 0;
	g_sBluetoothMessage.ucbLen[1] = 0;
	g_sBluetoothMessage.ucType=ANS;
	memcpy(pucBuf, g_sBluetoothMessage.ucbHead, 9);
	//add type,index,Len
	i1ByteXOR(pucBuf + 2, 7, &ucLRC);
	//add head
	pucBuf[9] = ucLRC; 
	pucBuf[10] =0XCC;
	pucBuf[11] = 0X33;
	vBluetooth_SendData(pucBuf, 12);	
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vGetTradeTime(unsigned char *pucTime)
{
#if 1
	unsigned char ucLen = 7;
	unsigned char ucBuf[20];
	RTC_Get();//更新时间  

	ucBuf[0] = (g_sCalendar.w_year / 1000) | 0x30;
	ucBuf[1] = ((g_sCalendar.w_year % 1000) / 100) | 0x30;	
	ucBuf[2] = ((g_sCalendar.w_year % 100) / 10) | 0x30;
	ucBuf[3] = (g_sCalendar.w_year % 10) | 0x30;	

	ucBuf[4] = (g_sCalendar.w_month / 10) | 0x30;
	ucBuf[5] = (g_sCalendar.w_month % 10) | 0x30;	
		
	ucBuf[6] = (g_sCalendar.w_date / 10) | 0x30;
	ucBuf[7] = (g_sCalendar.w_date % 10) | 0x30;

	ucBuf[8] = (g_sCalendar.hour / 10) | 0x30;
	ucBuf[9] = (g_sCalendar.hour % 10) | 0x30;

	ucBuf[10] = (g_sCalendar.min / 10) | 0x30;
	ucBuf[11] = (g_sCalendar.min % 10) | 0x30;

	ucBuf[12] = (g_sCalendar.sec / 10) | 0x30;
	ucBuf[13] = (g_sCalendar.sec % 10) | 0x30;

   	ucLen = 14;

	iASCIIToBCD(ucBuf, &ucLen, pucTime);
#endif
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vCriticalSection(unsigned char ucMod)
{
	if(ucMod == 0)
	{
//		TIM_Cmd(TIM2, DISABLE);
		
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART_Cmd(USART2, DISABLE); 
	}
	else
	{
//		TIM_Cmd(TIM2, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE); 
	}
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iPayThroughMSC(unsigned char *pucCardNo, unsigned char * pucCardNoLen)
{
	int iRet;
	iRet = MSR_iMSCardTask();
	if(0 == iRet)
	{
		iRet = MSR_iGetCardNum(pucCardNo, pucCardNoLen);
		if(0 == iRet)
		{
			iBuzzer_On(1);
		}
		else	
		{
			iBuzzer_On(3);
		}
	}
	else{}
	return iRet;
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iMSCardIsICCard(unsigned char *pucCardTrack, unsigned char ucCardTrackLen)
{
	unsigned char ucLoop = 0;
	while(ucLoop < ucCardTrackLen)
	{
		if(pucCardTrack[ucLoop] == '=')
		{
			ucLoop++;
			ucLoop += 4;  	//yymm
			if((pucCardTrack[ucLoop]	== '2') || (pucCardTrack[ucLoop]	== '6'))
			{
				memcpy(g_sCardReaderInfo.ucbCardServiceCodes, pucCardTrack + ucLoop, 3);
				return (0);
			}
			else
			{
				return (1);
			}
		}
		else{}
		ucLoop++;
	}
	return (2);
}
//***************************************************************************************
//* Function Name  : ucPayThroughICC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iPayThroughICC(unsigned char *pucInData, unsigned char ucMod, unsigned char * pucField55Data, unsigned int * pucField55DataLen)
{
	int iRet;
	
	if((pucInData == NULL) || (pucField55Data == NULL) || (pucField55DataLen == NULL))
	{
		return ERROR_FORMAL_PARAMETER;
	}
	else{}
	
	g_APDU.PP3 = 12;
	g_APDU.PP2 = ucMod;
	memcpy(g_APDU.pInData, pucInData, 12);
	g_APDU.RLE = 0;
	iRet = APP_POS_PAY_55CardUpload_DataPackage();
	g_APDU.SW1 = (unsigned char)((iRet & 0xff00) >> 8);
	g_APDU.SW2 = (unsigned char)(iRet & 0xff);
	if(1 < g_APDU.RLE)
	{
		memcpy(pucField55Data, g_APDU.pOutData, g_APDU.RLE);
		*pucField55DataLen = g_APDU.RLE;
		return (0);
	}
	else
	{
		g_APDU.RLE = 0;
		return (2);//读卡失败
	}	
}

int iPayQPOCFlow(unsigned char *pucInData, unsigned char ucMod, unsigned char * pucField55Data, unsigned int * pucField55DataLen)
{
	int iRet;
	
	if((pucInData == NULL) || (pucField55Data == NULL) || (pucField55DataLen == NULL))
	{
		return ERROR_FORMAL_PARAMETER;
	}
	else{}
	
	g_APDU.PP3 = 12;
	g_APDU.PP2 = ucMod;
	memcpy(g_APDU.pInData, pucInData, 12);
	g_APDU.RLE = 0;
	iRet = App_qPBOC_Purchase(g_sCardReaderInfo.ucFlagForceOnline);
	g_APDU.SW1 = (unsigned char)((iRet & 0xff00) >> 8);
	g_APDU.SW2 = (unsigned char)(iRet & 0xff);
	if(1 < g_APDU.RLE)
	{
		memcpy(pucField55Data, g_APDU.pOutData, g_APDU.RLE);
		*pucField55DataLen = g_APDU.RLE;
		return (0);
	}
	else
	{
		g_APDU.RLE = 0;
		return (2);//读卡失败
	}	
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD7700_UToP_Device_Boot(void)
{
	unsigned char ucFlag = 0xff;
	IROM_Write(BOOTLOADER_FLAG_ADDR, &ucFlag, 1);
	return (0);
}


//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0101_UToP_GetDeviceVersion(unsigned char * pucOutData, unsigned int * puiLen)
{
	//
	IROM_Read(FLASH_DEVICE_VERSION_INFO+4,pucOutData,FLASH_ADDR_DEVICE_INFO_MAX);
	//memcpy(pucOutData,device_info_init.SecurityAuthSerial,FLASH_ADDR_DEVICE_INFO_MAX);
	*puiLen = FLASH_ADDR_DEVICE_INFO_MAX;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0107_UToP_ResetDevice(void)
{
	g_sCardReaderInfo.ucCardStatus =0xff;
	g_sKeyboardInfo.ucTimeout = 0;
	vLCD_MainUI();
	if(NFC_USE==Device_NFC_State())
	{
		NFC_FieldOff();
		g_NFC_State=0;
	}	
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0108_UToP_GetRTC(unsigned char * pucOutData, unsigned int * puiLen)
{
	unsigned char ucBuf[20];
#if 1
	RTC_Get();//更新时间  

	ucBuf[0] = (g_sCalendar.w_year / 1000) | 0x30;
	ucBuf[1] = ((g_sCalendar.w_year % 1000) / 100) | 0x30;	
	ucBuf[2] = ((g_sCalendar.w_year % 100) / 10) | 0x30;
	ucBuf[3] = (g_sCalendar.w_year % 10) | 0x30;	

	ucBuf[4] = (g_sCalendar.w_month / 10) | 0x30;
	ucBuf[5] = (g_sCalendar.w_month % 10) | 0x30;	
		
	ucBuf[6] = (g_sCalendar.w_date / 10) | 0x30;
	ucBuf[7] = (g_sCalendar.w_date % 10) | 0x30;

	ucBuf[8] = (g_sCalendar.hour / 10) | 0x30;
	ucBuf[9] = (g_sCalendar.hour % 10) | 0x30;

	ucBuf[10] = (g_sCalendar.min / 10) | 0x30;
	ucBuf[11] = (g_sCalendar.min % 10) | 0x30;

	ucBuf[12] = (g_sCalendar.sec / 10) | 0x30;
	ucBuf[13] = (g_sCalendar.sec % 10) | 0x30;
#endif

	memcpy(pucOutData, ucBuf, 14);
	*puiLen = 14;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0109_UToP_SetRTC(unsigned char * pucInData, unsigned int * puiLen)
{
#if 1
	unsigned char temp;
	unsigned short usYear;
	unsigned char ucMon;
	unsigned char ucDay; 
	unsigned char ucHour;
	unsigned char ucMin; 
	unsigned char ucSec;
	
	usYear = (pucInData[0] - '0') * 1000 + (pucInData[1] - '0') * 100 + (pucInData[2] - '0') * 10 + (pucInData[3] - '0');
	ucMon = (pucInData[4] - '0') * 10 + (pucInData[5] - '0');
	ucDay = (pucInData[6] - '0') * 10 + (pucInData[7] - '0');
	ucHour = (pucInData[8] - '0') * 10 + (pucInData[9] - '0');
	ucMin = (pucInData[10] - '0') * 10 + (pucInData[11] - '0');
	ucSec = (pucInData[12] - '0') * 10 + (pucInData[13] - '0');

   	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
	{}
	else{}	 			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	BKP_DeInit();	//复位备份区域 	
	RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
	temp = 0;
	g_uiTime100ms_Timeout = 30;
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
	{
		temp++;
		Delay_ms(10);
		if(g_uiTime100ms_Timeout == 0)
		{
			return 1;
		}
		else{}
	}
	if(temp>=250)	return 2;//初始化时钟失败,晶振有问题	    
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_WaitForSynchro();		//等待RTC寄存器同步  
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_EnterConfigMode();/// 允许配置	
	RTC_SetPrescaler(32767); //设置RTC预分频的值
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_Set(usYear,ucMon,ucDay,ucHour,ucMin,ucSec);  //设置时间	
	RTC_ExitConfigMode(); //退出配置模式  
	BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据 
#endif
	*puiLen = 0;
	return (0);

}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD010A__GetDeviceParameter(unsigned char * pucOutData, unsigned int * puiLen)
{
	memcpy(pucOutData, g_cucbDevicePara, FLASH_ADDR_DEVICE_PARA_MAX);
	*puiLen = FLASH_ADDR_DEVICE_PARA_MAX;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD010B_PToU_SendPrintStatus(unsigned char * pucOutData, unsigned int * puiLen)
{
	*puiLen = 0;
	return CMD_ERROR_PRINT;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned int g_uiLenOfFile;
unsigned int g_uiOffsetOfFile;
unsigned char g_UpdateFlag=0;
unsigned char g_macdata[8];
unsigned char g_fileflag=0;

int iRecv_CMD010C_Firmware(unsigned char *pucInData, unsigned char *pucSHA1Data, unsigned int *puiOffset)
{
	unsigned int uiOffset = 0;
	unsigned int uiLen,uilen1;	
	
	//addr is operated
	uiOffset = (*(pucInData)) << 24;
	uiOffset += (*(pucInData + 1)) << 16;
	uiOffset += (*(pucInData + 2)) << 8;
	uiOffset += (*(pucInData + 3));
	//data length
	uiLen = (*(pucInData + 4) << 8) + *(pucInData + 5);

	g_uiOffsetOfFile = uiOffset + uiLen;	
	//写数据
	SPI_FLASH_BufferWrite(pucInData + 6, SPI_FLASH_FIRMWARE_ADDR + uiOffset, uiLen);
	//读出来校验，保证写到FLASH数据正确性
	MSD_ReadBlock(pucInData + 6, SPI_FLASH_FIRMWARE_ADDR + uiOffset, uiLen);
	if(g_uiLenOfFile==g_uiOffsetOfFile)
	{
		uilen1=uiLen-8;
		//解密
		DES3_App((unsigned char *)BOOT_KEY_ADDR, 1, NULL, pucInData + 6, &uilen1, ucTempBuf);
		//计算MAC
		DES3_App((unsigned char *)BOOT_KEY_ADDR, 0, g_macdata, ucTempBuf,  &uilen1, ucTempBuf);
		memcpy(g_macdata, ucTempBuf+uilen1-8, 8);
		if(memcmp(g_macdata,pucInData+6+uilen1,8)==0)
		{
			g_fileflag=1;
		}
		else
		{
			g_fileflag=0;
		}
	}
	else
	{
		
		//解密
		DES3_App((unsigned char *)BOOT_KEY_ADDR, 1, NULL, pucInData + 6, &uiLen, ucTempBuf);
		//计算MAC
		DES3_App((unsigned char *)BOOT_KEY_ADDR, 0, g_macdata,  ucTempBuf,  &uiLen, ucTempBuf);
		
		memcpy(g_macdata, ucTempBuf+uiLen -8, 8);
	}
	
#if 0
	//sha1
	SHA1Reset(&SHA1);
	SHA1Input(&SHA1,pucInData + 6,uiLen);
	SHA1Result(&SHA1, pucSHA1Data);	
#endif
	memcpy(pucSHA1Data,pucInData+6,uiLen);
	SHA1Arigth(pucSHA1Data, uiLen);
	
	*puiOffset = g_uiOffsetOfFile;
	return CMD_OPERATION_SUCCESS;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD010C_UToP_UpdateDeviceFirmware(unsigned char * pucInData, unsigned int * puiLen, unsigned char * pucOutData)
{
	unsigned char ucStatus;
	int iRet;
	ucStatus = pucInData[0];
	if(0x01 == ucStatus) 	//start
	{
		g_uiLenOfFile = (*(pucInData + 1)) << 24;
		g_uiLenOfFile += (*(pucInData + 2)) << 16;
		g_uiLenOfFile += (*(pucInData + 3)) << 8;
		g_uiLenOfFile += (*(pucInData + 4));
		
		SPI_FLASH_Block64Erase(SPI_FLASH_FIRMWARE_ADDR);
		SPI_FLASH_Block64Erase(SPI_FLASH_FIRMWARE_ADDR+0x10000);
		SPI_FLASH_Block64Erase(SPI_FLASH_FIRMWARE_ADDR+0x20000);
		
		pucOutData[0] = 0x01;
		pucOutData[1] = 0x30;
		pucOutData[2] = 0x30;
		memset(pucOutData + 3, 0, 24);
		*puiLen = 1 + 2 + 20 + 4;
		vLCD_ShowFirmwareUpdateBegin();
		g_uiOffsetOfFile = 0;
		memset(g_macdata,0,8);
		g_fileflag=0;
		return (0);
	}
	else if(0x2 == ucStatus)	//continue
	{
		g_uiTime100ms_LCDShow=600;//禁止刷屏
		iRet = iRecv_CMD010C_Firmware(pucInData+5, pucOutData + 3, puiLen);
		pucOutData[0] = 0x02;
		pucOutData[1] = iRet >> 8;
		pucOutData[2] = iRet;
		pucOutData[23] = (*puiLen) >> 24;
		pucOutData[24] = (*puiLen) >> 16;
		pucOutData[25] = (*puiLen) >> 8;
		pucOutData[26] = (*puiLen);
		*puiLen = 1 + 2 + 20 + 4;
		vLCD_ShowFirmwareUpdateProgress(g_uiOffsetOfFile, g_uiLenOfFile);
		return (0);
	}
	else if(0x3 == ucStatus)	//end
	{
		if((g_uiOffsetOfFile == g_uiLenOfFile)&&g_fileflag)
		{
			ucStatus = 0x55;
			IROM_Write(BOOTLOADER_FLAG_ADDR, &ucStatus, 1);
			pucOutData[0] = (g_uiLenOfFile >> 24) & 0xff;
			pucOutData[1] = (g_uiLenOfFile >> 16) & 0xff;
			pucOutData[2] = (g_uiLenOfFile >> 8) & 0xff;
			pucOutData[3] = (g_uiLenOfFile) & 0xff;	
			g_uiLenOfFile = 0;
			g_uiOffsetOfFile = 0;
			IROM_Write(BOOTLOADER_LEN_OF_FILE_ADDR, pucOutData, 4);
			pucOutData[0] = 0x03;
			pucOutData[1] = 0x30;
			pucOutData[2] = 0x30;
			memset(pucOutData + 3, 0, 24);
			*puiLen = 1 + 2 + 20 + 4;
			vLCD_ShowFirmwareUpdateEnd();
			g_UpdateFlag=1;
			return 0;
		}
		else
		{
			vLCD_ShowFirmwareUpdateFail();
			pucOutData[0] = 0x03;
			pucOutData[1] = 0x30;
			pucOutData[2] = 0x32;
			memset(pucOutData + 3, 0, 24);
			*puiLen = 1 + 2 + 20 + 4;
			return (0);
		}
	}
	else	 //error
	{ 
		*puiLen = 0;
		return CMD_ERROR_NO_SUPPORT;
	}	
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0202_UToP_UpdateDeviceWorkKey(unsigned char * pucInData, unsigned int * puiLen)
{
	KEY_INFO sKeyInfo;
	unsigned char ucRet;

	unsigned char ucbCheck[8];
	unsigned char ucbTMK[20];
	
	if(*puiLen != 67)
	{
		*puiLen = 1;
		return CMD_ERROR_VAR_LEN;
	}
	else{}
	memcpy((unsigned char *)&sKeyInfo, pucInData, *puiLen);

	ucRet =	createPublicFile(0x0006);

	ucRet = seleteBinaryFile(0x0006);	
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_NO_TMK;
	}
	else{}
#if 1			
	ucRet = ReadBinaryFile(UN_TMK_ADDR, ucbTMK, 0x10);	
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_NO_TMK;
	}
	else{}	
#endif			
	//TrackKey
	ucRet = ThreeDesBykey(sKeyInfo.ucTDK, 0x10, ucbTMK, 0x10);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
		
	memset(ucbCheck, 0, 8);
	ucRet = ThreeDesBykey(ucbCheck, 0x08, sKeyInfo.ucTDK, 0x00);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}	
		
	if((sKeyInfo.ucTDKCheck[0] != ucbCheck[0]) || 
	(sKeyInfo.ucTDKCheck[1] != ucbCheck[1]) || 
	(sKeyInfo.ucTDKCheck[2] != ucbCheck[2]) || 
	(sKeyInfo.ucTDKCheck[3] != ucbCheck[3]))
	{
		*puiLen = 1;
		return CMD_ERROR_CHECK_VALUE;
	}
	else{}
		
	ucRet = updateBinaryFile(UN_TRACK_ADDR, &sKeyInfo.ucTDKIndex, 22);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
		
	//PinKey
	ucRet = ThreeDesBykey(sKeyInfo.ucPIK, 0x10, ucbTMK, 0x10);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
			
	memset(ucbCheck, 0, 8);
	ucRet = ThreeDesBykey(ucbCheck, 0x08, sKeyInfo.ucPIK, 0x00);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
			
	if((sKeyInfo.ucPIKCheck[0] != ucbCheck[0]) || 
	(sKeyInfo.ucPIKCheck[1] != ucbCheck[1]) || 
	(sKeyInfo.ucPIKCheck[2] != ucbCheck[2]) || 
	(sKeyInfo.ucPIKCheck[3] != ucbCheck[3]))
	{
		*puiLen = 1;
		return CMD_ERROR_CHECK_VALUE;
	}
	else{}
			
	ucRet = updateBinaryFile(UN_PIN_ADDR, &sKeyInfo.ucPIKIndex, 22);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}

	//MacKey
	ucRet = ThreeDesBykey(sKeyInfo.ucMAK, 0x10, ucbTMK, 0x10);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
			
	memset(ucbCheck, 0, 8);
	ucRet = ThreeDesBykey(ucbCheck, 0x08, sKeyInfo.ucMAK, 0x00);
	if(0 != ucRet)
	{ 
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}
			
	if((sKeyInfo.ucMAKCheck[0] != ucbCheck[0]) || 
	(sKeyInfo.ucMAKCheck[1] != ucbCheck[1]) || 
	(sKeyInfo.ucMAKCheck[2] != ucbCheck[2]) || 
	(sKeyInfo.ucMAKCheck[3] != ucbCheck[3]))
	{
		*puiLen = 1;
		return CMD_ERROR_CHECK_VALUE;
	}
	else{}
			
	ucRet = updateBinaryFile(UN_MAC_ADDR, &sKeyInfo.ucMAKIndex, 22);
	if(0 != ucRet)
	{
		*puiLen = 1;
		return CMD_ERROR_DATABASE;
	}
	else{}

	*puiLen = 0;
	return (0);
}

int iEncryptDataByTDKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge)
{
	int iRet;
	unsigned char ucbDefaultKey[16];
	unsigned char ucbDivKey[16];

	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return 0x70;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_TRACK_ADDR + 2, ucbDefaultKey, 0x10);	
	if(0 != iRet)
	{
		return 0x71;
	}
	
	iRet = iDiversify3Des(ucbDefaultKey, pucChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return (0x72);
	}
	else{}

	iRet = ThreeDesBykey(pucData, uiDataLen, ucbDivKey, 0x00);
	if(0 != iRet)
	{
	 	return (0x73);
	}
	else{}

	return (0);

}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptDataByMAKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge)
{
	int iRet;
	unsigned char ucbDefaultKey[16];
	unsigned char ucbDivKey[16];
	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return 0x70;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbDefaultKey, 0x10);	
	if(0 != iRet)
	{
		return 0x71;
	}

	iRet = ThreeDesBykey(pucData, uiDataLen, ucbDivKey, 0x00);
	if(0 != iRet)
	{
	 	return (0x73);
	}
	else{}

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptDataByPIKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge)
{
	int iRet;
	unsigned char ucbDefaultKey[16];
	unsigned char ucbDivKey[16];
	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return 0x70;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_PIN_ADDR + 2, ucbDefaultKey, 0x10);	
	if(0 != iRet)
	{
		return 0x71;
	}

	iRet = iDiversify3Des(ucbDefaultKey, pucChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return (0x72);
	}
	else{}

	iRet = ThreeDesBykey(pucData, uiDataLen, ucbDivKey, 0x00);
	if(0 != iRet)
	{
	 	return (0x73);
	}
	else{}

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0204_UToP_VerifyMAC(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	MAC_INFO sMACInfo;
	int iRet;
	unsigned int uiLen;
	unsigned char ucbMacBuf[1200];
	unsigned char ucbMacValue[16];
	unsigned char ucbMACKey[20];
	unsigned char ucbDivKey[20];
	if(*puiLen > 1200)
	{
		return CMD_ERROR_VAR_LEN;
	}
	else{}

	sMACInfo.pucData = ucbMacBuf + 20;

	memcpy(ucbMacBuf, pucInData, *puiLen);
	sMACInfo.ucType = ucbMacBuf[0];
	sMACInfo.ucIndex = ucbMacBuf[1];
	memcpy(sMACInfo.ucbChallenge, ucbMacBuf + 2, 8);
	memcpy(sMACInfo.ucbMAC, ucbMacBuf + 10, 8);
	memcpy(sMACInfo.ucbLen, ucbMacBuf + 18, 2);
	uiLen = (sMACInfo.ucbLen[0] << 8) + sMACInfo.ucbLen[1];

	iDataFillingWith(sMACInfo.pucData, &uiLen, 0x00);


	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return (0x71);
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbMACKey, 0x10);	
	if(0 != iRet)
	{
		return (0x72);
	}
	else{}

	iRet = iDiversify3Des(ucbMACKey, sMACInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
   	memset(ucbMacValue, 0x00, 8);
	iRet = iMPOS_EncryptMACData(sMACInfo.pucData, uiLen, ucbMacValue, ucbDivKey);
	if(iRet != 0)
	{
		return iRet;
	}
	else{}

	iRet = memcmp(ucbMacValue, sMACInfo.ucbMAC, 8);
	if(iRet != 0)
	{
		pucOutData[0] = '0';
		pucOutData[1] = 'D';
		*puiLen = 2;
		return CMD_ERROR_OTHER;
	}
	else{}
	pucOutData[0] = '0';
	pucOutData[1] = '0';
	*puiLen = 2;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0207_UToP_EncriptData(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	ENCRIPT_DATA_INFO sEnDataInfo;
	unsigned int uiLen;
	unsigned char ucbEnDataBuf[600];
	int iRet;
	unsigned char ucbTrackKey[20];
	unsigned char ucbDivKey[20];

	if(*puiLen > 600)
	{
		return CMD_ERROR_VAR_LEN;
	}
	else{}

	memcpy(ucbEnDataBuf, pucInData, *puiLen);
	sEnDataInfo.ucKeyIndex = ucbEnDataBuf[0];
	sEnDataInfo.ucTimeout = ucbEnDataBuf[1];
	sEnDataInfo.ucTitleLen = ucbEnDataBuf[2];
	memcpy(sEnDataInfo.ucbTitle, ucbEnDataBuf + 3, sEnDataInfo.ucTitleLen);
	sEnDataInfo.ucDataLen = ucbEnDataBuf[3 + sEnDataInfo.ucTitleLen];
	memcpy(sEnDataInfo.ucbData, ucbEnDataBuf + 4 + sEnDataInfo.ucTitleLen, sEnDataInfo.ucDataLen);

	vLCD_ClearALL();
	vLCD_GUI_Show12X12_Ex(sEnDataInfo.ucbTitle, sEnDataInfo.ucTitleLen, 2, 0);
	vLCD_GUI_Show12X12_Ex(sEnDataInfo.ucbData, sEnDataInfo.ucDataLen,3, 0);
	vLCD_GUI_Show12X12_Ex("请确定",6,5,128-36);
	g_uiTime100ms_Timeout = (sEnDataInfo.ucTimeout - 1) * 10;
	iRet = iWaitForESCOrOK();
	vLCD_MainUI();
	if(0 == iRet)
	{
	}
	else if(1 == iRet) 
	{
		//esc ,redo
		return (0x01);
	}
	else
	{
		//timeout	
		return (0x0c);
	}	
   	
	iRet = GetChallenge(g_sCardReaderInfo.ucbChallenge, 8);
 	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}

	memcpy(pucOutData, g_sCardReaderInfo.ucbChallenge, 8);

	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_TRACK_ADDR + 2, ucbTrackKey, 0x10);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}

	iRet = iDiversify3Des(ucbTrackKey, g_sCardReaderInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}

	ucbEnDataBuf[0] = sEnDataInfo.ucDataLen;
	memcpy(ucbEnDataBuf + 1, sEnDataInfo.ucbData, sEnDataInfo.ucDataLen);
	ucbEnDataBuf[1 + sEnDataInfo.ucDataLen] = 24;
	memcpy(ucbEnDataBuf + 1 + sEnDataInfo.ucDataLen + 1, pdevice_version_info->DeviceSerialNO, 24);

   	uiLen = 1 + sEnDataInfo.ucDataLen + 1 + 24;

	iDataFillingWith(ucbEnDataBuf, &uiLen, 0xff);

	iRet = ThreeDesBykey(ucbEnDataBuf, uiLen, ucbDivKey, 0x00);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}

	pucOutData[8] = uiLen;
	memcpy(pucOutData + 9, ucbEnDataBuf, uiLen);
   	*puiLen = 8 + 1 + uiLen;

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0304_PToU_SendCardReaderStatus(unsigned char ucMod)
{
	unsigned char * pucData = g_sBluetoothMessage.ucallbagdata+ 2;

	*pucData = ucMod;
	pucData++;
	if(g_sCardReaderInfo.ucICFlag == 1)
	{
		memset(pucData, 'F', 3);
	}
	else
	{
		memcpy(pucData, g_sCardReaderInfo.ucbCardServiceCodes, 3);	
	}
	g_sBluetoothMessage.uibaglen= 2 + 4;
	g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0304_UToP_OpenCardReader(unsigned char * pucInData, unsigned int *puiLen)
{
	int iRet;
	unsigned int uilen;

	memset(&g_sCardReaderInfo.ucICFlag,0,sizeof(CARD_READER_INFO));
	//银行卡/行业卡标志
	g_sCardReaderInfo.ucICFlag = *pucInData;
	pucInData++;
	//读卡模式
	g_sCardReaderInfo.ucReaderType = *pucInData;
	pucInData++;
	//操作超时时间
	g_sCardReaderInfo.ucTimeout	= *pucInData;
	pucInData++;
	//PIK索引
	g_sCardReaderInfo.ucPIKIndex = *pucInData;
	pucInData++;
	//MAK索引
	g_sCardReaderInfo.ucMAKIndex = *pucInData;
	pucInData++;
	//TDK索引
	g_sCardReaderInfo.ucTDKIndex = *pucInData;
	pucInData++;
	//交易类型	  ucbTradeType = "\t余额查询"	 “\t\t\t收款”	"\t\t\t撤销"
	memcpy(g_sCardReaderInfo.ucbTradeType, pucInData, 10);
	pucInData += 10;
	//交易金额 
	memcpy(g_sCardReaderInfo.ucbTradeAmount, pucInData, 6);
	
	if(iTradeAmountIsNull(g_sCardReaderInfo.ucbTradeAmount))
	{
		g_sCardReaderInfo.ucDisTradeAmountLen = 6;	
	}
	else
	{
		g_sCardReaderInfo.ucDisTradeAmountLen = 0;		
	}


	pucInData += 6; 
	//自定义数据长度
	g_sCardReaderInfo.ucCustomDataLen = *pucInData;
	pucInData++;
	if(g_sCardReaderInfo.ucCustomDataLen > 256)
	{
		return CMD_ERROR_PARAM;
	}
	else{}
	//自定义数据
	if(g_sCardReaderInfo.ucCustomDataLen != 0)
	{
		memcpy(g_sCardReaderInfo.ucbCustomData, pucInData, g_sCardReaderInfo.ucCustomDataLen);
		pucInData += g_sCardReaderInfo.ucCustomDataLen;
	}
	else{}
	//交易显示模式
	g_sCardReaderInfo.ucTradeDisplayType = *pucInData;
	pucInData++; 
	//自定义显示长度
	g_sCardReaderInfo.ucCustomDisplayDataLen = *pucInData;
	g_sCardReaderInfo.ucCustomDisplayDataLen_Lcd = *pucInData;
	pucInData++;
	if(g_sCardReaderInfo.ucCustomDisplayDataLen > 256)
	{
		return CMD_ERROR_PARAM;
	}
	else{}	
	//自定义显示内容
	if(g_sCardReaderInfo.ucCustomDisplayDataLen != 0)
	{
		memcpy(g_sCardReaderInfo.ucbCustomDisplayData, pucInData, g_sCardReaderInfo.ucCustomDisplayDataLen);
		memcpy(g_sCardReaderInfo.ucbCustomDisplayData_Lcd, pucInData, g_sCardReaderInfo.ucCustomDisplayDataLen_Lcd);
		pucInData += g_sCardReaderInfo.ucCustomDisplayDataLen;
	}
	else{}


	//自定义加密长度
	g_sCardReaderInfo.ucCustomEncryptDataLen = *pucInData;
	pucInData++;
	if(g_sCardReaderInfo.ucCustomEncryptDataLen > 256)
	{
		return CMD_ERROR_PARAM;
	}
	else{}	
	//自定义加密内容
	if(g_sCardReaderInfo.ucCustomEncryptDataLen != 0)
	{
		memcpy(g_sCardReaderInfo.ucbCustomEncryptData, pucInData, g_sCardReaderInfo.ucCustomEncryptDataLen);
		pucInData += g_sCardReaderInfo.ucCustomEncryptDataLen;
	}
	else{}
	//结果提示方式
	g_sCardReaderInfo.ucResultsReportType = *pucInData;

	vGetTradeTime(g_sCardReaderInfo.ucTimestamp);
	
	GetChallenge(g_sCardReaderInfo.ucbChallenge, 8); 	

	if(g_sCardReaderInfo.ucCustomDisplayDataLen != 0)
	{
		uilen=g_sCardReaderInfo.ucCustomDisplayDataLen;
		iDataFillingWith(g_sCardReaderInfo.ucbCustomDisplayData, &uilen, 0xff);
		g_sCardReaderInfo.ucCustomDisplayDataLen=uilen;
		iRet = iEncryptDataByTDKIndex(g_sCardReaderInfo.ucbCustomDisplayData, 
										g_sCardReaderInfo.ucCustomDisplayDataLen, 
										g_sCardReaderInfo.ucTDKIndex, 
										g_sCardReaderInfo.ucbChallenge);
		if(0 != iRet)
		{
		 	return CMD_ERROR_EXE;
		}	 	
	}
	if(g_sCardReaderInfo.ucCustomEncryptDataLen != 0)
	{
		uilen=g_sCardReaderInfo.ucCustomEncryptDataLen;
		iDataFillingWith(g_sCardReaderInfo.ucbCustomEncryptData, &uilen, 0xff);
		g_sCardReaderInfo.ucCustomEncryptDataLen=uilen;
		iRet = iEncryptDataByTDKIndex(g_sCardReaderInfo.ucbCustomEncryptData, 
										g_sCardReaderInfo.ucCustomEncryptDataLen, 
										g_sCardReaderInfo.ucTDKIndex, 
										g_sCardReaderInfo.ucbChallenge);
		if(0 != iRet)
		{
		 	return CMD_ERROR_EXE;
		}
		
	 	
	}

	if(g_sCardReaderInfo.ucTimeout != 0)
	{
		g_uiTime100ms_Timeout = g_sCardReaderInfo.ucTimeout  * 10;
	}
	else
	{
		g_uiTime100ms_Timeout = 600;	
	}

	*puiLen = 0;
	g_sCardReaderInfo.ucCardStatus = 0x08;

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptTrackData(unsigned char *pucOutData, unsigned char * pucOutDataLen)
{
	unsigned char ucRet;
	unsigned char ucbTrackKey[20];
	unsigned char ucLen1;
	unsigned char ucLen2;
	if((NULL == pucOutData) || (NULL == pucOutDataLen))
	{
		return (0x70);
	}
	else{}
	
//	ucLen1 = getMsg2Data(pucOutData + 1, 1);
	pucOutData[0] = ucLen1;
#if 1		
	ucRet = seleteBinaryFile(0x0006);	
	if(0 != ucRet)
	{
		return (0x71);
	}
	else{}
		
	ucRet = ReadBinaryFile(UN_TRACK_ADDR, ucbTrackKey, 0x10);	
	if(0 != ucRet)
	{
		return (0x72);
	}
	else{}
		
	ucRet = ThreeDesBykey(pucOutData + ucLen1 + 1 - 9, 8, ucbTrackKey, 0x00);
	if(0 != ucRet)
	{
		return (0x73);
	}
	else{}
#endif

//  	ucLen2 = getMsg3Data(pucOutData + ucLen1 + 2, 1);
	pucOutData[ucLen1 + 1] = ucLen2;

#if 1
	if(ucLen2 != 0)
	{
		ucRet = ThreeDesBykey(pucOutData + ucLen1 + ucLen2 + 2 - 9, 8, ucbTrackKey, 0x00);
		if(0 != ucRet)
		{
			return (0x74);
		}
		else{}
	}
	else{}
#endif
	
		
	*pucOutDataLen = ucLen1 + ucLen2 + 2;
	
	return (0);
}

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0305_GetCardNo(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	int iRet;
	unsigned char ucbMACKey[20];
	unsigned char ucbDivKey[20];
	unsigned char ucbAmountAndCardNo[256];
	unsigned int uiAmountAndCardNoLen;
	unsigned char ucbMACBlock[1024];
	unsigned int uiMACBlockLen;

	memcpy(g_sCardReaderInfo.ucbField55InDataBuf, g_sCardReaderInfo.ucbTradeAmount, 6);	   	
	memcpy(g_sCardReaderInfo.ucbField55InDataBuf + 6, g_sCardReaderInfo.ucTimestamp + 1, 6);

	g_sCardReaderInfo.ucTimeout = *pucInData;
	if(g_sCardReaderInfo.ucTimeout != 0)
	{
		g_uiTime100ms_Timeout = g_sCardReaderInfo.ucTimeout  * 10;
   	}
	else
	{
		g_uiTime100ms_Timeout = 600;
	}

	if(g_sCardReaderInfo.ucCardNoLen == 0)
	{		
		iRet = APP_PBOC_Simple_ReadCardNo(g_sCardReaderInfo.ucbField55InDataBuf,12,g_sCardReaderInfo.ucbCardNo, &g_sCardReaderInfo.ucCardNoLen);
		if(iRet != 0x9000)
		{
			return CMD_ERROR_EXE;
		}
		else{}		
   	}
	
	//amount + card no
	memcpy(ucbAmountAndCardNo, g_sCardReaderInfo.ucbTradeAmount, 6);
//	memcpy(ucbAmountAndCardNo + 6, g_sCardReaderInfo.ucbCardNo, g_sCardReaderInfo.ucCardNoLen);
	*(ucbAmountAndCardNo + 6) = g_sCardReaderInfo.ucCardNoLen;
	if((g_sCardReaderInfo.ucCardNoLen % 2))
	{
		g_sCardReaderInfo.ucbCardNo[g_sCardReaderInfo.ucCardNoLen] = 'F';
		g_sCardReaderInfo.ucCardNoLen += 1;
	}
	else{}
	iASCIIToBCD(g_sCardReaderInfo.ucbCardNo,& g_sCardReaderInfo.ucCardNoLen, ucbAmountAndCardNo + 7);
	uiAmountAndCardNoLen = 6 + 1 + g_sCardReaderInfo.ucCardNoLen ;
   	iDataFillingWith(ucbAmountAndCardNo, &uiAmountAndCardNoLen, 0xff);

	iRet = iEncryptDataByTDKIndex(ucbAmountAndCardNo, uiAmountAndCardNoLen, g_sCardReaderInfo.ucTDKIndex, g_sCardReaderInfo.ucbChallenge);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}

   	//c mac
	memcpy(ucbMACBlock, g_sCardReaderInfo.ucbCustomData, g_sCardReaderInfo.ucCustomDataLen);
   	uiMACBlockLen = g_sCardReaderInfo.ucCustomDataLen;
	memcpy(ucbMACBlock + uiMACBlockLen, ucbAmountAndCardNo, uiAmountAndCardNoLen);
	uiMACBlockLen += uiAmountAndCardNoLen;

	if(g_sCardReaderInfo.ucCustomDisplayDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomDisplayDataLen;
	}
	else{}
	if(g_sCardReaderInfo.ucCustomEncryptDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomEncryptDataLen;
	}
	else{}

	memcpy(ucbMACBlock + uiMACBlockLen,pdevice_version_info->DeviceSerialNO, FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN);
	uiMACBlockLen += FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN;
	
	iHexToASCIIEx(g_sCardReaderInfo.ucTimestamp, 7, ucbMACBlock + uiMACBlockLen);
	uiMACBlockLen += 14;
	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbChallenge, 8);
	uiMACBlockLen += 8;
	
	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbMACKey, 0x10);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}

	iRet = iDiversify3Des(ucbMACKey, g_sCardReaderInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
	memset(g_sCardReaderInfo.ucbMACValue, 0x00 , 8); 
	iRet = iMPOS_EncryptMACData(ucbMACBlock, uiMACBlockLen, g_sCardReaderInfo.ucbMACValue, ucbDivKey);
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}

	//output
	memcpy(pucOutData, g_sCardReaderInfo.ucTimestamp, 7);
	*puiLen = 7;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbChallenge, 8);
	*puiLen += 8;
	*(pucOutData + *puiLen) = uiAmountAndCardNoLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, ucbAmountAndCardNo, uiAmountAndCardNoLen);
   	*puiLen += uiAmountAndCardNoLen;
	*(pucOutData + *puiLen) = g_sCardReaderInfo.ucCustomDisplayDataLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
   	*puiLen += g_sCardReaderInfo.ucCustomDisplayDataLen;
	*(pucOutData + *puiLen) = g_sCardReaderInfo.ucCustomEncryptDataLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
   	*puiLen += g_sCardReaderInfo.ucCustomEncryptDataLen;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbMACValue, 8);
   	*puiLen += 8;

	return (0);			
}
//***************************************************************************************
//* Function Name  : iDisplayPin
//* Description    : 
//*       
//* Input          : ucCount: index of pin .
//*									ucMod:1-add '*',0-clear '*'
//* Output         : None
//* Return         : None
//***************************************************************************************
int iDisplayPinOnLCD(unsigned char ucCount, unsigned char ucMod)
{
	if(ucMod == 1)
	{
		vLCD_GUI_Show12X12_Ex("*", 1,4,70 + (ucCount << 3));
	}
	else if(ucMod == 0)
	{
		vLCD_GUI_Show12X12_Ex(" ", 1,4,70 + (ucCount << 3));
	}
	else{}
	return (0);
}
//***************************************************************************************
//* Function Name  : iGetAndDisplayPin
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iGetAndDisplayPin(unsigned char * pucPin, unsigned char * pucPinLen, unsigned char ucPinLenMin, unsigned char ucPinLenMax)
{
	unsigned char ucRet;
	unsigned char ucbTemp[20];
	unsigned char ucCount;
	
	if((NULL == pucPin) || (NULL == pucPinLen))
	{
		return (0x70);
	}
	else{}
		
	ucCount = 0;
	g_BluetoothDateIN=2;
	while(1)
	{
		ucRet = ucKey_Scan(1); 
		if(('0' <= ucRet) && (ucRet <= '9'))		//number key
		{
			if(ucCount < 6)
			{
				iDisplayPinOnLCD(ucCount, 1);
				ucbTemp[ucCount] = ucRet;
				ucCount++;
			}
			else{}
		}
		else if(ucRet == 'C')		//clear key
		{
			if(ucCount > 0)
			{
				ucCount--;
				iDisplayPinOnLCD(ucCount, 0);
			}
			else{}
		}
		else if(ucRet == 'E')		//Esc key
		{
			*pucPinLen = 0;
			return (0x02);                             
		}
		else if(ucRet == 'O')		//ok key
		{
			if(ucCount == 0)
			{
				*pucPinLen = 0;
				return (0x01);
			}
			else if((ucCount >= ucPinLenMin) && (ucCount <= ucPinLenMax))
			{
				memcpy(pucPin, ucbTemp, ucCount);
				*pucPinLen = ucCount;
				return (0);
			}
		}
		else{}
		if(g_BluetoothDateIN==1)
			return (0x04);  //上位机取消操作
		if((g_BluetoothState==BLUETOOTH_ACCESS)||(g_BluetoothState==BLUETOOTH_SHUT_DOWN))	//蓝牙断开
		{
			return (0x04);                             
		}	
			
		ucRet =vLCD_ShowResTime(g_uiTime100ms_Timeout );
		if(ucRet == 1)
		{
			*pucPinLen = 0;
			return (0x03);
		}
		else{}
	}
}
//***************************************************************************************
//* Function Name  : iGetAndDisplayPin
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iWaitForESCOrOK(void)
{
	unsigned char ucRet;
	g_BluetoothDateIN=2;
	while(1)
	{
	//	g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;//执行时不关机
		ucRet = ucKey_Scan(1); 
	
		if(ucRet == 'E')		//Esc key
		{
			return (0x01);                             
		}
		else if(ucRet == 'O')		//ok key
		{
			return (0);
		}
		else{}
		if(g_BluetoothDateIN==1)
			return (0x03);  //上位机取消操作       
		if((g_BluetoothState==BLUETOOTH_ACCESS)||(g_BluetoothState==BLUETOOTH_SHUT_DOWN))	
		{
			return (0x01);                             
		}	
		ucRet =vLCD_ShowResTime(g_uiTime100ms_Timeout );
		if(ucRet == 1)
		{
			return (0x02);
		}
		else{}
	}

}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptPinData(unsigned char *pucData)
{
	unsigned char ucRet;
	unsigned char ucbPinKey[20];
	unsigned char ucbDivKey[20];
	if((NULL == pucData))
	{
		return (0x70);
	}
	else{}
		
	ucRet = seleteBinaryFile(0x0006);	
	if(0 != ucRet)
	{
		return (0x71);
	}
	else{}
		
	ucRet = ReadBinaryFile(UN_PIN_ADDR + 2, ucbPinKey, 0x10);	
	if(0 != ucRet)
	{
		return (0x72);
	}
	else{}
	//memset(ucbPinKey,0,16);
	ucRet = iDiversify3Des(ucbPinKey, g_sKeyboardInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != ucRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
		
	ucRet = ThreeDesBykey(pucData, 8, ucbDivKey, 0x00);
	if(0 != ucRet)
	{
		return (0x73);
	}
	else{}
		
	return (0);
}

unsigned char iCMD_InputPIN(void)
{
	int iPINStatus;

	vLCD_InputPIN();
	vClearKeyBuf();
	g_uiTime100ms_Timeout = g_sKeyboardInfo.ucTimeout  * 10;
	iPINStatus = iGetAndDisplayPin(g_sKeyboardInfo.ucbCardPin, &g_sKeyboardInfo.ucCardPinLen, g_sKeyboardInfo.ucPinMin, g_sKeyboardInfo.ucPinMax);
	if(0 == iPINStatus)
	{		
		g_sKeyboardInfo.ucbStatus=KEY_INPUT_SUC;		
   	}
	else if(0x01 == iPINStatus)
	{
		g_sKeyboardInfo.ucbStatus=KEY_INPUT_EMPTY;
	}
	else if(0x02 == iPINStatus)
	{
		g_sKeyboardInfo.ucbStatus=KEY_INPUT_CANCLE;
	}
	else if(0x03 == iPINStatus)
	{
		g_sKeyboardInfo.ucbStatus=KEY_INPUT_TIMEOUT;
	}

	return iPINStatus;

}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0404_PToU_UpLoadKeyboardStatus(unsigned char *pucOut,unsigned int *uiLen)
{
	int iRet;
	unsigned char ucbTrackKey[20];
	unsigned char ucbDivKey[20];
	unsigned char ucbMACBlock[32];
	unsigned int uiMACBlockLen;
	unsigned char * pucOutData = pucOut;

	pucOutData+=2;

	if(KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)
	{
		iFillPinBlock(g_sKeyboardInfo.ucbCardPin, g_sKeyboardInfo.ucCardPinLen, 
					g_sCardReaderInfo.ucbCardNo, g_sCardReaderInfo.ucCardNoLen, g_sKeyboardInfo.ucbCardPin);
		iRet = iEncryptDataByPIKIndex(g_sKeyboardInfo.ucbCardPin, 8, g_sCardReaderInfo.ucPIKIndex, g_sKeyboardInfo.ucbChallenge);
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;	
		}
	}

	if((KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)||(KEY_INPUT_EMPTY==g_sKeyboardInfo.ucbStatus))
	{
		memset(g_sKeyboardInfo.ucbTradeAmount + 6, 'F', 2);

		iRet = iEncryptDataByTDKIndex(g_sKeyboardInfo.ucbTradeAmount, 8, g_sCardReaderInfo.ucTDKIndex, g_sKeyboardInfo.ucbChallenge);
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;
		}
		else{}

		if(KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)
		{
			memcpy(ucbMACBlock, g_sKeyboardInfo.ucbCardPin, 8);
			uiMACBlockLen = 8;
		}
		else if(KEY_INPUT_EMPTY==g_sKeyboardInfo.ucbStatus)
		{
		   uiMACBlockLen = 0;
		}
		memcpy(ucbMACBlock + uiMACBlockLen, g_sKeyboardInfo.ucbTradeAmount, 8);
	   	uiMACBlockLen += 8;

		iRet = seleteBinaryFile(0x0006);	
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;
		}
		else{}
			
		iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbTrackKey, 0x10);	
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;
		}

		iRet = iDiversify3Des(ucbTrackKey, g_sKeyboardInfo.ucbChallenge, 1, ucbDivKey);
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;
		}
		else{}

		memset(g_sKeyboardInfo.ucbMACValue, 0x00 , 8); 
		iRet = iMPOS_EncryptMACData(ucbMACBlock, uiMACBlockLen, g_sKeyboardInfo.ucbMACValue, ucbDivKey);
		if(0 != iRet)
		{
			return CMD_ERROR_EXE;
		}
		else{}
	}

	

	*pucOutData = g_sKeyboardInfo.ucbStatus;
	pucOutData++;
	if(KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)
	{
		memcpy(pucOutData, g_sKeyboardInfo.ucbCardPin, 8);
		pucOutData += 8;
	}
	if((KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)||(KEY_INPUT_EMPTY==g_sKeyboardInfo.ucbStatus))
	{
		memcpy(pucOutData, g_sKeyboardInfo.ucbTradeAmount, 8);
		pucOutData += 8;
		memcpy(pucOutData, g_sKeyboardInfo.ucbMACValue, 8);
		pucOutData += 8;
		
	}		
	*uiLen= pucOutData -pucOut;
	g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;	
	return iRet;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0404_UToP_OpenKeyboard(unsigned char * pucInData, unsigned int *puiLen)
{
	memcpy(&g_sKeyboardInfo, pucInData, *puiLen);
	memset(g_sKeyboardInfo.ucbCardPin, 0x00, 8);
	g_sCardReaderInfo.ucCardStatus = 0x06;
	g_uiTime100ms_Timeout = (g_sKeyboardInfo.ucTimeout - 1) * 10;
	*puiLen=0;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0405_UToP_GetPIN(unsigned char * pucOutData, unsigned int *puiLen)
{
	pucOutData[0] = g_sKeyboardInfo.ucCardPinLen;
	memcpy(pucOutData + 1, g_sKeyboardInfo.ucbCardPin, 8);
	*puiLen = 1 + 8;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0501_UpdateRSAPublicKey(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	unsigned char ucOption;
	unsigned int uiTempLen;

	unsigned char rid[5];
	unsigned char pki;
	
	if((pucInData == NULL) || (puiLen == NULL) || (pucOutData == NULL))
   	{
		return (0x70);
	}
	else{}
   	ucOption = *pucInData;
	pucInData++;
	uiTempLen = *puiLen - 1;
	if(uiTempLen > 512)
	{
		*puiLen = 0;
		return CMD_ERROR_PARAM;
	}
	else{}

	if(ucOption == 0x01)
	{
		CAPK_DeleteALL();
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x02)
	{
		CAPK_ADD(pucInData,uiTempLen);
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x03)
	{
		if(uiTempLen==8)
		{
			memcpy(rid,pucInData+3,5);
			CAPK_DeleteRID(rid);
		}
		else if(uiTempLen==12)
		{
			memcpy(rid,pucInData+3,5);
			pki=*(pucInData+11);
			CAPK_DeleteRIDandPKI(rid,pki);			
		}
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x04)
	{
		CAPK_GetInfo(pucOutData,puiLen);
		return 0;
	}
	else
	{
		*puiLen = 0;
		return CMD_ERROR_PARAM;
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0502_UpdateAID(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	unsigned char ucOption;
	unsigned int uiTempLen;
	if((pucInData == NULL) || (puiLen == NULL) || (pucOutData == NULL))
   	{
		return (0x70);
	}
	else{}
   	ucOption = *pucInData;
	pucInData++;
	uiTempLen = *puiLen - 1;
	if(uiTempLen > 512)
	{
		*puiLen = 0;
		return CMD_ERROR_PARAM;
	}
	else{}

	if(ucOption == 0x01)
	{
		ucOption = 0;
		IROM_Write(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x02)
	{
		unsigned char * pucTemp;
		IROM_Read(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);		
		if(ucOption > 20)
		{
			ucOption = 20;
		}
		else{}
		pucTemp = (unsigned char *)FLASH_ADDR_PBOC_AID;
		pucTemp += ucOption * FLASH_ADDR_PBOC_AID_MAX_LEN;
		IROM_Write((unsigned int)(pucTemp), pucInData, uiTempLen);
		ucOption++;
		IROM_Write(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);	
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x03)
	{
		IROM_Read(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);
		if(ucOption == 0)
		{
			ucOption = 0;
		}
		else
		{
			ucOption--;
		}	
		IROM_Write(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);
		*puiLen = 0;
		return (0);
	}
	else if(ucOption == 0x04)
	{
		unsigned char ucLoop;
		unsigned char * pucBuf;
		unsigned char * pucTemp;
		IROM_Read(FLASH_ADDR_PBOC_AID_CNT, &ucOption, 1);
		if(ucOption > 20)
		{
			ucOption = 20;
		}
		else{}
		*pucOutData	= ucOption;
		pucOutData++;
		*puiLen = 1;
		pucBuf = (unsigned char *)FLASH_ADDR_PBOC_AID;
		for(ucLoop = 0; ucLoop < ucOption; ucLoop++)
		{
			pucBuf += ucLoop * FLASH_ADDR_PBOC_AID_MAX_LEN;
			pucTemp = SearchTagforScript(pucBuf, FLASH_ADDR_PBOC_AID_MAX_LEN, 0x9f06);
			*pucOutData = *(pucTemp + 2);
			pucOutData += 1;
			*puiLen += 1;
			memcpy(pucOutData, pucTemp + 3, *(pucTemp + 2));
			pucOutData += *(pucTemp + 2);
			*puiLen += *(pucTemp + 2);
		}
		return (0);
	}
	else
	{
		*puiLen = 0;
		return CMD_ERROR_PARAM;
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0503_UToP_StartPBOC(unsigned char * pucInData, unsigned int *puiLen)
{
	unsigned int uiLen;
	unsigned char *pucData;
	if(*puiLen > 256)
	{
		return CMD_ERROR_VAR_LEN;
	}
	else{}
	uiLen = *puiLen;

	vPBOC_Init();
	pucData = SearchTagforScript(pucInData, uiLen, 0x9f02);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}

	memcpy(g_sCardReaderInfo.ucbTradeAmount, pucData + 3, 6);

	if(iTradeAmountIsNull(g_sCardReaderInfo.ucbTradeAmount))
	{
		g_sCardReaderInfo.ucTradeType = 0x00;
	}
	else
	{
		g_sCardReaderInfo.ucTradeType = 0x01;
		memset(g_sCardReaderInfo.ucbTradeAmount, 0x00, 6);	
	}	

	pucData = SearchTagforScript(pucInData, uiLen, 0x9f03);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}
	memcpy(puchAmountOther, pucData + 3, 6);

	pucData = SearchTagforScript(pucInData, uiLen, 0x009c);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}	
//	memcpy(puchTranType, pucData + 2, 1);
//	g_sCardReaderInfo.ucTradeType = *(pucData + 2);

	pucData = SearchTagforScript(pucInData, uiLen, 0xdf7c);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}
   	g_sCardReaderInfo.ucPBOCType = *(pucData + 3);


	pucData = SearchTagforScript(pucInData, uiLen, 0xdf71);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}
	g_sCardReaderInfo.ucPBOCFlow = *(pucData + 3);


	pucData = SearchTagforScript(pucInData, uiLen, 0xdf72);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}
	
	g_sCardReaderInfo.ucFlagForceOnline = *(pucData + 3);

	pucData = SearchTagforScript(pucInData, uiLen, 0xdf73);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}

	pucData = SearchTagforScript(pucInData, uiLen, 0xdf74);
	if(NULL == pucData)
	{
		return CMD_ERROR_FRAME; 	
	}
	else{}

   	g_sCardReaderInfo.ucTimeout = *(pucData + 3);

	memcpy(&g_sKeyboardInfo, pucData + 4, 3); 

	g_sCardReaderInfo.ucCardStatus = 0x02;
	*puiLen = 0;
	return (0);
}

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0503_PToU_UploadPBOCData(unsigned char ucStatus)
{
	int iRet;
	unsigned char ucbMACKey[20];
	unsigned char ucbDivKey[20];
	unsigned char ucbMACBlock[1024];
	unsigned int uiMACBlockLen;

	unsigned char * pucOutData = g_sBluetoothMessage.ucallbagdata+2;
	unsigned char ucbRecordIndex[2] = {0, 0};

//	g_sBTSendMessage.ucStatus = ucStatus;
//	if(ucStatus != 0)
//	{
//		return ucStatus;
//	}
//	else{}
	*pucOutData = ucStatus;
	pucOutData++;
	memcpy(pucOutData, ucbRecordIndex, 2);
   	pucOutData += 2;
	memcpy(pucOutData, g_sCardReaderInfo.ucTimestamp, 7);
	pucOutData += 7;
	memcpy(pucOutData, g_sCardReaderInfo.ucbChallenge, 8);
	pucOutData += 8;

	*pucOutData = g_sKeyboardInfo.ucbStatus;
	pucOutData++;
	if(g_sCardReaderInfo.ucPBOCType == 0x01)	//标准的授权过程
	{
		if(KEY_INPUT_SUC==g_sKeyboardInfo.ucbStatus)
		{
			iFillPinBlock(g_sKeyboardInfo.ucbCardPin, g_sKeyboardInfo.ucCardPinLen, 
					g_sCardReaderInfo.ucbCardNo, g_sCardReaderInfo.ucCardNoLen, g_sKeyboardInfo.ucbCardPin);
			iRet = iEncryptDataByPIKIndex(g_sKeyboardInfo.ucbCardPin, 8, g_sCardReaderInfo.ucPIKIndex, g_sCardReaderInfo.ucbChallenge);
			if(0 == iRet)
			{				
				memcpy(pucOutData, g_sKeyboardInfo.ucbCardPin, 8);
				pucOutData += 8;	
			}
		}

		
	}
	
   	// not filed 55
	iRet = APP_POS_PAY_No55CardUpload_DataPackage();
	if(0x9000 == iRet)
	{
		unsigned int len;
		
		memcpy(g_sCardReaderInfo.ucbNotField55DataBuf, g_APDU.pOutData, g_APDU.RLE);
		g_sCardReaderInfo.ucNotField55DataLen = g_APDU.RLE;
		len=g_sCardReaderInfo.ucNotField55DataLen;
		iDataFillingWith(g_sCardReaderInfo.ucbNotField55DataBuf, &len, 0xFF);
		g_sCardReaderInfo.ucNotField55DataLen=len;
		iRet = iEncryptDataByTDKIndex(g_sCardReaderInfo.ucbNotField55DataBuf, g_sCardReaderInfo.ucNotField55DataLen, g_sCardReaderInfo.ucTDKIndex, g_sCardReaderInfo.ucbChallenge);
		if(0 != iRet)
		{
			*pucOutData = 0;
			pucOutData++;
			return CMD_ERROR_EXE;
		}
		else{}
		*pucOutData = g_sCardReaderInfo.ucNotField55DataLen;
		pucOutData++;
		memcpy(pucOutData, g_sCardReaderInfo.ucbNotField55DataBuf, g_sCardReaderInfo.ucNotField55DataLen);
	   	pucOutData += g_sCardReaderInfo.ucNotField55DataLen; 
	}
	else
	{
		*pucOutData = 0;
		pucOutData++;
		return CMD_ERROR_EXE;
	}
	//dis
	*pucOutData = g_sCardReaderInfo.ucCustomDisplayDataLen;
	pucOutData++;
	memcpy(pucOutData, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
	pucOutData += g_sCardReaderInfo.ucCustomDisplayDataLen;
	//enc
	*pucOutData = g_sCardReaderInfo.ucCustomEncryptDataLen;
	pucOutData++;
	memcpy(pucOutData, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
	pucOutData += g_sCardReaderInfo.ucCustomEncryptDataLen;
	
	//c mac
	memcpy(ucbMACBlock, g_sCardReaderInfo.ucbCustomData, g_sCardReaderInfo.ucCustomDataLen);
   	uiMACBlockLen = g_sCardReaderInfo.ucCustomDataLen;
	if(g_sCardReaderInfo.ucCustomDisplayDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomDisplayDataLen;
	}
	else{}
	if(g_sCardReaderInfo.ucCustomEncryptDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomEncryptDataLen;
	}
	else{}

	memcpy(ucbMACBlock + uiMACBlockLen, pdevice_version_info->DeviceSerialNO, FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN);
	uiMACBlockLen += FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN;
	
	iHexToASCIIEx(g_sCardReaderInfo.ucTimestamp, 7, ucbMACBlock + uiMACBlockLen);
	uiMACBlockLen += 14;
	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbChallenge, 8);
	uiMACBlockLen += 8;
	if(g_sCardReaderInfo.ucNotField55DataLen != 0)
	{
		memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbNotField55DataBuf, g_sCardReaderInfo.ucNotField55DataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucNotField55DataLen;
	}
	else{}

	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbMACKey, 0x10);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}

	iRet = iDiversify3Des(ucbMACKey, g_sCardReaderInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
	memset(g_sCardReaderInfo.ucbMACValue, 0x00 , 8); 
	iRet = iMPOS_EncryptMACData(ucbMACBlock, uiMACBlockLen, g_sCardReaderInfo.ucbMACValue, ucbDivKey);
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
	//mac
	memcpy(pucOutData, g_sCardReaderInfo.ucbMACValue, 8);
	pucOutData += 8;
	//filed 55
	*pucOutData = (g_sCardReaderInfo.uiField55OutDataLen >> 8) & 0xff;
	pucOutData++;
	*pucOutData = (g_sCardReaderInfo.uiField55OutDataLen) & 0xff;
	pucOutData++;
	memcpy(pucOutData, g_sCardReaderInfo.ucbField55OutDataBuf, g_sCardReaderInfo.uiField55OutDataLen); 
	pucOutData += g_sCardReaderInfo.uiField55OutDataLen;

	g_sBluetoothMessage.ucbStatus=0;
	g_sBluetoothMessage.uibaglen=pucOutData-g_sBluetoothMessage.ucallbagdata;
	g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;

	return (0);
}



//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0505_UToP_GetPBOC(unsigned char * pucOutData, unsigned int *puiLen)
{

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0506_UToP_StartPBOCIssuers(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData)
{
	int iRet;
	unsigned char ucbMACKey[20];
	unsigned char ucbDivKey[20];
	unsigned char ucbMACBlock[1024];
	unsigned int uiMACBlockLen;
	g_sCardReaderInfo.uiField55IssuersLen = *puiLen;
	memcpy(g_sCardReaderInfo.ucbField55IssuersBuf, pucInData, *puiLen);

	g_APDU.PP2 = g_sCardReaderInfo.ucTradeType;
	g_APDU.PP3 = 7 + g_sCardReaderInfo.uiField55IssuersLen;
//	g_APDU.pInData[0] = !g_sCardReaderInfo.ucFlagOnline;
	g_APDU.pInData[0] = 0;
	memcpy(g_APDU.pInData + 1, g_sCardReaderInfo.ucbTradeAmount, 6);
	memcpy(g_APDU.pInData + 7, g_sCardReaderInfo.ucbField55IssuersBuf, g_sCardReaderInfo.uiField55IssuersLen);
	
	iRet = APP_POS_PAY_CardIssuers_Script();
	if(0x9000 == iRet)
	{
		g_sCardReaderInfo.uiField55IssuersLen = g_APDU.RLE - 1;
		memcpy(g_sCardReaderInfo.ucbField55IssuersBuf, g_APDU.pOutData + 1, g_sCardReaderInfo.uiField55IssuersLen);
		*pucOutData = 0x00;
		*puiLen = 1; 
	}
	else
	{
		g_sCardReaderInfo.uiField55IssuersLen = 0;
		*pucOutData = 0x07;
		*puiLen = 1;			
	}
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucTimestamp, 7);
	*puiLen += 7;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbChallenge, 8);
	*puiLen += 8;
   	*(pucOutData + *puiLen) = g_sCardReaderInfo.uiField55IssuersLen >> 8;
	*puiLen += 1;
	*(pucOutData + *puiLen) = g_sCardReaderInfo.uiField55IssuersLen;
	*puiLen += 1;
	if(g_sCardReaderInfo.uiField55IssuersLen != 0)
	{
		memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbField55IssuersBuf, g_sCardReaderInfo.uiField55IssuersLen);
		*puiLen += g_sCardReaderInfo.uiField55IssuersLen;
	}
	else{}

	//c mac
	iHexToASCIIEx(g_sCardReaderInfo.ucTimestamp, 7, ucbMACBlock);
	uiMACBlockLen = 14;
	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbChallenge, 8);
	uiMACBlockLen += 8;
	if(g_sCardReaderInfo.uiField55IssuersLen != 0)
	{
		memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbField55IssuersBuf, g_sCardReaderInfo.uiField55IssuersLen);
		uiMACBlockLen += g_sCardReaderInfo.uiField55IssuersLen;
	}
	else{}

   	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbMACKey, 0x10);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}

	iRet = iDiversify3Des(ucbMACKey, g_sCardReaderInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
	memset(g_sCardReaderInfo.ucbMACValue, 0x00 , 8); 
	iRet = iMPOS_EncryptMACData(ucbMACBlock, uiMACBlockLen, g_sCardReaderInfo.ucbMACValue, ucbDivKey);
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}

	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbMACValue, 8);
	*puiLen += 8;

	g_sCardReaderInfo.ucTimeout = 0;

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0507_PToU_UploadPBOCIssuersStatus(unsigned char ucMode)
{
	return (0);
}
int iCMD0510_UToP_ClosedEMV(unsigned char *pucIn,unsigned int *puiLen)
{
	if(NFC_USE==Device_NFC_State())
	{
		NFC_FieldOff();
		g_NFC_State=0;
	}	
	*puiLen=0;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0701_UToP_LCDShow(unsigned char * pucInData, unsigned int *puiLen)
{
	unsigned char ucLoop = 0;


	g_sLCDInfo.ucOffset1 = pucInData[ucLoop];
	ucLoop++;
	g_sLCDInfo.ucLen1 = pucInData[ucLoop];
   	ucLoop++;
	if((g_sLCDInfo.ucOffset1 + g_sLCDInfo.ucLen1) > 22)
	{
		return (0x04);
	}
	else{}
	memcpy(g_sLCDInfo.ucbBuf1, pucInData + ucLoop, g_sLCDInfo.ucLen1);
	ucLoop += g_sLCDInfo.ucLen1;

	g_sLCDInfo.ucOffset2 = pucInData[ucLoop];
	ucLoop++;
	g_sLCDInfo.ucLen2 = pucInData[ucLoop];
   	ucLoop++;
	if((g_sLCDInfo.ucOffset2 + g_sLCDInfo.ucLen2) > 22)
	{
		return (0x04);
	}
	else{}
	memcpy(g_sLCDInfo.ucbBuf2, pucInData + ucLoop, g_sLCDInfo.ucLen2);
	ucLoop += g_sLCDInfo.ucLen2;

	g_sLCDInfo.ucOffset3 = pucInData[ucLoop];
	ucLoop++;
	g_sLCDInfo.ucLen3 = pucInData[ucLoop];
   	ucLoop++;
	if((g_sLCDInfo.ucOffset3 + g_sLCDInfo.ucLen3) > 22)
	{
		return (0x04);
	}
	else{}
	memcpy(g_sLCDInfo.ucbBuf3, pucInData + ucLoop, g_sLCDInfo.ucLen3);
	ucLoop += g_sLCDInfo.ucLen3;

	g_sLCDInfo.ucTimeout = pucInData[ucLoop];

	vLCD_ClearWorkPlace2345();
	vLCD_GUI_Show12X12_Ex(g_sLCDInfo.ucbBuf1, g_sLCDInfo.ucLen1, 2, g_sLCDInfo.ucOffset1 * 8);
	vLCD_GUI_Show12X12_Ex(g_sLCDInfo.ucbBuf2, g_sLCDInfo.ucLen2, 3, g_sLCDInfo.ucOffset2 * 8);
	vLCD_GUI_Show12X12_Ex(g_sLCDInfo.ucbBuf3, g_sLCDInfo.ucLen3, 4, g_sLCDInfo.ucOffset3 * 8);

	g_uiTime100ms_LCDShow = g_sLCDInfo.ucTimeout * 10;
	g_LCDShowInfo=1;
	
	*puiLen = 0;
	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************

int iFillTrackData(void)
{
	unsigned char ucLen;
	memcpy(g_sCardReaderInfo.ucbCardTrack, g_sCardReaderInfo.ucbTradeAmount, 6);
	g_sCardReaderInfo.uiCardTrackLen = 6;
	if(g_sCardReaderInfo.ucICFlag == 0)
	{
	 	if(MSR_ucTrack2Len == 0)
		{
			return (0x71);
		}
		else{}
		ucLen = MSR_ucTrack2Len - 3;
		
		*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = ucLen;
		g_sCardReaderInfo.uiCardTrackLen++;	
	
		if(0 != (ucLen % 2))
		{
			ucLen++;
		} 
		else{}
		
		iHexToBCD(MSR_ucTrack2Buf + 1, ucLen, g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen);	
		g_sCardReaderInfo.uiCardTrackLen += ucLen / 2;
	
		if(MSR_ucTrack3Len != 0)
		{
			ucLen = MSR_ucTrack3Len - 3;		
	
			*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = ucLen; 
			g_sCardReaderInfo.uiCardTrackLen++;
	
			if(0 != (ucLen % 2))
			{
				ucLen++;
			} 
			else{}
	
			iHexToBCD(MSR_ucTrack3Buf + 1, ucLen, g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen);		
			g_sCardReaderInfo.uiCardTrackLen += ucLen / 2;
		}
		else
		{
		 	*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = 0; 
			g_sCardReaderInfo.uiCardTrackLen++;
		}
	}
	else if(g_sCardReaderInfo.ucICFlag == 1)
	{
		if(MSR_ucTrack1Len != 0)
		{
			ucLen = MSR_ucTrack1Len - 3;		
	
			*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = ucLen; 
			g_sCardReaderInfo.uiCardTrackLen++;
	
			if(0 != (ucLen % 2))
			{
				ucLen++;
			} 
			else{}
	
			iHexToBCD(MSR_ucTrack1Buf + 1, ucLen, g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen);		
			g_sCardReaderInfo.uiCardTrackLen += ucLen / 2;
		}
		else
		{
		 	*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = 0; 
			g_sCardReaderInfo.uiCardTrackLen++;
		}

		if(MSR_ucTrack2Len != 0)
		{
			ucLen = MSR_ucTrack2Len - 3;		
	
			*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = ucLen; 
			g_sCardReaderInfo.uiCardTrackLen++;
	
			if(0 != (ucLen % 2))
			{
				ucLen++;
			} 
			else{}
	
			iHexToBCD(MSR_ucTrack2Buf + 1, ucLen, g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen);		
			g_sCardReaderInfo.uiCardTrackLen += ucLen / 2;
		}
		else
		{
		 	*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = 0; 
			g_sCardReaderInfo.uiCardTrackLen++;
		}

		if(MSR_ucTrack3Len != 0)
		{
			ucLen = MSR_ucTrack3Len - 3;		
	
			*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = ucLen; 
			g_sCardReaderInfo.uiCardTrackLen++;
	
			if(0 != (ucLen % 2))
			{
				ucLen++;
			} 
			else{}
	
			iHexToBCD(MSR_ucTrack3Buf + 1, ucLen, g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen);		
			g_sCardReaderInfo.uiCardTrackLen += ucLen / 2;
		}
		else
		{
		 	*(g_sCardReaderInfo.ucbCardTrack + g_sCardReaderInfo.uiCardTrackLen) = 0; 
			g_sCardReaderInfo.uiCardTrackLen++;
		}
	}
	else
	{
		return (0xff);
	}

	if(g_sCardReaderInfo.uiCardTrackLen % 8)
	{
		iDataFillingWith(g_sCardReaderInfo.ucbCardTrack, &g_sCardReaderInfo.uiCardTrackLen, 0xFF);
	}
	else{}

	return (0);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCMD0306__GetCardTrack(unsigned char * pucOutData, unsigned int *puiLen)
{
	int iRet;
	unsigned char ucbMACKey[20];
	unsigned char ucbDivKey[20];
	unsigned char ucbAmountAndCardTrack[256];
	unsigned int uiAmountAndCardTrackLen;
	unsigned char ucbMACBlock[1024];
	unsigned int uiMACBlockLen;

	//amount + card no
	iRet = iFillTrackData();
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
	memcpy(ucbAmountAndCardTrack, g_sCardReaderInfo.ucbCardTrack, g_sCardReaderInfo.uiCardTrackLen);
	uiAmountAndCardTrackLen = g_sCardReaderInfo.uiCardTrackLen;
	iRet = iEncryptDataByTDKIndex(ucbAmountAndCardTrack, uiAmountAndCardTrackLen, g_sCardReaderInfo.ucTDKIndex, g_sCardReaderInfo.ucbChallenge);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
   	//c mac
	memcpy(ucbMACBlock, g_sCardReaderInfo.ucbCustomData, g_sCardReaderInfo.ucCustomDataLen);
   	uiMACBlockLen = g_sCardReaderInfo.ucCustomDataLen;
	if(g_sCardReaderInfo.ucCustomDisplayDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomDisplayDataLen;
	}
	else{}
	if(g_sCardReaderInfo.ucCustomEncryptDataLen != 0)
	{
	 	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
		uiMACBlockLen += g_sCardReaderInfo.ucCustomEncryptDataLen;
	}
	else{}
	memcpy(ucbMACBlock + uiMACBlockLen, pdevice_version_info->DeviceSerialNO, FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN);
	uiMACBlockLen += FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN;
	iHexToASCIIEx(g_sCardReaderInfo.ucTimestamp, 7, ucbMACBlock + uiMACBlockLen);
	uiMACBlockLen += 14;
	memcpy(ucbMACBlock + uiMACBlockLen, g_sCardReaderInfo.ucbChallenge, 8);
	uiMACBlockLen += 8;
	memcpy(ucbMACBlock + uiMACBlockLen, ucbAmountAndCardTrack, uiAmountAndCardTrackLen);
	uiMACBlockLen += uiAmountAndCardTrackLen;

	iRet = seleteBinaryFile(0x0006);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}
		
	iRet = ReadBinaryFile(UN_MAC_ADDR + 2, ucbMACKey, 0x10);	
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}

	iRet = iDiversify3Des(ucbMACKey, g_sCardReaderInfo.ucbChallenge, 1, ucbDivKey);
	if(0 != iRet)
	{
	 	return CMD_ERROR_EXE;
	}
	else{}
	memset(g_sCardReaderInfo.ucbMACValue, 0x00 , 8); 
	iRet = iMPOS_EncryptMACData(ucbMACBlock, uiMACBlockLen, g_sCardReaderInfo.ucbMACValue, ucbDivKey);
	if(0 != iRet)
	{
		return CMD_ERROR_EXE;
	}
	else{}

	//output
	if(g_sCardReaderInfo.ucICFlag == 0)
	{
		memcpy(pucOutData, g_sCardReaderInfo.ucbCardNo, g_sCardReaderInfo.ucCardNoLen);
		*puiLen = g_sCardReaderInfo.ucCardNoLen;
		memset(pucOutData + 6, '*', g_sCardReaderInfo.ucCardNoLen - 10);
		if(g_sCardReaderInfo.ucCardNoLen < 19)
		{
			memset(pucOutData + *puiLen, ' ', 19 - g_sCardReaderInfo.ucCardNoLen);
			*puiLen = 19;	
		}
		else{}
	}
	else if(g_sCardReaderInfo.ucICFlag == 1)
	{
		memset(pucOutData, 'F', 19);
		*puiLen = 19;	
	}
	else
	{
		return (0x73);
	}
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucTimestamp, 7);
	*puiLen += 7;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbChallenge, 8);
	*puiLen += 8;
	if(g_sCardReaderInfo.ucICFlag == 0)
	{
		if(g_sCardReaderInfo.ucCardNameLen != 0)
		{
			memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbCardName, g_sCardReaderInfo.ucCardNameLen);
			*puiLen += g_sCardReaderInfo.ucCardNameLen;	
			memset(pucOutData + *puiLen, ' ', 26 - g_sCardReaderInfo.ucCardNameLen);
			*puiLen += 26 - g_sCardReaderInfo.ucCardNameLen;	
		}
		else
		{
			memset(pucOutData + *puiLen, 'F', 26);
			*puiLen += 26;	
		}	
	}
	else if(g_sCardReaderInfo.ucICFlag == 1)
	{
		memset(pucOutData + *puiLen, 'F', 26);
		*puiLen += 26;	
	}
	else
	{
		return (0x73);
	}

	*(pucOutData + *puiLen) = uiAmountAndCardTrackLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, ucbAmountAndCardTrack, uiAmountAndCardTrackLen);
   	*puiLen += uiAmountAndCardTrackLen;
	*(pucOutData + *puiLen) = g_sCardReaderInfo.ucCustomDisplayDataLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbCustomDisplayData, g_sCardReaderInfo.ucCustomDisplayDataLen);
   	*puiLen += g_sCardReaderInfo.ucCustomDisplayDataLen;
	*(pucOutData + *puiLen) = g_sCardReaderInfo.ucCustomEncryptDataLen;
	*puiLen += 1;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbCustomEncryptData, g_sCardReaderInfo.ucCustomEncryptDataLen);
   	*puiLen += g_sCardReaderInfo.ucCustomEncryptDataLen;
	memcpy(pucOutData + *puiLen, g_sCardReaderInfo.ucbMACValue, 8);
   	*puiLen += 8;
	return (0);		
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vTask0306_UToP_LCDShow(void)
{}

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iTaskGetAndDisplayInputPin(unsigned char ucFirstKey, unsigned char * pucCardNo, unsigned char * pucCardNoLen)
{
	unsigned char ucCount = 0;
	unsigned char ucKey;
	unsigned char ucColunm = 0;
	if((pucCardNo == NULL) || (pucCardNoLen == NULL))
	{
		return (0x70);
	}	
	else{}

	pucCardNo[ucCount] = ucFirstKey;
	ucCount++;

	vLCD_ClearWorkPage(4);
	vLCD_ClearWorkPage(5);

	vLCD_GUI_ShowMid12X12("输完请按确认",5);
	vLCD_GUI_Show12X12_Ex(&ucFirstKey,1,4, ucColunm);

	ucColunm += 6;
	while(1)
	{
		ucKey = ucKey_Scan(1);

		if(ucColunm > 122)
		{
			ucColunm = 0;
		}
		else{}
		if(ucCount > 20)
		{
			ucCount = 0;
		}
		else{}
		if(('0' <= ucKey) && (ucKey <= '9')&&(ucCount<20))		//number key
		{
			vLCD_GUI_Show12X12_Ex(&ucKey,1,4, ucColunm);
			ucColunm += 6;
			pucCardNo[ucCount] = ucKey;
			ucCount++;
		}	
		else if(ucKey == 'E')		//Esc key
		{
			return (1);
		}
		else if(ucKey == 'C')		//clear key
		{
			ucColunm -= 6;
			if(ucColunm > 122)
			{
				ucColunm = 0;
			}
			else{}
 
			vLCD_GUI_Show12X12_Ex(" ",1,4, ucColunm);
						
			ucCount--;
			
			if(ucCount > 20)
			{
				ucCount = 0;
			}
		}
		else if(ucKey == 'O')		//ok key
		{
			if((ucCount < 16) || (ucCount > 19))
			{
				vLCD_ClearWorkPage(5);
				vLCD_GUI_ShowMid12X12("卡号长度错误,请检查", 5);
			}
			else
			{
				break;	
			}
			
		}
 		else{}

		if(1 == vLCD_ShowResTime(g_uiTime100ms_Timeout))
		{
			return (2);	
		}
		else{}

	}

	*pucCardNoLen = ucCount;
	return (0);
}
unsigned char iCMD_Factory_DeviceInit(unsigned char * pucInData, unsigned char uciLen)
{
	unsigned char *p;
	unsigned char ucRet;

	p=SearchTag(pucInData,uciLen,0x08);//设备号
	if(p!=NULL)
	{
		if(p[1]!=24)//长度不符合
		{
			return 1;
		}
		else
		{
			Update_DeviceSerialNO(p+2);
		}
	}
	else
	{
		return 1;
	}

	p=SearchTag(pucInData,uciLen,0x0a);//密钥
	if(p!=NULL)
	{
		if(p[1]!=0x10)//长度不符合
		{
			return 1;
		}
		else
		{
			ucRet = seleteBinaryFile(0x0006);	
			if(0 != ucRet)
			{
				return 2;
			}
			else{}

			ucRet = updateBinaryFile(UN_TMK_ADDR, p+2, 0x10);	
			if(0 != ucRet)
			{
				return 2;
			}
		}
	}
	else
	{
		return 1;
	}
	ucRet=0xff;
	IROM_Write(FLASH_ADDR_FACTORY_INIT, &ucRet, 1);
	return 0;//success
}


//************************************end of file****************************************
