#ifndef __CCID_VALUE_H
#define __CCID_VALUE_H

//#define 	POS_ENCPKT_CLA 							0x7f
//#define		POS_PLAINPKT_CLA					    0x7e

#define     DOUBLE_PACKET							0x01
#define 	DOUBLE_PACKET_OFF						0x00

/*成功完成并返回成功
*/
	//处理成功
	#define SW_OPERATION_SUCCESS	0x9000
	
	//需要后续处理
	#define SW_OPERATION_CONTINUE	0x6100

/*处理完成并返回报警，EEPROM不发生任何变化
*/
	//读出Le数据前文件或记录已结束	
	#define SW_DATA_LESSTHANLE		0x6282
	
	//选择的文件无效
	#define SW_SELECTFILE_INVALID	0x6283

	#define SW_DIS_TIMEOUT			0x6284
	
/*处理完成并返回报警，EEPROM发生变化
*/
	//身份验证错误错误，相关计数值发生变化
	#define SW_AUTH_ERROR			0X63C0

	
/*执行差错，指令在执行过程中，例如在做某种运算过程中出现异常问题，EEPROM不发生任何变化
*/
	//没有准确诊断
	#define SW_UNKOWN_ERROR			0X6400
	
/*执行差错，指令在执行过程中，例如在写文件过程中出现异常问题，EEPROM发生变化
*/
	//存储器故障
	#define SW_MEMORY_ERROR			0X6581

	//存储器越界
	#define SW_MEMORY_BEYOND		0X6582

/*检验差错，在接受到指令后检验指令的合法性；并将指令与资源相匹配
*/
/*长度错误
*/
	//Lc或Le长度错误
	#define SW_LCLE_ERROR			0X6700
	
/*0x68xx CLA的功能不支持
*/
	//不支持逻辑信道
	#define SW_CHANNEL_NONSUPPORT	0X6881
	
	//不支持安全报文
	#define SW_SM_NONSUPPORT		0X6882

	//CLA错误
	#define SW_CLA_ERROR			0X6883

/*0x69xx 不允许的INS
*/
	//命令与文件不匹配（例如用读写二进制文件的指令去读写记录文件）
	#define	SW_INS_FILE_NOTMATCH	0X6981
	
	//安全状态不满足	
	#define SW_SECURITY_DISSATISFY	0X6982
	
	//认证方式被阻塞
	#define SW_AUTH_LOCK			0x6983
	
	//引用的数据无效（例如要使用随机数，但随机数不存在)
	#define SW_REFERENCEDATA_INVALID 0x6984
	
	//使用的条件不被满足
	#define SW_CONDITION_DISSATISFY	0X6985
	
	//命令不允许（当前没有选中的EF，但有对EF的操作）	
	#define SW_INS_FORBID			0X6986
	
	//安全报文数据对象丢失	
	#define SW_SMOBJ_LOST			0X6987
	
	//安全报文数据对象不正确	
	#define SW_SMOBJ_ERROR			0X6988

	//卡片被锁定
	#define SW_CARD_LOCK			0X6989

	//应用被临时锁定
	#define SW_APPLICATION_LOCK		0X6990

	//应用被永久锁定
	#define SW_APPLICATION_LOCK_FOREVER		0X6991

	
//P1 P2错误
	//数据字段中相关数据验证错误	
	#define SW_DATAFIELD_ERROR		0X6A80
	
	//功能不支持	
	#define SW_FUNCTION_NOTSUPPORT	0X6A81
	
	//目录或文件未找到
	#define SW_FILE_NOTFOUND		0X6A82
	
	//记录未找到
	#define SW_RECORD_NOTFOUND		0X6A83
	
	//文件中没有足够空间	
	#define SW_FILESPACE_NOTENOUGH	0X6A84

	//P1 P2不正确
	#define SW_P1P2_ERROR			0x6a86

	//P1 P2与LC不匹配
	#define SW_P1P2_LC_NOTMATCH		0x6a87

	//引用的数据未找到
	#define SW_KEY_NOTFOUND			0x6a88

	#define SW_CAPABILITY_LENGTH	0x6c00

	#define SW_COMMAND_UNVALID		0x6d00

//自定义错误码
	//卡片上电失败
	#define CARD_POWERON_FAIL		0x1111
	//PIN加密错误
	#define KEY_ENCRYPT_ERROR		0x2222



#define     CREATE_FILE			    0x80
#define     DELETE_FILE			    0x81
#define     SELECT_FILE			    0x82
#define     UPDATE_BINARY			0x83
#define     READ_BINARY			    0x84
#define     GET_FILEINFO	        0x85
#define     VERIFY_PIN			    0x86
#define     CHANGE_PIN  			0x87
#define     PIN_UNBLOCK_KEY			0x88
#define     EXTERNAL_AUTHENTICATION	0x89
#define     INTERNAL_AUTHENTICATION	0x8a
#define     WRITE_KEY   			0x8b
#define     GET_CHALLENGE			0x8c
#define     GENERATE_SECKEY			0x8d
#define     ENCRYPT_DECRYPT         0x8e
#define     GENERATE_RSAKEY			0x8f
#define     RSA_SIGN			    0x90
#define     RSA_VERIFY			    0x91

#define 	ACTIVE_DF				0x92
#define     APPLICATION_BLOCK		0x93
#define     APPLICATION_UNBLOCK		0x94
#define     CARD_BLOCK			    0x95

#define     GET_RESPONSE			0x96
#define     GET_INFO			    0x97
#define     SET_DATA				0x98


#endif 
