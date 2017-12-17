//***************************************************************************************
//*  File Name		UP_MPOS_CMD.h
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			05/20/2014 
//*  Description	
//*
//***************************************************************************************
#ifndef __UP_MPOS_CMD__
#define __UP_MPOS_CMD__

typedef struct
{
	unsigned char ucIndex;
	unsigned char ucbLen[2];
	unsigned char * pucData;
	unsigned char ucLRC;
}CACHE_DATA; 



typedef struct
{
	unsigned char ucTMKIndex;
	unsigned char ucTDKIndex;
	unsigned char ucTDKLen;
	unsigned char ucTDK[16];
	unsigned char ucTDKCheck[4];
	unsigned char ucMAKIndex;
	unsigned char ucMAKLen;
	unsigned char ucMAK[16];
	unsigned char ucMAKCheck[4];
	unsigned char ucPIKIndex;
	unsigned char ucPIKLen;
	unsigned char ucPIK[16];
	unsigned char ucPIKCheck[4];
}KEY_INFO;

typedef struct
{
	unsigned char ucType;
	unsigned char ucIndex;
	unsigned char ucbChallenge[8];
	unsigned char ucbMAC[8];
	unsigned char ucbLen[2];
	unsigned char * pucData;
}MAC_INFO;

typedef struct
{
	unsigned char ucKeyIndex;
	unsigned char ucTimeout;
	unsigned char ucTitleLen;
	unsigned char ucbTitle[256];
	unsigned char ucDataLen;
	unsigned char ucbData[256];
}ENCRIPT_DATA_INFO;

#if 0
typedef struct
{
	unsigned char ucReaderType;
	unsigned char ucbServiceActivationCode[8];
	unsigned char ucFlagInputCardNO;		//0x00--no,  0x01--ok
	unsigned char ucTimeout;
	unsigned char ucbTradeType[10];
	unsigned char ucRowShowTradeType;
	unsigned char ucbTradeAmount[6];
	unsigned char ucRowShowTradeAmount;
	unsigned char ucFlagEncriptTradeAmount;
	unsigned char ucRowShowCardNo;
	unsigned char ucFlagEncriptCardNo;
	unsigned char ucDataLen;
	unsigned char ucbData[256];
	unsigned char ucDisplayType;
	unsigned char ucDisplayDataLen;
   	unsigned char ucbDisplayData[64];
	unsigned char ucRowShowDisplayData;
	unsigned char ucFlagEncriptDisplayData;
	unsigned char ucResultsType;

	unsigned char ucTradeAmountLen;
	unsigned char ucTradeType;
	unsigned char ucFlagOnline;
	unsigned char ucCardStatus;
	unsigned char ucCardNoLen;
	unsigned char ucbCardNo[20];
   	unsigned char ucCardTrackLen;
	unsigned char ucbChallenge[24];
	unsigned char ucbCardTrack[128];
	unsigned char ucbField55InDataBuf[12];
	unsigned char ucField55OutDataLen;
	unsigned char ucbField55OutDataBuf[256];
   	unsigned char ucNotField55DataLen;
	unsigned char ucbNotField55DataBuf[256];
	unsigned char ucField55IssuersLen;
	unsigned char ucbField55IssuersBuf[256];
	unsigned char ucbMACValue[8];
	unsigned char ucFlag_FuNongTong;	   	
}CARD_READER_INFO;
#else 

typedef struct
{
	unsigned char ucICFlag;	
	unsigned char ucReaderType;
	unsigned char ucTimeout;
	unsigned char ucPIKIndex;
	unsigned char ucMAKIndex;
	unsigned char ucTDKIndex;
	unsigned char ucbTradeType[11];//增加一个字节字符串补0
	unsigned char ucbTradeAmount[6];
	unsigned char ucCustomDataLen;
	unsigned char ucbCustomData[256];
	unsigned char ucTradeDisplayType;
	unsigned char ucCustomDisplayDataLen;
   	unsigned char ucbCustomDisplayData[256];
	unsigned char ucCustomEncryptDataLen;
   	unsigned char ucbCustomEncryptData[256];
   	unsigned char ucResultsReportType;

	unsigned char ucCustomDisplayDataLen_Lcd;
   	unsigned char ucbCustomDisplayData_Lcd[256];
	unsigned char ucTimestamp[20];
	unsigned char ucDisTradeAmountLen;
	unsigned char ucbDisTradeAmount[20];
	unsigned char ucTradeType;
	unsigned char ucFlagForceOnline;
	unsigned char ucCardStatus;
	unsigned char ucPBOCType;//交易类型
	unsigned char ucPBOCFlow;//交易流程
	unsigned char ucbCardServiceCodes[3];
	unsigned char ucCardNameLen;
	unsigned char ucbCardName[26];
	unsigned char ucCardNoLen;
	unsigned char ucbCardNo[20]; 	
	unsigned char ucbChallenge[24];
	unsigned int uiCardTrackLen;
	unsigned char ucbCardTrack[256];
	unsigned char ucbField55InDataBuf[12];
	unsigned int uiField55OutDataLen;
	unsigned char ucbField55OutDataBuf[256];
   	unsigned char ucNotField55DataLen;
	unsigned char ucbNotField55DataBuf[256];
	unsigned int uiField55IssuersLen;
	unsigned char ucbField55IssuersBuf[256];
	unsigned char ucbMACValue[8];	   	
}CARD_READER_INFO;
#endif
typedef struct
{
	unsigned char ucTimeout;
	unsigned char ucPinMin;
	unsigned char ucPinMax;
	unsigned char ucbChallenge[8];
	unsigned char ucbTradeAmount[8];
	unsigned char ucbCardPin[12];
	unsigned char ucCardPinLen;
	unsigned char ucbMACValue[8];
	unsigned char ucbStatus;
}KEYBOARD_INFO;

typedef enum
{
	KEY_INPUT_SUC=0,
	KEY_INPUT_EMPTY,
	KEY_INPUT_CANCLE,
	KEY_INPUT_TIMEOUT
}KEYBOARD_STATUS;
typedef struct
{
	unsigned char ucOffset1;
	unsigned char ucOffset2;
	unsigned char ucOffset3;
	unsigned char ucLen1;
	unsigned char ucLen2;
	unsigned char ucLen3;
   	unsigned char ucbBuf1[30];
	unsigned char ucbBuf2[30];
	unsigned char ucbBuf3[30];
	unsigned char ucTimeout;
}LCD_INFO;

//0x00 刷卡成功，加密处理成功
//0x01 刷卡失败
//0x02 刷卡成功，加密处理失败
//0x03 刷卡超时
//0x04 取消刷卡
//0x05 IC卡已插入
//0x06 手输成功
//0x07 非接IC卡

#define CMD_ERROR_NO_TMK		0x01//0x01	主密钥不存在
#define CMD_ERROR_NO_TCK		0x02//0x02	工作密钥不存在
#define CMD_ERROR_CHECK_VALUE	0x03//0x03	CHECKVALUE错误
#define CMD_ERROR_PARAM			0x04//0x04	参数错误
#define CMD_ERROR_VAR_LEN		0x05//0x05	可变数据域长度错误
#define CMD_ERROR_FRAME			0x06//0x06	帧格式错误
#define CMD_ERROR_EXE			0x07//0x07	执行异常
#define CMD_ERROR_DATABASE		0x08//0x08	数据库操作失败
#define CMD_ERROR_PRINT			0x09//0x09	无打印机
#define CMD_ERROR_UNKNOWN		0x0a//0x0A	未知指令
#define CMD_ERROR_LRC			0x0b//0x0B	LRC校验失败
#define CMD_ERROR_TIMEOUT		0x0c//0x0C	交易超时
#define CMD_ERROR_OTHER			0x0d//0x0D	其它
#define CMD_ERROR_LOCK			0x0e//0x0E	终端锁机
#define CMD_ERROR_NO_SUPPORT	0x0f//0x0F	暂不支持该参数
#define CMD_ERROR_ESC			0x10//0x10	取消


//操作返回码
#define CMD_OPERATION_SUCCESS	0x3030
#define CMD_P1P2_ERROR			0x6a86//P1 P2不正确
#define CMD_MEMORY_ERROR			0X6581 //存储器故障
#define CMD_MEMORY_BEYOND		0X6582 //存储器越界
#define CMD_VERIFY_CRC_ERROR		0X7000 //CRC错误
#define CMD_DATA_LEN_ERROR		0X3031 //CRC错误
#define CMD_ENCRIPT_DATA_ERROR	0X3032 //

#define UN_TMK_ADDR				(0)
#define UN_TRACK_ADDR			(0x20)
#define UN_PIN_ADDR				(0x40)
#define UN_MAC_ADDR				(0x60)
#define UN_CAIC_ADDR			(0x80)
#define UN_ZMK_ADDR				(0xa0)
#define UN_DEV_ADDR				(0xc0)


typedef enum
{
	DEVICE_INFO_SAFE_AUTH_NO,
	DEVICE_INFO_UP_WEB_NO = 6,
	DEVICE_INFO_DEV_TYPE = DEVICE_INFO_UP_WEB_NO + 5,
	DEVICE_INFO_DEV_VER = DEVICE_INFO_DEV_TYPE + 8,
	DEVICE_INFO_FIRM_VER =DEVICE_INFO_DEV_VER+20,
	DEVICE_INFO_DEV_APP_SRC = DEVICE_INFO_FIRM_VER+ 20,
	DEVICE_INFO_DEV_APP_VER = DEVICE_INFO_DEV_APP_SRC + 2,
	DEVICE_INFO_DEV_SN = DEVICE_INFO_DEV_APP_VER + 6,
	DEVICE_INFO_DEV_UPLOAD=DEVICE_INFO_DEV_SN + 24,
	DEVICE_INFO_DEV_STA = DEVICE_INFO_DEV_UPLOAD+1,	
	DEVICE_INFO_MAX = DEVICE_INFO_DEV_STA + 8
}DEVICE_INFO;


#define mPOS_ACK	0x58
#define mPOS_NAK	0x59

extern CARD_READER_INFO g_sCardReaderInfo;

extern KEYBOARD_INFO g_sKeyboardInfo;

extern LCD_INFO g_sLCDInfo;

extern unsigned char g_UpdateFlag;

void vGetTradeTime(unsigned char *pucTime);

void vCriticalSection(unsigned char ucMod);

int iPayThroughMSC(unsigned char *pucCardNo, unsigned char * pucCardNoLen);

int iMSCardIsICCard(unsigned char *pucCardTrack, unsigned char ucCardTrackLen);

int iPayThroughICC(unsigned char *pucInData, unsigned char ucMod, unsigned char * pucField55Data, unsigned int * pucField55DataLen);

int iPayQPOCFlow(unsigned char *pucInData, unsigned char ucMod, unsigned char * pucField55Data, unsigned int * pucField55DataLen);

int iCMD7700_UToP_Device_Boot(void);

int iCMD0101_UToP_GetDeviceVersion(unsigned char * pucOutData, unsigned int * puiLen);

int iCMD0102_UToP_SetCacheDataOnRAM(unsigned char * pucInData, unsigned int * puiLen);

int iCMD0103_UToP_ClearCacheDataOnRAM(unsigned char * pucInData, unsigned int * puiLen);

int iCMD0104_UToP_SetCacheDataOnFlash(unsigned char * pucInData, unsigned int * puiLen);

int iCMD0105_UToP_GetCacheDataOnFlash(unsigned char * pucInData, unsigned char * pucOutData, unsigned int * puiLen);

int iCMD0107_UToP_ResetDevice(void);

int iCMD0108_UToP_GetRTC(unsigned char * pucOutData, unsigned int * puiLen);

int iCMD0109_UToP_SetRTC(unsigned char * pucInData, unsigned int * puiLen);

int iCMD010A_UToP_Print(unsigned char * pucInData, unsigned int * puiLen);

int iCMD010B_PToU_SendPrintStatus(unsigned char * pucOutData, unsigned int * puiLen);

int iCMD010C_UToP_UpdateDeviceFirmware(unsigned char * pucInData, unsigned int * puiLen, unsigned char * pucOutData);

int iCMD0202_UToP_UpdateDeviceWorkKey(unsigned char * pucInData, unsigned int * puiLen);

int iEncryptDataByTDKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge);

int iEncryptDataByMAKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge);

int iEncryptDataByPIKIndex(unsigned char *pucData, unsigned int uiDataLen, unsigned char ucKeyIndex, unsigned char *pucChallenge);

int iCMD0204_UToP_VerifyMAC(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0207_UToP_EncriptData(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0304_PToU_SendCardReaderStatus(unsigned char ucMod);

int iCMD0304_UToP_OpenCardReader(unsigned char * pucInData, unsigned int *puiLen);

int iCMD0305_GetCardNo(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0306__GetCardTrack(unsigned char * pucOutData, unsigned int *puiLen);

int iGetAndDisplayPin(unsigned char * pucPin, unsigned char * pucPinLen, unsigned char ucPinLenMin, unsigned char ucPinLenMax);

int iWaitForESCOrOK(void);

int iEncryptPinData(unsigned char *pucData);

unsigned char iCMD_InputPIN(void);

int iCMD0404_PToU_UpLoadKeyboardStatus(unsigned char *pucOut,unsigned int *uiLen);

int iCMD0404_UToP_OpenKeyboard(unsigned char * pucInData, unsigned int *puiLen);

int iCMD0405_UToP_GetPIN(unsigned char * pucOutData, unsigned int *puiLen);

int iCMD0501_UpdateRSAPublicKey(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0502_UpdateAID(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0503_UToP_StartPBOC(unsigned char * pucInData, unsigned int *puiLen);

int iCMD0503_PToU_UploadPBOCData(unsigned char ucStatus);

int iFill_CMD0505_Packet(void);

int iCMD0505_UToP_GetPBOC(unsigned char * pucOutData, unsigned int *puiLen);

int iCMD0506_UToP_StartPBOCIssuers(unsigned char * pucInData, unsigned int *puiLen, unsigned char * pucOutData);

int iCMD0507_PToU_UploadPBOCIssuersStatus(unsigned char ucMode);

int iCMD0510_UToP_ClosedEMV(unsigned char *pucIn,unsigned int *puiLen);

int iCMD0508_UToP_GetPBOCIssuers(unsigned char * pucOutData, unsigned int *puiLen);

int iCMD0701_UToP_LCDShow(unsigned char * pucInData, unsigned int *puiLen);

void vTask0306_UToP_LCDShow(void);

int iTaskGetAndDisplayInputPin(unsigned char ucFirstKey, unsigned char * pucCardNo, unsigned char * pucCardNoLen);

unsigned char iCMD_Factory_DeviceInit(unsigned char * pucInData, unsigned char uciLen);

unsigned char mPOS_FrameAnalyse(void);

void mPOS_ACKorNAK(unsigned char ANS);

#endif
//************************************end of file****************************************
