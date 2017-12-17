#ifndef __Variable_H__
#define __Variable_H__

#include "MID_PBOC.h"
#include "HAL_7816.h"


//#include "safety.h"


//#include "EMV.h"
#include "EMV_Tag.h"
#include "EMV_Parameter.h"




typedef struct{
	unsigned char type;
	unsigned char length[4];
	unsigned char slot;
	unsigned char seq;
	unsigned char param[3];
	unsigned char dat[1];
}CCID_HDR;

// typedef struct{

// 	unsigned char *pCommBuff;	//输入输出数据缓冲区BUF指针，用于通信

// 	unsigned char CLA;
// 	unsigned char INS;
// 	unsigned char PP1;
// 	unsigned char PP2;
// 	unsigned char  PP3;
// 	unsigned char  LC;
// 	unsigned char  LE;
// 	unsigned char *pInData;		//除去命令之外的数据区指针

// 	unsigned int  RLE;		   	//实际要返回的数据长度
// 	unsigned char *pOutData;	//要返回的数据区的指针
// 	unsigned char SW1;
// 	unsigned char SW2;
// }APDU;



// extern APDU   g_APDU;
// extern  APDU	T_APDU;	





//Candidate Type
typedef struct{
	unsigned char ADF_Valid;
	unsigned char App_Name[16];				  //M
	unsigned char App_Name_Len;
	unsigned char App_Label[16];			  //M
	unsigned char App_Label_Len;
	unsigned char App_Preferred_Name[16];	  //O
	unsigned char App_Preferred_Name_Len;
	unsigned char IssuerCodeTableIndex;
	unsigned char API;						  //O
	unsigned char ASI;						  //O
} CANDIDATE_TYPE;  							  //Terminal Parameter

//AID List
typedef struct{
	unsigned char   AID[16];
	unsigned char   AID_len;
	unsigned char		ASI;
} AID_TYPE;

//DDF Stack
typedef struct{
	unsigned char Sfi;
	unsigned char Record_Number; 
	unsigned char Rest_Len;
	unsigned char Rest_Data[255];
} DDF_STACK;

//终端参数
extern APDU			G_APDU;
extern APDU			T_APDU;	
//extern APDU			Z8_APDU;

extern unsigned char 	  G_B_GACResult;
extern unsigned char  	uchTacDefault[5];  //终端行为码应该保存在Flash中
extern unsigned char  	uchTacDenial[5];   //同上
extern unsigned char  	uchTacOnline[5];   //同上
extern unsigned char  	g_b_plain_pin[PIN_BUF_LENGTH];
extern unsigned char 	  ICC_Protocol;
extern unsigned char    internal_state;
extern unsigned int		  g_response_len;

//extern SHA1Context			g_SHA1Context;

extern unsigned int 	ThresholdforBiasedRandomSelection;         //must be zero or a positive number less than the floor limit
extern unsigned char  PercentforRandomSelection;   //0~99
extern unsigned char 	MaxPercentforBiasedRandomSelection;   //in the range of 0 to 99 but at least as high as the previous ‘Target Percentage to be Used for Random Selection
extern unsigned int   current_transaction_amount;
extern unsigned char 	funckey_pressed;
extern unsigned char  current_item;
extern unsigned char	current_record;

extern const unsigned char 	G_ATR_BUF[24];

extern unsigned char 	*puchAmount;
extern unsigned char  *puchBinAmount;
extern unsigned char  *puchAmountOther;
extern unsigned char 	*puchCurrencyCode;
extern unsigned char  *puchIFDSerailNO;
extern unsigned char  *puchTermCapability;
extern unsigned char  *puchTermType;
extern unsigned char  *puchTermCountryCode;
extern unsigned char  *puchTermID;
extern unsigned char  *puchAcquirerID;
extern unsigned char  *puchMerchantID;
extern unsigned char  *puchMerchantCat;
extern unsigned char  *puchPOSEntryMode;
extern unsigned char 	*puchTvr;
extern unsigned char 	*puchDate;
extern unsigned char 	*puchTranType;
extern unsigned char 	*puchUnpreData;
extern unsigned char 	*puchTime;
extern unsigned char 	*puchTsi;
extern unsigned char 	*puchCvmRes;	//CVM验证结果
extern unsigned char 	*puchArc;			//授权响应码
extern unsigned char 	*puchVer; 		//终端应用版本
extern unsigned char  *puchIAD;
extern unsigned char  *puchTSC;
extern unsigned char  *puchTCHash;	//TC Hash Value	
extern unsigned char  *puchTerminalAID;
extern unsigned char  *puchTerminalFloorLimit;

extern unsigned char 	*puchTerminalTransProp;

extern DDF_STACK		  ddf_stack[DDF_STACK_DEPTH];	
extern AID_TYPE		    AID_List[MAX_AID_NUMBER];
extern CANDIDATE_TYPE   Candidate_List[MAX_CANDIDATE_NUMBER];

extern unsigned char  	Reader_VarDo_List[READER_VARDO_LIST_SIZE];
extern unsigned char  	Card_Do_List[CARD_DO_LIST_SIZE];
extern unsigned char Reader_ConstantDo_List[READER_CONSTANTDO_LIST_SIZE];

extern unsigned char 	CardNoLen;
extern unsigned char 	CardNo[20];
extern unsigned char 	g_b_trans_state;

//extern unsigned char 	g_apdu_buf[300];
//extern unsigned char 	t_apdu_buf[300];
extern unsigned char 	g_Apdu_Buffer[1200];
extern unsigned char 	T_Apdu_Buffer[300];
//extern unsigned char 	encrypt_buf[600];

extern unsigned char 	atr_ok;
extern unsigned char 	second_packet;

extern unsigned char    Nok_Reason;
extern unsigned char    Current_DF_Name[16];
extern unsigned char	  Current_DF_Len;
extern unsigned char    Candidate_Number;

//保存卡片中获取的DO值，LV格式
extern unsigned char  	uchAc[9];
extern unsigned char  	uchCid[2];
extern unsigned char  	uchIad[33];	//发卡行应用数据
extern unsigned char 		uchAtc[3];
extern unsigned char  	uchAip[3];
extern unsigned char  	uchAid[17];
extern unsigned char  	uchAppVer[3];
extern unsigned char  	uchMstrip2[20];	//磁条2等效数据
extern unsigned char  	uchHolderName[27];	
extern unsigned char  	uchHolderNum[41];	//持卡人证件号
extern unsigned char  	uchEffectiveDate[4];
extern unsigned char  	uchExpirationDate[4];
extern unsigned char  	uchPan[11];
extern unsigned char  	uchPanNum[2];
extern unsigned char  	uchAuc[3];
extern unsigned char  	uchCvm[254];
extern unsigned char  	uchIacDefault[6];
extern unsigned char  	uchIacDenial[6];
extern unsigned char  	uchIacOnline[6];
extern unsigned char  	uchCountryCode[3];	   //发卡行国家代码
extern unsigned char  	uchServiceCode[3];	   //服务码
extern unsigned char  	uchCdol1[254];		   //??卡片最大可以返回255字节
extern unsigned char  	uchCdol2[254];		   //??卡片最大可以返回255字节
extern unsigned char 		uchAppCurrencyCode[3];	//应用货币代码
extern unsigned char  	uchMstrip1[100];
extern unsigned char  	uchCAKeyIndex[2];

extern unsigned char   uchAppCurrencyEx[2];
extern unsigned char   uchAppDiscretionData[33];
extern unsigned char   uchAFL[254];
extern unsigned char   uchADFName[17];
extern unsigned char   uchAppLabel[17];
extern unsigned char   uchAPI[2];
extern unsigned char   uchAppRefCur[9];
extern unsigned char   uchAppRefCurEx[5];
extern unsigned char   uchBIC[12];
extern unsigned char   uchHolderNameExt[46];
extern unsigned char   uchDAC[3];
extern unsigned char   uchDDOL[254];
extern unsigned char   uchDDFName[17];
extern unsigned char   uchFCI[224];
extern unsigned char   uchICCDynamNum[9];
extern unsigned char   uchICCPINEncCACert[NI_NUM+1];
extern unsigned char   uchICCPINEncCAExp[4];
extern unsigned char   uchICCPINEncCAReminder[NPE_NUM-NI_NUM+42+1];
extern unsigned char   uchICCKeyCert[NI_NUM];
extern unsigned char   uchICCKeyExp[4];
extern unsigned char   uchICCKeyReminder[NIC_NUM-NI_NUM+42+1];
extern unsigned char   uchIBAN[35];
extern unsigned char   uchIssuerCodeTableIndex[2];
extern unsigned char   uchIssuerCountryCode2[3];
extern unsigned char   uchIssuerCountryCode3[4];
extern unsigned char   uchIIN[4];
extern unsigned char   uchIssuerKeyCert[NCA_NUM];
extern unsigned char   uchIssuerKeyExp[4];
extern unsigned char   uchIssuerKeyReminder[NI_NUM-NCA_NUM+36+1];
extern unsigned char   uchIssuerURL[100];
extern unsigned char   uchLangPref[9];
extern unsigned char   uchLATC[3];
extern unsigned char   uchLogEntry[3];
extern unsigned char   uchLogFormat[30];
extern unsigned char   uchLCOL[2];
extern unsigned char   uchPTC[2];
extern unsigned char   uchSFI[2];
extern unsigned char   uchSDAData[NIC_NUM+1];
extern unsigned char   uchSSAData[NI_NUM+1];
extern unsigned char   uchSDATagList[100];
extern unsigned char   uchTrack2Data[100];
extern unsigned char   uchUCOL[2];
extern unsigned char   uchTDOL[254];
extern unsigned char   uchAppPrefName[17];
extern unsigned char   uchPDOL[255];
extern unsigned char   uchTag73[254];
//extern unsigned char   uchLogFile[500];
extern unsigned short  uchLogFileLen;
extern unsigned short  uchLogRecordLen;
extern unsigned char  G_flag_BalanceOrLoad; 

extern unsigned char uchAvailableOFFAmount[7];

extern unsigned char uchICCardProp[3];
extern unsigned char uchICAuthenticationData[17];

//endlihuan

extern void GVAR_Init(void);
extern void APDU_BuffFormat(APDU * psAPDUData, unsigned char * CommBuff);
extern void T_APDU_BuffFormat(APDU * psAPDUData, unsigned char * CommBuff);
void vPBOC_Init(void);


#endif
