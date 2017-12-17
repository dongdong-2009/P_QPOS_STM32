//***************************************************************************************
//*  File Name		Roc_LittleTool.c
//*  Author     	Roc       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#include "Roc_LittleTool.h"
#include "string.h"
#include "Sercrity.h"

const unsigned char auchCRCHi[] = 
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};

//µÍÎ»×Ö½Ú±í
const unsigned char auchCRCLo[] = 
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned int uiCalcCrc16(unsigned char *pbMsg, unsigned int uiDataLen)  
{
    unsigned char bCRCHi = 0xFF ; /* ??????*/
    unsigned char bCRCLo = 0xFF; /* ??????*/
    unsigned char bIndex ;		 /*?CRC?*/
	unsigned char i = 0;
    while(uiDataLen--) /*???????*/ 
    {
        bIndex	= bCRCHi ^ pbMsg[i]; /*??CRC */
	    i++;
        bCRCHi	= bCRCLo ^ auchCRCHi[bIndex] ;
        bCRCLo	= auchCRCLo[bIndex] ;
    }
	return bCRCHi<<8|bCRCLo;
}
//***************************************************************************************
//* Function Name  : XOR8Bytes
//* Description    : 
//*       
//* Input          : pucDefault: data input Default, data output final value
//* Output         : None
//* Return         : None
//***************************************************************************************
int iHexToASCII(unsigned char * pucInData, unsigned char * pucLen, unsigned char *pucOutData)
{
	unsigned char ucbTemp[256];
	unsigned char ucLoop;
	
	if((pucInData == NULL) || (pucLen == NULL) || (pucOutData == NULL) || (*pucLen > 128))
	{
		*pucLen = 0;
		return (0x70);
	}
	else{};
		
	ucLoop = 0;
	while(ucLoop < (*pucLen))
	{
		ucbTemp[ucLoop * 2] = ((pucInData[ucLoop] & 0xF0) >> 4);
		ucbTemp[ucLoop * 2] |= 0x30;		
		ucbTemp[ucLoop * 2 + 1] = (pucInData[ucLoop] & 0x0F);
		ucbTemp[ucLoop * 2 + 1] |= 0x30;
		ucLoop++;
	}
	(*pucLen) *= 2;
	memcpy(pucOutData, ucbTemp, (*pucLen));	
	
	return (0);
}
//***************************************************************************************
//* Function Name  : XOR8Bytes
//* Description    : 
//*       
//* Input          : pucDefault: data input Default, data output final value
//* Output         : None
//* Return         : None
//***************************************************************************************
int iHexToASCIIEx(unsigned char * pucInData, unsigned char ucLen, unsigned char *pucOutData)
{
	unsigned char ucbTemp[512];
	unsigned char ucLoop;
	
	if((pucInData == NULL) || (pucOutData == NULL))
	{
		return (0x70);
	}
	else{};
		
	ucLoop = 0;
	while(ucLoop < ucLen)
	{
		ucbTemp[ucLoop * 2] = ((pucInData[ucLoop] & 0xF0) >> 4);
		if((ucbTemp[ucLoop * 2] <= 9))
		{	
			ucbTemp[ucLoop * 2] += 0x30;		
		}
		else if((10 <= ucbTemp[ucLoop * 2]) && (ucbTemp[ucLoop * 2] <= 15))
		{
			ucbTemp[ucLoop * 2] += 0x37;
		}
		else{}
		
		ucbTemp[ucLoop * 2 + 1] = (pucInData[ucLoop] & 0x0F);
		if((ucbTemp[ucLoop * 2 + 1] <= 9))
		{	
			ucbTemp[ucLoop * 2 +1] += 0x30;		
		}
		else if((10 <= ucbTemp[ucLoop * 2 + 1]) && (ucbTemp[ucLoop * 2 + 1] <= 15))
		{
			ucbTemp[ucLoop * 2 + 1] += 0x37;
		}
		else{}
		ucLoop++;
	}
	memcpy(pucOutData, ucbTemp, ucLen * 2);	
	
	return (0);
}
//***************************************************************************************
//* Function Name  : XOR8Bytes
//* Description    : 
//*       
//* Input          : pucDefault: data input Default, data output final value
//* Output         : None
//* Return         : None
//***************************************************************************************
int iASCIIToBCD(unsigned char * pucInData, unsigned char * pucLen, unsigned char *pucOutData)
{
	unsigned char ucbTemp[256];
	unsigned char ucData;
	unsigned char ucLoop;
	
	if((pucInData == NULL) || (pucLen == NULL) || (pucOutData == NULL))
	{
		*pucLen = 0;
		return (0x70);
	}
	else{};
		
	ucLoop = 0;
	while(ucLoop < ((*pucLen) / 2))
	{
		ucData = pucInData[ucLoop * 2];
		if(('0' <= ucData) && (ucData <= '9'))
		{
			ucbTemp[ucLoop] = (ucData - '0') << 4;
		}
		else if(('A' <= ucData) && (ucData <= 'F'))
		{
			ucbTemp[ucLoop] = (ucData - 0x37) << 4;
		}
		else if(('a' <= ucData) && (ucData <= 'f'))
		{
			ucbTemp[ucLoop] = (ucData - 0x57) << 4;
		}
		else
		{
			*pucLen = 0;
			return (0x71);
		}
			
		ucData = pucInData[ucLoop * 2 + 1];
		if(('0' <= ucData) && (ucData <= '9'))
		{
			ucbTemp[ucLoop] += (ucData - '0') & 0x0f;
		}
		else if(('A' <= ucData) && (ucData <= 'F'))
		{
			ucbTemp[ucLoop] += (ucData - 0x37) & 0x0f;
		}
		else if(('a' <= ucData) && (ucData <= 'f'))
		{
			ucbTemp[ucLoop] += (ucData - 0x57) & 0x0f;
		}
		else
		{
			*pucLen = 0;
			return (0x72);
		}
		ucLoop++;
	}
	
	(*pucLen) /= 2;
	
	memcpy(pucOutData, ucbTemp, (*pucLen));	
	
	return (0);
}
//***************************************************************************************
//* Function Name  : XOR8Bytes
//* Description    : 
//*       
//* Input          : pucDefault: data input Default, data output final value
//* Output         : None
//* Return         : None
//***************************************************************************************
int i8ByteXOR(unsigned char * pucData, unsigned int uiLen, unsigned char *pucDefault)
{
	unsigned char ucbTemp[8];
	unsigned int uiLoop;
	
	if((pucDefault == NULL) || (pucData == NULL) || (uiLen % 8))
	{
		return (0x70);
	}
	else{};
	
	uiLoop = 0;
	memcpy(ucbTemp, pucDefault, 8);
	while(uiLoop < uiLen)
	{
		ucbTemp[0] ^= pucData[uiLoop];
		ucbTemp[1] ^= pucData[uiLoop + 1];
		ucbTemp[2] ^= pucData[uiLoop + 2];
		ucbTemp[3] ^= pucData[uiLoop + 3];
		ucbTemp[4] ^= pucData[uiLoop + 4];
		ucbTemp[5] ^= pucData[uiLoop + 5];
		ucbTemp[6] ^= pucData[uiLoop + 6];
		ucbTemp[7] ^= pucData[uiLoop + 7];
		uiLoop += 8;
	}
	
	memcpy(pucDefault, ucbTemp, 8);
	
	return (0);
}
//***************************************************************************************
//* Function Name  : ucNewCMD_0288_PBOC_MSCard
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : CRC
//***************************************************************************************
int i1ByteXOR(unsigned char *pucData, unsigned int uiLen, unsigned char * pucDefault)
{
	unsigned int uiLoop;
	unsigned char ucCRC=0;
	
	if(pucData == NULL)
	{
		return (0x70);
	}
	else{}
		
	for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
	{
			ucCRC ^= pucData[uiLoop];
	}
	
	*pucDefault = ucCRC;
	
	return (0);
}
//***************************************************************************************
//* Function Name  : BCDCompression
//* Description    : 
//*       
//* Input          : None
//* Output         : None
//* Return         : None
//***************************************************************************************
int iDataFillingWith(unsigned char * pucData, unsigned int * puiLen, unsigned char ucDefault)
{
	unsigned int uiLen;
	if((pucData == NULL) || (puiLen == NULL))
	{
		return (0x70);
	}
	else{}
		
	uiLen = *puiLen;	
		
	while(uiLen % 8)
	{
		pucData[uiLen] = ucDefault;
		uiLen++;	
	}
	
	*puiLen = uiLen;	
	return (0);
}
//***************************************************************************************
//* Function Name  : BCDCompression
//* Description    : 
//*       
//* Input          : None
//* Output         : None
//* Return         : None
//***************************************************************************************
int iDataFilling(unsigned char * pucData, unsigned int * puiLen)
{
	unsigned int uiLen;
	unsigned char ucDefault;
	if((pucData == NULL) || (puiLen == NULL))
	{
		return (0x70);
	}
	else{}
		
	uiLen = *puiLen;	
	ucDefault = 8 - (uiLen % 8);	
	while(uiLen % 8)
	{
		pucData[uiLen] = ucDefault;
		uiLen++;	
	}
	
	*puiLen = uiLen;	
	return (0);
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iFillPinBlock(unsigned char *pucPinData, unsigned char pucPinDataLen,
									unsigned char *pucCardNoData, unsigned char pucCardNoDataLen, unsigned char * pucOutData)
{
	unsigned char ucbPin[16];
	unsigned char ucbPan[16];
	unsigned char ucbMacValue[8];
	unsigned char ucLen;
	
	if((NULL == pucPinData) || (NULL == pucCardNoData) || (14 < pucPinDataLen) || (13 > pucCardNoDataLen))
	{
		return (0x70);
	}
	else{}
		
	ucbPin[0] = pucPinDataLen;
	memcpy(ucbPin + 1, pucPinData, pucPinDataLen);
	memset(ucbPin + 1 + pucPinDataLen, 'F', 14 - pucPinDataLen);
	ucLen = 14;
	iASCIIToBCD(ucbPin + 1, &ucLen, ucbPin + 1);
	
	memset(ucbPan, '0', 4);
	memcpy(ucbPan + 4, pucCardNoData + pucCardNoDataLen - 13, 12);
	ucLen = 16;	
	iASCIIToBCD(ucbPan, &ucLen, ucbPan);
#if 1	
	memcpy(ucbPin + 8, ucbPan, 8);	
  	memset(ucbMacValue, 0x00, 8);		
	i8ByteXOR(ucbPin, 16, ucbMacValue);	
	memcpy(pucOutData, ucbMacValue, 8);
#else
	memcpy(pucOutData, ucbPin, 8);
#endif
	return (0);
}
//***************************************************************************************
//* Function Name  : ucCMD_0208_ICC_MSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iAmountAddPoint(unsigned char * pucInData, unsigned char * pucDataLen, unsigned char * pucOutData)
{
	unsigned char ucbTemp[12];
	unsigned char ucLen;
	unsigned char ucLoop = 0;
	
	if((pucInData == NULL) || (pucDataLen == NULL) || (pucOutData == NULL))
	{
		return (0x70);
	}
	else{}
	
	ucLen = *pucDataLen;	
	if(6 != ucLen)
	{
		return (0x71);
	}
	else{}

	memcpy(ucbTemp, pucInData, ucLen);

	iHexToASCII(ucbTemp, &ucLen, ucbTemp);

	ucLoop = 0;
	while(ucbTemp[ucLoop] == '0')
	{
		ucLoop++;
		if(ucLoop > 11)
		{
			*pucDataLen = 0;	
			return (1);
		}
		else{}
	}

	ucLen = 12 - ucLoop;


		
	if(ucLen > 2)
	{
		memcpy(pucOutData, ucbTemp + ucLoop, ucLen - 2);
		pucOutData[ucLen - 2] = '.';
		pucOutData[ucLen - 1] = ucbTemp[10];
		pucOutData[ucLen] = ucbTemp[11];
		ucLen += 1;		
	}
	else
	{
		pucOutData[0] = '0';
		pucOutData[1] = '.';
		if(2 == ucLen)
		{
			pucOutData[2] = ucbTemp[10];
			pucOutData[3] = ucbTemp[11];
		}
		else if(1 == ucLen)
		{
			pucOutData[2] = '0';
			pucOutData[3] = ucbTemp[11];
		}
		else
		{}
		ucLen = 4;	
	}
	
	*pucDataLen = ucLen;	
	return (0);
}
//***************************************************************************************
//* Function Name  : vAmountASCIITo6ByteBCD
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iAmountASCIITo6ByteBCD(unsigned char * pucInData, unsigned char ucDataLen, unsigned char * pucOutData)
{
	unsigned char ucbTemp[13];
	unsigned char ucLen;
	unsigned char ucLoop;
	int iStatus;
	
	if((pucInData == NULL) || (pucOutData == NULL) || (ucDataLen > 12))
	{
		return (0x70);
	}
	else{}
	
	memset(ucbTemp, 0x30, 12);
	if(1 == ucDataLen)
	{
		ucDataLen--;
		ucbTemp[11] = pucInData[ucDataLen];
	}
	else if(2 == ucDataLen)
	{
		ucDataLen--;
		ucbTemp[11] = pucInData[ucDataLen];
		ucDataLen--;
		ucbTemp[10] = pucInData[ucDataLen];
	}
	else if(2 < ucDataLen)
	{
		ucLoop = 12;
		while(ucDataLen)
		{
			ucDataLen--;
			ucLoop--;
			if('.' == pucInData[ucDataLen])
			{
				if(0 != ucDataLen)
				{
					ucDataLen--;
				}
				else
				{
					return (0x71);
				}
			}
			else{}
			ucbTemp[ucLoop] = pucInData[ucDataLen];	
		}	
	}
	else
	{
		return (0x72);
	}
	ucLen = 12;
	iStatus = iASCIIToBCD(ucbTemp, &ucLen, pucOutData);
	return iStatus << 8;
}
//***************************************************************************************
//* Function Name  : vAmountASCIITo6ByteBCD
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iFillBitMap(unsigned char ucFieldValue, unsigned char * pucBitMap)
{
	if((pucBitMap == NULL) || (ucFieldValue > 64))
	{
		return (0x70);
	}
	else{}
		
	ucFieldValue--;
	pucBitMap[ucFieldValue / 8] |= (0x80 >> (ucFieldValue % 8)); 	
	return (0);
}
//***************************************************************************************
//* Function Name  : vAmountASCIITo6ByteBCD
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iHexToBCD(unsigned char * pucInData, unsigned char ucLen, unsigned char * pucOutData)
{
	unsigned char ucLoop;
	
	if((NULL == pucInData) || (NULL == pucOutData))
	{
		return (0x70);
	}
	else{}
		
	ucLoop = 0;
	ucLen = ucLen / 2;
	while(ucLoop < ucLen)
	{
		pucOutData[ucLoop] = (pucInData[ucLoop * 2] & 0x0f) << 4;
		pucOutData[ucLoop] |= (pucInData[ucLoop * 2 + 1] & 0x0f);
		ucLoop++;
	}
	return (0);
}
//***************************************************************************************
//* Function Name  : vAmountASCIITo6ByteBCD
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iFindTagData(unsigned char * pucInData, unsigned char ucInDataLen, unsigned char * pucTag, unsigned char ucTagLen, unsigned char * pucOutData, unsigned char ucOutDataLen)
{
	unsigned char ucLoop = 0;
	if((NULL == pucInData) || (NULL == pucTag) || (NULL == pucOutData))
	{
		return (0x70);
	}
	else{}
		
	while(ucLoop < ucInDataLen)
	{
		if(1 == ucTagLen)
		{
			if(pucInData[ucLoop] == pucTag[0])
			{
				ucLoop++;	
				break;
			}
			else{}
		}
		else if(2 == ucTagLen)
		{
			if(pucInData[ucLoop] == pucTag[0])
			{
				if(pucInData[ucLoop + 1] == pucTag[1])
				{
					ucLoop += 2;	
					break;
				}
				else{}
			}
			else{}
		}
		else	
		{
			return (0x71);
		}
		ucLoop++;	
	}
	memcpy(pucOutData, pucInData + ucLoop, ucOutDataLen);
	return (0);
}
//***************************************************************************************
//* Function Name  : iCompareTimestamp
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCompareTimestamp(unsigned char *pucLast, unsigned char * pucNew)
{
	if((NULL == pucLast) || (NULL == pucNew))
	{
		return (0x70);
	}
	else{}
		
	if(pucNew[0] < pucLast[0])
	{
		return (0x70);
	}
	else if(pucNew[0] > pucLast[0])
	{
		return (0);
	}
	else
	{
		if(pucNew[1] < pucLast[1])
		{
			return (0x70);
		}
		else if(pucNew[1] > pucLast[1])
		{
			return (0);
		}
		else
		{
			if(pucNew[2] < pucLast[2])
			{
				return (0x70);
			}
			else if(pucNew[2] > pucLast[2])
			{
				return (0);
			}
			else
			{}
		}
	}
	return (0x71);	
}
//***************************************************************************************
//* Function Name  : iCompareTimestamp
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iCalXOR(unsigned char * pucBuf, unsigned int uiLen)
{
    unsigned char ucXOR = 0;
    unsigned int uiLoop;

    for(uiLoop = 0; uiLoop < uiLen; uiLoop++)
    {
        ucXOR ^= pucBuf[uiLoop];
    }

    if(ucXOR != 0)
	{
        return (1);
	}
    else{}

    return (0);
}
//***************************************************************************************
//* Function Name  : _IS_AssignKey
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptMACData9_1_9(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey)
{
	unsigned int uiStatus;
	unsigned int uiLoop = 0;
	unsigned char ucbIcv[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	if(pucInData == NULL || pucOutData == NULL)
	{
		return (0x70);
	}
	else{}

	while(uiLoop < uiInDataLen)
	{
		i8ByteXOR(pucInData + uiLoop, 8, ucbIcv);
		uiStatus = SingleDesBykey(ucbIcv, 8, pucMACKey, 0x00);
		if(0 != uiStatus)
		{
			return (0x73);
		}
		else{}	
		uiLoop += 8;
	}

	uiStatus = SingleDesBykey(ucbIcv, 8, pucMACKey + 8, 0x10);
	if(0 != uiStatus)
	{
		return (0x74);
	}
	else{};

	uiStatus = SingleDesBykey(ucbIcv, 8, pucMACKey, 0x00);
	if(0 != uiStatus)
	{
		return (0x75);
	}
	else{};
	memcpy(pucOutData, ucbIcv, 8);

	return (0);
}
//***************************************************************************************
//* Function Name  : _IS_AssignKey
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iUnionPay_Ansi919Mac(unsigned char *puszKey, unsigned char *puszMsg, int iMsgLen, unsigned char *puszMac)
{
	int iRet;
	unsigned char uszTmp[8];
	int iFlg = 0;
	int iCount;
	int i;
	
	memset(uszTmp, 0, sizeof(uszTmp));

	iFlg = iMsgLen%8;

	if(iFlg)
	{
		iCount = iMsgLen / 8 + 1;
	}
	else
	{
		iCount = iMsgLen / 8;
	}

	for (i = 0; i < iCount - 1; i++)	
	{
//		MyXor(puszMac, puszMsg+(i<<3), 8);	
		i8ByteXOR(puszMsg+(i<<3), 8, puszMac);
//		Des_Ecb_Encrypt(puszKey, puszMac,8, puszMac);
		iRet = SingleDesBykey(puszMac, 8, puszKey, 0x00);
		if(0 != iRet)
		{
			return (0x73);
		}
		else{}	
	}
	
	if (iFlg == 0)
	{
//		MyXor(puszMac, puszMsg+((iCount-1)<<3), 8);
		i8ByteXOR(puszMsg+((iCount-1)<<3), 8, puszMac);
	}
	else
	{
		memcpy(uszTmp, puszMsg+((iCount-1)<<3), iFlg);
//		MyXor(puszMac, uszTmp, 8);
		i8ByteXOR(uszTmp, 8, puszMac);		
	}	
//	TDes2Key_Ecb_Encrypt(puszKey, puszMac, 8, puszMac);	
	iRet = ThreeDesBykey(puszMac, 8, puszKey, 0x00);
	if(0 != iRet)
	{
		return (0x74);
	}
	else{}
	return 0;
}

//***************************************************************************************
//* Function Name  : _IS_AssignKey
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptMACData9_9(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey)
{
	unsigned int uiStatus;
	unsigned int uiLoop = 0;
	unsigned char ucbIcv[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	if(pucInData == NULL || pucOutData == NULL)
	{
		return (0x70);
	}
	else{}

	while(uiLoop < uiInDataLen)
	{
		i8ByteXOR(pucInData + uiLoop, 8, ucbIcv);
		uiStatus = SingleDesBykey(ucbIcv, 8, pucMACKey, 0x00);
		if(0 != uiStatus)
		{
			return (0x73);
		}
		else{}	
		uiLoop += 8;
	}

	memcpy(pucOutData, ucbIcv, 8);

	return (0);
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iEncryptMACData(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey)
{
	int iRet;
	unsigned char ucbMacValue[16];
	unsigned char ucbTemp[8];
	unsigned char ucLen;
	
	if((NULL == pucInData) || (NULL == pucOutData))
	{
		return (0x70);
	}
	else{}
	
	memset(ucbMacValue, 0x00, 8);
	i8ByteXOR(pucInData, uiInDataLen, ucbMacValue);	
	ucLen = 8;	
	iHexToASCIIEx(ucbMacValue, ucLen, ucbMacValue);
		
	iRet = ThreeDesBykey(ucbMacValue, 8, pucMACKey, 0x00);
//	iRet = SingleDesBykey(ucbMacValue, 8, pucMACKey, 0x00);
	if(0 != iRet)
	{
		return (0x73);
	}
	else{}	
		
	memset(ucbTemp, 0x00, 8);
	i8ByteXOR(ucbMacValue, 16, ucbTemp);		
	
	iRet = ThreeDesBykey(ucbTemp, 8, pucMACKey, 0x00);
//	iRet = SingleDesBykey(ucbMacValue, 8, pucMACKey, 0x00);
	if(0 != iRet)
	{
		return (0x73);
	}
	else{}
		
	ucLen = 8;	
	iHexToASCIIEx(ucbTemp, ucLen, ucbMacValue);	
	memcpy(pucOutData, ucbMacValue, 8);	
	return (0);	
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iDiversify3Des(unsigned char *pucKey, unsigned char *pucDivData, int iCount, unsigned char *pucDivKey)
{
	unsigned char ucbDivBuf8_15[8];
	unsigned char ucKeyBuf[16];
	unsigned char ucbDivBuf0_7[8];
	int iRet;
	int i, k;

	memcpy(ucKeyBuf, pucKey, 16);

	for(k = 0; k < iCount; k++)
	{
		for (i = 0; i < 8; i++)
		{
        	ucbDivBuf8_15[i]= 0xFF - pucDivData[(k<<3) + i];
		}
		memcpy(ucbDivBuf0_7, pucDivData +(k << 3), 8);
//		TDes2Key_Ecb_Encrypt(ucKeyBuf, pucDivData +(k << 3), 8, pucDivKey);
		iRet = ThreeDesBykey(ucbDivBuf0_7, 8, ucKeyBuf, 0x00);
		if(0 != iRet)
		{
			return (1);
		}
		else{}
		memcpy(pucDivKey, ucbDivBuf0_7, 8);
//		TDes2Key_Ecb_Encrypt(ucKeyBuf, ucbDivBuf, 8, pucDivKey + 8);
		iRet = ThreeDesBykey(ucbDivBuf8_15, 8, ucKeyBuf, 0x00);
		if(0 != iRet)
		{
			return (1);
		}
		else{}
		memcpy(pucDivKey + 8, ucbDivBuf8_15, 8);
		memcpy(ucKeyBuf, pucDivKey, 16);
	}
	return (0);
}
//***************************************************************************************
//* Function Name  : ucPayThroughMSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iMPOS_EncryptMACData(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey)
{
	int iRet;
	unsigned char ucbMacValue[16];

	memset(ucbMacValue, 0x00, 16);
	iDataFillingWith(pucInData, &uiInDataLen, 0x00);

   	i8ByteXOR(pucInData, uiInDataLen, ucbMacValue);	
	ucbMacValue[8] = 0x80;
	memset(ucbMacValue + 9, 0x00, 7);
	iRet = iUnionPay_Ansi919Mac(pucMACKey, ucbMacValue, 16, pucOutData);
	if(0 != iRet)
	{
		return (1);
	}
	else{}
	return (0);
}
//***************************************************************************************
//* Function Name  : ucCMD_0208_ICC_MSC
//* Description    : 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
int iTradeAmountIsNull(unsigned char * pucInData)
{
	unsigned char ucbTemp[6];
	int iLoop;

	memset(ucbTemp, 0x00, 6);
	iLoop = 0;
	while(iLoop < 6)
	{
		if(pucInData[iLoop] != ucbTemp[iLoop])
		{
			break;
		}
		else{}
		iLoop++;
		if(iLoop == 6)
		{
			return (0);
		}
		else{}
	}

	memset(ucbTemp, 0xff, 6);
	iLoop = 0;
	while(iLoop < 6)
	{
		if(pucInData[iLoop] != ucbTemp[iLoop])
		{
			break;
		}
		else{}
		iLoop++;
		if(iLoop == 6)
		{
			return (0);
		}
		else{}
	}

	return (1);
}
//*****************************end of file***********************************************
