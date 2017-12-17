//******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
//* File Name          : IROM.h
//* Author             : Roc Chao
//*												
//* Version            : V1.0
//* Date               : 04/12/2013
//* Description        : 安全方案
//********************************************************************************/
#ifndef __IROM_H__
#define __IROM_H__

#define FLASH_STARD_ADDR			0x8000000  			//flash基地址
#define FLASH_ADDR_MASK				0x803ffff			//

#define PNULL						0xffffffff
#define FLASH_SECTOR_SIZE  			0x800
#define	FLASH_SECTOR_ADDR_LO		(FLASH_SECTOR_SIZE - 1)//0x1FF	
#define FLASH_MAX_SIZE				0x3ffff
#define	FLASH_SECTOR_ADDR_HI		(FLASH_MAX_SIZE - FLASH_SECTOR_ADDR_LO)//0xFE00

#define	REVERSAL_FLAG				0x803a000
#define	REVERSAL_BUFLEN				REVERSAL_FLAG + 1      
#define	REVERSAL_BUF				REVERSAL_FLAG + 2      
#define	TRANS_SEQ_COUNTER			REVERSAL_FLAG + 256	
#define	TRANS_SEQ_COUNTER_LEN		4
/*
FLASH 内存规划信息


*/
#define	BT_IVT_CONF_FLAG			0x803a000+512
#define	BT_IVT_CONF_FLAG_LEN		1
#define	BT_IVT_ADDR			BT_IVT_CONF_FLAG+BT_IVT_CONF_FLAG_LEN
#define	BT_IVT_ADDR_LEN		12
#define	BT_IVT_PARA			BT_IVT_ADDR+BT_IVT_ADDR_LEN
#define	BT_IVT_PARA_LEN			256

#define FLASH_ADDR_PBOC_AID_CNT		0x803b000
#define FLASH_ADDR_PBOC_AID_CNT_LEN	0x01		
#define FLASH_ADDR_PBOC_AID			(FLASH_ADDR_PBOC_AID_CNT + FLASH_ADDR_PBOC_AID_CNT_LEN)
#define FLASH_ADDR_PBOC_AID_MAX_LEN		256

/***************************/

//AID_INFO
//#define	FLASH_AID_ADDR	0x803b000

//RID_INFO
//#define FLASH_CAPK_ADDR	0x803c000

//PARAMETER	_INFO

//#define	FLASH_DEVICE_VERSION_INFO	0x803F001

#define FLASH_ADDR_RTC           	0x803F000
#define FLASH_ADDR_RTC_LEN        	0x01

#define FLASH_ADDR_DEVICE_INFO_SAFE_AUTH_NO_LEN	0x06
#define FLASH_ADDR_DEVICE_INFO_UP_WEB_NO_LEN		0x05
#define FLASH_ADDR_DEVICE_INFO_DEV_TYPE_LEN			0x08
#define FLASH_ADDR_DEVICE_INFO_DEV_VER_LEN			20
#define FLASH_ADDR_DEVICE_INFO_FIRM_VER_LEN			20
#define FLASH_ADDR_DEVICE_INFO_DEV_APP_SRC_LEN		2
#define FLASH_ADDR_DEVICE_INFO_DEV_APP_VER_LEN		6
#define FLASH_ADDR_DEVICE_INFO_DEV_SN_LEN			24
//mPOS V2.0
//批上送指示标志
#define FLASH_ADDR_DEVICE_INFO_UPLOAD_LEN			1
#define FLASH_ADDR_DEVICE_INFO_DEV_STA_LEN			8
#define FLASH_ADDR_DEVICE_INFO_MAX  			(6 + 5 + 8 + 20 + 20 + 2 + 6 + 24 +1+ 8)

#define FLASH_ADDR_DEVICE_PARA_MAX  			82

//#define BOOTLOADER_FLAG_ADDR		0x8003f03

#define BOOT_DATA_BASE_ADDR		0x08005E00

#define BOOT_KEY_ADDR				BOOT_DATA_BASE_ADDR
#define BOOT_KEY_LEN				16

#define BOOTLOADER_FLAG_ADDR		0x08005E00+0x10
#define BOOTLOADER_FLAG_ADDR_LEN		1 

#define BOOTLOADER_LEN_OF_FILE_ADDR 	(BOOTLOADER_FLAG_ADDR + BOOTLOADER_FLAG_ADDR_LEN)	//3
#define BOOTLOADER_LEN_OF_FILE_ADDR_LEN		4	//3

#define SPI_FLASH_FIRMWARE_ADDR	   	0x00100000		//外接SPI FLASH写入地址

//设备状态信息
#define DEVICE_INFO_DEV_VER_ADDR	0x08005E00+128
#define DEVICE_INFO_DEV_VER_ADDR_LEN	10

#define FLASH_ADDR_FACTORY_TEST	DEVICE_INFO_DEV_VER_ADDR+DEVICE_INFO_DEV_VER_ADDR_LEN   //??????
#define FLASH_ADDR_FACTORY_INIT	FLASH_ADDR_FACTORY_TEST+1	//?????

#define	Bluetooth_BM77		'B'
#define	Bluetooth_IVT		'V'
#define	NFC_USE			'N'	//nfc
#define	Contact				'C'	//接触

#define Device_NFC_State()	*(volatile unsigned char *)(DEVICE_INFO_DEV_VER_ADDR)
#define Bluetooth_Type()		'V'//*(volatile unsigned char *)(DEVICE_INFO_DEV_VER_ADDR+1)

extern unsigned char ucTempBuf[];

void IROM_Write(unsigned int uiFlashAddr, unsigned char* pucDataBuff, unsigned int uiLen);

void IROM_Read(unsigned int uiFlashAddr, unsigned char* pucDataBuff, unsigned int uiLen);

void IROM_Erase(unsigned int uiFlashAddr, unsigned int uiLen);

#endif
/********************************end fo file *************************************/
