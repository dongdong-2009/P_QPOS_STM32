//***************************************************************************************
//*  File Name		Roc_LittleTool.h
//*  Author     	Roc       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#ifndef __ROC_LITTLE_TOOL_H__
#define __ROC_LITTLE_TOOL_H__

unsigned int uiCalcCrc16(unsigned char *pbMsg, unsigned int uiDataLen);

int iHexToASCII(unsigned char * pucInData, unsigned char * pucLen, unsigned char *pucOutData);

int iHexToASCIIEx(unsigned char * pucInData, unsigned char ucLen, unsigned char *pucOutData);

int iASCIIToBCD(unsigned char * pucInData, unsigned char * pucLen, unsigned char *pucOutData);

int i8ByteXOR(unsigned char * pucData, unsigned int uiLen, unsigned char *pucDefault);

int i1ByteXOR(unsigned char *pucData, unsigned int uiLen, unsigned char * pucDefault);

int iDataFillingWith(unsigned char * pucData, unsigned int * puiLen, unsigned char ucDefault);

int iDataFilling(unsigned char * pucData, unsigned int * puiLen);

int iFillPinBlock(unsigned char *pucPinData, unsigned char pucPinDataLen, unsigned char *pucCardNoData, unsigned char pucCardNoDataLen, unsigned char * pucOutData);

int iAmountAddPoint(unsigned char * pucInData, unsigned char * pucDataLen, unsigned char * pucOutData);									

int iAmountASCIITo6ByteBCD(unsigned char * pucInData, unsigned char ucDataLen, unsigned char * pucOutData);

int iFillBitMap(unsigned char ucFieldValue, unsigned char * pucBitMap);

int iHexToBCD(unsigned char * pucInData, unsigned char  ucLen, unsigned char * pucOutData);

int iFindTagData(unsigned char * pucInData, unsigned char ucInDataLen, unsigned char * pucTag, unsigned char ucTagLen, unsigned char * pucOutData, unsigned char ucOutDataLen);

int iCompareTimestamp(unsigned char *pucLast, unsigned char * pucNew);

int iCalXOR(unsigned char * pucBuf, unsigned int uiLen);

int iEncryptMACData9_1_9(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey);

int iUnionPay_Ansi919Mac(unsigned char *puszKey, unsigned char *puszMsg, int iMsgLen, unsigned char *puszMac);

int iEncryptMACData9_9(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey);

int iEncryptMACData(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey);

int iDiversify3Des(unsigned char *pucKey, unsigned char *pucDivData, int iCount, unsigned char *pucDivKey);

int iMPOS_EncryptMACData(unsigned char *pucInData, unsigned int uiInDataLen, unsigned char *pucOutData, unsigned char *pucMACKey);

int iTradeAmountIsNull(unsigned char * pucInData);

#endif
//*****************************end of file***********************************************
