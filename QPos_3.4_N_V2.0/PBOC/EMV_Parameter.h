#ifndef  __EMV_PARAMETER_H__
#define  __EMV_PARAMETER_H__

//EMV Level 2相关
#define	 	CONTACT_PSE_NAME					"1PAY.SYS.DDF01"	   //0x31,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31
#define	 	CONTACTLESS_PSE_NAME				"2PAY.SYS.DDF01"	   //0x32,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31

#define		MAX_PIN_LENGTH						12
#define		PIN_BUF_LENGTH						8
#define   LOG_RECORD_LEN						45

#define  	EMV_PROTOCOL_T0						0x00   //T = 0  
#define  	EMV_PROTOCOL_T1						0x01   //T = 1 
#define  	EMV_PROTOCOL_UNKNOWN				0xFF   //未知的协议类型

#define  	ASI_EXACT_MATCH_REQUIRED			0x0000   //仅完全匹配的应用可加入候选应用列表
#define  	ASI_PARTIAL_MATCH_ALLOWED			0x0001   //部分匹配的应用即可加入候选应用列表

#define  	DDF_STACK_DEPTH						2					 //DDF Stack深度为10
#define  	DDF_STACK_EMPTY						0	 
#define	 	MAX_AID_NUMBER						35
#define	 	MAX_CANDIDATE_NUMBER				16
#define	 	MAX_PAGES_FOR_AID_DISPLAY			4

#define  	SELECT_FIRST						0
#define  	SELECT_NEXT							2

#define		READER_VARDO_LIST_SIZE					177
#define		CARD_DO_LIST_SIZE								512
#define		READER_CONSTANTDO_LIST_SIZE			140

#define		GOODS_PURCHASE									0x00
#define		SERVIVE_PURCHASE								0x00
#define		CASH_BACK												0x09
#define		CASH_DEPOSE											0x21
#define		INQUIRY													0x31
#define		TRANSFER												0x40
#define		PAYMENT													0x50
#define		LOAD_SPECIFIED_ACCOUNT					0x60
#define		LOAD_UNSPECIFIED_ACCOUNT				0x62
#define		LOAD_CASH												0x63

#define	  	SERVICE_NOT_ALLOWED						0x01
#define	  	PIN_TRY_LIMIT_EXCEEDED				0x02
#define	  	ISSUER_AUTH_FAILED						0x03
#define	  	ADVICE_REQUIRED								0x08

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
#define		NOT_ALLOWED						0x01
#define		NOT_ALLOWED_ADVICE		0x09

#define   REVERSAL_MSG					0x02
#define   CONFIRM_MSG						0x00

#define		CONFIG_DATA_START					0x8600

#define		TYPE_NOT_FOUND									0x00
#define		NUMERIC_TAG											0x01
#define		COMPRESS_NUMERIC_TAG						0x02
#define		BINARY_TAG											0x03
#define		AN_TAG													0x04
#define		ANS_TAG													0x05
#define		ALPHABETIC_TAG									0x06
#define		OTHER_TAG												0x07

#define		TAG_NOT_FOUND										0x04

#define		NI_NUM													0xFF
#define		NCA_NUM													0xFF
#define		NIC_NUM													0xFF
#define		NPE_NUM													0xFF

#define		PDOL_FLAG												0x01
#define		TDOL_FLAG												0x02
#define		CDOL1_FLAG											0x03
#define		CDOL2_FLAG											0x04

//Terminal Internal State
#define		SELECT_BY_PSE										0x01
#define		SELECT_BY_AID										0x02
#define		APP_FINAL_SELECTION							0x03
#define		GPO_START												0x04
#define		READ_APP_DATA										0x05
#define		PROCESSING_RESTRICTION					0x06
#define		CARDHOLDER_VERIFICATION					0x07
#define		TERMINAL_RISK_MANAGEMENT				0X08
#define		TERMINAL_ACTION_ANALYSIS				0X09
#define		WAIT_FOR_TRANSACTION						0x0A



#endif
