/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : msd.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : MSD card driver source file.
*                      Pin assignment:
*             ----------------------------------------------
*             |  STM32F10x    |     MSD          Pin        |
*             ----------------------------------------------
*             | CS            |   ChipSelect      1         |
*             | MOSI          |   DataIn          2         |
*             |               |   GND             3 (0 V)   |
*             |               |   VDD             4 (3.3 V) |
*             | SCLK          |   Clock           5         |
*             |               |   GND             6 (0 V)   |
*             | MISO          |   DataOut         7         |
*             -----------------------------------------------
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "msd.h"
#include "Display.h"
#include "SysTick_delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Select MSD Card: ChipSelect pin low  */
#define MSD_CS_LOW()     GPIO_ResetBits(MSD_CS_PORT, MSD_CS_PIN)
/* Deselect MSD Card: ChipSelect pin high */
#define MSD_CS_HIGH()    GPIO_SetBits(MSD_CS_PORT, MSD_CS_PIN)
static uint8_t CmpString[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} ;
uint8_t VerString[16];
extern void USB_Cable_Config (FunctionalState NewState);
extern uint8_t Down8Flag;
extern uint8_t Rx_Buffer[] ;

/* Private function prototypes -----------------------------------------------*/
//static void SPI_Config(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : MSD_Init
* Description    : Initializes the MSD/SD communication.
* Input          : None
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
//uint8_t MSD_Init(void)
//{  
//	int i = 0;
//	/* Initialize MSD_SPI */
//	SPI_Config();
//	/* MSD chip select low */
//	MSD_CS_HIGH();
//	SPI_FLASH_GlobalProtect();
//	MSD_ReadBuffer(VerString, 0, 16);
//	SPI_FLASH_GlobalUnprotect();
//	if( 0 == Buffercmp(CmpString,VerString,16))
//	{
//		if(1 == Down2And8Flag)
//		{
//		   	for(i = 0; i<512; i++)
//			{
//				SPI_FLASH_SectorErase(4096*i);
//			}
//			//SPI_FLASH_ChipErase();
//		}
//	}  
//	return (MSD_GoIdleState());
//}
void MSD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
   RCC_APB2PeriphClockCmd(MSD_CS_GPIO_PORT_CLOCK, ENABLE);
#if 0
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
  
	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Confugure MISO pin as Input Floating  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
*/
#endif
#if 1
	 /*Enable SPI1.NSS as a GPIO*/
	  SPI_SSOutputCmd(SPI1, ENABLE);
	  
	  /*Configure PA.4(NSS)--------------------------------------------*/
	  GPIO_InitStructure.GPIO_Pin =MSD_CS_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	  GPIO_Init(MSD_CS_PORT, &GPIO_InitStructure);
#endif  

}

void MSD_SPI_Config(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;	

	 SPI_Cmd(SPI1, DISABLE);  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;   
	SPI_Init(SPI1,&SPI_InitStructure);	
	SPI_Cmd(SPI1, ENABLE);
}
void MSD_Init()
{
   	MSD_GPIO_Config();
	//MSD_SPI_Config();
	SPI_FLASH_ReadID();
}
void SPI_FLASH_GlobalUnprotect()
{ 
	uint8_t StatusByte0 = MSD_GetStatus();
	uint8_t StatusByte1 = 0;
	SPI_FLASH_WriteEnable();
	
	MSD_CS_LOW();
	MSD_WriteByte(CMD_WRSR);
	MSD_WriteByte(GLOUPS & StatusByte0);
	MSD_WriteByte(StatusByte1);
	MSD_CS_HIGH();
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_GlobalProtect()
{ 
	uint8_t StatusByte0 = MSD_GetStatus();
	uint8_t StatusByte1 = 0;
	SPI_FLASH_WriteEnable();
	MSD_CS_LOW();
	MSD_WriteByte(CMD_WRSR);
	MSD_WriteByte(GLOPS | StatusByte0);
	MSD_WriteByte(StatusByte1);
	MSD_CS_HIGH();
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_ChipErase()
{  SPI_FLASH_WriteEnable();
   MSD_CS_LOW();
   MSD_WriteByte(CMD_CE);
   MSD_CS_HIGH();
   SPI_FLASH_WaitForWriteEnd();
}

uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 0;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 1;
}
/*******************************************************************************
* Function Name  : MSD_WriteBlock
* Description    : Writes a block on the MSD
* Input          : - pBuffer : pointer to the buffer containing the data to be
*                    written on the MSD.
*                  - WriteAddr : address to write on.
*                  - NumByteToWrite: number of data to write
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_WriteBlock(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t rvalue = MSD_RESPONSE_FAILURE;
  MSD_ReadBuffer(VerString, WriteAddr, 16);

  if( 0 == Buffercmp(CmpString,VerString,16))
  {
  SPI_FLASH_SectorErase(WriteAddr);
  }
  SPI_FLASH_BufferWrite(pBuffer, WriteAddr, NumByteToWrite);

  rvalue = MSD_RESPONSE_NO_ERROR;

  return rvalue;
}

/*******************************************************************************
* Function Name  : MSD_ReadBlock
* Description    : Reads a block of data from the MSD.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the MSD.
*                  - ReadAddr : MSD's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the MSD.
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_ReadBlock(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint32_t i = 0;
	uint8_t rvalue = MSD_RESPONSE_FAILURE;
	
	/* MSD chip select low */
	MSD_CS_LOW();
	Delay_us(200);
	MSD_WriteByte(CMD_READ);
	/* Send ReadAddr high nibble address byte to read from */
	MSD_WriteByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	MSD_WriteByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	MSD_WriteByte(ReadAddr & 0xFF);
	
	/* Read the MSD block data : read NumByteToRead data */
	for (i = 0; i < NumByteToRead; i++)
	{
		/* Save the received data */
		*pBuffer = MSD_ReadByte();
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
	}
	/* Set response value to success */
	rvalue = MSD_RESPONSE_NO_ERROR;
	
	/* MSD chip select high */
	MSD_CS_HIGH();
	Delay_us(200);
	return rvalue;
}

/*******************************************************************************
* Function Name  : MSD_WriteBuffer
* Description    : Writes many blocks on the MSD
* Input          : - pBuffer : pointer to the buffer containing the data to be
*                    written on the MSD.
*                  - WriteAddr : address to write on.
*                  - NumByteToWrite: number of data to write
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint32_t i = 0, NbrOfBlock = 0, Offset = 0;
  uint8_t rvalue = MSD_RESPONSE_FAILURE;

  /* Calculate number of blocks to write */
  NbrOfBlock = NumByteToWrite / BLOCK_SIZE;
  /* MSD chip select low */
  MSD_CS_LOW();

  /* Data transfer */
  while (NbrOfBlock --)
  {
    /* Send CMD24 (MSD_WRITE_BLOCK) to write blocks */
    MSD_SendCmd(MSD_WRITE_BLOCK, WriteAddr + Offset, 0xFF);

    /* Check if the MSD acknowledged the write block command: R1 response (0x00: no errors) */
    if (MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
    {
      return MSD_RESPONSE_FAILURE;
    }
    /* Send dummy byte */
    MSD_WriteByte(DUMMY_BYTE);
    /* Send the data token to signify the start of the data */
    MSD_WriteByte(MSD_START_DATA_SINGLE_BLOCK_WRITE);
    /* Write the block data to MSD : write count data by block */
    for (i = 0; i < BLOCK_SIZE; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /* Set next write address */
    Offset += 512;
    /* Put CRC bytes (not really needed by us, but required by MSD) */
    MSD_ReadByte();
    MSD_ReadByte();
    /* Read data response */
    if (MSD_GetDataResponse() == MSD_DATA_OK)
    {
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_RESPONSE_FAILURE;
    }
  }

  /* MSD chip select high */
  MSD_CS_HIGH();
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY_BYTE);
  /* Returns the reponse */
  return rvalue;
}

/*******************************************************************************
* Function Name  : MSD_ReadBuffer
* Description    : Reads multiple block of data from the MSD.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the MSD.
*                  - ReadAddr : MSD's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the MSD.
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  return MSD_ReadBlock(pBuffer,ReadAddr,NumByteToRead);
}





/*******************************************************************************
* Function Name  : MSD_SendCmd
* Description    : Send 5 bytes command to the MSD card.
* Input          : - Cmd: the user expected command to send to MSD card
*                  - Arg: the command argument
*                  - Crc: the CRC
* Output         : None
* Return         : None
*******************************************************************************/
void MSD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
  uint32_t i = 0x00;
  uint8_t Frame[6];

  /* Construct byte1 */
  Frame[0] = (Cmd | 0x40);
  /* Construct byte2 */
  Frame[1] = (uint8_t)(Arg >> 24);
  /* Construct byte3 */
  Frame[2] = (uint8_t)(Arg >> 16);
  /* Construct byte4 */
  Frame[3] = (uint8_t)(Arg >> 8);
  /* Construct byte5 */
  Frame[4] = (uint8_t)(Arg);
  /* Construct CRC: byte6 */
  Frame[5] = (Crc);

  /* Send the Cmd bytes */
  for (i = 0; i < 6; i++)
  {
    MSD_WriteByte(Frame[i]);
  }
}

/*******************************************************************************
* Function Name  : MSD_GetDataResponse
* Description    : Get MSD card data response.
* Input          : None
* Output         : None
* Return         : The MSD status: Read data response xxx0<status>1
*                   - status 010: Data accecpted
*                   - status 101: Data rejected due to a crc error
*                   - status 110: Data rejected due to a Write error.
*                   - status 111: Data rejected due to other error.
*******************************************************************************/
uint8_t MSD_GetDataResponse(void)
{
  uint32_t i = 0;
  uint8_t response, rvalue;

  while (i <= 64)
  {
    /* Read resonse */
    response = MSD_ReadByte();
    /* Mask unused bits */
    response &= 0x1F;

    switch (response)
    {
      case MSD_DATA_OK:
      {
        rvalue = MSD_DATA_OK;
        break;
      }

      case MSD_DATA_CRC_ERROR:
        return MSD_DATA_CRC_ERROR;

      case MSD_DATA_WRITE_ERROR:
        return MSD_DATA_WRITE_ERROR;

      default:
      {
        rvalue = MSD_DATA_OTHER_ERROR;
        break;
      }
    }
    /* Exit loop in case of data ok */
    if (rvalue == MSD_DATA_OK)
      break;
    /* Increment loop counter */
    i++;
  }
  /* Wait null data */
  while (MSD_ReadByte() == 0);
  /* Return response */
  return response;
}

/*******************************************************************************
* Function Name  : MSD_GetResponse
* Description    : Returns the MSD response.
* Input          : None
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_GetResponse(uint8_t Response)
{
  uint32_t Count = 0xFFF;

  /* Check if response is got or a timeout is happen */
  while ((MSD_ReadByte() != Response) && Count)
  {
    Count--;
  }

  if (Count == 0)
  {
    /* After time out */
    return MSD_RESPONSE_FAILURE;
  }
  else
  {
    /* Right response got */
    return MSD_RESPONSE_NO_ERROR;
  }
}

/*******************************************************************************
* Function Name  : MSD_GetStatus
* Description    : Returns the MSD status.
* Input          : None
* Output         : None
* Return         : The MSD status.
*******************************************************************************/
uint16_t MSD_GetStatus(void)
{
  uint16_t Status = 0;

  /* MSD chip select low */
  MSD_CS_LOW();
  /* Send CMD13 (MSD_SEND_STATUS) to get MSD status */
  MSD_WriteByte(CMD_RDSR);
  Status = MSD_ReadByte();

  /* MSD chip select high */
  MSD_CS_HIGH();
  /* Send dummy byte 0xFF */

  return Status;
}

/*******************************************************************************
* Function Name  : MSD_GoIdleState
* Description    : Put MSD in Idle state.
* Input          : None
* Output         : None
* Return         : The MSD Response: - MSD_RESPONSE_FAILURE: Sequence failed
*                                    - MSD_RESPONSE_NO_ERROR: Sequence succeed
*******************************************************************************/
uint8_t MSD_GoIdleState(void)
{
  uint32_t FlashID = 0;
  FlashID = SPI_FLASH_ReadID();
  if (FlashID == FLASH_ID)
  {
   return MSD_RESPONSE_NO_ERROR;
  }
 
  return MSD_RESPONSE_FAILURE;
}

/*******************************************************************************
* Function Name  : MSD_WriteByte
* Description    : Write a byte on the MSD.
* Input          : Data: byte to send.
* Output         : None
* Return         : None.
*******************************************************************************/
void MSD_WriteByte(uint8_t Data)
{
  /* Wait until the transmit buffer is empty */
  while (SPI_I2S_GetFlagStatus(MSD_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_I2S_SendData(MSD_SPI, Data);

  /* Wait until a data is received */
  while (SPI_I2S_GetFlagStatus(MSD_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Get the received data */
  SPI_I2S_ReceiveData(MSD_SPI);
  
}

/*******************************************************************************
* Function Name  : MSD_ReadByte
* Description    : Read a byte from the MSD.
* Input          : None.
* Output         : None
* Return         : The received byte.
*******************************************************************************/
uint8_t MSD_ReadByte(void)
{
  uint8_t Data = 0;

  /* Wait until the transmit buffer is empty */
  while (SPI_I2S_GetFlagStatus(MSD_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_I2S_SendData(MSD_SPI, DUMMY_BYTE);

  /* Wait until a data is received */
  while (SPI_I2S_GetFlagStatus(MSD_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Get the received data */
  Data = SPI_I2S_ReceiveData(MSD_SPI);

  /* Return the shifted data */
  return Data;
}

/*******************************************************************************
* Function Name  : SPI_Config
* Description    : Initializes the MSD_SPI and CS pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* MSD_SPI_PORT and MSD_CS_PORT Periph clock enable */
//  RCC_APB2PeriphClockCmd(MSD_SPI_GPIO_PORT_CLOCK | MSD_CS_GPIO_PORT_CLOCK , ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
  						|RCC_APB2Periph_GPIOB 
						| RCC_APB2Periph_GPIOC
						|RCC_APB2Periph_GPIOD
						| RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

  /* MSD_SPI Periph clock enable */
#ifdef USE_STM3210B_EVAL
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
#elif defined (USE_STM3210C_EVAL)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
#endif /* USE_STM3210B_EVAL */

  /* Remap the SPI pins if needed */
#ifdef SPI_REMAPPED
  GPIO_PinRemapConfig(MSD_SPI_GPIO_REMAP, ENABLE);
#endif /* SPI_REMAPPED */

  /* Configure SPI pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  /* Configure SPI pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		  /* Configure SPI pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	

  /* Configure I/O for Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  MSD_CS_LOW();
//  MSD_CS_HIGH() ;
  /* MSD_SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* MSD_SPI enable */
  SPI_Cmd(SPI2, ENABLE);
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : ¶Áµ½flashÐ¾Æ¬IDºÅ¡£
* Input          : None
* Output         : None
* Return         : ID
*******************************************************************************/

 uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  MSD_CS_LOW();
//  Delay_us(500);
  /* Send "RDID " instruction */
  MSD_WriteByte(CMD_RPD_HPM);

  /* Read a byte from the FLASH */
  MSD_ReadByte();

  /* Read a byte from the FLASH */
  MSD_ReadByte();

  /* Read a byte from the FLASH */
  MSD_ReadByte();

  Temp = MSD_ReadByte();

  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : ¶Áµ½flashÐ¾Æ¬IDºÅ¡£
* Input          : None
* Output         : None
* Return         : ID
*******************************************************************************/
uint32_t SPI_FLASH_JEDEC_ID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  MSD_CS_LOW();

  /* Send "RDID " instruction */
  MSD_WriteByte(CMD_RDID);

  /* Read a byte from the FLASH */
  Temp0 = MSD_ReadByte();

  /* Read a byte from the FLASH */
  Temp1 = MSD_ReadByte();

  /* Read a byte from the FLASH */
  Temp2 = MSD_ReadByte();

//  Temp3 = MSD_ReadByte();

  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);
//  Temp = (Temp0 << 24)|(Temp1 << 16) | (Temp2 << 8) | Temp3;
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}


void SPI_FLASH_WriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	MSD_CS_LOW();
//	Delay_us(500);
	/* Send "Write Enable" instruction */
	MSD_WriteByte(CMD_WREN);
	
	/* Deselect the FLASH: Chip Select high */
	MSD_CS_HIGH();
//	Delay_us(500);
	
	SPI_FLASH_WaitForWriteEnd();
	
	SPI_FLASH_WaitForWriteEnable();
}

void SPI_FLASH_SectorUnprotect(uint32_t SectorAddr)
{
  SPI_FLASH_WriteEnable();  
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  
  MSD_CS_LOW();
//  Delay_us(500);
  /* Send Sector Erase instruction */
  MSD_WriteByte(CMD_UPS);
  /* Send SectorAddr high nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  MSD_WriteByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
//#define BLOCK_SIZE    512
//#define SPI_FLASH_PAGESIZE    256
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* Send write enable instruction */

  SPI_FLASH_WriteEnable();
   
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  
  MSD_CS_LOW();
//  Delay_us(500);
  /* Send Sector Erase instruction */
  MSD_WriteByte(CMD_SE);
  /* Send SectorAddr high nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  MSD_WriteByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_Block64Erase(uint32_t SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */ 
  MSD_CS_LOW();
//  Delay_us(500);
  /* Send Sector Erase instruction */
  MSD_WriteByte(CMD_BE64);
  /* Send SectorAddr high nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  MSD_WriteByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  MSD_WriteByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}



void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t flashstatus = 0;
	unsigned int uiLoop = 0;	
	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Select the FLASH: Chip Select low */
		MSD_CS_LOW();
//		Delay_us(500);
		/* Send "Read Status Register" instruction */
		MSD_WriteByte(CMD_RDSR);
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		flashstatus = MSD_ReadByte();
		/* Deselect the FLASH: Chip Select high */
		MSD_CS_HIGH();
//		Delay_us(500);
		uiLoop++;
		if(uiLoop > 45000000)
		{
			return;
		}
		else{}
	}
	while ((flashstatus & BUSY_FLAG));/* Write in progress */
}

void SPI_FLASH_WaitForWriteEnable(void)
{
	uint8_t flashstatus = 0;
	unsigned int uiLoop = 0;	
	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Select the FLASH: Chip Select low */
		MSD_CS_LOW();
//		Delay_us(500);
		/* Send "Read Status Register" instruction */
		MSD_WriteByte(CMD_RDSR);
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		flashstatus = MSD_ReadByte();
		/* Deselect the FLASH: Chip Select high */
		MSD_CS_HIGH();
//		Delay_us(500);
		uiLoop++;
		if(uiLoop > 45000000)
		{
			return;
		}
		else{}
	}
	while((flashstatus & WE_FLAG) == RESET);/* Write in progress */
}

void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  MSD_CS_LOW();
//  Delay_us(500);
  /* Send "Write to Memory " instruction */
  MSD_WriteByte(CMD_WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  MSD_WriteByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  MSD_WriteByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  MSD_WriteByte(WriteAddr & 0xFF);

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    MSD_WriteByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  MSD_CS_HIGH();
//  Delay_us(500);
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}


void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PAGESIZE;
  count = SPI_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        pBuffer += SPI_FLASH_PAGESIZE;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
        WriteAddr +=  SPI_FLASH_PAGESIZE;
        pBuffer += SPI_FLASH_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
