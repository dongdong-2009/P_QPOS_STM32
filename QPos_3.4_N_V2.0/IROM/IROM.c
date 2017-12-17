//******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
//* File Name          : IROM.c
//* Author             : Roc Chao
//*												
//* Version            : V1.0
//* Date               : 04/12/2013
//* Description        : 安全方案
//********************************************************************************/
#include "string.h"
#include "stm32f10x.h"
#include "IROM.h"
unsigned char ucTempBuf[FLASH_SECTOR_SIZE];
//***************************************************************************************
//* Function Name  :IROM_Read 
//* Description    : 从指定地址处连续读取片内Flash若干字节数据
//*       
//* Input          : 
//* Output         :
//* Return         : 
//***************************************************************************************
void IROM_Read(unsigned int uiFlashAddr, unsigned char* pucDataBuff, unsigned int uiLen)
{
	unsigned int uiCount = 0;
	unsigned int uiAddr;
	if(pucDataBuff == NULL)
	{
		return;
	}
	else{}

	uiAddr = (uiFlashAddr & FLASH_ADDR_MASK) | FLASH_STARD_ADDR;
	while(uiCount < uiLen) 
	{
		*(pucDataBuff + uiCount) = *(__IO uint8_t*)uiAddr++;
		uiCount++;
	}
}
//***************************************************************************************
//* Function Name  :IROM_WritePage 
//* Description    : 从片内Flash指定地址处连续写若干字节数据
//*       
//* Input          :  
//* Output         :
//* Return         :
//***************************************************************************************
void IROM_WritePage(unsigned int uiFlashAddr, unsigned char *pucDataBuf, unsigned int uiLen)
{
	unsigned int uiIdx = 0;
	
	/* Data received are Word multiple */	 
	for (uiIdx = 0; uiIdx <  uiLen; uiIdx = uiIdx + 4)
	{
		FLASH_ProgramWord(uiFlashAddr, *(unsigned int *)(pucDataBuf + uiIdx));  
		uiFlashAddr += 4;
	} 
}
//***************************************************************************************
//* Function Name  :IROM_Write 
//* Description    :向指定地址处连续写入片内Flash若干字节数据，无掉电保护 
//*       
//* Input          : 
//* Output         : 
//* Return         :
//***************************************************************************************
void IROM_Write(unsigned int uiFlashAddr, unsigned char* pucDataBuff, unsigned int uiLen)
{  
	unsigned int uiOffset, uiAddr;
	unsigned int uiFlashPageAddr;
	unsigned int uiLength, uiCopyLen;
	
	unsigned char *pucTemp;
    if(uiFlashAddr + uiLen > FLASH_ADDR_MASK)
        return ;

	uiAddr = (uiFlashAddr & FLASH_ADDR_MASK) | FLASH_STARD_ADDR;  			//flash绝对地址
//	uiAddr = uiFlashAddr;  			//flash绝对地址	
	uiFlashPageAddr = uiAddr  & FLASH_SECTOR_ADDR_HI;   //获取目的地址所在扇区的起始地址
    uiOffset = uiAddr  & FLASH_SECTOR_ADDR_LO;			//获取目的地址在扇区中的偏移量
   	uiCopyLen= FLASH_SECTOR_SIZE - uiOffset ;				//计算需要从输入缓冲中copy过来的数据块大小
    
	if(uiCopyLen > uiLen)
	{
        uiCopyLen = uiLen;
	}
	else{}

	uiLength = uiLen;
    pucTemp = pucDataBuff;
	
    do
    {        
		if(uiCopyLen < FLASH_SECTOR_SIZE)
		{
			IROM_Read(uiFlashPageAddr, ucTempBuf, FLASH_SECTOR_SIZE);
		}
		else{}		
        memcpy(ucTempBuf + uiOffset, pucTemp, uiCopyLen);	//更新缓冲区中的扇区内容        
        FLASH_ErasePage(uiFlashPageAddr);   	//擦除整个扇区 
		IROM_WritePage(uiFlashPageAddr, ucTempBuf, FLASH_SECTOR_SIZE);			       
        uiFlashPageAddr += FLASH_SECTOR_SIZE;
        uiOffset = 0;
        uiLength -= uiCopyLen;
        pucTemp += uiCopyLen;

        if(uiLength > FLASH_SECTOR_SIZE)
		{
            uiCopyLen = FLASH_SECTOR_SIZE;
        }
		else
		{
            uiCopyLen = uiLength;
		}
    }while( uiLength > 0);
}
//***************************************************************************************
//* Function Name  :IROM_Write 
//* Description    :向指定地址处连续写入片内Flash若干字节数据，无掉电保护 
//*       
//* Input          : 
//* Output         : 
//* Return         :
//***************************************************************************************
void IROM_Erase(unsigned int uiFlashAddr, unsigned int uiLen)
{
	unsigned int i;

	if((uiFlashAddr & (FLASH_SECTOR_SIZE - 1)) != 0)
	{
		IROM_WritePage(uiFlashAddr, (unsigned char *)PNULL, uiLen);
		return;
	}
	if((uiLen & (FLASH_SECTOR_SIZE - 1)) != 0)
	{
		IROM_WritePage(uiFlashAddr, (unsigned char *)PNULL, uiLen);
		return;
	}

	for(i = 0; i < (uiLen / FLASH_SECTOR_SIZE); i++)
	{
		FLASH_ErasePage(uiFlashAddr + FLASH_SECTOR_SIZE * i);	
	}
}
/********************************end fo file *************************************/
