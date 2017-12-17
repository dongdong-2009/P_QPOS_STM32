//***************************************************************************************
//*  File Name		Z8D256.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#include "Z8D256.h"
#include "sercrity.h"
#include "stm32f10x.h"
#include "Roc_LittleTool.h"

psamAPDU psamapdu;
CRCst crcValue;
OptionSt optionStruct;

unsigned char keykey[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
Z8D256_RECV_STATUS Z8D256_RecvString_XIEYI(unsigned char* buffer)
{
//	unsigned int i = 0;
    unsigned short templen = 0,tempCrc = 0,Crc = 0;
    unsigned char ret = 0;
    ret = iZ8D256_RecvData(buffer,1);
    if(ret == 0)
    {
        if(buffer[0] != 0x55)
        {
            g_eZ8D256_RecvStatus = Z8D256_HEADER_ERROR;
//            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            return g_eZ8D256_RecvStatus;               
        }        
            
    }            
    else
    {
        g_eZ8D256_RecvStatus = Z8D256_NO_DATA;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;           
    }
    if(iZ8D256_RecvData(buffer+1,2) == 1)
    {
        g_eZ8D256_RecvStatus = Z8D256_LENGTH_ERROR;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;          
    }   
    templen = buffer[1];
    templen = (templen << 8) + buffer[2];    
    if(iZ8D256_RecvData(buffer+3,templen) == 1)
    {
        g_eZ8D256_RecvStatus = Z8D256_LENGTH_ERROR;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;         
    }
    if(iZ8D256_RecvData(crcValue.crcbuf,2) == 1)
    {
        g_eZ8D256_RecvStatus = Z8D256_XOR_ERROR;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;          
    }
    LittleToBig_short(crcValue.tempcrc,&tempCrc);
    Crc = uiCalcCrc16(buffer,templen+3);
    if(Crc != tempCrc)
    {
        g_eZ8D256_RecvStatus = Z8D256_XOR_ERROR;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;  
    }
    if(*(buffer+1+templen) != 0x90)
    {
        g_eZ8D256_RecvStatus = Z8D256_XOR_ERROR;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;          
    }
    else
    {
        g_eZ8D256_RecvStatus = Z8D256_SUCEESCE;
//        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        return g_eZ8D256_RecvStatus;    
    }
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//RSA算法的大小端转化
void LitteToBigStr(unsigned char buffer[],unsigned short len)
{
    unsigned char temp,i = 0;
    for(i = 0;i < len/2;i++)
    {
        temp = buffer[i];
        buffer[i] = buffer[len-1-i];
        buffer[len - 1 - i] = temp;
    }    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void LittleToBig_short(const unsigned short in,unsigned short *pout)
{
	unsigned short out = 0;
    out =((in >> 8) & 0xFF ); 
	out |=((in >> 0) & 0xFF ) << 8; 
	*pout = out ;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static void psamSendCommand(void)
{
    unsigned short templen = 0,tempCrc = 0;
    LittleToBig_short(psamapdu.psamcombuf.Psamcommand.dataLen.length,&templen);
    tempCrc = uiCalcCrc16(psamapdu.psamcombuf.psamcommandbuff+1,templen+3);
    LittleToBig_short(tempCrc,&crcValue.tempcrc);
    vZ8D256_SendData(psamapdu.psamcombuf.psamcommandbuff+1,0x03);
    vZ8D256_SendData(psamapdu.psamcombuf.psamcommandbuff+4,templen);
    vZ8D256_SendData(crcValue.crcbuf,2);  
    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
static unsigned char psamRecvAnswer(void)
{
    unsigned int loop = 0x80000000;
    while(g_eZ8D256_RecvStatus == Z8D256_NO_DATA)
    {
		g_eZ8D256_RecvStatus = Z8D256_RecvString_XIEYI(psamapdu.psamansBuf.psamanswerbuff+1);
		if(g_eZ8D256_RecvStatus == Z8D256_SUCEESCE)  
	    {
	        g_eZ8D256_RecvStatus = Z8D256_NO_DATA;
	        return 0;
	    }
		else{}
        loop--;
        if(loop == 0)
            return 0xFF;         
    } 
       
	g_eZ8D256_RecvStatus = Z8D256_NO_DATA;
	return 1;             
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char readfromFlash(unsigned int addr,unsigned short len)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xa0;
    psamapdu.psamcombuf.Psamcommand.pp1= addr >> 8;
    psamapdu.psamcombuf.Psamcommand.pp2= addr & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= len; 
//    for(i = 0; i< datalen;i++)
//    {
//         psamapdu.psamcombuf.Psamcommand.databuf[i] = 
//    }
    psamSendCommand();
    return psamRecvAnswer(); 
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char WriteIntoFlash(unsigned int addr,unsigned char buffer[],unsigned short len)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = (len+5)>>8;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = (len+5)& 0xFF;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xa1;
    psamapdu.psamcombuf.Psamcommand.pp1= addr >> 8;
    psamapdu.psamcombuf.Psamcommand.pp2= addr & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= len; 
    for(i = 0; i< len;i++)
    {
         psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i];
    }
    psamSendCommand();
    return psamRecvAnswer(); 
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char EraseMF(void)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x07;
    psamapdu.psamcombuf.Psamcommand.cla = 0xBF;
    psamapdu.psamcombuf.Psamcommand.ins = 0xCE;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x02; 
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x3F;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x00;
    psamSendCommand();
    return psamRecvAnswer();   
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//55 00 17 80 E0 00 00 12 3F 00 0F 0F 00 00 00 00 00 00 00 FF 12 34 56 78 91 23 E8 8A
unsigned char CreateMF(void)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x17;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xE0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x12; 
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x3F;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[2] = 0x0F;
    psamapdu.psamcombuf.Psamcommand.databuf[3] = 0x0F;
    psamapdu.psamcombuf.Psamcommand.databuf[4] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[5] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[6] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[7] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[8] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[9] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[10] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[11] = 0xFF;
    psamapdu.psamcombuf.Psamcommand.databuf[12] = 0x12;
    psamapdu.psamcombuf.Psamcommand.databuf[13] = 0x34;
    psamapdu.psamcombuf.Psamcommand.databuf[14] = 0x56;
    psamapdu.psamcombuf.Psamcommand.databuf[15] = 0x78;
    psamapdu.psamcombuf.Psamcommand.databuf[16] = 0x91;
    psamapdu.psamcombuf.Psamcommand.databuf[17] = 0x23;
    psamSendCommand();
    return psamRecvAnswer();     
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char createKeyFile(unsigned int fileSize,unsigned char reloadAt,unsigned char reloadAc,unsigned char reloadKeyID)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x0E;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xE0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x0B;
    psamapdu.psamcombuf.Psamcommand.lc= 0x09; 
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x25 ;//
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[2] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[3] = fileSize >> 8;
    psamapdu.psamcombuf.Psamcommand.databuf[4] = fileSize & 0xFF;
    psamapdu.psamcombuf.Psamcommand.databuf[5] = 0x00;//保留
    psamapdu.psamcombuf.Psamcommand.databuf[6] = 0x00;//重载属性
    psamapdu.psamcombuf.Psamcommand.databuf[7] = 0x00;//重载权限
    psamapdu.psamcombuf.Psamcommand.databuf[8] = 0x00;//重载密钥标识
    psamSendCommand();
    return psamRecvAnswer();     
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//装载主控密钥
//55 00 15 80 D4 01 00 10 00 01 00 04 0F 05 00 FF 11 22 33 44 55 66 77 88 CD D3
unsigned char loadMainkey(void)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x15;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xD4;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x01;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x10; 
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x00 ;//密钥索引
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x01;//密钥类型
    psamapdu.psamcombuf.Psamcommand.databuf[2] = 0x00;//密钥版本
    psamapdu.psamcombuf.Psamcommand.databuf[3] = 0x04;//密钥算法标识;0x00 3DES 0x04 DES
    psamapdu.psamcombuf.Psamcommand.databuf[4] = 0x0F;//使用权：
    psamapdu.psamcombuf.Psamcommand.databuf[5] = 0x05;//密钥的后续状态
    psamapdu.psamcombuf.Psamcommand.databuf[6] = 0x00;//密钥的修改权限
    psamapdu.psamcombuf.Psamcommand.databuf[7] = 0xFF;//重试次数
    //密钥值CD A8 B8 B6 D6 A7 BE A9
    psamapdu.psamcombuf.Psamcommand.databuf[8] = 0xCD;
    psamapdu.psamcombuf.Psamcommand.databuf[9] = 0xA8;
    psamapdu.psamcombuf.Psamcommand.databuf[10] = 0xB8;
    psamapdu.psamcombuf.Psamcommand.databuf[11] = 0xB6;
    psamapdu.psamcombuf.Psamcommand.databuf[12] = 0xD6;
    psamapdu.psamcombuf.Psamcommand.databuf[13] = 0xA7;
    psamapdu.psamcombuf.Psamcommand.databuf[14] = 0xBE;
    psamapdu.psamcombuf.Psamcommand.databuf[15] = 0xA9;    
    psamSendCommand();
    return psamRecvAnswer();      
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//装载应用密钥
unsigned char loadDESkey(keyUion *pkeyui)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    if((*pkeyui).keyStruct.kArigth== 0x04)
    {
        psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 8+13;
        psamapdu.psamcombuf.Psamcommand.lc= 8+8;
        
    }        
    if((*pkeyui).keyStruct.kArigth == 0x05)
    {
        psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 16+13;
        psamapdu.psamcombuf.Psamcommand.lc= 16+8;
    }
        
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xD4;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= (*pkeyui).keyStruct.kid;
    
    for(i = 0;i< psamapdu.psamcombuf.Psamcommand.lc;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = (*pkeyui).buffer[i];
    }
    psamSendCommand();
    return psamRecvAnswer();  
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//装载应用密钥
unsigned char changeDESkey(unsigned char buffer[],unsigned char KID,unsigned char len)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;     
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = len + 5;     
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xD4;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x01;
    psamapdu.psamcombuf.Psamcommand.pp2= KID;
    psamapdu.psamcombuf.Psamcommand.lc = len;
    for(i = 0;i< psamapdu.psamcombuf.Psamcommand.lc;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i];
    }
    psamSendCommand();
    return psamRecvAnswer();  
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//创建二进制文件存放公钥和私钥文件 注意私钥文件的可读权限为0xF0
unsigned char createBinaryFile(BinaryUion *pbui)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x13;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xE0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x03;
    psamapdu.psamcombuf.Psamcommand.lc= 0x0E; 
    

    for(i = 0;i < 0x0E;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = (*pbui).buffer[i];
    }
    psamSendCommand();
    return psamRecvAnswer();     
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
// 设置BankID+UID
//UID:     		20???
//BankID:				8???
unsigned char setDeviceInfo(unsigned char*pbuff)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x21;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xDA;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x01;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x1C; 
    
    for(i = 0;i < psamapdu.psamcombuf.Psamcommand.lc;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = pbuff[i];
    }
    psamSendCommand();
    return psamRecvAnswer();     
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//设置PSAMID
unsigned char setPSAMInfo(unsigned char*pbuff)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x0D;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xDA;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x02;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x08; 
    

    for(i = 0;i < psamapdu.psamcombuf.Psamcommand.lc;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = pbuff[i];
    }
    psamSendCommand();
    return psamRecvAnswer();     
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//获取bankID和UID。
//mode 0:bankid 1:UID
unsigned char getDeviceInfo(unsigned char*pbuff,unsigned char mode)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x21;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xCA;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x01;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x1C; 
    
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    if(mode == 0)
    {
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[0] = 0x00;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[1] = 0x08;
        for(i = 0;i < 0x8;i++)
        {
            psamapdu.psamansBuf.Psamanswer.dataBuf[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];            
            pbuff[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
        } 
        psamapdu.psamansBuf.Psamanswer.dataBuf[0x08] = 0x90;
        psamapdu.psamansBuf.Psamanswer.dataBuf[0x09] = 0x00;       
    }   
    if(mode == 1)
    {
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[0] = 0x00;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[1] = 0x14;
        for(i = 0;i < 0x14;i++)
        {
            psamapdu.psamansBuf.Psamanswer.dataBuf[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[8+i];            
            pbuff[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
        } 
        psamapdu.psamansBuf.Psamanswer.dataBuf[0x14] = 0x90;
        psamapdu.psamansBuf.Psamanswer.dataBuf[0x15] = 0x00;         
    }
    return 0;      
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//获取PSAMID
unsigned char getPSAMInfo(unsigned char*pbuff)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xCA;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x02;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x08; 
    
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0;i < 0x08;i++)
    {
        pbuff[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }

    return 0;      
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char setApplykey(unsigned char *pbuffer,unsigned char kid)
{
    unsigned char i = 0;
    optionStruct.keyui.keyStruct.kid = kid;//各个密钥的索引的低字节
    optionStruct.keyui.keyStruct.ktype = 0x01;
    optionStruct.keyui.keyStruct.kversion = 0x00;
    optionStruct.keyui.keyStruct.kArigth = 0x05;
    optionStruct.keyui.keyStruct.kuseAc = 0x0F;
    optionStruct.keyui.keyStruct.kstate = 0x00;
    optionStruct.keyui.keyStruct.kchangeAc = 0x00;
    optionStruct.keyui.keyStruct.times = 0xFF;
    for(i = 0; i < 16; i++)
    {
          optionStruct.keyui.keyStruct.keybuf[i] = pbuffer[i];
    }    
    return loadDESkey(&optionStruct.keyui);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char chanageApplykey(unsigned char *pbuffer,unsigned char kid,unsigned char len)
{   
    return changeDESkey(pbuffer,kid,len);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char setTrackkey(unsigned char *pbuffer,unsigned char kid)
{
    unsigned char i = 0;
    optionStruct.keyui.keyStruct.kid = kid;//各个密钥的索引的低字节
    optionStruct.keyui.keyStruct.ktype = 0x01;
    optionStruct.keyui.keyStruct.kversion = 0x00;
    optionStruct.keyui.keyStruct.kArigth = 0x04;
    optionStruct.keyui.keyStruct.kuseAc = 0x0F;
    optionStruct.keyui.keyStruct.kstate = 0x00;
    optionStruct.keyui.keyStruct.kchangeAc = 0x00;
    optionStruct.keyui.keyStruct.times = 0xFF;
    for(i = 0; i < 8; i++)
    {
          optionStruct.keyui.keyStruct.keybuf[i] = pbuffer[i];
    }    
    return loadDESkey(&optionStruct.keyui);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//55 00 07 00 A4 02 00 02 00 01 22 54
unsigned char seleteBinaryFile(unsigned int fileID)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x07;
    psamapdu.psamcombuf.Psamcommand.cla = 0x00;
    psamapdu.psamcombuf.Psamcommand.ins = 0xA4;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x02;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x02;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = fileID >> 8;    
    psamapdu.psamcombuf.Psamcommand.databuf[1] = fileID & 0xFF;
    psamSendCommand();
    return psamRecvAnswer();  
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char updateBinaryFile(unsigned short addr,unsigned char *pbuffer,unsigned char len)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = (len+5)>> 8;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = (len+5)&0xFF;
    psamapdu.psamcombuf.Psamcommand.cla = 0x00;
    psamapdu.psamcombuf.Psamcommand.ins = 0xD6;
    psamapdu.psamcombuf.Psamcommand.pp1= addr >> 8;
    psamapdu.psamcombuf.Psamcommand.pp2= addr & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= len;
    for(i = 0; i < len;i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = pbuffer[i]; 
    }
    psamSendCommand();
    return psamRecvAnswer(); 
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char ReadBinaryFile(unsigned short addr,unsigned char *pbuffer,unsigned char len)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x00;
    psamapdu.psamcombuf.Psamcommand.ins = 0xB0;
    psamapdu.psamcombuf.Psamcommand.pp1= addr >> 8;
    psamapdu.psamcombuf.Psamcommand.pp2= addr & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= len;
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0;i < len;i++)
    {
        pbuffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    return 0;   
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char createPublicFile(unsigned short publicFileID)
{
    optionStruct.binaryui.BinaryStruct.btype = 0x21;//文件索引
    optionStruct.binaryui.BinaryStruct.bFidh = publicFileID >> 8;//文件fidh
    optionStruct.binaryui.BinaryStruct.bFidl = publicFileID & 0xFF;//文件fidl
    optionStruct.binaryui.BinaryStruct.bFsizeh = 0x02;//文件大小高
    optionStruct.binaryui.BinaryStruct.bFsizel = 0x13;//文件大小低
    optionStruct.binaryui.BinaryStruct.bReadAtr = 0x00;//读属性
    optionStruct.binaryui.BinaryStruct.bWriteAtr = 0x00;//写属性
    optionStruct.binaryui.BinaryStruct.bReadAc = 0x00;//读权限
    optionStruct.binaryui.BinaryStruct.bWriteAc= 0x00;//写权限
    optionStruct.binaryui.BinaryStruct.bUseAc = 0x00;//使用权限
    optionStruct.binaryui.BinaryStruct.bReadKid = 0x00;//读密钥标识
    optionStruct.binaryui.BinaryStruct.bWriteKid = 0x00;//写密钥标识
    optionStruct.binaryui.BinaryStruct.bUseKid = 0x00;//使用密钥标识
    return createBinaryFile(&optionStruct.binaryui);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char createPrivateFile(unsigned short privateFileID)
{
    
    optionStruct.binaryui.BinaryStruct.btype = 0x21;//文件索引
    optionStruct.binaryui.BinaryStruct.bFidh = privateFileID >> 8;//文件fidh
    optionStruct.binaryui.BinaryStruct.bFidl = privateFileID & 0xFF;//文件fidl
    optionStruct.binaryui.BinaryStruct.bFsizeh = 0x02;//文件大小高
    optionStruct.binaryui.BinaryStruct.bFsizel = 0x13;//文件大小低
    optionStruct.binaryui.BinaryStruct.bReadAtr = 0x00;//读属性
    optionStruct.binaryui.BinaryStruct.bWriteAtr = 0x00;//写属性
    optionStruct.binaryui.BinaryStruct.bReadAc = 0xF0;//读权限
    optionStruct.binaryui.BinaryStruct.bWriteAc= 0x00;//写权限
    optionStruct.binaryui.BinaryStruct.bUseAc = 0x00;//使用权限
    optionStruct.binaryui.BinaryStruct.bReadKid = 0x00;//读密钥标识
    optionStruct.binaryui.BinaryStruct.bWriteKid = 0x00;//写密钥标识
    optionStruct.binaryui.BinaryStruct.bUseKid = 0x00;//使用密钥标识
    return createBinaryFile(&optionStruct.binaryui);  
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//55 00 05 00 B0 00 00 0A AE 55
unsigned char ReadPublicFile(unsigned short publicFileID)
{
    unsigned char i = 0;
    i = seleteBinaryFile(publicFileID);
    if(i != 0)
        return i;
    i = ReadBinaryFile(0x0003,psamapdu.publickbuf.publicstruct.restorebuff,4);
    if(i != 0)
        return i;
    LitteToBigStr(psamapdu.publickbuf.publicstruct.restorebuff,4);
    i = ReadBinaryFile(0x0009,psamapdu.publickbuf.publicstruct.restorebuff+4,0x80);
    if(i != 0)
        return i;
    LitteToBigStr(psamapdu.publickbuf.publicstruct.restorebuff+4,0x80);
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char getPSAMUID(unsigned char *pbuffer,unsigned char len)
{
    unsigned char i = 0;
    i =  getDeviceInfo(pbuffer,1);
    if(i != 0)
        return i;  
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//先将n e 保存到psamapdu.publickbuf.publicstruct.restorebuff，然后在调用该函数。
unsigned char updatePublickeyFile(unsigned short publicFileID)
{
    unsigned char i = 0;
    unsigned char buffer[2] = {0};
    i = seleteBinaryFile(publicFileID);
    if(i != 0)
        return i;  
    buffer[0] = 0xC2;
    i = updateBinaryFile(0x0000,buffer,1);
    if(i != 0)
        return i;  
    buffer[0] = 0x91;
    buffer[1] = 0x04;
    i = updateBinaryFile(0x0001,buffer,2);
    if(i != 0)
        return i; 
    LitteToBigStr(psamapdu.publickbuf.publicstruct.restorebuff,4);
    i = updateBinaryFile(0x0003,psamapdu.publickbuf.publicstruct.restorebuff,4);
    if(i != 0)
        return i;   
    buffer[0] = 0x92;
    buffer[1] = 0x80;
    i = updateBinaryFile(0x0007,buffer,2);
    if(i != 0)
        return i;   
    LitteToBigStr(psamapdu.publickbuf.publicstruct.restorebuff+4,0x80);
    i = updateBinaryFile(0x0009,psamapdu.publickbuf.publicstruct.restorebuff+4,0x80);
    if(i != 0)
        return i;   
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//生成ＲＳＡ密钥，公钥保存到文件0x0001中，私钥保存到0x0002中
unsigned char genaryRsaKey(unsigned short publicFileID,unsigned short privateFileID)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x09;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xD0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x04;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = publicFileID >>8;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = publicFileID & 0xFF;
    psamapdu.psamcombuf.Psamcommand.databuf[2] = privateFileID >>8;
    psamapdu.psamcombuf.Psamcommand.databuf[3] = privateFileID & 0xFF;    
    psamSendCommand();
    return psamRecvAnswer();      
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//取随机数
unsigned char GetChallenge(unsigned char buffer[],unsigned char len)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x00;
    psamapdu.psamcombuf.Psamcommand.ins = 0x84;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= len;  
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;  
    for(i = 0; i < len;i++)
        buffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//DES加解密
//加密后的数据保存到srcbuffer[]中。
//mode : 0x00 :ECB encrty 0x10 :ECB disEncrty
//       0x01 :CBC encrty 0x11 :CBC disEncrty
//55 00 17 80 C6 04 A0 12 00 04 11 22 33 44 55 66 77 88 5a 5a 5a 5a 5a 5a 5a 5a F0 A3
unsigned char SingleDesBykey(unsigned char srcbuffer[],unsigned char len,unsigned char *pkey,unsigned char mode)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = len + 0x0F;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xC6;
    if(mode == 0x00)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x04;
    if(mode == 0x10)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x44;
    if(mode == 0x01)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x14;
    if(mode == 0x11)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x54;    
    psamapdu.psamcombuf.Psamcommand.pp2= 0xA0;
    psamapdu.psamcombuf.Psamcommand.lc= len+0x0A;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x00;   
    for(i = 0; i < 8; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[2+i] = pkey[i]; 
    }
    for(i = 0; i < len; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[10+i] = srcbuffer[i]; 
    }    
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < len ; i++)
    {
        srcbuffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    return 0;    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//3DES加解密
//加密后的数据保存到srcbuffer[]中。
//mode : 0x00 :ECB encrty 0x10 :ECB disEncrty
//       0x01 :CBC encrty 0x11 :CBC disEncrty
//55 00 17 80 C6 04 A0 12 00 04 11 22 33 44 55 66 77 88 5a 5a 5a 5a 5a 5a 5a 5a F0 A3
unsigned char ThreeDesBykey(unsigned char srcbuffer[],unsigned short len,unsigned char *pkey,unsigned char mode)
{
    int i = 0;
	
	unsigned short len1;
	len1+=0x17;
	
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = (len1>>8)&0xff;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = len1&0xff;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xC6;
    if(mode == 0x00)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x05;
    if(mode == 0x10)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x45;
    if(mode == 0x01)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x15;
    if(mode == 0x11)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x55;    
    psamapdu.psamcombuf.Psamcommand.pp2= 0xA0;
    psamapdu.psamcombuf.Psamcommand.lc= len+0x12;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x00;   
    for(i = 0; i < 16; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[2+i] = pkey[i]; 
    }
    for(i = 0; i < len; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[18+i] = srcbuffer[i]; 
    }    
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < len ; i++)
    {
        srcbuffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    return 0;    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//DES加解密
//加密后的数据保存到srcbuffer[]中。len必须是8的倍数
//mode : 0x00 :ECB encrty 0x10 :ECB disEncrty
//       0x01 :CBC encrty 0x11 :CBC disEncrty
unsigned char SingleDesEntryOrDisEntry(unsigned char srcbuffer[],unsigned char len,unsigned char KID,unsigned char mode)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = len + 9;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xC6;
    if(mode == 0x00)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x04;
    if(mode == 0x10)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x44;
    if(mode == 0x01)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x14;
    if(mode == 0x11)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x54;    
    psamapdu.psamcombuf.Psamcommand.pp2= 0x80;
    psamapdu.psamcombuf.Psamcommand.lc= len+4;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = KID;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x01;
    psamapdu.psamcombuf.Psamcommand.databuf[2] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[3] = 0x00;    
    for(i = 0; i < len; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[4+i] = srcbuffer[i]; 
    }
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < len ; i++)
    {
        srcbuffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    return 0;    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//3DES加解密
//加密后的数据保存到srcbuffer[]中。len必须是8个倍数
//mode : 0x00 :ECB encrty 0x10 :ECB disEncrty
//       0x01 :CBC encrty 0x11 :CBC disEncrty
//55 00 11 80 C6 05 80 0C 01 01 00 00 81 C2 E1 52 47 80 BA CB CC 37
unsigned char threeDesEntryOrDisEntry(unsigned char srcbuffer[],unsigned char len,unsigned char KID,unsigned char mode)
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = len + 9;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xC6;
    if(mode == 0x00)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x05;
    if(mode == 0x10)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x45;
    if(mode == 0x01)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x15;
    if(mode == 0x11)
        psamapdu.psamcombuf.Psamcommand.pp1= 0x55;    
    psamapdu.psamcombuf.Psamcommand.pp2= 0x80;
    psamapdu.psamcombuf.Psamcommand.lc= len+4;
    psamapdu.psamcombuf.Psamcommand.databuf[0] = KID;
    psamapdu.psamcombuf.Psamcommand.databuf[1] = 0x01;
    psamapdu.psamcombuf.Psamcommand.databuf[2] = 0x00;
    psamapdu.psamcombuf.Psamcommand.databuf[3] = 0x00;    
    for(i = 0; i < len; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[4+i] = srcbuffer[i]; 
    }
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < len ; i++)
    {
        srcbuffer[i] = psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    return 0;      
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//外部认证
unsigned char ExternaAuthen(unsigned char keyFileID)
{
    int i = 0;
    unsigned char buffer[8] = {0};
    i = GetChallenge(buffer,8);
    if(i != 0)
        return i;
    i = threeDesEntryOrDisEntry(buffer,8,keyFileID,0x00);
    if(i != 0)
        return i;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x0D;
    psamapdu.psamcombuf.Psamcommand.cla = 0x00;
    psamapdu.psamcombuf.Psamcommand.ins = 0x82;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;
    psamapdu.psamcombuf.Psamcommand.pp2= keyFileID;
    psamapdu.psamcombuf.Psamcommand.lc= 0x08;      
    for(i = 0;i<0x08;i++)
        psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i];
    psamSendCommand();
    return psamRecvAnswer(); 
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char XORArigth(const unsigned char *buffer,unsigned short len,unsigned char *XORBuff)
{
    unsigned short j = 0;
    if(len % 8)
        return 1;
    for(j = 0; j < len ;j++)
    {
        XORBuff[j % 8] = (buffer[j] ^ XORBuff[j % 8]);
    } 
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//MAC
//注意需要将要加密的数据保存到    
/*
    for(i = 0; i < len; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i];
    }
*/
unsigned char ToGetMAC(const unsigned char buffer[],unsigned short len,unsigned char *mac)
{
    unsigned char macBuff[8];
    unsigned short i = 0,j = 0;
    for(i = 0;i < len;i++)
    {
        psamapdu.psamansBuf.Psamanswer.dataBuf[i] =buffer[i];
    }
    //将数据补到8的倍数
    i = len % 8;    
    if(i != 0)
    {
        for(j = 0;j < (8 - i);j++)
        {
            psamapdu.psamansBuf.Psamanswer.dataBuf[len] = 0x00;
            len = len + 1;
        }
    }
    if(len % 8)
    {
        psamapdu.psamansBuf.Psamanswer.answerHead = 0x55;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[0] = 0x00;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[1] = 0x02;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[0] = 0x67;
        psamapdu.psamansBuf.Psamanswer.dataLen.lenbuffer[1] = 0x00;
        return 1;
    }
        
    //8个字节异或运算
    for(i = 0; i < 8 ; i++)
        macBuff[i] = 0x00;
    XORArigth(psamapdu.psamansBuf.Psamanswer.dataBuf,len,macBuff);
    i = SingleDesEntryOrDisEntry(macBuff,0x08,0x04,0x00);
    if(i != 0)
        return i;
    for(i = 0;i < 4; i++)
        mac[i] = macBuff[i];
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//RSA公钥加解密
unsigned char RsaEncrty(unsigned short publicFileID,unsigned char buffer[])
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x85;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xF6;
    psamapdu.psamcombuf.Psamcommand.pp1= publicFileID >> 8;   
    psamapdu.psamcombuf.Psamcommand.pp2= publicFileID & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= 0x80;  
    LitteToBigStr(buffer,128);    
    for(i = 0; i < psamapdu.psamcombuf.Psamcommand.lc; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i]; 
    }
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < 0x80 ; i++)
    {
        buffer[i]= psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    LitteToBigStr(buffer,128);
    return 0;   
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
//RSA私钥加解密
unsigned char RsaDisEncrty(unsigned short privateFileID,unsigned char buffer[])
{
    int i = 0;
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x85;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xF8;
    psamapdu.psamcombuf.Psamcommand.pp1= privateFileID >> 8;  
    psamapdu.psamcombuf.Psamcommand.pp2= privateFileID & 0xFF;
    psamapdu.psamcombuf.Psamcommand.lc= 0x80;   
    LitteToBigStr(buffer,128);
    for(i = 0; i < psamapdu.psamcombuf.Psamcommand.lc; i++)
    {
        psamapdu.psamcombuf.Psamcommand.databuf[i] = buffer[i]; 
    }
    psamSendCommand();
    if((unsigned char)(i = psamRecvAnswer()))
        return i;
    for(i = 0; i < 0x80; i++)
    {
        buffer[i]= psamapdu.psamansBuf.Psamanswer.dataBuf[i];
    }
    LitteToBigStr(buffer,128);
    return 0;        
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char TobootZ8(void)
{
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xFF;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;  
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x00;    
    psamSendCommand();   
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
/*
55 00 05 80 F0 00 00 00 2E 23
55 00 02 90 00
55 00 06 80 F0 00 01 01 61 61 FE 
55 00 02 90 00
55 00 06 80 F0 00 01 01 62 60 BE
55 00 02 90 00
55 00 06 80 F0 00 01 01 63 A0 7F
55 00 02 90 00
55 00 05 80 F0 00 02 00 4E 22
55 00 16 A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D 90 00
mode 1:启动 2：传输数据 3：获取数据摘要
*/
unsigned char SHA1Arigth(unsigned char *pbuf,unsigned int len)
{
    unsigned char i = 0,j = 0;
    unsigned  char num = 0;
    //启动
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xF0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;  
    psamapdu.psamcombuf.Psamcommand.pp2= 0x00;
    psamapdu.psamcombuf.Psamcommand.lc= 0x00;
    psamSendCommand();
//    if((unsigned char)(i = psamRecvAnswer()))
//        return i;
	i = psamRecvAnswer();
	if(i != 0)
	{
		return i;	
	}
	else{}           
           
    num = len / 0x40;
    for(j = 0; j < num;j++)
    {
        psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
        psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
        psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x45;
        psamapdu.psamcombuf.Psamcommand.cla = 0x80;
        psamapdu.psamcombuf.Psamcommand.ins = 0xF0;
        psamapdu.psamcombuf.Psamcommand.pp1= 0x00;  
        psamapdu.psamcombuf.Psamcommand.pp2= 0x01;
        psamapdu.psamcombuf.Psamcommand.lc= 0x40;
        for(i = 0; i< 0x40;i++)
        {
            psamapdu.psamcombuf.Psamcommand.databuf[i] = *(pbuf + (0x40 * j) + i);
        }
        psamSendCommand();
//        if((unsigned char)(i = psamRecvAnswer()))
//            return i;  
		i = psamRecvAnswer();
		if(i != 0)
		{
			return i;	
		}
		else{}        
    }             
    num = len % 0x40;
	if(num)
	{
	    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
	    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
	    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = num + 5;
	    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
	    psamapdu.psamcombuf.Psamcommand.ins = 0xF0;
	    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;  
	    psamapdu.psamcombuf.Psamcommand.pp2= 0x01;
	    psamapdu.psamcombuf.Psamcommand.lc= num;
	    for(i = 0; i< num;i++)
	    {
	        psamapdu.psamcombuf.Psamcommand.databuf[i] = *(pbuf + (0x40 * j) + i);
	    }
	    psamSendCommand();
	//    if((unsigned char)(i = psamRecvAnswer()))
	//        return i;           
		i = psamRecvAnswer();
		if(i != 0)
		{
			return i;	
		}
		else{} 
	}

   
    psamapdu.psamcombuf.Psamcommand.commandHead = 0x55;    
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[0] = 0x00;
    psamapdu.psamcombuf.Psamcommand.dataLen.lenbuffer[1] = 0x05;
    psamapdu.psamcombuf.Psamcommand.cla = 0x80;
    psamapdu.psamcombuf.Psamcommand.ins = 0xF0;
    psamapdu.psamcombuf.Psamcommand.pp1= 0x00;  
    psamapdu.psamcombuf.Psamcommand.pp2= 0x02;
    psamapdu.psamcombuf.Psamcommand.lc= 0x00;
    psamSendCommand();
//    if((unsigned char)(i = psamRecvAnswer()))
//        return i; 
		
	i = psamRecvAnswer();
	if(i != 0)
	{
		return i;	
	}
	else{}
	   
    for(j = 0; j < 20;j++)
        pbuf[j] = psamapdu.psamansBuf.Psamanswer.dataBuf[j];
    return 0;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void sercrityTest(void)
{
//    unsigned char i = 0;
//    //读取flash中的数据
//    unsigned char buu33[16] = {0x89,0x00,0x00,0x00,0x00,0x00,0x00,0x06};
//    unsigned char buu[128] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
//    unsigned char buu22[40] = {0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x22,0x33,0x22,0x33,0x22,0x33,0x22,0x33,0x22,0x33,0x22,0x33,0x22,0x33};
//    unsigned char PINKEY[16] = {0x16,0x62 ,0xe6,0x30,0xf9,0x2f,0x82,0x5e,0xA7,0x1C,0x66,0x44,0x71,0xA7,0x53,0xB7};
//    unsigned char MACKEY[16] = {0x5E,0xED,0x33,0x85,0x88,0x83,0x0F,0xA5,0x21,0xB8,0x51,0x06,0x50,0x32,0x84,0x24};//{0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0xAA,0xBB,0xCC,0xDD};
//    unsigned char TCKKEY[16] = {0x7D,0xf3,0x35,0xE8,0x0C,0x5C,0x35,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//    readfromFlash(0x0300,0xFF);
//    readfromFlash(0x0400,0xFF);
//    readfromFlash(0x0500,0xFF);
//    readfromFlash(0x0600,0xFF);
////    readfromFlash(0x0700,0xFF);
//    readfromFlash(0x0800,0xFF);
//    readfromFlash(0x0900,0xFF);
//    readfromFlash(0x0A00,0xFF);
//    readfromFlash(0x0B00,0xFF); 
//    readfromFlash(0x0A00,0xFF);
//    readfromFlash(0x0B00,0xFF);
//    readfromFlash(0x0C00,0xFF);
//    readfromFlash(0x0D00,0xFF);
//    readfromFlash(0x0E00,0xFF);
//    readfromFlash(0x0F00,0xFF);
////    RsaEncrty(0x0001,buu);
////    //RSA加密
////    RsaEncrty(0x0004,buu);
//////    //RSA解密
////    RsaDisEncrty(0x0004,buu);    
////    //向flash中写入数据
////    //WriteIntoFlash(0x0000,(unsigned char*)"123456",0x08);
////    //擦除MF55 00 07 BF CE 00 00 02 3F 00 CC 65
//    EraseMF();
//    CreateMF();
//    createKeyFile(0x0600,0x00,0x00,0x00);
////    //用于Z8d256和St主控芯片之间的数据加密传输
////    loadMainkey();
//    //设置PINKEY
//    setApplykey(PINKEY,0x01);
//    changeDESkey(PINKEY,0x01,16);
//    //设置MACKEY
//    setApplykey(MACKEY,0x02);
//    //设置TCKKEY
//    setApplykey(TCKKEY,0x03);
//    for(i = 0;i < 8; i++)
//        buu[i] = 0x00;
//    XORArigth(TCKKEY,16,buu);
//    setTrackkey(buu,0x04);    
//    changeDESkey(buu,0x04,8);
//     
//    //创建公私钥文件来保存RSA密钥对的生成。
//    createPublicFile(0x0001);
//    createPrivateFile(0x0002);
//    //生成密钥对。
//    genaryRsaKey(0x0001,0x0002);

//    //读取公钥文件发送出去
//    ReadPublicFile(0x0001);
//    //创建公钥文件来保存配置服务器公钥
//    createPublicFile(0x0003);
//    seleteBinaryFile(0x0003);
//    seleteBinaryFile(0x0003);
//    //用于配置服务器公钥文件的更新。   
//    updatePublickeyFile(0x0003);    
//    //创建公钥文件来保存管理服务器公钥
//    createPublicFile(0x0004);
//    //用于管理服务器公钥文件的更新。   
//    updatePublickeyFile(0x0004);  
//    //RSA加密
//    RsaEncrty(0x0001,buu);
//    //RSA解密
//    RsaDisEncrty(0x0002,buu);

//    
//    //更新终端号
//    setDeviceInfo(buu22);
//    //更新PSAMID
//    setPSAMInfo(buu33);
//    //bankID 软件ID固定
//    //读取bankID
//    getDeviceInfo(buu,1);
//    //读取PSAMID
//    getPSAMInfo(buu);
//    //bankID 软件ID固定    

//    
////使用到的算法     
//    //DES加密
//    SingleDesEntryOrDisEntry(buu,0x08,0x02,0x00);
//    //DES解密
//    SingleDesEntryOrDisEntry(buu,0x08,0x02,0x10);
//    //3DESECB加密
//    threeDesEntryOrDisEntry(buu,0x08,0x01,0x00);
//    //3DESECB解密
//    threeDesEntryOrDisEntry(buu,0x08,0x01,0x10);

//    //获取随机数
//    GetChallenge(buu,4);
//    GetChallenge(buu,8);
//    GetChallenge(buu,16);
//    //外部认证
//    ExternaAuthen(0x02);
//    //用于配置服务器公钥文件的更新。   
//    updatePublickeyFile(0x0003); 
//    //MAC算法    
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char QPOSFileStruct(void)
{
    unsigned char buff[28] = {0};
    unsigned ret;
    ret = EraseMF();    
    if(ret != 0)
    {
        ret = WriteIntoFlash(0x400,buff,7);
        if(ret != 0)
            return ret;
    }        
    ret = CreateMF();
    if(ret != 0)
        return ret;
    ret = createKeyFile(0x0600,0x00,0x00,0x00);
    if(ret != 0)
        return ret;
    //用于Z8d256和St主控芯片之间的数据加密传输
    ret = loadMainkey();
    if(ret != 0)
        return ret;
    ret = setApplykey(keykey,0x01);
    if(ret != 0)
        return ret;
		
#if 0    
    //创建公私钥文件来保存RSA密钥对的生成。
    ret = createPublicFile(0x0001);
    if(ret != 0)
        return ret;
    ret = createPrivateFile(0x0002);
    if(ret != 0)
        return ret;
    //创建公钥文件来保存配置服务器公钥
    ret = createPublicFile(0x0003);
    if(ret != 0)
        return ret; 
    //创建公钥文件来保存管理服务器公钥
    ret = createPublicFile(0x0004);
    if(ret != 0)
        return ret;
#endif
//    EEPROMWrite(buff,DEVICEIDSTART,10);//清除设备id10个字节
//    EEPROMWrite(buff,EEPROMQPOSCLOSE88,6);//清除设备ID PSAMID BANKID标志字节            
    ret = setPSAMInfo(buff);//清除PSAMID
    if(ret != 0)
        return ret;
    return 0; 
}
/*******************************end of  file**********************************************/

