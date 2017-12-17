#ifndef __APP_PBOC_H__
#define __APP_PBOC_H__

//#include "usb_ccid.h"
#include "HAL_7816.h"



// #define BOOTLOADER_FLAG_ADDR		   (0x08003f03)

// #define FLASH_DEVICE_SERIAL_NUMBER 	   (0x0801f400)
// #define FLASH_MSCARD_KEY			   (FLASH_DEVICE_SERIAL_NUMBER + 0x10)	   /**/
// #define FLASH_PIN_KEY				   (FLASH_MSCARD_KEY + 0x10)			   /**/
// #define FLASH_MAC_KEY				   (FLASH_PIN_KEY + 0x10)	   /**/
// #define FLASH_PRESHARE_KEY			   (FLASH_MAC_KEY + 0x10)
// /*unused*/
// #define FLASH_MASTER_KEY			   FLASH_PRESHARE_KEY + 0x10
// #define FLASH_PRESERVE_KEY			   FLASH_MASTER_KEY + 0x10
// #define FLASH_TRANSFER_KEY1			   FLASH_PRESERVE_KEY + 0x10
// #define FLASH_TRANSFER_KEY2			   FLASH_TRANSFER_KEY1 + 0x10











/**********************************************************
*   交易状态的定义 
***********************************************************
*/
#define     TRANS_IDLE              		0x01
#define     TRANS_LOAD_START        		0x02
#define     TRANS_LOAD_READ_PAN     		0x03
#define     TRANS_PURCHASE_START        	0x05
#define     TRANS_PURCHASE_READ_PAN    	 	0x06
#define     TRANS_PRIBALANCE_START        	0x08
#define     TRANS_PRIBALANCE_READ_PAN     	0x09

#define     TRANS_GETRELOAD_START      		0x0A
#define     TRANS_RELOAD_START        		0x0B

//取卡号
unsigned int APP_PBOC_Simple_ReadCardNo(unsigned char *pInData,unsigned char uclen,unsigned char * pucCardNo, unsigned char * pucCardNoLen);

//电子现金账户余额Get Primary Acount Balance
unsigned int APP_POS_PAY_CDGetCashBalance(APDU *psAPDU);
//最大圈存金额
unsigned int APP_POS_PAY_UpLimitLoadPrice(APDU *psAPDU);
//发卡行脚本处理
unsigned int APP_POS_PAY_CardIssuers_Script (void);	
// 55域数据组包上传 
unsigned int APP_POS_PAY_55CardUpload_DataPackage(void);				
//初始化应用
void APP_POS_PAY_Init(void);
//非55域数据
unsigned int APP_POS_PAY_No55CardUpload_DataPackage(void);
//读取冲正信息
unsigned int APP_POS_PAY_CDReversalInfo(APDU *psAPDU);
//有无IC卡
unsigned int APP_POS_PAY_CardSlotState(APDU *psAPDU);
void Clear_Cardlist(void);
int CheckCardState(void);


#endif
