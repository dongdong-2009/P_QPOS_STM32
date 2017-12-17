#include "string.h"
#include	"IC_interface.h"
#include "IROM.h"
#include "MID_PBOC.h"

AID_LIST_STRUCT *_AID_LIST=(AID_LIST_STRUCT *)FLASH_AID_ADDR;
unsigned char AID_COUNT;

unsigned char AID_GetCount(void)
{
	unsigned char i;
	AID_COUNT=0;
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if((_AID_LIST->AID_LIST[i].m_AID[0]>4)&&(_AID_LIST->AID_LIST[i].m_AID[0]<17))//长度
		{
			AID_COUNT++;
		}
	}
	return AID_COUNT;
}
void	AID_DeleteALL(void)
{
	unsigned char i;
	unsigned char buf[5]={0xff};
	unsigned int addr;
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		addr=(unsigned int)&(_AID_LIST->AID_LIST[i]);
		IROM_Write(addr,buf,5);		
	}
}
void	AID_DeleteAID(unsigned char *puchAID,unsigned char uchAIDLen)
{
	unsigned char i;
	unsigned char buf[5]={0xff};
	unsigned int addr;
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if(_AID_LIST->AID_LIST[i].m_AID[0]==uchAIDLen)
		{
			if(!memcmp(puchAID,_AID_LIST->AID_LIST[i].m_AID+1,uchAIDLen))
			{
				addr=(unsigned int)&(_AID_LIST->AID_LIST[i]);
				IROM_Write(addr,buf,5);
				return;
			}
		}		
	}
}

unsigned char AID_Check(unsigned char *puchAID,unsigned char uchAIDLen)
{
	unsigned char i;
	
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if(_AID_LIST->AID_LIST[i].m_AID[0]==uchAIDLen)
		{
			if(!memcmp(puchAID,_AID_LIST->AID_LIST[i].m_AID+1,uchAIDLen))
			{
				return i;
			}
		}
	}
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if(_AID_LIST->AID_LIST[i].m_AID[0]==0xff)
		{
			return i;
		}
	}
	return 0xff;
}

unsigned char AID_ADD(unsigned char *pucIn,unsigned int uiLen)
{
	AID_STRUCT	aid;
	unsigned char num;
	unsigned int addr;
	unsigned char *p;
	p=SearchTag(pucIn,uiLen,0x9f06);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_AID,p+2,*(p+2)+1);
	}
	p=SearchTag(pucIn,uiLen,0xdf01);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		aid.m_ASI=*(p+3);
	}
	p=SearchTag(pucIn,uiLen,0x9f09);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_APPVer,p+3,2);
	}
	p=SearchTag(pucIn,uiLen,0xdf11);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_TAC_Default,p+3,5);
	}
	p=SearchTag(pucIn,uiLen,0xdf12);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_TAC_Onlie,p+3,5);
	}
	p=SearchTag(pucIn,uiLen,0xdf13);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_TAC_Denial,p+3,5);
	}
	p=SearchTag(pucIn,uiLen,0x9f1b);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_ThresholdAmount,p+3,4);
	}
	p=SearchTag(pucIn,uiLen,0xdf15);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_ThresholdforBiasRandomSelection,p+3,4);
	}

	p=SearchTag(pucIn,uiLen,0xdf16);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		aid.m_MAXPercentforBiasRandomSelection=*(p+3);
	}

	p=SearchTag(pucIn,uiLen,0xdf17);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		aid.m_PercentforBiasRandomSelection=*(p+3);
	}

	p=SearchTag(pucIn,uiLen,0xdf14);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_DDOL_Default,p+2,*(p+2)+1);
	}

	p=SearchTag(pucIn,uiLen,0xdf18);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		aid.m_OnlinePIN=*(p+3);
	}

	p=SearchTag(pucIn,uiLen,0xdf20);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_NFC_ThresholdAmount,p+3,6);
	}

	p=SearchTag(pucIn,uiLen,0xdf21);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(aid.m_NFC_CVMm_NFCThresholdAmount,p+3,6);
	}

	p=SearchTag(pucIn,uiLen,0xdf19);
	if(p==NULL)
	{
		//return 1;
	}
	else
	{
		memcpy(aid.m_NFC_OFFLineThresholdAmount,p+3,6);
	}

	p=SearchTag(pucIn,uiLen,0x9f7b);
	if(p==NULL)
	{
		//return 1;
	}
	else
	{
		memcpy(aid.m_ECTerminalTransLimit,p+3,6);
	}
	
	num=AID_Check(aid.m_AID+1,aid.m_AID[0]);
	if(num==0xff)
	{
		return 1;
	}
	addr=(unsigned int)&(_AID_LIST->AID_LIST[num]);
	IROM_Write(addr,(unsigned char*)&aid,sizeof(aid));
	return 0;
	
}

AID_STRUCT* AID_GetOne(unsigned char *puchAID,unsigned char uchAIDLen)
{
	unsigned char i;
	AID_STRUCT *pAID;
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if(_AID_LIST->AID_LIST[i].m_AID[0]==uchAIDLen)
		{
			if(!memcmp(puchAID,_AID_LIST->AID_LIST[i].m_AID+1,uchAIDLen))
			{
				break;
			}
		}
		if(i==MAX_AID_COUNT-1)
			return 0;
	}
	pAID=&(_AID_LIST->AID_LIST[i]);
	return pAID;
}

void AID_GetInfo(unsigned char *pucOut,unsigned char *uiLen)
{
	unsigned char i,num;
	
	unsigned char offset=1;
	
	for(i=0;i<MAX_AID_COUNT;i++)
	{
		if((_AID_LIST->AID_LIST[i].m_AID[0]>4)&&(_AID_LIST->AID_LIST[i].m_AID[0]<17))//长度
		{
			memcpy(pucOut+offset,_AID_LIST->AID_LIST[i].m_AID,_AID_LIST->AID_LIST[i].m_AID[0]+1);
			offset+=_AID_LIST->AID_LIST[i].m_AID[0]+1;
			num++;
		}
	}
	pucOut[0]=num;
	*uiLen=offset;
}

/******************************************/
////////////////RID INTERFACE////////////////////
/******************************************/

CAPK_LIST_STRUCT *_CAPK_LIST=(CAPK_LIST_STRUCT *)FLASH_CAPK_ADDR;
unsigned char CAPK_COUNT;

unsigned char CAPK_GetCount(void)
{
	unsigned char i;
	CAPK_COUNT=0;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(_CAPK_LIST->CAPK_LIST[i].m_RID[0]!=0xff)
		{
			CAPK_COUNT++;
		}
	}
	return CAPK_COUNT;
}
void	CAPK_DeleteALL(void)
{
	unsigned char i;
	unsigned char buf[5]={0xff};
	unsigned int addr;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		addr=(unsigned int)&(_CAPK_LIST->CAPK_LIST[i]);
		IROM_Write(addr,buf,5);		
	}
}
void	CAPK_DeleteRID(unsigned char ucRID[5])
{
	unsigned char i;
	unsigned char buf[5]={0xff};
	unsigned int addr;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(!memcmp(ucRID,_CAPK_LIST->CAPK_LIST[i].m_RID,5))
		{
			addr=(unsigned int)&(_CAPK_LIST->CAPK_LIST[i]);
			IROM_Write(addr,buf,5);
		}
		
	}
}

void	CAPK_DeleteRIDandPKI(unsigned char ucRID[5],unsigned char pki)
{
	unsigned char i;
	unsigned char buf[5]={0xff};
	unsigned int addr;
	unsigned char rid[6];
	memcpy(rid,ucRID,5);
	rid[5]=pki;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(!memcmp(rid,_CAPK_LIST->CAPK_LIST[i].m_RID,6))
		{
			addr=(unsigned int)&(_CAPK_LIST->CAPK_LIST[i]);
			IROM_Write(addr,buf,5);
		}
		
	}
}
unsigned char CAPK_Check(unsigned char uchRID[5],unsigned char uchpki)
{
	unsigned char i;
	unsigned char rid[6];
	memcpy(rid,uchRID,5);
	rid[5]=uchpki;
	
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(_CAPK_LIST->CAPK_LIST[i].m_RID[0]!=0xff)
		{
			if(!memcmp(_CAPK_LIST->CAPK_LIST[i].m_RID,rid,6))
			{
				return i;
			}
		}
	}
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(_CAPK_LIST->CAPK_LIST[i].m_RID[0]==0xff)
		{
			return i;
		}
	}
	return 0xff;
}

unsigned char CAPK_ADD(unsigned char *pucIn,unsigned int uiLen)
{
	CAPK capk;
	unsigned char num;
	unsigned int addr;
	unsigned char *p;
	p=SearchTag(pucIn,uiLen,0x9f06);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(capk.m_RID,p+3,5);
	}
	p=SearchTag(pucIn,uiLen,0x9f22);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		capk.m_CapkIndex=*(p+3);
	}
	p=SearchTag(pucIn,uiLen,0xdf05);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(capk.m_Validtime,p+3,4);
	}
	p=SearchTag(pucIn,uiLen,0xdf06);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		capk.m_Hashalgorithm=*(p+3);
	}
	p=SearchTag(pucIn,uiLen,0xdf07);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		capk.m_Publicflag=*(p+3);
	}
	p=SearchTag(pucIn,uiLen,0xdf02);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(capk.m_Modules,p+2,*(p+2)+1);
	}
	p=SearchTag(pucIn,uiLen,0xdf04);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(capk.m_Exponents,p+2,*(p+2)+1);
	}
	p=SearchTag(pucIn,uiLen,0xdf03);
	if(p==NULL)
	{
		return 1;
	}
	else
	{
		memcpy(capk.m_Checksum,p+2,*(p+2)+1);
	}

	num=CAPK_Check(capk.m_RID,capk.m_CapkIndex);
	if(num==0xff)
	{
		return 1;
	}
	addr=(unsigned int)&(_CAPK_LIST->CAPK_LIST[num]);
	IROM_Write(addr,(unsigned char*)&capk,sizeof(capk));
	return 0;
	
}

CAPK * CAPK_GetOneKey(unsigned char ucRID[5],unsigned char pki)
{
	unsigned char i;
	unsigned char rid[6];

	CAPK *pCAPK;
	
	memcpy(rid,ucRID,5);
	rid[5]=pki;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(_CAPK_LIST->CAPK_LIST[i].m_RID[0]!=0xff)
		{
			if(!memcmp(_CAPK_LIST->CAPK_LIST[i].m_RID,rid,6))
			{
				break;
			}
		}
		if(i==MAX_CAPK_COUNT-1)
			return 0;
	}
	pCAPK=&(_CAPK_LIST->CAPK_LIST[i]);
	return pCAPK;
}

void CAPK_GetInfo(unsigned char *pucOut,unsigned int *uiLen)
{
	unsigned char i,num;
	num=0;
	for(i=0;i<MAX_CAPK_COUNT;i++)
	{
		if(_CAPK_LIST->CAPK_LIST[i].m_RID[0]!=0xff)
		{
			memcpy(pucOut+1+num*6,_CAPK_LIST->CAPK_LIST[i].m_RID,6);
			num++;
		}
	}
	pucOut[0]=num;
	*uiLen=num*6+1;
}
