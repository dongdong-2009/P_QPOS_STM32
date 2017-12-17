
#include "APP_PBOC.h"
#include "string.h"
#include "stdlib.h"
#include "PBOC_Var.h"

#include "HAL_7816.h"
#include "CCID_VALUE.h"
//#include "safety.h"
//#include "EMV.h"
#include "EMV_Tag.h"
#include "EMV_Parameter.h"
#include "stm32f10x.h"
#include "sercrity.h"

#include "IROM.h"
#include "Roc_LittleTool.h"
#include "UP_MPOS_CMD.h"

typedef unsigned int u32; 
//extern APDU g_APDU;
//extern unsigned char g_pucMSCardKey[];

extern unsigned short	EMV_SelectADFforLogEntry(unsigned char* AID,unsigned char AID_len);
//unsigned char ucKey111[16];
//	unsigned char uchMstrip2_copy[80];





unsigned char Reader_ConstantDo_List[READER_CONSTANTDO_LIST_SIZE] = {
/*00*/   	0x9F, 0x7A, 0x01, 0x01,					//电子现金终端指示器			 		???ICC Data
///*04*/   	0x5F, 0x2A, 0x02, 0x08, 0x40, 			//交易货币代码
/*04*/   	0x5F, 0x2A, 0x02, 0x01, 0x56, 																			
/*09*/	 	0x5F, 0x36, 0x01, 0x02, 					//Transaction Currency Exponent
///*13*/   	0x9F, 0x1A, 0x02, 0x08, 0x40, 			//终端国家代码
/*13*/   	0x9F, 0x1A, 0x02, 0x01, 0x56, 	
///*18*/   	0x9F, 0x33, 0x03, 0xA0, 0x48, 0x00,			//终端性能
/*18*/   	0x9F, 0x33, 0x03, 0xE0, 0xF1, 0xC8,																
/*24*/   	0x9F, 0x3A, 0x04, 0x00, 0x00, 0x00, 0x00,	  		//Amount,Reference Currency (Binary)
/*31*/   	0x9F, 0x40, 0x05, 0x6E, 0x00, 0xB0, 0x10, 0x01,     	  	//终端附加性能,编码表未指定
/*39*/   	0x9F, 0x1E, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,	//终端IFD序号
///*50*/   	0x9F, 0x35, 0x01, 0x34,		 									//终端类型
/*50*/   	0x9F, 0x35, 0x01, 0x22,		 																				
/*54*/   	0x9F, 0x1C, 0x08, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 	//终端标识
/*65*/   	0x5F, 0x57, 0x01, 0x00,											//账户类型:默认-未指定
/*69*/   	0x9F, 0x01, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  			//收单行标识
/*78*/	 	0x9F, 0x1D, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 	//Terminal Risk Management Data
/*89*/   	0x9F, 0x4E, 0x14,0xd6, 0xd0, 0xb9, 0xfa, 0xd2, 0xf8, 									//商户名称
				   		 0xc1, 0xaa, 0xd4, 0xda, 0xcf, 0xdf, 0xd6, 0xa7, 0xb8, 0xb6, 0, 0, 0, 0, 
/*112*/  	0x9F, 0x16, 0x0F, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 
						0x42, 0x43, 0x44, 0x45, 0x46, 		//Merchant Identifier
/*130*/  	0x9F, 0x15, 0x02, 0x11, 0x022,													//Merchant Category Code
/*135*/  	0x9F, 0x39, 0x01, 0x09, 													//POS Entry Mode
};			 
unsigned char Reader_VarDo_List[READER_VARDO_LIST_SIZE]={
//const unsigned char Con_Reader_VarDo_List[READER_VARDO_LIST_SIZE] = {					 
/*00*/		0x9F, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//授权金额 (Numeric)
/*09*/     	0x81, 0x04, 0x00, 0x00, 0x00, 0x00,						//授权金额 (Binary)
/*15*/		0x9F, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//其他金额 (Numeric)
/*24*/	   	0x9F, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//第二其他金额  (Binary)	
/*33*/		0x9F, 0x3C, 0x02, 0x08, 0x40, 							//Transaction Reference Currency Code
/*38*/		0x9F, 0x3D, 0x01, 0x01, 								//Transaction Reference Currency Exponent
/*42*/     	0x95, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00,	            //TVR
/*49*/	   	0x9A, 0x03, 0x12, 0x06, 0x20,							//交易日期
/*54*/		0x9F, 0x21, 0x03, 0x00, 0x00, 0x00,	  					//交易时间
/*60*/	   	0x9C, 0x01, 0x00,	            						//交易类型
/*63*/	   	0x9F, 0x37, 0x04, 0x00, 0x00, 0x00, 0x00,				//不可预知数
/*70*/		0x9B, 0x02, 0x00, 0x00,	              					//TSI
/*74*/	   	0x9F, 0x09, 0x02, 0x00, 0x8C,		  					//终端应用版本号
/*79*/	   	0x9F, 0x34, 0x03, 0x00, 0x00, 0x00,    					//CVM验证结果
/*85*/	   	0x8A, 0x02, 0x00, 0x00, 	                             //授权响应码			  ???Issuer Data
/*89*/	   	0x9F, 0x1B, 0x04, 0x00, 0x00, 0x00, 0x00, 				//终端最低限额	 		???
/*96*/	   	0x91, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//发卡行认证数据
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*114*/		0x9F, 0x41, 0x04, 0x00, 0x00, 0x00, 0x01,							//Transaction Sequence Counter
/*121*/	   	0x99, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		  				//PIN	
/*129*/	   	0x98, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		  
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//TC Hash Value
/*151*/		0x9F, 0x06, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 		//Terminal AID
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	
/*170*/		 0x9F, 0x66, 0x04, 0x7c, 0x00 , 0x00 , 0x80
};	 		 
unsigned char		Card_Do_List[CARD_DO_LIST_SIZE] = {	 
//unsigned char	Con_Card_Do_List[CARD_DO_LIST_SIZE] = {
/*00*/	   	0x9F, 0x26, 0x04, 0x00, 0x00, 0x00, 0x00,		//AC
/*07*/	   	0x9F, 0x27, 0x04, 0x00, 0x00, 0x00, 0x00,		//CID
/*14*/	   	0x9F, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00,		//发卡行应用数据
/*21*/	   	0x9F, 0x36, 0x04, 0x00, 0x00, 0x00, 0x00,		//ATC
/*28*/	   	0x82, 0x04, 0x00, 0x00, 0x00, 0x00,					//AIP
/*34*/	   	0x84, 0x04, 0x00, 0x00, 0x00, 0x00,					//AID
/*40*/	   	0x9F, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00,		//应用版本
/*47*/	   	0x57, 0x04, 0x00, 0x00, 0x00, 0x00,					//磁条2等效数据	 max.19
/*53*/	   	0x5F, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00,		//持卡人姓名 2~26   
/*60*/	   	0x9F, 0x61, 0x04, 0x00, 0x00, 0x00, 0x00,		//持卡人证件号 1~40		???
/*67*/	   	0x5F, 0x25, 0x04, 0x00, 0x00, 0x00, 0x00,		//应用生效期
/*74*/	   	0x5F, 0x24, 0x04, 0x00, 0x00, 0x00, 0x00, 		//应用失效期
/*81*/	   	0x5a, 0x04, 0x00, 0x00, 0x00, 0x00,					//PAN, max. 10
/*87*/	   	0x5F, 0x34, 0x04, 0x00, 0x00, 0x00, 0x00,		//PAN序列号
/*94*/	   	0x9F, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00,		//AUC应用用途控制
/*101*/	   	0x8e, 0x04, 0x00, 0x00, 0x00, 0x00,					//CVM,10~252
/*107*/	   	0x9F, 0x0D, 0x04, 0x00, 0x00, 0x00, 0x00,		//发卡行行为码-default
/*114*/	   	0x9F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,		//发卡行行为码-denial
/*121*/	   	0x9F, 0x0F, 0x04, 0x00, 0x00, 0x00, 0x00,		//发卡行行为码-online
/*128*/	   	0x5F, 0x28, 0x04, 0x00, 0x00, 0x00, 0x00,		//发卡行国家代码
/*135*/	   	0x5F, 0x30, 0x04, 0x00, 0x00, 0x00, 0x00,		//服务码
/*142*/	   	0x8c, 0x04, 0x00, 0x00, 0x00, 0x00,					//CDOL1, max252
/*148*/	   	0x8d, 0x04, 0x00, 0x00, 0x00, 0x00,					//CDOL2, max252
/*154*/		0x9F, 0x42, 0x04, 0x00, 0x00, 0x00, 0x00,		//应用货币代码
/*161*/	   	0X9F, 0x1F, 0x04, 0x00, 0x00, 0x00, 0x00, 		//Track 1 Data
/*168*/		0x8F, 0x04, 0x00, 0x00, 0x00, 0x00,    			//CA Key Index
/*174*/		0x9F, 0x44, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Application Currency Exponent
/*181*/		0x9F, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Application Discretionary Data
/*188*/		0x94, 0x04, 0x00, 0x00, 0x00, 0x00, 				//AFL
/*194*/		0x4F, 0x04, 0x00, 0x00, 0x00, 0x00, 				//ADF Name
/*200*/		0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 				//AppLabel
/*206*/		0x87, 0x04, 0x00, 0x00, 0x00, 0x00, 				//API
/*212*/		0x9F, 0x3B, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Application Reference Currency
/*219*/		0x9F, 0x43, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Application Reference Currency Exponent
/*226*/		0x5F, 0x54, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Bank Identifier Code
/*233*/		0x9F, 0x0B, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Cardholder Name Extended
/*240*/		0x9F, 0x45, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Data Authentication Code
/*247*/		0x9F, 0x49, 0x04, 0x00, 0x00, 0x00, 0x00, 	//DDOL
/*254*/		0x9D, 0x04, 0x00, 0x00, 0x00, 0x00, 				//DDF Name
/*260*/		0xBF, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00, 	//FCI Issuer Discretionary Data
/*267*/		0x9F, 0x4C, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC Dynamic Number
/*274*/		0x9F, 0x2D, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC PIN Encipherment Public Key Certificate
/*281*/		0x9F, 0x2E, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC PIN Encipherment Public Key Exponent
/*288*/		0x9F, 0x2F, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC PIN Encipherment Public Key Reminder
/*295*/		0x9F, 0x46, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC Public Key Certificate
/*302*/		0x9F, 0x47, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC Public Key Exponent
/*309*/		0x9F, 0x48, 0x04, 0x00, 0x00, 0x00, 0x00, 	//ICC Public Key Reminder
/*316*/		0x5F, 0x53, 0x04, 0x00, 0x00, 0x00, 0x00, 	//International Bank Account Number
/*323*/		0x9F, 0x11, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Issuer Code Table Index
/*330*/		0x5F, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Issuer Country Code (alpha2 format)
/*337*/		0x5F, 0x56, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Issuer Country Code (alpha3 format)
/*344*/		0x42, 0x04, 0x00, 0x00, 0x00, 0x00, 				//Issuer Identification Number (IIN)
/*350*/		0x90, 0x04, 0x00, 0x00, 0x00, 0x00, 				//Issuer Public Key Certificate
/*356*/		0x9F, 0x32, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Issuer Public Key Exponent
/*363*/		0x92, 0x04, 0x00, 0x00, 0x00, 0x00, 				//Issuer Public Key Remainder
/*369*/		0x5F, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Issuer URL
/*376*/		0x5F, 0x2D, 0x04, 0x00, 0x00, 0x00, 0x00, 	//language Preference
/*383*/		0x9F, 0x13, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Last ATC
/*390*/		0x9F, 0x4D, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Log Entry
/*397*/		0x9F, 0x4F, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Log Format
/*404*/		0x9F, 0x14, 0x04, 0x00, 0x00, 0x00, 0x00, 	//LCOL
/*411*/		0x9F, 0x17, 0x04, 0x00, 0x00, 0x00, 0x00, 	//PIN Try Counter
/*418*/		0x88, 0x04, 0x00, 0x00, 0x00, 0x00, 				//SFI 	
/*424*/		0x9F, 0x4B, 0x04, 0x00, 0x00, 0x00, 0x00, 	//SDA Data
/*431*/		0x93, 0x04, 0x00, 0x00, 0x00, 0x00, 				//SSA Data
/*437*/		0x9F, 0x4A, 0x04, 0x00, 0x00, 0x00, 0x00, 	//SDA Tag List
/*444*/		0x9F, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 	//Track 2 Discretionary Data
/*451*/		0x9F, 0x23, 0x04, 0x00, 0x00, 0x00, 0x00, 	//UCOL
/*458*/		0x97, 0x04, 0x00, 0x00, 0x00, 0x00, 				//TDOL
/*464*/		0x9F, 0x12, 0x04, 0x00, 0x00, 0x00, 0x00, 	//App Prefered Name
/*471*/		0x9F, 0x38, 0x04, 0x00, 0x00, 0x00, 0x00, 	//PDOL														

			 //add by leehuan		  
/*478*/		0x9f, 0x6c, 0x04, 0x00, 0x00, 0x00, 0x00,  //qpboc 卡片交易属性
/*485*/		0x9f, 0x5d, 0x04, 0x00, 0x00, 0x00, 0x00,//可用脱机余额
};		 

void APP_POS_PAY_Init(void)
{
	unsigned int  addr;
	unsigned char	 revFlag = 0;
	
	//memcpy(Reader_VarDo_List, Con_Reader_VarDo_List, READER_VARDO_LIST_SIZE);
	//memcpy(Card_Do_List, Con_Card_Do_List, CARD_DO_LIST_SIZE); 

	//初始化卡片数据对象列表
	 
	addr = (u32)uchAc;
	memcpy(Card_Do_List+3, (unsigned char *)&addr, 4);
	addr = (u32)uchCid;
	memcpy(Card_Do_List+10, (unsigned char *)&addr, 4);
	addr = (u32)uchIad;
	memcpy(Card_Do_List+17, (unsigned char *)&addr, 4);
	addr = (u32)uchAtc;
	memcpy(Card_Do_List+24, (unsigned char *)&addr, 4);
	addr = (u32)uchAip;
	memcpy(Card_Do_List+30, (unsigned char *)&addr, 4);
	addr = (u32)uchAid;
	memcpy(Card_Do_List+36, (unsigned char *)&addr, 4);
	addr = (u32)uchAppVer;
	memcpy(Card_Do_List+43, (unsigned char *)&addr, 4);
	addr = (u32)uchMstrip2;
	memcpy(Card_Do_List+49, (unsigned char *)&addr, 4);
	addr = (u32)uchHolderName;
	memcpy(Card_Do_List+56, (unsigned char *)&addr, 4);
	addr = (u32)uchHolderNum;
	memcpy(Card_Do_List+63, (unsigned char *)&addr, 4);
	addr = (u32)uchEffectiveDate;
	memcpy(Card_Do_List+70, (unsigned char *)&addr, 4);
	addr = (u32)uchExpirationDate;
	memcpy(Card_Do_List+77, (unsigned char *)&addr, 4);
	addr = (u32)uchPan;
	memcpy(Card_Do_List+83, (unsigned char *)&addr, 4);
	addr = (u32)uchPanNum;
	memcpy(Card_Do_List+90, (unsigned char *)&addr, 4);
	addr = (u32)uchAuc;
	memcpy(Card_Do_List+97, (unsigned char *)&addr, 4);
	addr = (u32)uchCvm;
	memcpy(Card_Do_List+103, (unsigned char *)&addr, 4);
	addr = (u32)uchIacDefault;
	memcpy(Card_Do_List+110, (unsigned char *)&addr, 4);
	addr = (u32)uchIacDenial;
	memcpy(Card_Do_List+117, (unsigned char *)&addr, 4);
	addr = (u32)uchIacOnline;
	memcpy(Card_Do_List+124, (unsigned char *)&addr, 4);
	addr = (u32)uchCountryCode;
	memcpy(Card_Do_List+131, (unsigned char *)&addr, 4);
	addr = (u32)uchServiceCode;
	memcpy(Card_Do_List+138, (unsigned char *)&addr, 4);
	addr = (u32)uchCdol1;
	memcpy(Card_Do_List+144, (unsigned char *)&addr, 4);
	addr = (u32)uchCdol2;
	memcpy(Card_Do_List+150, (unsigned char *)&addr, 4);
	addr = (u32)uchAppCurrencyCode;
	memcpy(Card_Do_List+157, (unsigned char *)&addr, 4);
	addr = (u32)uchMstrip1;
	memcpy(Card_Do_List+164, (unsigned char *)&addr, 4);
	addr = (u32)uchCAKeyIndex;
	memcpy(Card_Do_List+170, (unsigned char *)&addr, 4);
	addr = (u32)uchAppCurrencyEx;
	memcpy(Card_Do_List+177, (unsigned char *)&addr, 4);
	addr = (u32)uchAppDiscretionData;
	memcpy(Card_Do_List+184, (unsigned char *)&addr, 4);
	addr = (u32)uchAFL;
	memcpy(Card_Do_List+190, (unsigned char *)&addr, 4);
	addr = (u32)uchADFName;
	memcpy(Card_Do_List+196, (unsigned char *)&addr, 4);
	addr = (u32)uchAip;
	memcpy(Card_Do_List+202, (unsigned char *)&addr, 4);
	addr = (u32)uchAPI;
	memcpy(Card_Do_List+208, (unsigned char *)&addr, 4);
	addr = (u32)uchAppRefCur;
	memcpy(Card_Do_List+215, (unsigned char *)&addr, 4);
	addr = (u32)uchAppRefCurEx;
	memcpy(Card_Do_List+222, (unsigned char *)&addr, 4);
	addr = (u32)uchBIC;
	memcpy(Card_Do_List+229, (unsigned char *)&addr, 4);
	addr = (u32)uchHolderNameExt;
	memcpy(Card_Do_List+236, (unsigned char *)&addr, 4);
	addr = (u32)uchDAC;
	memcpy(Card_Do_List+243, (unsigned char *)&addr, 4);
	addr = (u32)uchDDOL;
	memcpy(Card_Do_List+250, (unsigned char *)&addr, 4);
	addr = (u32)uchDDFName;
	memcpy(Card_Do_List+256, (unsigned char *)&addr, 4);
	addr = (u32)uchFCI;
	memcpy(Card_Do_List+263, (unsigned char *)&addr, 4);
	addr = (u32)uchICCDynamNum;
	memcpy(Card_Do_List+270, (unsigned char *)&addr, 4);
	addr = (u32)uchICCPINEncCACert;
	memcpy(Card_Do_List+277, (unsigned char *)&addr, 4);
	addr = (u32)uchICCPINEncCAExp;
	memcpy(Card_Do_List+284, (unsigned char *)&addr, 4);
	addr = (u32)uchICCPINEncCAReminder;
	memcpy(Card_Do_List+291, (unsigned char *)&addr, 4);
	addr = (u32)uchICCKeyCert;
	memcpy(Card_Do_List+298, (unsigned char *)&addr, 4);
	addr = (u32)uchICCKeyExp;
	memcpy(Card_Do_List+305, (unsigned char *)&addr, 4);
	addr = (u32)uchICCKeyReminder;
	memcpy(Card_Do_List+312, (unsigned char *)&addr, 4);
	addr = (u32)uchIBAN;
	memcpy(Card_Do_List+319, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerCodeTableIndex;
	memcpy(Card_Do_List+326, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerCountryCode2;
	memcpy(Card_Do_List+333, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerCountryCode3;
	memcpy(Card_Do_List+340, (unsigned char *)&addr, 4);
	addr = (u32)uchIIN;
	memcpy(Card_Do_List+346, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerKeyCert;
	memcpy(Card_Do_List+352, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerKeyExp;
	memcpy(Card_Do_List+359, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerKeyReminder;
	memcpy(Card_Do_List+365, (unsigned char *)&addr, 4);
	addr = (u32)uchIssuerURL;
	memcpy(Card_Do_List+372, (unsigned char *)&addr, 4);
	addr = (u32)uchLangPref;
	memcpy(Card_Do_List+379, (unsigned char *)&addr, 4);
	addr = (u32)uchLATC;
	memcpy(Card_Do_List+386, (unsigned char *)&addr, 4);
	addr = (u32)uchLogEntry;
	memcpy(Card_Do_List+393, (unsigned char *)&addr, 4);
	addr = (u32)uchLogFormat;
	memcpy(Card_Do_List+400, (unsigned char *)&addr, 4);
	addr = (u32)uchLCOL;
	memcpy(Card_Do_List+407, (unsigned char *)&addr, 4);
	addr = (u32)uchPTC;
	memcpy(Card_Do_List+414, (unsigned char *)&addr, 4);
	addr = (u32)uchSFI;
	memcpy(Card_Do_List+420, (unsigned char *)&addr, 4);
	addr = (u32)uchSDAData;
	memcpy(Card_Do_List+427, (unsigned char *)&addr, 4);
	addr = (u32)uchSSAData;
	memcpy(Card_Do_List+433, (unsigned char *)&addr, 4);
	addr = (u32)uchSDATagList;
	memcpy(Card_Do_List+440, (unsigned char *)&addr, 4);
	addr = (u32)uchTrack2Data;
	memcpy(Card_Do_List+447, (unsigned char *)&addr, 4);
	addr = (u32)uchUCOL;
	memcpy(Card_Do_List+454, (unsigned char *)&addr, 4);
	addr = (u32)uchTDOL;
	memcpy(Card_Do_List+460, (unsigned char *)&addr, 4);
	addr = (u32)uchAppPrefName;
	memcpy(Card_Do_List+467, (unsigned char *)&addr, 4);
	addr = (u32)uchPDOL;
	memcpy(Card_Do_List+474, (unsigned char *)&addr, 4);

	addr = (unsigned int)uchICCardProp;
	memcpy(Card_Do_List+451, (unsigned char *)&addr, 4);
	addr = (unsigned int)uchAvailableOFFAmount;
	memcpy(Card_Do_List+250, (unsigned char *)&addr, 4);
	

	puchAmount 			= Reader_VarDo_List + 3;
	puchBinAmount 		= Reader_VarDo_List + 11;	
	puchAmountOther 	= Reader_VarDo_List + 18;	
	puchCurrencyCode 	= Reader_ConstantDo_List + 7;
	puchTermCountryCode 	= Reader_ConstantDo_List + 16;
	puchIFDSerailNO 	= Reader_ConstantDo_List + 42;
	puchTermCapability	= Reader_ConstantDo_List + 21;
	puchTermType		= Reader_ConstantDo_List + 53;	
	puchTermID 			= Reader_ConstantDo_List + 57;
	puchAcquirerID		= Reader_ConstantDo_List + 72;
	puchMerchantID		= Reader_ConstantDo_List + 115;
	puchMerchantCat		= Reader_ConstantDo_List + 133;	
	puchPOSEntryMode	= Reader_ConstantDo_List + 138;	
	puchTvr 			= Reader_VarDo_List + 44;
	puchDate 			= Reader_VarDo_List + 51;
	puchTranType 		= Reader_VarDo_List + 62;
	puchUnpreData 		= Reader_VarDo_List + 66;
	puchTime 					= Reader_VarDo_List + 57;
	puchTsi 					= Reader_VarDo_List + 72;
	puchVer 					= Reader_VarDo_List + 77;   
	puchCvmRes 				= Reader_VarDo_List + 82;		
	puchArc 					= Reader_VarDo_List + 87;		
	puchTerminalFloorLimit = Reader_VarDo_List + 92; 
  puchIAD 					= Reader_VarDo_List + 98; 
	puchTSC						= Reader_VarDo_List + 117; 
	puchTCHash				= Reader_VarDo_List + 131; 
	puchTerminalAID		= Reader_VarDo_List + 154; 
	puchTerminalTransProp		= Reader_VarDo_List + 173; 
	

	EMV_InitDDFStack();
	EMV_InitAIDList();
	EMV_ClearCandidateList();

  //100.00 4-byte binary numbers  0x00002710
  puchTerminalFloorLimit[0] = 0;						
	puchTerminalFloorLimit[1] = 0;
	puchTerminalFloorLimit[2] = 0x27;
	puchTerminalFloorLimit[3] = 0x10;
	ThresholdforBiasedRandomSelection = 5000;         						// 0xFA0  40.00  4-byte binary numbers
	PercentforRandomSelection = 00;   															//0~99
	MaxPercentforBiasedRandomSelection = 00;											//0~99
	
	MID_NVMEM_Read(REVERSAL_FLAG,1,&revFlag);
	if ((0x30 != revFlag)&&(0x31 != revFlag))
	{
		revFlag = 0x30;
		MID_NVMEM_Write(REVERSAL_FLAG,1,&revFlag);
		revFlag = 0;
		MID_NVMEM_Write(REVERSAL_BUFLEN,1,&revFlag); 
		
		puchTSC[0] =  0x00;
		puchTSC[1] =  0x00;
		puchTSC[2] =  0x00;
		puchTSC[3] =  0x01;
		MID_NVMEM_Write(TRANS_SEQ_COUNTER,4,puchTSC);
	}
	MID_NVMEM_Read(REVERSAL_FLAG,1,&revFlag);	
	if (0x30 == revFlag)
	{
	   revFlag = 0;
	   MID_NVMEM_Write(REVERSAL_BUFLEN,1,&revFlag); 
	}	 

	//封闭指令标志位清零 2011.12.20
	//update puchTerminalAID	
	return;
}

void Clear_Cardlist(void)
{											 
	//Card List 列表数据清空
	memset(uchAc,0x00,sizeof(uchAc)); 
	memset(uchCid,0x00,sizeof(uchCid));
	memset(uchIad,0x00,sizeof(uchIad)); 
	memset(uchAtc,0x00,sizeof(uchAtc)); 
	memset(uchAip,0x00,sizeof(uchAip)); 
	memset(uchAid,0x00,sizeof(uchAid)); 
	memset(uchAppVer,0x00,sizeof(uchAppVer));
	memset(uchMstrip2,0x00,sizeof(uchMstrip2)); 
	memset(uchHolderName,0x00,sizeof(uchHolderName)); 
	memset(uchHolderNum,0x00,sizeof(uchHolderNum)); 
	memset(uchEffectiveDate,0x00,sizeof(uchEffectiveDate));
	memset(uchExpirationDate,0x00,sizeof(uchExpirationDate));
	memset(uchPan,0x00,sizeof(uchPan)); 
	memset(uchPanNum,0x00,sizeof(uchPanNum)); 
	memset(uchAuc,0x00,sizeof(uchAuc)); 
	memset(uchCvm,0x00,sizeof(uchCvm)); 
	memset(uchIacDefault,0x00,sizeof(uchIacDefault)); 
	memset(uchIacDenial,0x00,sizeof(uchIacDenial)); 
	memset(uchIacOnline,0x00,sizeof(uchIacOnline)); 
	memset(uchCountryCode,0x00,sizeof(uchCountryCode)); 
	memset(uchServiceCode,0x00,sizeof(uchServiceCode)); 
	memset(uchCdol1,0x00,sizeof(uchCdol1)); 
	memset(uchCdol2,0x00,sizeof(uchCdol2)); 
	memset(uchAppCurrencyCode,0x00,sizeof(uchAppCurrencyCode));
	memset(uchMstrip1,0x00,sizeof(uchMstrip1)); 
	memset(uchCAKeyIndex,0x00,sizeof(uchCAKeyIndex)); 
	memset(uchAppCurrencyEx,0x00,sizeof(uchAppCurrencyEx)); 
	memset(uchAppDiscretionData,0x00,sizeof(uchAppDiscretionData)); 
	memset(uchAFL,0x00,sizeof(uchAFL)); 
	memset(uchADFName,0x00,sizeof(uchADFName)); 
	memset(uchAip,0x00,sizeof(uchAip)); 
	memset(uchAPI,0x00,sizeof(uchAPI)); 
	memset(uchAppRefCur,0x00,sizeof(uchAppRefCur)); 
	memset(uchAppRefCurEx,0x00,sizeof(uchAppRefCurEx)); 
	memset(uchBIC,0x00,sizeof(uchBIC)); 
	memset(uchHolderNameExt,0x00,sizeof(uchHolderNameExt)); 
	memset(uchDAC,0x00,sizeof(uchDAC));
	memset(uchDDOL,0x00,sizeof(uchDDOL)); 
	memset(uchDDFName,0x00,sizeof(uchDDFName)); 
	memset(uchFCI,0x00,sizeof(uchFCI)); 
	memset(uchICCDynamNum,0x00,sizeof(uchICCDynamNum)); 
	memset(uchICCPINEncCACert,0x00,sizeof(uchICCPINEncCACert)); 
	memset(uchICCPINEncCAExp,0x00,sizeof(uchICCPINEncCAExp)); 
	memset(uchICCPINEncCAReminder,0x00,sizeof(uchICCPINEncCAReminder)); 
	memset(uchICCKeyCert,0x00,sizeof(uchICCKeyCert)); 
	memset(uchICCKeyExp,0x00,sizeof(uchICCKeyExp)); 
	memset(uchICCKeyReminder,0x00,sizeof(uchICCKeyReminder)); 
	memset(uchIBAN,0x00,sizeof(uchIBAN));
	memset(uchIssuerCodeTableIndex,0x00,sizeof(uchIssuerCodeTableIndex)); 
	memset(uchIssuerCountryCode2,0x00,sizeof(uchIssuerCountryCode2)); 
	memset(uchIssuerCountryCode3,0x00,sizeof(uchIssuerCountryCode3)); 
	memset(uchIIN,0x00,sizeof(uchIIN)); 
	memset(uchIssuerKeyCert,0x00,sizeof(uchIssuerKeyCert));
	memset(uchIssuerKeyExp,0x00,sizeof(uchIssuerKeyExp));
	memset(uchIssuerKeyReminder,0x00,sizeof(uchIssuerKeyReminder));
	memset(uchIssuerURL,0x00,sizeof(uchIssuerURL));
	memset(uchLangPref,0x00,sizeof(uchLangPref));
	memset(uchLATC,0x00,sizeof(uchLATC));
	memset(uchLogEntry,0x00,sizeof(uchLogEntry));
	memset(uchLogFormat,0x00,sizeof(uchLogFormat));
	memset(uchLCOL,0x00,sizeof(uchLCOL));
	memset(uchPTC,0x00,sizeof(uchPTC));
	memset(uchSFI,0x00,sizeof(uchSFI));
	memset(uchSDAData,0x00,sizeof(uchSDAData));
	memset(uchSSAData,0x00,sizeof(uchSSAData));
	memset(uchSDATagList,0x00,sizeof(uchSDATagList));
	memset(uchTrack2Data,0x00,sizeof(uchTrack2Data));
	memset(uchUCOL,0x00,sizeof(uchUCOL));
	memset(uchTDOL,0x00,sizeof(uchTDOL));
	memset(uchAppPrefName,0x00,sizeof(uchAppPrefName));
	memset(uchPDOL,0x00,sizeof(uchAppPrefName));
	
//	memset(uchLogFile,0x00,500);
  uchLogFileLen = 0;
	
	current_transaction_amount = 0;

  //终端Tag清零
	memset(puchAmount,0x00,6); 	
	memset(puchBinAmount,0x00,4); 	
	memset(puchTvr,0x00,5);  	
	memset(puchDate ,0x00,3); 				
	memset(puchUnpreData,0x00,4);  		
	memset(puchTime,0x00,3);  					
	memset(puchTsi,0x00,2);  				
	memset(puchCvmRes,0x00,3); 
	memset(puchTerminalAID,0x00,16);	
	memset(puchArc,0x00,2);  				
	memset(puchIAD,0x00,16);  				
	memset(puchTCHash,0x00,20);
  	memset(g_b_plain_pin,0x00,PIN_BUF_LENGTH);

	internal_state = WAIT_FOR_TRANSACTION;
	
	return;
}
//***************************************************************************************
//* Function Name  : APP_POS_PAY_CDGetCashBalance  
//* Description    : 脱机查询电子现金余额
//*       
//* Input          : APDU  
//*					
//* Output         : int
//*					 
//* Return         : 1.0x9000正确；要返回的数据在    g_APDU.pOutData 里	 长度给RLE（不包括SW1和SW2）
						
//					 2.非0x9000错误；
//***************************************************************************************
unsigned int APP_POS_PAY_CDGetCashBalance(APDU *psAPDU)
{
	unsigned int   	uintResult = 0;		
//	if(!G_CardStats)
//	{
//		return CARD_POWERON_FAIL;
//	}
	//每次交易开始时，上次交易卡片读出的标签值全部清零
	Clear_Cardlist();	
	EMV_InitAIDList();		
	uintResult = EMV_BuildCLbyPSE();	 //通过选择PSE构建Candidate List
	if(NOK == uintResult)
	{
		if(CARD_BLOCKED == Nok_Reason)
		{
			return NOK;
		}
		else 								//if(NONE == Nok_Reason)
		{
			uintResult = EMV_BuildCLbyAID();   	//使用AID列表构建Candidate List
			if(NOK == uintResult)
			{
				Nok_Reason = NONE;
				return NOK;
			}
			Nok_Reason = NONE;
		}
	}
	
	uintResult = EMV_FinalSelection();
	if((NOK == uintResult)&&((Nok_Reason == NOT_ACCEPTED)||(Nok_Reason == EMPTY_LIST)))
	{
		return NOK;
	}
	
	//读取卡片电子现金余额
	uintResult = GetCash(psAPDU);
	if (SW_OPERATION_SUCCESS != uintResult)
	{
		psAPDU->RLE = 0;
		uintResult = 0x0803;
//		MID_DISP_SmallPic(SMALLPIC_JIAOYIZHONGZHI);
		return	uintResult; 
	}
	return 	 SW_OPERATION_SUCCESS;
}

unsigned int APP_PBOC_Simple_ReadCardNo(unsigned char *pInData,unsigned char uclen,unsigned char * pucCardNo, unsigned char * pucCardNoLen)
{
	unsigned int uintResult;

	//保存交易金额，日期，时间
	memcpy(puchAmount,pInData,6);			  //金额		  
	memcpy(puchDate,pInData + 6,3);			  //日期
	memcpy(puchTime,pInData + 9,3);			  //时间
	
	//应用初始化
	uintResult = EMV_AppInitialization();
	if(NOK == uintResult)
	{	
		return (0x7701);
	}
	else{}	
	//读取应用记录
	uintResult = EMV_ReadAppData();
	if(NOK == uintResult)
	{ 	
		return (0x7702);
	}
	else{}

	*pucCardNoLen = uchPan[0];
	iHexToASCII(uchPan + 1, &uchPan[0], pucCardNo);
	*pucCardNoLen = uchPan[0] ;
	if(0x3F == pucCardNo[*pucCardNoLen - 1])
	{
		*pucCardNoLen -= 1;
	}
	else{}
	return (0x9000);
}
//***************************************************************************************
//* Function Name  : APP_POS_PAY_UpLimitLoadPrice  
//* Description    : 最大圈存金额
//*       
//* Input          : APDU  
//*					
//* Output         : int  
//*					 
//* Return         : 1.0x9000正确；要返回的数据在    g_APDU.pOutData 里	 长度给RLE（不包括SW1和SW2）
						
//					 2.非0x9000错误；
//***************************************************************************************
unsigned int APP_POS_PAY_UpLimitLoadPrice(APDU *psAPDU)
{
	unsigned int   	uintResult = 0;		
//	if(!G_CardStats)
//	{
//		return CARD_POWERON_FAIL;
//	}
	//每次交易开始时，上次交易卡片读出的标签值全部清零
	Clear_Cardlist();	
	EMV_InitAIDList();		
	uintResult = EMV_BuildCLbyPSE();	 //通过选择PSE构建Candidate List
	if(NOK == uintResult)
	{
		if(CARD_BLOCKED == Nok_Reason)
		{
			return NOK;
		}
		else 								//if(NONE == Nok_Reason)
		{
			uintResult = EMV_BuildCLbyAID();   	//使用AID列表构建Candidate List
			if(NOK == uintResult)
			{
				Nok_Reason = NONE;
				return NOK;
			}
			Nok_Reason = NONE;
		}
	}	
	uintResult = EMV_FinalSelection();
	if((NOK == uintResult)&&((Nok_Reason == NOT_ACCEPTED)||(Nok_Reason == EMPTY_LIST)))
	{
		return NOK;
	}	
	/*-----------4. 读金额上限------------*/
	//读取电子现金余额上限 
	uintResult = ReadUpLimitPrice(psAPDU);
	if (SW_OPERATION_SUCCESS != uintResult)
	{
		psAPDU->RLE = 0;
		uintResult = 0x0803;
		return	uintResult; 
	}	
	return SW_OPERATION_SUCCESS;
}



 //***************************************************************************************
//* Function Name  : APP_POS_PAY_CardIssuers_Script  
//* Description    : 发卡行脚本处理
//*       
//* Input          : APDU  
//*					
//* Output         : int  
//*					 
//* Return         : 1.0x9000正确；要返回的数据在    g_APDU.pOutData 里	 长度给RLE（不包括SW1和SW2）
						
//					 2.非0x9000错误；
//***************************************************************************************
unsigned int APP_POS_PAY_CardIssuers_Script (void)				  
{
//reflag
//0:查询
//1:消费
//2:圈存
//3:交易

//	unsigned int    uintResult=0;
//	unsigned char   i;

//	//判断交易状态
//	if(TRANS_PRIBALANCE_READ_PAN == g_b_trans_state)
//	{
//		g_b_trans_state = TRANS_IDLE;
//	}
//	else
//	{
//		G_PIN_FLAG = 0;
//		g_APDU.RLE = 1;
//		*g_APDU.pOutData = 0x01;
//		g_b_trans_state = TRANS_IDLE;
//		MID_DISP_SmallPic(SMALLPIC_JIAOYIZHONGZHI);
//		return SW_P1P2_ERROR;
//	}

	switch(g_APDU.PP2)
	{
		case 0x02:		  //圈存55域数据
			G_flag_BalanceOrLoad = 2;
	   		break;

		case 0x01:		//联机查余额获取55域数据
			G_flag_BalanceOrLoad = 0;
		    break;

		case 0x00:		//消费交易获取55域数据
			G_flag_BalanceOrLoad = 1;
		    break;	

		default:
		    G_flag_BalanceOrLoad = 0;
			break;
	}
//	if(1 == G_flag_BalanceOrLoad )
//	{
//		reflag = 2;
//	}
//	else
//	{
//		reflag = 0;
//	}
	//联机处理
	if (SW_OPERATION_SUCCESS != EMV_OnlineProcessing(G_flag_BalanceOrLoad))
	{
//		G_PIN_FLAG = 0;
		g_APDU.RLE = 0;
		g_b_trans_state = TRANS_IDLE;
//		MID_DISP_SmallPic(SMALLPIC_JIAOYISHIBAI);
		return SW_P1P2_ERROR;
//		return SW_UNKOWN_ERROR;
	} 
	else
	{
//		G_PIN_FLAG = 0;
		g_b_trans_state = TRANS_IDLE;
		return SW_OPERATION_SUCCESS;
	}

}

//***************************************************************************************
//* Function Name  : APP_POS_PAY_55CardUpload_DataPackage  
//* Description    : 55域数据组包上传
//*       
//* Input          : APDU  
//*					
//* Output         : int  
//*					 
//* Return         : 1.0x9000正确；要返回的数据在    g_APDU.pOutData 里	 长度给RLE（不包括SW1和SW2）
						
//					 2.非0x9000错误；
//***************************************************************************************
unsigned int APP_POS_PAY_55CardUpload_DataPackage(void)
{
	unsigned char 	temp = 0;
	unsigned int    uintResult=0;
	unsigned char   i;		

	if (g_APDU.PP3 < 12) 
	{
		g_APDU.RLE = 1;
		*g_APDU.pOutData = 0x01;
		g_b_trans_state = TRANS_IDLE;
		uintResult =  0x0800;  	
		return SW_OPERATION_SUCCESS;
	}

	switch(g_APDU.PP2)
	{
		case 0x02:		  //圈存55域数据
			//交易类型
			*puchTranType = LOAD_SPECIFIED_ACCOUNT;	
			G_flag_BalanceOrLoad = 1;
	   		break;

		case 0x01:		//联机查余额获取55域数据
			//交易类型
			*puchTranType = INQUIRY;
			G_flag_BalanceOrLoad = 0;
		    break;

		case 0x00:		//消费交易获取55域数据
			//交易类型
			*puchTranType = GOODS_PURCHASE;
			G_flag_BalanceOrLoad = 0;
		    break;	

		default:
		    G_flag_BalanceOrLoad = 0;
			break;
	}

	Clear_Cardlist();

	//保存交易金额，日期，时间
	memcpy(puchAmount,g_APDU.pInData,6);			  //金额		  
	memcpy(puchDate,g_APDU.pInData + 6,3);			  //日期
	memcpy(puchTime,g_APDU.pInData + 9,3);			  //时间



	for(i = 0; i < 8; i++)
	{
		current_transaction_amount *= 10;
		if (i%2)
		{
			current_transaction_amount += puchAmount[2+i/2]&0x0F;
		}
		else
		{
			current_transaction_amount += (puchAmount[2+i/2]&0xF0)>>4;
		}
	}
	
	if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
	{
		*puchTranType = 0x60;	
		//冲正信息保存 Sean Li 2011.11.04
		temp = 0x31;
		MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);			
		MID_NVMEM_Write(REVERSAL_BUF+1,g_APDU.LC,g_APDU.pInData); 
		temp = g_APDU.LC+1;
		MID_NVMEM_Write(REVERSAL_BUFLEN,1,&temp);
	}

	//应用初始化
	uintResult = EMV_AppInitialization();
	if(NOK == uintResult)
	{
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			//冲正标志位清零
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);					
		}
	   //		G_PIN_FLAG = 0;
		g_APDU.RLE = 1;
		*g_APDU.pOutData = 0x01;
		g_b_trans_state = TRANS_IDLE;
		uintResult =  0x0801;  	
		return SW_OPERATION_SUCCESS;
	}	
	//读取应用记录
	uintResult = EMV_ReadAppData();
	if(NOK == uintResult)
	{
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			//冲正标志位清零
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);					
		}
	   //		G_PIN_FLAG = 0;
		g_APDU.RLE = 1;
		*g_APDU.pOutData = 0x01;
		g_b_trans_state = TRANS_IDLE;
		uintResult =  0x0802;  	
		return SW_OPERATION_SUCCESS;
	}
			
	//处理限制
	uintResult = EMV_ProcessingRestriction();
	if(NOK == uintResult)
	{
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			//冲正标志位清零
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);					
		}
	   //		G_PIN_FLAG = 0;
		g_APDU.RLE = 1;
		*g_APDU.pOutData = 0x01;
		g_b_trans_state = TRANS_IDLE;
		uintResult =  0x0803;  	
		return SW_OPERATION_SUCCESS;
	}
		
	//持卡人验证

	uintResult = EMV_CardholderVerification();
	if(SW_OPERATION_SUCCESS != uintResult)
	{
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			//冲正标志位清零
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);					
		}
	   //		G_PIN_FLAG = 0;
		g_APDU.RLE = 1;
		*g_APDU.pOutData = 0x01;
		g_b_trans_state = TRANS_IDLE;
		uintResult =  0x0804;  	
		return SW_OPERATION_SUCCESS;
	}
		
	//终端风险管理
	EMV_TerminalRiskManagement();	
				
	//终端行为分析
	if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
	{
		if (SW_OPERATION_SUCCESS != EMV_TerminalActionAnalysis(1))
		{
			//冲正标志位清零
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);
			g_APDU.RLE  = 1;
			*g_APDU.pOutData = 0x01;
//			MID_DISP_SmallPic(SMALLPIC_JIAOYISHIBAI);
			g_b_trans_state = TRANS_IDLE;
			uintResult =  0x0806;  	
			return SW_OPERATION_SUCCESS;
		} 		
		if((G_B_GACResult & 0xf0) == 0x00)		//脱机拒绝
		{
			//冲正标志位清零
		    temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
//			g_b_trans_state = TRANS_IDLE;
//			uintResult =  0x0807;  	
//			return uintResult;
		}
		else
		{
			//冲正信息保存 Sean Li 2011.11.04
			temp = 0x31;
			MID_NVMEM_Write(REVERSAL_FLAG, 1, &temp);	
			MID_NVMEM_Write(REVERSAL_BUF, 1, g_APDU.pOutData + 6);  	   //保存交易结果
			MID_NVMEM_Write(REVERSAL_BUF + 1, 6, g_APDU.pOutData);         //保存圈存金额
			MID_NVMEM_Read(REVERSAL_BUFLEN, 1, &temp);
			MID_NVMEM_Write(REVERSAL_BUF + temp, g_APDU.RLE, g_APDU.pOutData);    	
			temp += g_APDU.RLE;
			MID_NVMEM_Write(REVERSAL_BUFLEN, 1, &temp);
		}
//		G_PIN_FLAG = 0;
		return SW_OPERATION_SUCCESS;	
	}
	else if(0 == G_flag_BalanceOrLoad)
	{
	 	if (SW_OPERATION_SUCCESS != EMV_TerminalActionAnalysis(0))
		{
		   //		G_PIN_FLAG = 0;
			g_APDU.RLE = 0;
			*g_APDU.pOutData = 0x01;
			g_b_trans_state = TRANS_IDLE;
			uintResult =  0x0805;  	
			return SW_OPERATION_SUCCESS;
		} 		
		else
		{
	//		G_PIN_FLAG = 0;
			return SW_OPERATION_SUCCESS;
		}
	}
	else{}
	return SW_OPERATION_SUCCESS;
}
//***************************************************************************************
//* Function Name  : APP_POS_PAY_No55CardUpload_DataPackage  
//* Description    : 非55域数据
//*       
//* Input          : APDU  
//*					
//* Output         : int  
//*					 
//* Return         : 1.0x9000正确；要返回的数据在    g_APDU.pOutData 里	 长度给RLE（不包括SW1和SW2）
						
//					 2.非0x9000错误；
//***************************************************************************************
unsigned int APP_POS_PAY_No55CardUpload_DataPackage(void)
{
	unsigned char *ptr = NULL;
//	unsigned char uRemainder_FF = 0;
	unsigned char temp = 0;
	//判断交易状态
//	if(TRANS_PRIBALANCE_START == G_Trans_State)
//	{
//		G_Trans_State = TRANS_PRIBALANCE_READ_PAN;
//	}
//	else
//	{
//		psAPDU->RLE = 0x00;
//	   	return SW_P1P2_ERROR;
//	}


	if((G_B_GACResult & 0xf0) == 0x00)	//脱机拒绝
	{
		g_APDU.RLE = 0;
		//冲正标志位清零
	    temp = 0x30;
		MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
	   	return SW_CONDITION_DISSATISFY;
	}
	else								//同意联机
	{

		//data block
		ptr = g_APDU.pOutData + 1;
		*ptr = 0x5a;
		ptr++;
		*ptr = uchPan[0];
		ptr++;
		memcpy(ptr, uchPan+1, uchPan[0]);
		ptr += uchPan[0];
		*ptr = 0x57;
		ptr++;
		*ptr = uchMstrip2[0];
		ptr++;
		memcpy(ptr, uchMstrip2+1, uchMstrip2[0]);
		ptr += uchMstrip2[0];
		*ptr = 0x5f;
		ptr++;	
		*ptr = 0x24;
		ptr++;
		*ptr = 3;
		ptr++;
		memcpy(ptr, uchExpirationDate + 1, 3);
		ptr += 3;
		g_APDU.pOutData[0] = ptr - (g_APDU.pOutData + 1);
		g_APDU.RLE = g_APDU.pOutData[0] + 1;

		//冲正标志位置位，冲正信息保存
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			temp = 0x31;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
			MID_NVMEM_Read(REVERSAL_BUFLEN,1,&temp);
			MID_NVMEM_Write(REVERSAL_BUF+ temp, g_APDU.RLE, g_APDU.pOutData); 	//保存PAN,PAN序列号,磁条2等效数据
			temp += g_APDU.RLE;
			MID_NVMEM_Write(REVERSAL_BUFLEN,1,&temp); 	//保存长度
		}
		
		//冲正标志位置位，冲正信息保存

		return 	SW_OPERATION_SUCCESS;
	}	
}



//读取冲正信息
unsigned int APP_POS_PAY_CDReversalInfo(APDU *psAPDU)
{ 	
	unsigned char temp = 0;

	if(g_APDU.PP1 == 0x05)
	{
		if(g_APDU.PP2 == 0)	 //读冲正信息
		{		
			MID_NVMEM_Read(REVERSAL_BUFLEN,1,&temp);
			MID_NVMEM_Read(REVERSAL_BUF,temp,g_APDU.pOutData);	 
			g_APDU.RLE = temp;
			return SW_OPERATION_SUCCESS;
		}
		else if(g_APDU.PP2 == 1)   //清冲正信息
		{
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);
			temp = 0;
			MID_NVMEM_Write(REVERSAL_BUFLEN,1,&temp);
			
			g_APDU.RLE = 0;
//			G_Trans_State	= TRANS_IDLE;
			return SW_OPERATION_SUCCESS;
		}
		else if(g_APDU.PP2 == 2)   //清脚本信息
		{
			g_APDU.RLE = 0;
//			G_Trans_State = TRANS_IDLE;
			return SW_OPERATION_SUCCESS;
		}
		else
		{
			g_APDU.RLE = 0;
			return SW_P1P2_ERROR;
		}
	}
	else
	{
		g_APDU.RLE = 0;
		return SW_P1P2_ERROR;
	}
} 

int CheckCardState(void)
{
	unsigned char ucbATR[24];
	unsigned char ucATRLen;
	
	if(IO_PIN_N8035_EN_SCC_EINT1_HIGH)
	{
		ucATRLen = HAL_7816_GetATR(0, ucbATR);
		if(ucATRLen==0)
		{
			G_CardStats = 2;       
		}
			
		else
		{
			G_CardStats=1;
		}
			
	}
	else
	{
		G_CardStats = 0;         
	}

	return G_CardStats;

}



