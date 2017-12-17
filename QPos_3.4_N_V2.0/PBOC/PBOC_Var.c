#include "PBOC_Var.h"
//#include "USB_CCID.h"
#include "MID_PBOC.h"
#include "APP_PBOC.h"
#include "string.h"


#include "HAL_7816.h"
// APDU    g_APDU;
// APDU		T_APDU;	





//终端变量
unsigned char 	G_B_GACResult;
unsigned char  	uchTacDefault[5];  //终端行为码应该保存在Flash中
unsigned char  	uchTacDenial[5];   //同上
unsigned char  	uchTacOnline[5];   //同上
unsigned char  	g_b_plain_pin[PIN_BUF_LENGTH];
unsigned char 	ICC_Protocol;
unsigned char   internal_state;
unsigned int		g_response_len;

unsigned int 	  ThresholdforBiasedRandomSelection;         //must be zero or a positive number less than the floor limit
unsigned char   PercentforRandomSelection;   //0~99
unsigned char 	MaxPercentforBiasedRandomSelection;   //in the range of 0 to 99 but at least as high as the previous ‘Target Percentage to be Used for Random Selection
unsigned int    current_transaction_amount;
unsigned char 	funckey_pressed;
unsigned char   current_item;
unsigned char	  current_record;

//SHA1Context			g_SHA1Context;

unsigned char 	*puchAmount;
unsigned char   *puchBinAmount;
unsigned char   *puchAmountOther;
unsigned char 	*puchCurrencyCode;
unsigned char   *puchIFDSerailNO;
unsigned char   *puchTermCapability;
unsigned char   *puchTermType;
unsigned char   *puchTermCountryCode;
unsigned char   *puchTermID;
unsigned char   *puchAcquirerID;
unsigned char   *puchMerchantID;
unsigned char   *puchMerchantCat;
unsigned char   *puchPOSEntryMode;
unsigned char 	*puchTvr;
unsigned char 	*puchDate;
unsigned char 	*puchTranType;
unsigned char 	*puchUnpreData;
unsigned char 	*puchTime;
unsigned char 	*puchTsi;
unsigned char 	*puchCvmRes;	//CVM验证结果
unsigned char 	*puchArc;			//授权响应码
unsigned char 	*puchVer; 		//终端应用版本
unsigned char   *puchIAD;
unsigned char   *puchTSC;
unsigned char   *puchTCHash;	//TC Hash Value	
unsigned char   *puchTerminalAID;
unsigned char   *puchTerminalFloorLimit;
unsigned char 	*puchTerminalTransProp;

DDF_STACK				ddf_stack[DDF_STACK_DEPTH];	
AID_TYPE				AID_List[MAX_AID_NUMBER];
CANDIDATE_TYPE  Candidate_List[MAX_CANDIDATE_NUMBER];

//unsigned char  	Reader_VarDo_List[READER_VARDO_LIST_SIZE];
//unsigned char  	Card_Do_List[CARD_DO_LIST_SIZE];

//unsigned char 	CardNoLen;      //////////////和原文件“qposmegapply.c”冲突
unsigned char 	CardNo[20];
unsigned char 	g_b_trans_state;

// unsigned char 	g_Apdu_Buffer[1200];
// unsigned char 	T_Apdu_Buffer[300];
//unsigned char 	encrypt_buf[600];

unsigned char 	atr_ok;
unsigned char 	second_packet;

unsigned char   Nok_Reason;
unsigned char   Current_DF_Name[16];
unsigned char	  Current_DF_Len;
unsigned char   Candidate_Number;

//保存卡片中获取的DO值，LV格式
unsigned char  	uchAc[9];
unsigned char  	uchCid[2];
unsigned char  	uchIad[33];	//发卡行应用数据
unsigned char 	uchAtc[3];
unsigned char  	uchAip[3];
unsigned char  	uchAid[17];
unsigned char  	uchAppVer[3];
unsigned char  	uchMstrip2[20];	//磁条2等效数据
unsigned char  	uchHolderName[27];	
unsigned char  	uchHolderNum[41];	//持卡人证件号
unsigned char  	uchEffectiveDate[4];
unsigned char  	uchExpirationDate[4];
unsigned char  	uchPan[11];
unsigned char  	uchPanNum[2];
unsigned char  	uchAuc[3];
unsigned char  	uchCvm[254];
unsigned char  	uchIacDefault[6];
unsigned char  	uchIacDenial[6];
unsigned char  	uchIacOnline[6];
unsigned char  	uchCountryCode[3];	   //发卡行国家代码
unsigned char  	uchServiceCode[3];	   //服务码
unsigned char  	uchCdol1[254];		   //??卡片最大可以返回255字节
unsigned char  	uchCdol2[254];		   //??卡片最大可以返回255字节
unsigned char 	uchAppCurrencyCode[3];	//应用货币代码	
unsigned char   uchMstrip1[100];
unsigned char   uchCAKeyIndex[2];

unsigned char   uchAppCurrencyEx[2];
unsigned char   uchAppDiscretionData[33];
unsigned char   uchAFL[254];
unsigned char   uchADFName[17];
unsigned char   uchAppLabel[17];
unsigned char   uchAPI[2];
unsigned char   uchAppRefCur[9];
unsigned char   uchAppRefCurEx[5];
unsigned char   uchBIC[12];
unsigned char   uchHolderNameExt[46];
unsigned char   uchDAC[3];
unsigned char   uchDDOL[254];
unsigned char   uchDDFName[17];
unsigned char   uchFCI[224];
unsigned char   uchICCDynamNum[9];
unsigned char   uchICCPINEncCACert[NI_NUM+1];
unsigned char   uchICCPINEncCAExp[4];
unsigned char   uchICCPINEncCAReminder[NPE_NUM-NI_NUM+42+1];
unsigned char   uchICCKeyCert[NI_NUM];
unsigned char   uchICCKeyExp[4];
unsigned char   uchICCKeyReminder[NIC_NUM-NI_NUM+42+1];
unsigned char   uchIBAN[35];
unsigned char   uchIssuerCodeTableIndex[2];
unsigned char   uchIssuerCountryCode2[3];
unsigned char   uchIssuerCountryCode3[4];
unsigned char   uchIIN[4];
unsigned char   uchIssuerKeyCert[NCA_NUM];
unsigned char   uchIssuerKeyExp[4];
unsigned char   uchIssuerKeyReminder[NI_NUM-NCA_NUM+36+1];
unsigned char   uchIssuerURL[100];
unsigned char   uchLangPref[9];
unsigned char   uchLATC[3];
unsigned char   uchLogEntry[3];
unsigned char   uchLogFormat[30];
unsigned char   uchLCOL[2];
unsigned char   uchPTC[2];
unsigned char   uchSFI[2];
unsigned char   uchSDAData[NIC_NUM+1];
unsigned char   uchSSAData[NI_NUM+1];
unsigned char   uchSDATagList[100];
unsigned char   uchTrack2Data[100];
unsigned char   uchUCOL[2];
unsigned char   uchTDOL[254];
unsigned char   uchAppPrefName[17];
unsigned char   uchPDOL[255];
unsigned char   uchTag73[254];
//unsigned char   uchLogFile[500];
unsigned short  uchLogFileLen;
unsigned short  uchLogRecordLen;

unsigned char uchAvailableOFFAmount[7];
unsigned char   uchICCStaticNum[2];

unsigned char uchICCardProp[3];
unsigned char uchICAuthenticationData[17];
 
const unsigned char  	G_ATR_BUF[24] = {
	0x00, 0x3b, 0x00, 0x11, 0x00, 0x81, 0x31, 0x46, 
	0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char  G_flag_BalanceOrLoad; 			  //是主账户查询余额还是圈存	0：主账户查询余额   1：圈存


void APDU_BuffFormat(APDU * psAPDUData, unsigned char * CommBuff)
{
// //	(*psAPDUData).pCommBuff =  CommBuff + 10;
// //	(*psAPDUData).pInData = CommBuff + 18;
// //	(*psAPDUData).pOutData = CommBuff + 512;


// 	(*psAPDUData).pCommBuff =  CommBuff + 10;
// 	(*psAPDUData).pInData = (*psAPDUData).pCommBuff + 18;
// 	(*psAPDUData).pOutData = (*psAPDUData).pInData + 512;
// 		(*psAPDUData).CLA = 0;
// 		(*psAPDUData).INS = 0;
// 		(*psAPDUData).PP1 = 0;
// 		(*psAPDUData).PP2 = 0;
// 		(*psAPDUData).PP3 = 0;
// 		(*psAPDUData).LC = 0;
// 		(*psAPDUData).LE = 0;
// 		(*psAPDUData).RLE = 0;
// 		(*psAPDUData).SW1 = 0;
// 		(*psAPDUData).SW2 = 0;
// 		return;
	return;	
}

void T_APDU_BuffFormat(APDU * psAPDUData, unsigned char * CommBuff)
{
// //	(*psAPDUData).pCommBuff =  CommBuff + 10;
// //	(*psAPDUData).pInData = CommBuff + 18;
// //	(*psAPDUData).pOutData = CommBuff + 512;


// 	(*psAPDUData).pCommBuff =  CommBuff + 10;
// 	(*psAPDUData).pInData = (*psAPDUData).pCommBuff + 18;
// 	(*psAPDUData).pOutData = (*psAPDUData).pInData + 12;

// 	(*psAPDUData).CLA = 0;
// 	(*psAPDUData).INS = 0;
// 	(*psAPDUData).PP1 = 0;
// 	(*psAPDUData).PP2 = 0;
// 	(*psAPDUData).PP3 = 0;
// 	(*psAPDUData).LC = 0;
// 	(*psAPDUData).LE = 0;
// 	(*psAPDUData).RLE = 0;
// 	(*psAPDUData).SW1 = 0;
// 	(*psAPDUData).SW2 = 0;
 	return;	
}


void GVAR_Init(void)
{  
	unsigned char *puchTvr = Reader_VarDo_List + 44;
	unsigned char *puchTsi = Reader_VarDo_List + 72;

	//初始化终端行为码，应该保存在Flash中
	memset(uchTacDefault, 0, 5);
	memset(uchTacDenial, 0, 5);
	memset(uchTacOnline, 0, 5);	 

	//初始化明文PIN值
	memset(g_b_plain_pin,0x00,PIN_BUF_LENGTH);

	//交易状态初始化
	g_b_trans_state = TRANS_IDLE; 
	ICC_Protocol = EMV_PROTOCOL_UNKNOWN;    //IC卡协议类型初始化

 	atr_ok =0;
	Nok_Reason = NONE;
	G_B_GACResult = 0;
	Current_DF_Len = 0;
	internal_state = WAIT_FOR_TRANSACTION;
	current_item = 0;
	current_record = 0;
	funckey_pressed = 0;
	g_response_len = 0;
 	memset(Current_DF_Name,0x00,16);
	memset(uchPDOL,0x00,255);
	memset(uchTag73,0x00,253);

	//初始化TVR, TSI
	memset(puchTvr, 0, 5);
	memset(puchTsi, 0, 2);	

//	TIM_Configuration();   //定时器2初始化
	return;
}
//***************************************************************************************
//* Function Name  : vPBOC_Init
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vPBOC_Init(void)
{
	HAL_7816_DeepShutdown();
	GVAR_Init();
	//APP_POS_PAY_Init();
}

