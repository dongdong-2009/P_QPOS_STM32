#include "App_boot.h"

#include "Flash_if.h"
#include "msd.h"

APDU	G_APDU;
unsigned char APDU_Buf[0x800];
unsigned char DATA_MAC[8];


//从指定地址处连续读取片内Flash若干字节数据
void BOOT_NVMEM_Read(unsigned int startAddr, unsigned char* puchDataBuff, unsigned int len)
{
	unsigned int uiCount = 0;
	unsigned int uiAddr;
	if(puchDataBuff == 0)
	{
		return;
	}
	
	uiAddr = (startAddr & FLASH_ADDR_MASK)|FLASH_START_ADDR;
	while(uiCount < len) 
	{
		puchDataBuff[uiCount++]= *(__IO uint8_t*)(uiAddr++);
	}

}

//向指定地址处连续写入片内Flash若干字节数据，无掉电保护
void BOOT_NVMEM_Write(unsigned int uiFlashAddr, unsigned char* pucDataBuff, unsigned int uiLen)
{  
	unsigned int uiOffset, uiAddr;
	unsigned int uiFlashPageAddr;
	unsigned int uiLength, uiCopyLen;

	unsigned char *pucTemp;

	if(uiFlashAddr + uiLen > FLASH_END_ADDR)
		return ;

	uiAddr = (uiFlashAddr & FLASH_ADDR_MASK);  			//flash绝对地址
	uiFlashPageAddr = uiAddr  & FLASH_PAGE_ADDR_HI;   //获取目的地址所在扇区的起始地址
	uiFlashPageAddr |= FLASH_START_ADDR;
	uiOffset = uiAddr  & FLASH_PAGE_ADDR_LO;			//获取目的地址在扇区中的偏移量
	uiCopyLen= FLASH_PAGE_SIZE - uiOffset ;				//计算需要从输入缓冲中copy过来的数据块大小

	if(uiCopyLen > uiLen)
	{
	uiCopyLen = uiLen;
	}
	else{}

	uiLength = uiLen;
	pucTemp = pucDataBuff;

	do
	{        
		if(uiCopyLen < FLASH_PAGE_SIZE)
		{
			BOOT_NVMEM_Read(uiFlashPageAddr, MAL_Buffer, FLASH_PAGE_SIZE);
		}
			
		memcpy(MAL_Buffer + uiOffset, pucTemp, uiCopyLen);	//更新缓冲区中的扇区内容        
		FLASH_If_Erase(uiFlashPageAddr);   	//擦除整个扇区 
		FLASH_If_Write(uiFlashPageAddr, FLASH_PAGE_SIZE);			       
		uiFlashPageAddr += FLASH_PAGE_SIZE;
		uiOffset = 0;
		uiLength -= uiCopyLen;
		pucTemp += uiCopyLen;

		if(uiLength > FLASH_PAGE_SIZE)
		{
			uiCopyLen = FLASH_PAGE_SIZE;
		}
		else
		{
			uiCopyLen = uiLength;
		}
	}while( uiLength > 0);

}

//从指定地址连续擦除片内Flash若干页
void BOOT_NVMEM_Erase(unsigned int baseAddr,unsigned int startAddr, unsigned int len)
{
	unsigned int i;
	unsigned int uiFlashAddr;
	unsigned int uiEraseAddr;

	uiFlashAddr = baseAddr | startAddr;

	uiEraseAddr = uiFlashAddr & FLASH_PAGE_ADDR_HI;
	uiEraseAddr |= FLASH_START_ADDR;
		
	if((len & (FLASH_PAGE_SIZE - 1)) != 0)
	{
		len = (len+FLASH_PAGE_SIZE)/FLASH_PAGE_SIZE*FLASH_PAGE_SIZE ; 
	}

	for(i = 0; i < (len / FLASH_PAGE_SIZE); i++)
	{
		FLASH_If_Erase(uiFlashAddr + FLASH_PAGE_SIZE * i);	
	}

	return;
}

void Format_APDU()
{
	memset(&G_APDU, 0, sizeof(G_APDU));
	
	G_APDU.pInData = APDU_Buf;
	G_APDU.pOutData = APDU_Buf;

}

unsigned int CMD_BOOT_Write_Key(void)
{
	unsigned int ret = 0;
	
	switch(G_APDU.PP1)
	{
	case 00:								 
		BOOT_NVMEM_Read(BOOT_KEY_ADDR, G_APDU.pOutData, BOOT_KEY_LEN);
		G_APDU.RLE = BOOT_KEY_LEN;
		ret = SW_OPERATION_SUCCESS;
	break;
   	case 01:
		BOOT_NVMEM_Write(BOOT_KEY_ADDR, G_APDU.pInData, BOOT_KEY_LEN);
		G_APDU.RLE = 0;
		ret = SW_OPERATION_SUCCESS;
	break;
	case 02:
		BOOT_NVMEM_Write(DEVICE_INFO_DEV_VER_ADDR, G_APDU.pInData, DEVICE_INFO_DEV_VER_ADDR_LEN);
		G_APDU.RLE = 0;
		ret = SW_OPERATION_SUCCESS;
	break;
	default:
		ret = SW_P1P2_ERROR;//------返回P1P2参数错误
		G_APDU.RLE = 0;
	}
			 
	return ret;
}

unsigned int CMD_BOOT_DNLOAD_DATA_DES3(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
//	unsigned int i=0;
	unsigned char* desptr;
    	unsigned char des3key[16];
	unsigned char tmpbuf[256];
		
	//addr is operated
	flashaddr += G_APDU.PP1 << 16;
	flashaddr += G_APDU.PP2 << 8;
	flashaddr += G_APDU.PP3;
	
	//data length
	len =  G_APDU.PP4 << 8;
	len +=  G_APDU.PP5;	

	desptr = G_APDU.pInData;
	
	
	if(flashaddr == 0)
	{
		memset(DATA_MAC, 0, 8);
	}

	//BOOT_NVMEM_Read(BOOT_KEY_ADDR, des3key, BOOT_KEY_LEN);

	// 3des-112 ecb 解密
	DES3_App((unsigned char *)BOOT_KEY_ADDR, 1, NULL, desptr, &len, desptr);

	//计算mac

	DES3_App((unsigned char *)BOOT_KEY_ADDR, 0, DATA_MAC, desptr, &len, MAL_Buffer);

	memcpy(DATA_MAC, MAL_Buffer+len -8, 8);

	flashaddr = flashaddr + APP_BASE_ADDR;
	
	BOOT_NVMEM_Write(flashaddr, desptr, len);

	G_APDU.RLE = 0;
	return SW_OPERATION_SUCCESS;
}

unsigned int CMD_BOOT_CRC_DATA(void)
{
	unsigned int len =0;
	unsigned int ret =0;
	unsigned char pccrc[8] = {0};
	unsigned char flag[2] = {0x00,0xAA};
	
	len = G_APDU.PP4 << 8;			//lc
	len += G_APDU.PP5;				//lc	

	if(len != 8)
	{
		ret = SW_VERIFY_CRC_ERROR;
		return ret;
	}
	
	memcpy(pccrc,G_APDU.pInData,len);			   //获取上位机传来的mac
	
	if(memcmp(pccrc,DATA_MAC,8))
	{ 
	   	G_APDU.RLE = 0;
		//BOOT_NVMEM_Write(BOOT_FLAG_ADDR, flag, BOOT_FLAG_LEN);
		ret = SW_VERIFY_CRC_ERROR;	   			
	}
	else
	{ 
		BOOT_NVMEM_Write(BOOT_FLAG_ADDR, flag+1, BOOT_FLAG_LEN);

		//关闭主电源
		//GPIO_ResetBits(GPIOB,GPIO_Pin_15);	
		G_APDU.RLE = 0;
	   	ret = SW_OPERATION_SUCCESS;	   
	} 
	return ret;

}

unsigned int CMD_BOOT_Write_SPIFlash(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
	//addr is operated
	flashaddr += G_APDU.PP1 << 16;
	flashaddr += G_APDU.PP2  << 8;
	flashaddr += G_APDU.PP3 ;

	flashaddr += SPI_FONT_BASE_ADDR;
			
	//data length
	len =   G_APDU.PP4 << 8;
	len += G_APDU.PP5 ;	
	MSD_WriteBlock(G_APDU.pInData, flashaddr, len);
	G_APDU.RLE = 0;
	return SW_OPERATION_SUCCESS;
}

unsigned int CMD_BOOT_Read_SPIFlash(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
	//addr is operated
	flashaddr += G_APDU.PP1 << 16;
	flashaddr += G_APDU.PP2  << 8;
	flashaddr += G_APDU.PP3 ;

	flashaddr += SPI_FONT_BASE_ADDR;
			
	//data length
	len =   G_APDU.PP4 << 8;
	len += G_APDU.PP5 ;	
	MSD_ReadBuffer(G_APDU.pOutData, flashaddr, len);
	G_APDU.RLE = len;
	return SW_OPERATION_SUCCESS;
}

unsigned int CMD_BOOT_GetID_SPIFlash(void)
{
	unsigned int Flash_ID = 0;

	//addr is operated
	Flash_ID = SPI_FLASH_ReadID();

	if(!Flash_ID)
	{
		Flash_ID = SPI_FLASH_ReadID();
	}

	G_APDU.pOutData[0] = Flash_ID >> 24;
	G_APDU.pOutData[1] = Flash_ID >> 16;
	G_APDU.pOutData[2] = Flash_ID >> 8;
	G_APDU.pOutData[3] = Flash_ID ;
	
	G_APDU.RLE = 4;
	return SW_OPERATION_SUCCESS;
}

#if BOOT_TEST
unsigned int CMD_BOOT_Test(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
//	unsigned int i=0;
	unsigned char* desptr;
    	unsigned char des3key[16];
	unsigned char tmpbuf[256];
		
	//addr is operated
	flashaddr += G_APDU.PP1 << 16;
	flashaddr += G_APDU.PP2 << 8;
	flashaddr += G_APDU.PP3;
	
	//data length
	len =  G_APDU.PP4 << 8;
	len +=  G_APDU.PP5;	

	desptr = G_APDU.pOutData;

	flashaddr |= APP_BASE_ADDR;
	BOOT_NVMEM_Read(flashaddr, desptr, len);

	G_APDU.RLE = len;
	return SW_OPERATION_SUCCESS;

}

#endif


void Boot_Command(void)
{
	unsigned int ret;

	if(G_APDU.CLA != 0x7B)
	{
		G_APDU.RLE = 0;
		G_APDU.SW1 = 0x6d;
		G_APDU.SW2 = 0x00;
		return;
	}

	switch(G_APDU.INS)
	{	
	case CMD_BOOT_VERSION:
		memcpy(G_APDU.pOutData, BOOT_VERSION, strlen(BOOT_VERSION));
		G_APDU.RLE = strlen(BOOT_VERSION);
		G_APDU.SW1 = 0x90;
		G_APDU.SW2 = 0x00;
		break;
	case CMD_BOOT_DNLOAD_KEY: //下载或读取密钥	
		ret = CMD_BOOT_Write_Key();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;	
	case CMD_DNLOAD_DATA_DES:
		ret = CMD_BOOT_DNLOAD_DATA_DES3();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
#if BOOT_TEST
	case 0xEF:
		ret =  CMD_BOOT_Test();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
#endif
	case CMD_BOOT_CRC:
		ret = CMD_BOOT_CRC_DATA();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
	case CMD_SPIFLASH_GETID:
		ret = CMD_BOOT_GetID_SPIFlash();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
	case CMD_SPIFLASH_READ:
		ret = CMD_BOOT_Read_SPIFlash();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
	case CMD_SPIFLASH_WRITE:
		ret = CMD_BOOT_Write_SPIFlash();
		if(ret != SW_OPERATION_SUCCESS)
			G_APDU.RLE = 0;	
		G_APDU.SW1 = ret>>8;
		G_APDU.SW2 = ret&0x00ff;
		break;
	default:
		G_APDU.RLE = 0;
		ret = 0x6D00;
		break;
	}
	
	G_APDU.SW1 = ret>>8;
	G_APDU.SW2 = ret&0x00ff;

}


