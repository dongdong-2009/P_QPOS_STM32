
#ifndef __MID_PBOC_H__
#define __MID_PBOC_H__

//#include "usb_ccid.h"
//#include "EMV_Return_Value.h"
//#include "EMV_Tag.h"
//#include "EMV_Command.h"
//#include "EMV_Parameter.h"
//#include "ccid_value.h"   

#include "HAL_7816.h"

#define	 	CONTACT_PSE_NAME					"1PAY.SYS.DDF01"	   //0x31,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31
#define	NOK									0xFFFF
#define	NONE								0x0000
#define	CANCELLED							0x0001
#define	CARD_BLOCKED						0x0002
#define	CARD_MISSING						0x0003
#define	IC_ERROR							0x0004
#define	DATA_ERROR							0x0005
#define	EMPTY_LIST							0x0006
#define	GPO6985								0x0007	
#define	MISSING_DATA						0x0008
#define	NO_CARD_INSERTED					0x0009
#define	NOT_ACCEPTED						0x000A	
#define	TIME_OUT							0x000B
#define	ABORTED								0x000C
#define	APPLICATION_BLOCKED					0x000D
#define GPO_ERROR							0x000E

#define	EMV_TRUE							0x0000
#define	EMV_FALSE							0x0001
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

extern const unsigned char cucAIDCnt;

extern const unsigned char cucAIDBuf[15 * 128];

//定时器2初始化
void TIM_Configuration(void);
//获取随机数
void MID_ALG_GenRand(unsigned char *pucBuf, unsigned int uiLen);
//从指定地址处连续读取片内Flash若干字节数据
unsigned int MID_NVMEM_Read(unsigned int StartAddr, unsigned int ReadLen, unsigned char * puchDataBuff);
unsigned int MID_NVMEM_Write(unsigned int StartAddr, unsigned int WriteLen, unsigned char* puchDataBuff);

//初始化应用
unsigned short EMV_AppInitialization(void);
//选择卡片PSE或DDF
unsigned short	EMV_SelectDDF(unsigned char* DF_Name,unsigned char DF_nameLen);
//Get Process On
unsigned short EMV_GetProcessOn(void);	
//Read Application Data
unsigned short EMV_ReadAppData(void); 
//处理限制
unsigned short  EMV_ProcessingRestriction(void); 
//持卡人验证
unsigned short EMV_CardholderVerification(void); 
//终端风险管理
void EMV_TerminalRiskManagement(void); 
//终端行为分析
unsigned short EMV_TerminalActionAnalysis(unsigned char revFlag); 
//联机处理
unsigned short EMV_OnlineProcessing(unsigned char revFlag); 
//建立候选应用列表 PSE方式
unsigned short  EMV_BuildCLbyPSE(void);	
//建立候选应用列表 List of AID方式
unsigned short  EMV_BuildCLbyAID(void);	
//最终选择
unsigned short	EMV_FinalSelection(void);
//Search Matched AID
unsigned short EMV_SearchAID(unsigned char *AID, unsigned char len);
//Clear Candidate List
void EMV_ClearCandidateList(void);
//Remove a Candidate from the Candidate List
unsigned short EMV_RemoveACandidate(unsigned char *AID, unsigned char len);
//Sort Candidate List
void EMV_SortCandidateList(void);
//Show Candidate List
unsigned short EMV_Show_Candidate_List(unsigned char Len);
//Search Tag
unsigned char *SearchTag(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag);
//Search Tag For Script
unsigned char *SearchTagforScript(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag);
/********************     EMV Application Interface     ********************/
//初始化应用参数
void APP_POS_PAY_Init(void);				
void EMV_InitDDFStack(void);
void EMV_InitAIDList(void);
//获得电子现金金额
unsigned int GetCash(APDU *psAPDU);
//获得电子现金限额
unsigned int ReadUpLimitPrice(APDU *psAPDU);
//联机密文PIN处理
unsigned short EMV_OnlineEncryptedPinProcessing(void);

#endif
