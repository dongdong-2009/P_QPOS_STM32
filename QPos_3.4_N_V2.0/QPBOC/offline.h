#ifndef __OFFLINE_H_
#define __OFFLINE_H_
#ifdef OFFLINE_GLOBAL
#define EXT 
#else 
#define EXT extern
#endif
//add by zxy
#define  PREPROCESSING_END  0
#define  PREPROCESSING_DONE 1
#define  REQUEST_MESSAGE 2


/***************  Common EMV Tags  **************/
#define  EMV_TAG_004F_AID							0X004F   
  //Tag 4F AID (Application Identifier). 
 
#define  EMV_TAG_0050_APPLICATION_LABEL				0X0050 
  //Tag 50 Application Label. 
 
#define  EMV_TAG_0057_TRACK2_EQUIVALENT_DATA		0X0057 
  //Tag 57 Track2 Equivalent Data. 
 
#define  EMV_TAG_005A_APPLICATION_PAN				0X005A 
  //Tag 5A Application PAN. 
 
#define  EMV_TAG_5F20_CARDHOLDER_NAME				0X5F20 
  //Tag 5F20 Cardholder Name. 
 
#define  EMV_TAG_5F24_APPLICATION_EXPIRATION_DATE   0X5F24 
  //Tag 5F24 Application Expiration Date. 
 
#define  EMV_TAG_5F25_APPLICATION_EFFECTIVE_DATE    0X5F25 
  //Tag 5F25 Application Effective Date. 
 
#define  EMV_TAG_5F28_ISSUER_COUNTRY_CODE			0X5F28 
  //Tag 5F28 Issuer Country Code. 
 
#define  EMV_TAG_5F2A_TRANSACTION_CURRENCY_CODE		0X5F2A 
  //Tag 5F2A Transaction Currency Code. 
 
#define  EMV_TAG_5F2D_LANGUAGE_PREFERENCE			0X5F2D 
  //Tag 5F2D Language Preference. 
 
#define  EMV_TAG_5F30_SERVICE_CODE					0X5F30 
  //Tag 5F30 Service Code. 
 
#define  EMV_TAG_5F34_APPLICATION_PAN_SEQUENCE		0X5F34 
  //Tag 5F34 Application PAN Sequence. 
 
#define  EMV_TAG_5F36_TRANSACTION_CURRENCY_EXPONENT 0X5F36 
  //Tag 5F36 Transaction Currency Exponent. 
 
#define  EMV_TAG_0061_APPLICATION_TEMPLATE			0X0061 
  //Tag 61 Application Template. 
 
#define  EMV_TAG_006F_FCI_TEMPLATE					0X006F 
  //Tag 6F FCI (FILE CONTROL INFO) Template. 
 
#define  EMV_TAG_0070_AEF_DATA_TEMPLATE				0X0070 
  //Tag 70 AEF (Application Elementary File) Data Template. 
 
#define  EMV_TAG_0071_ISSUER_SCRIPT_TEMPLATE_1		0X0071 
  //Tag 71 Issuer Script Template 1. 
 
#define  EMV_TAG_0072_ISSUER_SCRIPT_TEMPLATE_2		0X0072 
  //Tag 72 Issuer Script Template 2. 
 
#define  EMV_TAG_0073_DIRECTORY_DISCRETIONARY_TEMPLATE		0X0073 
  //Tag 73 Directory Discretionary Template. 
 
#define  EMV_TAG_0077_RESPONSE_MESSAGE_TEMPLATE_FORMAT_2	0X0077 
  //Tag 77 Response Message Template Format 2. 
 
#define  EMV_TAG_0080_RESPONSE_MESSAGE_TEMPLATE_FORMAT_1	0X0080 
  //Tag 80 Response Message Template Format 1. 
 
#define  EMV_TAG_0081_AMOUNT_AUTHORISED_BINARY		0X0081 
  //Tag 81 Amount Authorised Binary. 
 
#define  EMV_TAG_0082_AIP							0X0082 
  //Tag 82 AIP (Application Interchange Profile). 
 
#define  EMV_TAG_0083_COMMAND_TEMPLATE				0X0083 
  //Tag 83 Command Template. 
 
#define  EMV_TAG_0084_DF_NAME						0X0084 
  //Tag 84 DF (Dedicated File) Name. 
 
#define  EMV_TAG_0086_ISSUER_SCRIPT_COMMAND			0X0086 
  //Tag 86 Issuer Script Command. 
 
#define  EMV_TAG_0087_API							0X0087 
  //Tag 87 API (Application Priority Indicator). 
 
#define  EMV_TAG_0088_SFI							0X0088 
  //Tag 88 SFI (Short File Indicator). 
 
#define  EMV_TAG_0089_AUTH_CODE						0X0089 
  //Tag 89 Authorisation Code. 
 
#define  EMV_TAG_008A_AUTH_RESPONSE_CODE			0X008A 
  //Tag 8A Authorisation Response Code. 
 
#define  EMV_TAG_008C_CDOL_1						0X008C 
  //Tag 8C CDOL 1 (Card Risk Management Data Object List). 
 
#define  EMV_TAG_008D_CDOL_2						0X008D 
  //Tag 8D CDOL 2 (Card Risk Management Data Object List). 
 
#define  EMV_TAG_008E_CVM_LIST						0X008E 
  //Tag 8E CVM (Cardholder Verification Method) List. 
 
#define  EMV_TAG_008F_CERTIFICATION_AUTHORITY_PUBLIC_KEY_INDEX		0X008F 
  //Tag 8F Certification Authority Public Key Index. 
 
#define  EMV_TAG_0090_ISSUER_PUBLIC_KEY_CERTIFICATE					0X0090 
  //Tag 90 Issuer Public Key Certificate. 
 
#define  EMV_TAG_0091_ISSUER_AUTHENTICATION_DATA	0X0091 
  //Tag 91 Issuer Authentication Data. 
 
#define  EMV_TAG_0092_ISSUER_PUBLIC_KEY_REMAINDER   0X0092 
  //Tag 92 Issuer Public Key Remainder. 
 
#define  EMV_TAG_0093_SIGNED_STATIC_APPLICATION_DATA				0X0093 
  //Tag 93 Signed Static Application Data. 
 
#define  EMV_TAG_0094_AFL							0X0094 
  //Tag 94 AFL (Application File Locator). 
 
#define  EMV_TAG_0095_TVR							0X0095 
  //Tag 95 TVR (Terminal Verification Results). 
 
#define  EMV_TAG_0097_TDOL							0X0097 
  //Tag 97 TDOL (Transaction Certificate Data Object List). 
 
#define  EMV_TAG_0098_TC_HASH_VALUE					0X0098 
  //Tag 98 TC (Transaction Certificate) Hash Value. 
 
#define  EMV_TAG_0099_TRANSACTION_PIN_DATA			0X0099 
  //Tag 99 Transaction PIN (Personal Identification Number) Data. 
 
#define  EMV_TAG_009A_TRANSACTION_DATE				0X009A 
  //Tag 9A Transaction Date. 
 
#define  EMV_TAG_009B_TRANSACTION_STATUS_INFO		0X009B 
  //Tag 9B Transaction Status Info. 
 
#define  EMV_TAG_009C_TRANSACTION_TYPE				0X009C 
  //Tag 9C Transaction Type. 
 
#define  EMV_TAG_009D_DDF_NAME						0X009D 
  //Tag 9D DDF (Directory Definition File) Name. 
 
#define  EMV_TAG_9F01_ACQUIRER_IDENTIFIER			0X9F01 
  //Tag 9F01 Acquirer Identifier. 
 
#define  EMV_TAG_9F02_AMOUNT_AUTHORISED_NUMERIC		0X9F02 
  //Tag 9F02 Amount Authorised Numeric. 
 
#define  EMV_TAG_9F03_AMOUNT_OTHER_NUMERIC			0X9F03 
  //Tag 9F03 Amount Other Numeric. 
 
#define  EMV_TAG_9F04_AMOUNT_OTHER_BINARY			0X9F04 
  //Tag 9F04 Amount Other Binary. 
 
#define  EMV_TAG_9F05_APPLICATION_DISCRETIONARY_DATA				0X9F05 
  //Tag 9F05 Application Discretionary Data. 
 
#define  EMV_TAG_9F06_AID_TERMINAL					0X9F06 
  //Tag 9F06 AID (Terminal). 
 
#define  EMV_TAG_9F07_AUC							0X9F07 
  //Tag 9F07 AUC (Application Usage Control). 
 
#define  EMV_TAG_9F08_APPLICATION_VERSION_NUMBER_CARD				0X9F08 
  //Tag 9F08 Application Version Number Card. 
 
#define  EMV_TAG_9F09_APPLICATION_VERSION_NUMBER_TERMINAL			0X9F09 
  //Tag 9F09 Application Version Number Terminal. 
 
#define  EMV_TAG_9F0B_CARDHOLDER_NAME_EXTENDED		0X9F0B 
  //Tag 9F0B Cardholder Name Extended. 
 
#define  EMV_TAG_9F0D_ISSUER_ACTION_CODE_DEFAULT	0X9F0D 
  //Tag 9F0D Issuer Action Code Default. 
 
#define  EMV_TAG_9F0E_ISSUER_ACTION_CODE_DENIAL		0X9F0E 
  //Tag 9F0E Issuer Action Code Denial. 
 
#define  EMV_TAG_9F0F_ISSUER_ACTION_CODE_ONLINE		0X9F0F 
  //Tag 9F0F Issuer Action Code Online. 
 
#define  EMV_TAG_9F10_ISSUER_APPLICATION_DATA		0X9F10 
  //Tag 9F10 Issuer Application Data. 
 
#define  EMV_TAG_9F11_ISSUER_CODE_TABLE_INDEX		0X9F11 
  //Tag 9F11 Issuer Code Table Index. 
 
#define  EMV_TAG_9F12_APPLICATION_PREFERRED_NAME	0X9F12 
  //Tag 9F12 Application Preferred Name. 
 
#define  EMV_TAG_9F13_LAST_ONLINE_ATC_REGISTER		0X9F13 
  //Tag 9F13 Last Online ATC (Application Transaction Counter) Register. 
 
#define  EMV_TAG_9F14_LOWER_CONSECUTIVE_OFFLINE_LIMIT				0X9F14 
  //Tag 9F14 Lower Consecutive Offline Limit. 
 
#define  EMV_TAG_9F15_MERCHANT_CATEGORY_CODE		0X9F15 
  //Tag 9F15 Merchant Category Code. 
 
#define  EMV_TAG_9F16_MERCHANT_IDENTIFIER			0X9F16 
  //Tag 9F16 Merchant Identifier. 
 
#define  EMV_TAG_9F17_PIN_TRY_COUNTER				0X9F17 
  //Tag 9F17 PIN (Personal Identification Number) Try Counter. 
 
#define  EMV_TAG_9F18_ISSUER_SCRIPT_IDENTIFIER		0X9F18 
  //Tag 9F18 Issuer Script Identifier. 
 
#define  EMV_TAG_5F50_ISSUER_URL					0X5F50 
  //Tag 5F50 Issuer URL. 
 
#define  EMV_TAG_9F1A_TERMINAL_COUNTRY_CODE			0X9F1A 
  //Tag 9F1A Terminal Country Code. 
 
#define  EMV_TAG_9F1B_TERMINAL_FLOOR_LIMIT			0X9F1B 
  //Tag 9F1B Terminal Floor Limit. 
 
#define  EMV_TAG_9F1C_TERMINAL_IDENTIFICATION		0X9F1C 
  //Tag 9F1C Terminal Identification. 
 
#define  EMV_TAG_9F1D_TERMINAL_RISK_MANGEMENT_DATA  0X9F1D 
  //Tag 9F1D Terminal Risk Mangement Data. 
 
#define  EMV_TAG_9F1E_IFD_SERIAL_NUMBER				0X9F1E 
  //Tag 9F1E IFD (Interface Device) Serial Number. 
 
#define  EMV_TAG_9F1F_TRACK1_DISCRETIONARY_DATA		0X9F1F 
  //Tag 9F1F Track1 Discretionary Data. 
 
#define  EMV_TAG_9F20_TRACK2_DISCRETIONARY_DATA		0X9F20 
  //Tag 9F20 Track2 Discretionary Data. 
 
#define  EMV_TAG_9F21_TRANSACTION_TIME				0X9F21 
  //Tag 9F21 Transaction Time. 
 
#define  EMV_TAG_9F22_CERTIFICATION_AUTHORITY_PUBLIC_KEY_INDEX_ALT		0X9F22 
  //Tag 9F22 Certification Authority Public Key Index Alt. 
 
#define  EMV_TAG_9F23_UPPER_CONSECUTIVE_OFFLINE_LIMIT					0X9F23 
  //Tag 9F23 Upper Consecutive Offline Limit. 
 
#define  EMV_TAG_9F26_APPLICATION_CRYPTOGRAM		0X9F26 
  //Tag 9F26 Application Cryptogram. 
 
#define  EMV_TAG_9F27_CRYPTOGRAM_INFO_DATA			0X9F27 
  //Tag 9F27 Cryptogram Info Data. 
 
#define  EMV_TAG_9F2D_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_CERTIFICATE		0X9F2D 
  //Tag 9F2D Icc PIN Encipherment Public Key Certificate. 
 
#define  EMV_TAG_9F2E_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_EXPONENT			0X9F2E 
  //Tag 9F2E Icc PIN Encipherment Public Key Exponent. 
 
#define  EMV_TAG_9F2F_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_REMAINDER			0X9F2F 
  //Tag 9F2F Icc PIN Encipherment Public Key Remainder. 
 
#define  EMV_TAG_9F32_ISSUER_PUBLIC_KEY_EXPONENT	0X9F32 
  //Tag 9F32 Issuer Public Key Exponent. 
 
#define  EMV_TAG_9F33_TERMINAL_CAPABILITIES			0X9F33 
  //Tag 9F33 Terminal Capabilities. 
 
#define  EMV_TAG_9F34_CVM_RESULTS					0X9F34 
  //Tag 9F34 CVM (Cardholder Verification Method) Results. 
 
#define  EMV_TAG_9F35_TERMINAL_TYPE					0X9F35 
  //Tag 9F35 Terminal Type. 
 
#define  EMV_TAG_9F36_ATC							0X9F36 
  //Tag 9F36 ATC (Application Transaction Counter). 
 
#define  EMV_TAG_9F37_UNPREDICATABLE_NUMBER			0X9F37 
  //Tag 9F37 Unpredicatable Number. 
 
#define  EMV_TAG_9F38_PDOL							0X9F38 
  //Tag 9F38 PDOL (Processing Options Data Object List). 
 
#define  EMV_TAG_9F39_POS_ENTRY_MODE				0X9F39 
  //Tag 9F39 PoS (Point of Service) Entry Mode. 
 
#define  EMV_TAG_9F3A_AMOUNT_REFERENCE_CURRENCY		0X9F3A 
  //Tag 9F3A Amount Reference Currency. 
 
#define  EMV_TAG_9F3B_APPLICATION_REFERENCE_CURRENCY					0X9F3B 
  //Tag 9F3B Application Reference Currency. 
 
#define  EMV_TAG_9F3C_TRANSACTION_REFERENCE_CURRENCY					0X9F3C 
  //Tag 9F3C Transaction Reference Currency. 
 
#define  EMV_TAG_9F3D_TRANSACTION_REFERENCE_CURRENCY_EXPONENT			0X9F3D 
  //Tag 9F3D Transaction Reference Currency Exponent. 
 
#define  EMV_TAG_9F40_ADDITIONAL_TERMINAL_CAPABILITIES					0X9F40 
  //Tag 9F40 Additional Terminal Capabilities. 
 
#define  EMV_TAG_9F41_TRANSACTION_SEQUENCE_COUNTER  0X9F41 
  //Tag 9F41 Transaction Sequence Counter. 
 
#define  EMV_TAG_9F42_APPLICATION_CURRENCY_CODE		0X9F42 
  //Tag 9F42 Application Currency Code. 
 
#define  EMV_TAG_9F43_APPLICATION_REFERENCE_CURRENCY_EXPONENT			0X9F43 
  //Tag 9F43 Application Reference Currency Exponent. 
 
#define  EMV_TAG_9F44_APPLICATION_CURRENCY_EXPONENT 0X9F44 
  //Tag 9F44 Application Currency Exponent. 
 
#define  EMV_TAG_9F45_DATA_AUTHENTICATION_CODE		0X9F45 
  //Tag 9F45 Data Authentication Code. 
 
#define  EMV_TAG_9F46_ICC_PUBLIC_KEY_CERTIFICATE	0X9F46 
  //Tag 9F46 ICC Public Key Certificate. 
 
#define  EMV_TAG_9F47_ICC_PUBLIC_KEY_EXPONENT		0X9F47 
  //Tag 9F47 ICC Public Key Exponent. 
 
#define  EMV_TAG_9F48_ICC_PUBLIC_KEY_REMAINDER		0X9F48 
  //Tag 9F48 ICC Public Key Remainder. 
 
#define  EMV_TAG_9F49_DDOL							0X9F49 
  //Tag 9F49 DDOL (Dynamic Data Object List). 
 
#define  EMV_TAG_9F4A_SDA_TAG_LIST					0X9F4A 
  //Tag 9F4A SDA (Static Data Authentication) //Tag List. 
 
#define  EMV_TAG_9F4B_SIGNED_DYNAMIC_APPLICATION_DATA					0X9F4B 
  //Tag 9F4B Signed Dynamic Application Data. 
 
#define  EMV_TAG_9F4C_ICC_DYNAMIC_NUMBER			0X9F4C 
  //Tag 9F4C ICC Dynamic Number. 

#define  EMV_TAG_9F4D_ICC_LOG_ENTRY						0X9F4D 
  //Tag 9F4D ICC Log Entry. 
	
#define  EMV_TAG_9F4F_ICC_LOG_FORMAT					0X9F4F 
  //Tag 9F4F ICC Log Format. 	
 
#define  EMV_TAG_00A5_FCI_PROPRIETARY_TEMPLATE		0X00A5 
  //Tag A5 FCI (File Control Info) Proprietary Template. 
 
#define  EMV_TAG_BF0C_FCI_ISSUER_DISCRETIONARY_DATA						0XBF0C 
  //Tag Bf0C FCI (File Control Info) Issuer Discretionary Data. 
 
#define  EMV_TAG_DF06_ISSUER_PROPRIETARY_DATA						0XDF06 
  //Tag DF06 Issuer Proprietary Data  
	
#define  EMV_TAG_DF24_ISSUER_PROPRIETARY_DATA						0XDF24 
  //Tag DF24 Issuer Proprietary Data  
#define  EMV_TAG_9F0D_TERMINAL_ACTION_CODE_DEFAULT  0XE001 
 //Tag - Terminal Action Code Default. 
 
#define  EMV_TAG_9F0E_TERMINAL_ACTION_CODE_DENIAL   0XE002 
 //Tag - Terminal Action Code Denial. 
 
#define  EMV_TAG_9F0F_TERMINAL_ACTION_CODE_ONLINE   0XE003 
 //Tag - Terminal Action Code Online. 
 
#define  EMV_TAG_9F60_ISSUER_SCRIPT_RESULTS			0XE012 
 //Tag - 9F60 Issuer Script Results. 
 
#define  EMV_TAG_9F61_TARGET_PERCENTAGE				0XE013 
 //Tag - 9F61 Target Percentage. 
 
#define  EMV_TAG_9F62_MAX_TARGET_PERCENTAGE			0XE014 
 //Tag - 9F62 Max Target Percentage. 
 
#define  EMV_TAG_9F63_THRESHOLD_VALUE				0XE015 
 //Tag - 9F63 Threshold Value. 
 
#define  EMV_TAG_9F70_CAPK_SIGN_ALGORITHM			0XE01A 
 //Tag - 9F70 CAPK Sign Algorithm. 
 
#define  EMV_TAG_9F71_CAPK_HASH_ALGORITHM			0XE01B 
 //Tag - 9F71 CAPK Hash Algorithm. 
 
#define  EMV_TAG_9F72_CAPK_MODULUS					0XE01C 
 //Tag - 9F72 CAPK Modulus. 
 
#define  EMV_TAG_9F73_CAPK_EXPONENT					0XE01D 
 //Tag - 9F73 CAPK Exponent. 
 
#define  EMV_TAG_9F74_CAPK_HASH						0XE01E 
 //Tag - 9F74 CAPK Hash. 
 
#define  EMV_TAG_E030_ISSUER_SCRIPTS				0xE030 
 //Tag - E030 Issuer Scripts. 
#define		GOODS_PURCHASE						0x00
#define		SERVIVE_PURCHASE					0x00
#define		CASH_BACK							0x09
#define		CASH_DEPOSE							0x21
#define		INQUIRY								0x31
#define		TRANSFER							0x40
#define		PAYMENT								0x50
#define		LOAD_SPECIFIED_ACCOUNT				0x60
#define		LOAD_UNSPECIFIED_ACCOUNT			0x62
#define		LOAD_CASH							0x63

#define	  	SERVICE_NOT_ALLOWED					0x01
#define	  	PIN_TRY_LIMIT_EXCEEDED				0x02
#define	  	ISSUER_AUTH_FAILED					0x03
#define	  	ADVICE_REQUIRED						0x08

#define		FAIL_CVM_PROCESSING					0x00
#define		OFFLINE_PLAIN_PIN					0x01
#define		ONLINE_ENCIPERED_PIN				0x02
#define		OFFLINE_PLAINPIN_SIGNATURE			0x03
#define		OFFLINE_ENCIPEREDPIN				0x04
#define		OFFLINE_ENCIPEREDPIN_SIGNATURE		0x05
#define		PAPER_SIGNATURE						0x1E
#define		NO_CVM_REQUIRED						0x1F

#define		ALWAYS								0x00
#define		UNATTENED_CASH						0x01
#define		NOT_UNATTENED_CASH					0x02
#define		SUPPORT_CVM							0x03
#define		MANUAL_CASH							0x04
#define		PURCHASE_WITH_CASHBACK				0x05
#define		UNDRE_X								0x06
#define		OVER_X								0x07
#define		UNDRE_Y								0x08
#define		OVER_Y								0x09

#define		AAC_REQ								0x00
#define		ARQC_REQ							0x80		
#define		TC_REQ								0x40
#define		NOT_ALLOWED							0x01
#define		NOT_ALLOWED_ADVICE					0x09

#define     REVERSAL_MSG						0x02
#define     CONFIRM_MSG							0x00

#define		CONFIG_DATA_START					0x8600

#define		TYPE_NOT_FOUND						0x00
#define		NUMERIC_TAG							0x01
#define		COMPRESS_NUMERIC_TAG				0x02
#define		BINARY_TAG							0x03
#define		AN_TAG								0x04
#define		ANS_TAG								0x05
#define		ALPHABETIC_TAG						0x06
#define		OTHER_TAG							0x07

#define		TAG_NOT_FOUND						0x04

#define		NI_NUM								0xFF
#define		NCA_NUM								0xFF
#define		NIC_NUM								0xFF
#define		NPE_NUM								0xFF

#define		PDOL_FLAG							0x01
#define		TDOL_FLAG							0x02
#define		CDOL1_FLAG							0x03
#define		CDOL2_FLAG							0x04

//Terminal Internal State
#define		SELECT_BY_PSE						0x01
#define		SELECT_BY_AID						0x02
#define		APP_FINAL_SELECTION					0x03
#define		GPO_START							0x04
#define		READ_APP_DATA						0x05
#define		PROCESSING_RESTRICTION				0x06
#define		CARDHOLDER_VERIFICATION				0x07
#define		TERMINAL_RISK_MANAGEMENT			0X08
#define		TERMINAL_ACTION_ANALYSIS			0X09
#define		WAIT_FOR_TRANSACTION				0x0A

#define	NONE								0x0000
#define	CANCELLED							0x0001
//#define	CARD_BLOCKED						0x0002
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
#define     LOG_RECORD_LEN						45


#define CAKeyStartADDR 0x00034800
#define CAKeyEndADDR 0x00036000

typedef struct
{
	unsigned char nlen;  //0-RSA1024, 1-RSA2048
	unsigned char *pE;
	unsigned char *pN;
	
}RSA_PubKey;


typedef struct {
	unsigned char PK_Index;		//公钥索引
	unsigned char PK_Fid;     //公钥索引对应的文件ID
} PK_TYPE;

typedef struct{
	unsigned char Rid[5];		//RID
	PK_TYPE pk_list[9];        //RID对应的公钥索引列表
} CA_LIST_TYPE;


unsigned int EMV_OfflineDataAuthentication(void);
unsigned int GetIssuerPublicKey(void);
unsigned int GetCardPublicKey(void);
unsigned char DATAUPLOAD(unsigned char type);
void CAListInit(void);

void RSADecryption(RSA_PubKey rsapubkey, unsigned char *pin,unsigned char *pout);

#endif
