#include "string.h"
#include "offline.h"
#include "PBOC_Var.h"
#include "sercrity.h"
#include "Rsa.h"

RSA_PubKey CAPublicKey,IssuerPublicKey,ICCardPublicKey;

unsigned char keytimes;
unsigned int result;
unsigned char SHAData[512];
unsigned short SHADataLen;
unsigned char DataDecryption[248];
unsigned char RIDList[5][5]={{0xA0,0x00,0x00,0x03,0x33},
					{0xA0,0x00,0x00,0x00,0x03},
					{0xA0,0x00,0x00,0x99,0x99},
					{0xA0,0x00,0x00,0x00,0x04},
					{0xA0,0x00,0x00,0x00,0x65}};
unsigned int KeyExponent;

extern unsigned char FLAG_NO9F26;

extern  unsigned char SDAFlag;
extern unsigned char SDAData[248];
extern unsigned char  SDALen;
extern unsigned char SDANum;

unsigned char PublickeyN[256];
unsigned char PublickeyE[4];
unsigned char ddoldata[128];
unsigned char ddollen;

void RSADecryption(RSA_PubKey publickey, unsigned char *pIn,unsigned char *pOut)
{	
	R_RSA_PUBLIC_KEY	rsa_public_key;
	unsigned int  len;
	len=publickey.nlen;
	rsa_public_key.bits=publickey.nlen*8;
	memcpy(rsa_public_key.modulus,publickey.pN,publickey.nlen);
	memcpy(rsa_public_key.exponent,publickey.pE,4);
	LitteToBigStr(rsa_public_key.exponent,4);
	RSAPublicDecrypt(pOut,&len,pIn,len,&rsa_public_key);
}
const unsigned char uchCAPubKeyN0[] = 
{
	0xB0,0x62,0x7D,0xEE,0x87,0x86,0x4F,0x9C,0x18,0xC1,0x3B,0x9A,0x1F,0x02,0x54,0x48,
	0xBF,0x13,0xC5,0x83,0x80,0xC9,0x1F,0x4C,0xEB,0xA9,0xF9,0xBC,0xB2,0x14,0xFF,0x84,
	0x14,0xE9,0xB5,0x9D,0x6A,0xBA,0x10,0xF9,0x41,0xC7,0x33,0x17,0x68,0xF4,0x7B,0x21,
	0x27,0x90,0x7D,0x85,0x7F,0xA3,0x9A,0xAF,0x8C,0xE0,0x20,0x45,0xDD,0x01,0x61,0x9D,
	0x68,0x9E,0xE7,0x31,0xC5,0x51,0x15,0x9B,0xE7,0xEB,0x2D,0x51,0xA3,0x72,0xFF,0x56,
	0xB5,0x56,0xE5,0xCB,0x2F,0xDE,0x36,0xE2,0x30,0x73,0xA4,0x4C,0xA2,0x15,0xD6,0xC2,
	0x6C,0xA6,0x88,0x47,0xB3,0x88,0xE3,0x95,0x20,0xE0,0x02,0x6E,0x62,0x29,0x4B,0x55,
	0x7D,0x64,0x70,0x44,0x0C,0xA0,0xAE,0xFC,0x94,0x38,0xC9,0x23,0xAE,0xC9,0xB2,0x09,
	0x8D,0x6D,0x3A,0x1A,0xF5,0xE8,0xB1,0xDE,0x36,0xF4,0xB5,0x30,0x40,0x10,0x9D,0x89,
	0xB7,0x7C,0xAF,0xAF,0x70,0xC2,0x6C,0x60,0x1A,0xBD,0xF5,0x9E,0xEC,0x0F,0xDC,0x8A,
	0x99,0x08,0x91,0x40,0xCD,0x2E,0x81,0x7E,0x33,0x51,0x75,0xB0,0x3B,0x7A,0xA3,0x3D
};
unsigned int FILE_ReadCAKey()
{
	PublickeyN[0] = 176;
	memcpy(PublickeyN+1, uchCAPubKeyN0, 176);
	return 0x9000;
}

//解密发卡行证书
unsigned int GetIssuerPublicKey()
{	
	unsigned char i;
	unsigned char CAPublicKeyLen=PublickeyN[0];
	
	KeyExponent=3;

	SHADataLen=0;
	CAPublicKey.pN=&PublickeyN[1];
	CAPublicKey.pE=(unsigned char *)&KeyExponent;
	CAPublicKey.nlen=PublickeyN[0];
	
	if(0==uchIssuerKeyCert[0])
	{
		*puchTvr|=0x20;
		return 0;
	}
	if(CAPublicKeyLen!=uchIssuerKeyCert[0])  //长度不符合
	{
		return 0;			
	}
	else
		{
			RSADecryption(CAPublicKey,&uchIssuerKeyCert[1],DataDecryption);
			
			if(DataDecryption[CAPublicKeyLen-1]!=0xBC)
			{	
				return 0;
			}
			if(DataDecryption[0]!=0x6A)
			{
				return 0;
			}
			if(DataDecryption[1]!=0x02)
			{
				return 0;
			}
			for(i=0;i<4;i++)
			{
				if(DataDecryption[2+i]!=uchPan[1+i])
				{
					if(DataDecryption[2+i]==0xff)
						{}
					else if(DataDecryption[2+i]!=0xff)
					{
						if((DataDecryption[2+i]&0xf0)!=(uchPan[1+i]&0xf0))
							return 0;
						else
						{
							if((DataDecryption[2+i]&0x0f)!=0x0f)
								return 0;
						}
					}
				}					
			}
			if(DataDecryption[7]<*(puchDate)) //比较时间
			{
				return 0;
			}
			else if(DataDecryption[7]==*(puchDate)&&DataDecryption[6]<*(puchDate+1))
			{
				return 0;
			}
			if(DataDecryption[11]!=1||DataDecryption[12]!=1)
			{
				return 0;
			}
			SHADataLen=CAPublicKeyLen-22;//14+capublickeylen-36
			memcpy(SHAData,&DataDecryption[1],SHADataLen);			
			if(uchIssuerKeyReminder[0]!=0)//发卡行公钥余项是否存在
			{
				memcpy(&SHAData[SHADataLen],&uchIssuerKeyReminder[1],uchIssuerKeyReminder[0]);
				SHADataLen+=uchIssuerKeyReminder[0];
				if(0==uchIssuerKeyExp[0])
				{	
					*puchTvr|=0x20;
				 	return 0;
				}
				memcpy(&SHAData[SHADataLen],&uchIssuerKeyExp[1],uchIssuerKeyExp[0]);					
				SHADataLen+=uchIssuerKeyExp[0];					
			}
			else
			{
				if(DataDecryption[13]>CAPublicKeyLen-36)
				{	
					*puchTvr|=0x20;
				 	return 0;
				}
				if(0==uchIssuerKeyExp[0])
				{	
					*puchTvr|=0x20;
				 	return 0;
				}
				memcpy(&SHAData[SHADataLen],&uchIssuerKeyExp[1],uchIssuerKeyExp[0]);
				SHADataLen+=uchIssuerKeyExp[0];
			}
			SHA1Arigth(SHAData,SHADataLen);	
			if(memcmp(&DataDecryption[CAPublicKeyLen-21],SHAData,20)!=0)//比较哈希值
			{
				return 0;
			}			
			IssuerPublicKey.nlen=DataDecryption[13];
			memcpy(PublickeyN,&DataDecryption[15],CAPublicKeyLen-36);			
			memcpy(&PublickeyN[CAPublicKeyLen-36],&uchIssuerKeyReminder[1],uchIssuerKeyReminder[0]);			
			return 0x9000;	
		}
}
unsigned short StaticDataVeryfy()
{
	unsigned char SDADataLen=IssuerPublicKey.nlen;
	
	if(uchIssuerKeyExp[0]==1)
		KeyExponent=3;
	else if(uchIssuerKeyExp[0]==3)
		KeyExponent=65537;

	IssuerPublicKey.pE=(unsigned char*)&KeyExponent;
	IssuerPublicKey.pN=PublickeyN;
	SHADataLen=0;
	
	if(0==uchSSAData[0])
	{
		*puchTvr|=0x20;			
		return 0;	
	}
	
	if(SDADataLen!=uchSSAData[0])  //长度不符合
	{
		*puchTvr|=0x08;			
		return 0;			
	}
	
	else
	{
		RSADecryption(IssuerPublicKey,&uchSSAData[1],DataDecryption);
		if(DataDecryption[SDADataLen-1]!=0xBC)
			{
				*puchTvr|=0x08;
				return 0;
			}		
		if(DataDecryption[0]!=0x6A)
			{
				*puchTvr|=0x08;
				return 0;
			}
		if(DataDecryption[1]!=0x03)
			{
				*puchTvr|=0x08;
				return 0;
			}			
		
		SHADataLen=SDADataLen-22;//14+capublickeylen-36
		memcpy(SHAData,&DataDecryption[1],SHADataLen);		
		memcpy(&SHAData[SHADataLen],SDAData,SDALen);
		SHADataLen+=SDALen;
		if(uchSDATagList[0]!=0)
		{
			if(uchSDATagList[0]!=1&&uchSDATagList[1]!=0x82)
			{
				*puchTvr|=0x08;
				return 0;
			}
			else
			{
				memcpy(&SHAData[SHADataLen],&uchAip[1],2);
				SHADataLen+=2;
			}
		}
		SHA1Arigth(SHAData,SHADataLen);
		if(memcmp(&DataDecryption[SDADataLen-21],SHAData,20)!=0)//比较哈希值
		{
			*puchTvr|=0x08;
			return 0;
		}
		return 0x9000;
	}	
}

unsigned int GetCardPublicKey()
{
	//unsigned char CardDecryption[248];
	unsigned char CardKeyLen=IssuerPublicKey.nlen;	
	
	if(uchIssuerKeyExp[0]==1)
		KeyExponent=3;
	else if(uchIssuerKeyExp[0]==3)
		KeyExponent=65537;
	
	IssuerPublicKey.pE=(unsigned char*)&KeyExponent;
	IssuerPublicKey.pN=PublickeyN;
	SHADataLen=0;
	if(0==uchICCKeyCert[0])
	{
		*puchTvr|=0x20;
		return 0;
	}
	if(CardKeyLen!=uchICCKeyCert[0])  //长度不符合
	{
		*puchTvr|=0x08;			
		return 0;			
	}
	else
	{
		RSADecryption(IssuerPublicKey,&uchICCKeyCert[1],DataDecryption);
		//HAL_USB_BulkIn(2,DataDecryption,CardKeyLen);
		if(DataDecryption[CardKeyLen-1]!=0xBC)
		{
			*puchTvr|=0x08;
			return 0;
		}		
		if(DataDecryption[0]!=0x6A)
		{
			*puchTvr|=0x08;
			return 0;
		}
		if(DataDecryption[1]!=0x04)
		{
			*puchTvr|=0x08;
			return 0;
		}	
		if(memcmp(&DataDecryption[2],&uchPan[1],uchPan[0])!=0)
		{
			return 0;
		}
		if(DataDecryption[13]<*(puchDate)) //比较时间
		{
			*puchTvr|=0x08;
			return 0;
		}
		else if(DataDecryption[13]==*(puchDate)&&DataDecryption[12]<*(puchDate+1))
		{
			*puchTvr|=0x08;
			return 0;
		}
		if(DataDecryption[17]!=1||DataDecryption[18]!=1)
		{
			*puchTvr|=0x08;
			return 0;
		}
		SHADataLen=CardKeyLen-22;//14+capublickeylen-36
		memcpy(SHAData,&DataDecryption[1],SHADataLen);
		if(uchICCKeyReminder[0]!=0)//公钥余项是否存在
		{
			memcpy(&SHAData[SHADataLen],&uchICCKeyReminder[1],uchICCKeyReminder[0]);
			SHADataLen+=uchICCKeyReminder[0];
			if(0==uchIssuerKeyExp[0])
			{
				*puchTvr|=0x20;
				return 0;
			}
			memcpy(&SHAData[SHADataLen],&uchIssuerKeyExp[1],uchIssuerKeyExp[0]);
			memcpy(PublickeyE,&uchIssuerKeyExp[1],uchIssuerKeyExp[0]);
			SHADataLen+=uchIssuerKeyExp[0];
		}
		else
		{
			if(DataDecryption[19]>CardKeyLen-42)
			{	
				*puchTvr|=0x20;
			 	return 0;
			}
			if(0==uchIssuerKeyExp[0])
			{
				*puchTvr|=0x20;
				return 0;
			}
			memcpy(&SHAData[SHADataLen],&uchIssuerKeyExp[1],uchIssuerKeyExp[0]);
			SHADataLen+=uchIssuerKeyExp[0];
		}
		memcpy(&SHAData[SHADataLen],SDAData,SDALen);
		SHADataLen+=SDALen;
		//HAL_USB_BulkIn(2,uchSDATagList,3);
		if(uchSDATagList[0]!=0)
		{
			if(uchSDATagList[0]!=1&&uchSDATagList[1]!=0x82)
			{
				*puchTvr|=0x08;
				return 0;
			}
			else
			{
				memcpy(&SHAData[SHADataLen],&uchAip[1],2);
				SHADataLen+=2;
			}
		}
		SHA1Arigth(SHAData,SHADataLen);
		if(memcmp(&DataDecryption[CardKeyLen-21],SHAData,20)!=0)//比较哈希值
			{
				*puchTvr|=0x08;
				return 0;
			}		
		ICCardPublicKey.nlen=DataDecryption[19];
		memcpy(PublickeyN,&DataDecryption[21],IssuerPublicKey.nlen-42);			
		memcpy(&PublickeyN[IssuerPublicKey.nlen-42],&uchICCKeyReminder[1],uchICCKeyReminder[0]);
		return 0x9000;
	}
}
unsigned int DynamicDataVeryfy()
{
	//unsigned char DynamicDecryption[248];	
	unsigned char CardKeyLen=ICCardPublicKey.nlen;	
	if(uchIssuerKeyExp[0]==1)
		KeyExponent=3;
	else if(uchIssuerKeyExp[0]==3)
		KeyExponent=65537;
	
	ICCardPublicKey.pE=(unsigned char*)&KeyExponent;
	ICCardPublicKey.pN=PublickeyN;
	
	SHADataLen=0;
	if(CardKeyLen!=uchSDAData[0])  //长度不符合
	{
		*puchTvr|=0x08;			
		return 0;			
	}
	else
	{	
		RSADecryption(ICCardPublicKey,&uchSDAData[1],DataDecryption);
		//HAL_USB_BulkIn(2,DataDecryption,CardKeyLen);
		if(DataDecryption[CardKeyLen-1]!=0xBC)
				{
				*puchTvr|=0x08;
				return 0;
			}
		if(DataDecryption[0]!=0x6A)
			{
				*puchTvr|=0x08;
				return 0;
			}
		if(DataDecryption[1]!=0x05)
			{
				*puchTvr|=0x08;
				return 0;
			}
	
		SHADataLen=CardKeyLen-22;//14+capublickeylen-36
		memcpy(SHAData,&DataDecryption[1],SHADataLen);		
		if(FLAG_NO9F26==0)
		{
			memcpy(&SHAData[SHADataLen],puchUnpreData,4);
			SHADataLen+=4;
			if(uchICAuthenticationData[0]!=0)
			{
				memcpy(&SHAData[SHADataLen],puchAmount,6);
				SHADataLen+=6;
				memcpy(&SHAData[SHADataLen],puchCurrencyCode,2);
				SHADataLen+=2;
				memcpy(&SHAData[SHADataLen],uchICAuthenticationData+1,uchICAuthenticationData[0]);
				SHADataLen+=uchICAuthenticationData[0];
			}
		}
		if(FLAG_NO9F26==1)
		{
			memcpy(&SHAData[SHADataLen],ddoldata,ddollen);
			SHADataLen+=ddollen;
		}
		SHA1Arigth(SHAData,SHADataLen);
		if(memcmp(&DataDecryption[CardKeyLen-21],SHAData,20)!=0)//比较哈希值
		{
			*puchTvr|=0x08;
			return 0;
		}
		memcpy(uchICCDynamNum,&DataDecryption[4],DataDecryption[3]);
		return 0x9000;
		}
}
unsigned int OfflineStaticDataAuthentication()
{	
	*puchTsi |= 0x80;	//脱机数据认证已执行
	*puchTvr &= 0x00;
	result=FILE_ReadCAKey();	
	if(result!=0x9000)
	{
		return 0;
	}	
	result=GetIssuerPublicKey();
	if(result!=0x9000)
	{
		return 0;
	}	
	result =StaticDataVeryfy();
	if(result!=0x9000)
	{
		return 0;
	}
	return 0x9000;
}

unsigned int OfflineDynamicDataAuthentication()
{
	unsigned short result;

	result=FILE_ReadCAKey();	
	if(result!=0x9000)
	{
		return 0;
	}	
	result=GetIssuerPublicKey();
	if(result!=0x9000)
	{
		return 0;
	}	
	result =GetCardPublicKey();
	if(result!=0x9000)
	{
		return 0;
	}
		
	result=DynamicDataVeryfy();
	if(result!=0x9000)
	{
		return 0;
	}
	
	return 0x9000;
}


