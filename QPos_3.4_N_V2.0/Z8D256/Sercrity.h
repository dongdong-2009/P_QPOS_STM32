
#ifndef _SERCRITY_H_
#define _SERCRITY_H_

#include "Z8D256.h"

typedef union 
{
    unsigned short length;
    unsigned char lenbuffer[2];
}DataLength,*pDataLength;
typedef struct 
{
    unsigned char commandFlag;
    unsigned char commandHead;
    DataLength dataLen;
    unsigned char cla;
    unsigned char ins;
    unsigned char pp1;
    unsigned char pp2;
    unsigned char lc;
    unsigned char databuf[255];    
}CommandData,*pCommandData;

typedef struct
{  	
    unsigned char answerFlag; //协议头
	unsigned char answerHead; //协议头
	DataLength   dataLen;	
	unsigned char dataBuf[255]; //发送接收的数据缓冲区
}AnswerData,*pAnswerData;
typedef struct
{
    unsigned char recvbuff[139];//保存命令中的数据
    unsigned char restorebuff[132];//保存读取publicfile中的n e
}PbulickeySt;
typedef struct
{
    unsigned char recvbuff[139];//保存des加密命令数据
    unsigned char desbuffer[16];//保存des加密后的结果数据
}DesSt;
typedef union 
{
	  CommandData  Psamcommand;
	  unsigned char  psamcommandbuff[270];
}psamComBuffer;
typedef union 
{
	  AnswerData  Psamanswer;
	  unsigned char  psamanswerbuff[260];
}psamAnsBuffer;
typedef union
{
    PbulickeySt publicstruct;
    unsigned char bublicbuffer[271];
}PublickeyBuffer;
typedef union
{
    DesSt desStruct;
    unsigned char desDataBuffer[271];
}DesBuffer;
typedef union
{
	 psamComBuffer psamcombuf;
     psamAnsBuffer psamansBuf;
     PublickeyBuffer publickbuf;
     DesBuffer desbuf;
}psamAPDU;

typedef union
{
	unsigned short tempcrc;
	unsigned char crcbuf[2];
}CRCst;
typedef struct 
{
    unsigned char kid;
    unsigned char ktype;//密钥类型
    unsigned char kversion;//密钥版本
    unsigned char kArigth;//密钥算法标识;0x00 3DES 0x04 DES
    unsigned char kuseAc;//使用权：
    unsigned char kstate;//密钥的后续状态
    unsigned char kchangeAc;//密钥的修改权限
    unsigned char times;//重试次数   
    unsigned char keybuf[16];
}keySt;
typedef union
{
    keySt keyStruct;
    unsigned char buffer[24];
}keyUion;
typedef struct 
{
    unsigned char btype;//文件类型
    unsigned char bFidh;//文件ID高
    unsigned char bFidl;//文件ID低
    unsigned char bFsizeh;//文件大小高
    unsigned char bFsizel;//文件大小低
    unsigned char bRef;//保留
    unsigned char bReadAtr;//读属性
    unsigned char bWriteAtr;//写属性  
    unsigned char bReadAc;//读权限
    unsigned char bWriteAc;//写权限
    unsigned char bUseAc;//使用权限
    unsigned char bReadKid;//读密钥标识
    unsigned char bWriteKid;//写密钥标识
    unsigned char bUseKid; //使用密钥标识   
}BinarySt;
typedef union
{
    BinarySt BinaryStruct;
    unsigned char buffer[24];
}BinaryUion;
typedef union
{
    keyUion keyui;
    BinaryUion binaryui;
}OptionSt;

extern psamAPDU psamapdu;
extern CRCst crcValue;

Z8D256_RECV_STATUS Z8D256_RecvString_XIEYI(unsigned char* buffer);

void LitteToBigStr(unsigned char buffer[],unsigned short len);
void sercrityTest(void);
unsigned char EraseMF(void);
unsigned char CreateMF(void);
unsigned char createKeyFile(unsigned int fileSize,unsigned char reloadAt,unsigned char reloadAc,unsigned char reloadKeyID);
unsigned char loadMainkey(void);
unsigned char loadDESkey(keyUion *pkeyui);
unsigned char createBinaryFile(BinaryUion *pbui);
void LittleToBig_short(const unsigned short in,unsigned short *pout);
//mode 1: 设备id号 0：PSAMid
unsigned char getDeviceInfo(unsigned char*pbuff,unsigned char mode);
//设置设备信息
unsigned char setDeviceInfo(unsigned char*pbuff);
unsigned char setPSAMInfo(unsigned char*pbuff);
//设置应用密钥
unsigned char setApplykey(unsigned char *pbuffer,unsigned char kid);
unsigned char setTrackkey(unsigned char *pbuffer,unsigned char kid);

//创建私钥文件
unsigned char createPrivateFile(unsigned short privateFileID);
//创建公钥文件
unsigned char createPublicFile(unsigned short publicFileID);
//更新公钥文件
unsigned char updatePublickeyFile(unsigned short publicFileID);
//算取mac值
unsigned char ToGetMAC(const unsigned char buffer[],unsigned short len,unsigned char *macBuff);
//算取8个字节和8个字节的XOR值
unsigned char XORArigth(const unsigned char *buffer,unsigned short len,unsigned char *XORBuff);

//选择二进制文件
unsigned char seleteBinaryFile(unsigned int fileID);
//更新二进制文件
unsigned char updateBinaryFile(unsigned short addr,unsigned char *pbuffer,unsigned char len);

unsigned char ReadBinaryFile(unsigned short addr,unsigned char *pbuffer,unsigned char len);
//更新密钥
unsigned char changeDESkey(unsigned char buffer[],unsigned char KID,unsigned char len);
//单DES加解密
unsigned char SingleDesEntryOrDisEntry(unsigned char srcbuffer[],unsigned char len,unsigned char KID,unsigned char mode);
unsigned char SingleDesBykey(unsigned char srcbuffer[],unsigned char len,unsigned char *pkey,unsigned char mode);
unsigned char ThreeDesBykey(unsigned char srcbuffer[],unsigned short len,unsigned char *pkey,unsigned char mode);
unsigned char threeDesEntryOrDisEntry(unsigned char srcbuffer[],unsigned char len,unsigned char KID,unsigned char mode);


//生成RSA密钥对
unsigned char genaryRsaKey(unsigned short publicFileID,unsigned short privateFileID);
unsigned char ReadPublicFile(unsigned short publicFileID);
unsigned char RsaDisEncrty(unsigned short privateFileID,unsigned char buffer[]);
unsigned char RsaEncrty(unsigned short publicFileID,unsigned char buffer[]);
//PSAMUID
unsigned char getPSAMUID(unsigned char *pbuffer,unsigned char len);
unsigned char getPSAMInfo(unsigned char*pbuff);
unsigned char GetChallenge(unsigned char buffer[],unsigned char len);
//SHA1
unsigned char SHA1Arigth(unsigned char *pbuf,unsigned int len);

//to boot
unsigned char TobootZ8(void);

unsigned char QPOSFileStruct(void);


#endif //_SERCRITY_H_

