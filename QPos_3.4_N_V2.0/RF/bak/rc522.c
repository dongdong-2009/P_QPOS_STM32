#include <null.h>
#include "rc522.h"
#include "Rc522Reg.h"
#include "ErrCode.h"
#include "OpCtrl.h"
#include "HAL_SPI.h"
#include "MID_Display.h"
#include "Global_Var.h"
#include "APP_Error.h"
#include "APP_PBOC_Pay.h"
#include "HAL_Buzzer.h"

unsigned char *SearchTag(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag);
MfCmdInfo MInfo;
MfCmdInfo *MpIsrInfo = NULL;
unsigned char SerBuffer[20];

#define HS32U2_EMPCR   (* (volatile unsigned int * )0x00c20010) 
#define HS32U2_GPDDRF  (* (volatile unsigned char * )0x00c00014)
#define HS32U2_GPDORF  (* (volatile unsigned char * )0x00c00008)

void RC_RST(char PinHL)
{
	if(PinHL)
	{
		HS32U2_GPDORF |= 0x08;
	}
	else if(!PinHL)
	{
		HS32U2_GPDORF  &= 0xf7;
	}
	
}

 
//------------------------------------------
// 读慈胍桓鯾yte 
//------------------------------------------
unsigned char SPIReadByte(void)
{
	unsigned char tmp;
    
    tmp = HAL_SPI_ReadByte(SPI_PORT_RFID);
 //  	HAL_Delay_us(100);
    
	// Finally return the read data
	return tmp;
} 
//------------------------------------------
// 列慈胍桓鯾yte 
//------------------------------------------
void SPIWriteByte(unsigned char SPIData)
{
	HAL_SPI_WriteByte(SPI_PORT_RFID, SPIData);
//	HAL_Delay_us(100);
}                          


/*************************************************
Function:       RcSetReg
Description:	Write data to register of RC522
Parameter:
	RegAddr       The address of the regitster
	RegVal        The value to be writen
Return:
	None
**************************************************/
void RcSetReg(unsigned char RegAddr, unsigned char RegVal)
{
   unsigned char ucAddr;
    HAL_SPI_Init(SPI_PORT_RFID);
	NSS_LOW;
    HAL_Delay_us(100);
    	
    ucAddr = ((RegAddr<<1)&0x7E);

	SPIWriteByte(ucAddr);
	SPIWriteByte(RegVal);
	
	HAL_Delay_us(10);
 	NSS_HIGH;
}


/*************************************************
Function:       RcGetReg
Description:
	Write data to register of RC522
Parameter:
	RegAddr       The address of the regitster to be readed
Return:
	The value of the specify register

**************************************************/
unsigned char RcGetReg(unsigned char RegAddr)
{
	unsigned char ucAddr;
    unsigned char ucResult=0;
    
    HAL_SPI_Init(SPI_PORT_RFID);
	NSS_LOW;
   HAL_Delay_us(100);
  	
    ucAddr = ((RegAddr<<1)&0x7E)|0x80;
	
	SPIWriteByte(ucAddr);
	ucResult=SPIReadByte();
	
	HAL_Delay_us(10);
 	NSS_HIGH;
	
   return ucResult;
}


/*************************************************
Function:       RcModifyReg
Description:
Change some bits of the register
Parameter:
RegAddr       The address of the regitster
ModifyVal        The value to change to, set or clr?

MaskByte      Only the corresponding bit '1' is valid,
Return:
None
**************************************************/
void RcModifyReg(unsigned char RegAddr, unsigned char ModifyVal, unsigned char MaskByte)
{
	unsigned char RegVal;

	RegVal = RcGetReg(RegAddr);
	if(ModifyVal)
	{	// set some bits;
		RegVal |= MaskByte;
	}
	else
	{	// clear some bits;
		RegVal &= (~MaskByte);
	}
	RcSetReg(RegAddr, RegVal);
}


/*************************************************
Function:       SetPowerDown
Description:
	set the rc522 enter or exit power down mode
Parameter:
	ucFlag     0   --  exit power down mode
			   !0  --  enter power down mode
Return:
	short      status of implement
**************************************************/
void SetPowerDown(unsigned char ucFlag)
{
	unsigned char RegVal;
	/*
	Note: The bit Power Down can not be set when the SoftReset command has been activated.
	*/
	if(ucFlag)
	{
		RegVal = RcGetReg(JREG_COMMAND);  //enter power down mode
		RegVal |= 0x10;
		RcSetReg(JREG_COMMAND, RegVal);
	}
	else
	{
		RegVal = RcGetReg(JREG_COMMAND);  //disable power down mode
		RegVal &= (~0x10);
		RcSetReg(JREG_COMMAND, RegVal);
	}
}


/*************************************************
Function:       SetTimeOut
Description:
	Adjusts the timeout in 100us steps
Parameter:
	uiMicroSeconds   the time to set the timer(100us as a step)
Return:
	short      status of implement
**************************************************/
short SetTimeOut(unsigned int uiMicroSeconds)
{
	unsigned int RegVal;
	unsigned char TmpVal;

	RegVal = uiMicroSeconds / 100;

	/*
	 * NOTE: The supported hardware range is bigger, since the prescaler here
	 * is always set to 100 us.
	 */
	if(RegVal >= 0x7fff)
	{
		return STATUS_INVALID_PARAMETER;

	}
	RcModifyReg(JREG_TMODE, 1, JBIT_TAUTO);

	RcSetReg(JREG_TPRESCALER, 0xa6);

	TmpVal = RcGetReg(JREG_TMODE);
	TmpVal &= 0xf0;

	TmpVal |= 0x02;
	RcSetReg(JREG_TMODE, TmpVal);//82

	RcSetReg(JREG_TRELOADLO, ((unsigned char)(RegVal&0xff)));
	RcSetReg(JREG_TRELOADHI, ((unsigned char)((RegVal>>8)&0xff)));

	return STATUS_SUCCESS;
}


/*************************************************
Function:       ChangeJoinerBaudRate
Description:
	Changes the serial speed of the RC522.
	Note that the speed of the host interface (UART on PC) has to be also set to the
	appropriate one.
Parameter:
	baudrate   new baudrate for rc522
Return:
	short      status of implement
**************************************************/
short ChangeJoinerBaudRate(unsigned long baudrate)
{
	short   status = STATUS_SUCCESS;
	unsigned char setRegVal;

	switch(baudrate)
	{
	case 9600:
		setRegVal = 0xEB;
		break;

	case 14400:
		setRegVal = 0xDA;
		break;

	case 19200:
		setRegVal = 0xCB;
		break;

	case 38400:
		setRegVal = 0xAB;
		break;

	case 57600:
		setRegVal = 0x9A;
		break;

	case 115200:
		setRegVal = 0x7A;
		break;

	case 128000:
		setRegVal = 0x74;
		break;

	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}

	/* Set the appropriate value */

	if (status == STATUS_SUCCESS)
		RcSetReg(JREG_SERIALSPEED,setRegVal);

	/* Now the RC522 is set to the new speed*/

	return status;
}

/*************************************************
Function:       Rc522Init
Description:
	initialize rc522 as a mifare reader
Parameter:
	NONE
Return:
	short      -1 -- initialize fail
				0 -- success
**************************************************/
short Rc522Init(void)
{
	unsigned char RegVal;
	int i;
	int nWait;

	HAL_SPI_Init(SPI_PORT_RFID);
	
	HS32U2_EMPCR  |= 0x20; //设置PortF为GPIO 
	HS32U2_GPDDRF |= 0x08; //设置PortF.0(A0)及PortF.1(LCSB)为输出

	RC_RST(1) ;						//RC523 复位引脚拉高
	HAL_Delay_us(100);				//100ms延时
	RC_RST(0) ;					    //RC523 复位引脚拉低
	HAL_Delay_us(100);				//100ms延时
	RC_RST(1) ;						//RC523 复位引脚拉高


	RcSetReg(JREG_COMMAND, 0x0f); /*reset the RC522*/

	HAL_Delay_us(100);


	/* disable Crypto1 bit*/
	//RcModifyReg(JREG_STATUS2, 0, JBIT_CRYPTO1ON);

	/* do not touch bits: InvMod in register TxMode */
	//RegVal = RcGetReg(JREG_TXMODE);
	//RegVal = (unsigned char)(RegVal & JBIT_INVMOD);
	//RegVal = (unsigned char)(RegVal | JBIT_CRCEN | (RCO_VAL_RF106K << JOINER_SPEED_SHL_VALUE));

	/* TxCRCEn = 1; TxSpeed = x; InvMod, TXMix = 0; TxFraming = 0 */
	//RcSetReg(JREG_TXMODE, RegVal);

	/* do not touch bits: RxNoErr in register RxMode */
	//RegVal = RcGetReg(JREG_RXMODE);
	//RegVal = (unsigned char)(RegVal & JBIT_RXNOERR);
	//RegVal = (unsigned char)(RegVal | JBIT_CRCEN | (RCO_VAL_RF106K << JOINER_SPEED_SHL_VALUE));

	/* RxCRCEn = 1; RxSpeed = x; RxNoErr, RxMultiple = 0; TxFraming = 0 */
	//RcSetReg(JREG_RXMODE, RegVal);

	/* ADDIQ = 10b; FixIQ = 1; RFU = 0; TauRcv = 11b; TauSync = 01b */
	//RcSetReg(JREG_DEMOD, 0x6D);

	/* RxGain = 4*/
	RcSetReg(JREG_RFCFG, 0x78);

	/* do settings common for all functions */
	//RcSetReg(JREG_RXTRESHOLD, 0x55);    /* MinLevel = 5; CollLevel = 5 */

	//RcSetReg(JREG_MODWIDTH, 0x26);      /* Modwidth = 0x26 */
	//RcSetReg(JREG_GSN, 0xF0 | 0x04);     /* CWGsN = 0xF; ModGsN = 0x4 */

	/* Set the timer to auto mode, 5ms using operation control commands before HF is switched on to
	 * guarantee Iso14443-3 compliance of Polling procedure
	 */

	SetTimeOut(5000);

	/* Activate the field  */
	RcModifyReg(JREG_TXCONTROL, 1, JBIT_TX2RFEN | JBIT_TX1RFEN);
	//RcModifyReg(JREG_TXCONTROL, 1, /* JBIT_TX2RFEN | */ JBIT_TX1RFEN);

	/* start timer manually to check the initial waiting time */
	RcModifyReg(JREG_CONTROL, 1, JBIT_TSTARTNOW);


	/*
	 * After switching on the timer wait until the timer interrupt occures, so that
	 * the field is on and the 5ms delay have been passed.
	 */

	nWait = 1000;	/* maximum try, 1000 times */

	do {
		RegVal = RcGetReg(JREG_COMMIRQ);
		nWait--;
	} while(!(RegVal & JBIT_TIMERI) && (nWait > 0));

	if (nWait <= 0)
		return -1;

	/* Clear the status flag afterwards */
	RcSetReg(JREG_COMMIRQ, JBIT_TIMERI);

	/*
	 * Reset timer 1 ms using operation control commands (AutoMode and Prescaler are the same)
	 * set reload value
	 */
	SetTimeOut(5000);

	//RcSetReg(JREG_WATERLEVEL, 0x1A);
	//RcSetReg(JREG_TXSEL, 0x10);
	//RcSetReg(JREG_RXSEL, 0x84);

	/* Activate receiver for communication
	The RcvOff bit and the PowerDown bit are cleared, the command is not changed. */
	RcSetReg(JREG_COMMAND, JCMD_IDLE);

	RcSetReg(JREG_MODE, 0x3D);
	RcSetReg(JREG_TXASK, 0x40);

	/* Set timeout for REQA, ANTICOLL, SELECT to 200us */
	SetTimeOut(2000);


	return 0;
}

/*************************************************
Function:       M522PcdCmd
Description:
	implement a command
Parameter:
	cmd            command code
	ExchangeBuf    saved the data will be send to card and the data responed from the card
	info           some information for the command
Return:
	short      status of implement
**************************************************/
short M522PcdCmd(unsigned char cmd, unsigned char *ExchangeBuf, MfCmdInfo  *info)
{
	short          status    = STATUS_SUCCESS;
	short          istatus    = STATUS_SUCCESS;

	unsigned char  commIrqEn   = 0;
	unsigned char  divIrqEn    = 0;

	unsigned char  waitForComm = JBIT_ERRI | JBIT_TXI;
	unsigned char  waitForDiv  = 0;
	unsigned char  doReceive   = 0;
	unsigned char  i;
	unsigned char  getRegVal,setRegVal;

	unsigned char  nbytes, nbits;
	unsigned int counter;

	/*remove all Interrupt request flags that are used during function,
	  keep all other like they are*/
	RcSetReg(JREG_COMMIRQ, waitForComm);

	RcSetReg(JREG_DIVIRQ, waitForDiv);
	RcSetReg(JREG_FIFOLEVEL, JBIT_FLUSHBUFFER);

	/*disable command or set to transceive*/
	getRegVal = RcGetReg(JREG_COMMAND);
	if(cmd == JCMD_TRANSCEIVE)
	{
		/*re-init the transceive command*/
		setRegVal = (getRegVal & ~JMASK_COMMAND) | JCMD_TRANSCEIVE;
		RcSetReg(JREG_COMMAND, setRegVal);//0c
	}
	else
	{
		/*clear current command*/
		setRegVal = (getRegVal & ~JMASK_COMMAND);
		RcSetReg(JREG_COMMAND, setRegVal);
	}
	MpIsrInfo = info;

	switch(cmd)
	{
	case JCMD_IDLE:         /* values are 00, so return immediately after all bytes written to FIFO */
		waitForComm = 0;
		waitForDiv  = 0;
		break;

	case JCMD_CALCCRC:      /* values are 00, so return immediately after all bytes written to FIFO */
		waitForComm = 0;
		waitForDiv  = 0;
		break;

	case JCMD_TRANSMIT:
		commIrqEn = JBIT_TXI | JBIT_TIMERI;
		waitForComm = JBIT_TXI;
		break;

	case JCMD_RECEIVE:
		commIrqEn = JBIT_RXI | JBIT_TIMERI | JBIT_ERRI;
		waitForComm = JBIT_RXI | JBIT_TIMERI | JBIT_ERRI;
		doReceive = 1;
		break;

	case JCMD_TRANSCEIVE:
		commIrqEn = JBIT_RXI | JBIT_TIMERI | JBIT_ERRI;
		waitForComm = JBIT_RXI | JBIT_TIMERI | JBIT_ERRI;
		doReceive = 1;
		break;

	case JCMD_AUTHENT:
		commIrqEn = JBIT_IDLEI | JBIT_TIMERI | JBIT_ERRI;
		waitForComm = JBIT_IDLEI | JBIT_TIMERI | JBIT_ERRI;
		break;

	case JCMD_SOFTRESET:    /* values are 0x00 for IrqEn and for waitFor, nothing to do */
		waitForComm = 0;
		waitForDiv  = 0;
		break;

	default:
		status = STATUS_UNSUPPORTED_COMMAND;
		break;
	}

	if(status == STATUS_SUCCESS)
	{
		/* activate necessary communication Irq's */
		getRegVal = RcGetReg(JREG_COMMIEN);
		RcSetReg(JREG_COMMIEN, getRegVal | commIrqEn);

		/* activate necessary other Irq's */
		getRegVal = RcGetReg(JREG_DIVIEN);
		RcSetReg(JREG_DIVIEN, getRegVal | divIrqEn);

		/*write data to FIFO*/
		for(i=0; i<MpIsrInfo->nBytesToSend; i++)
		{
			RcSetReg(JREG_FIFODATA, ExchangeBuf[i]);
		}

		/*do seperate action if command to be executed is transceive*/
		if(cmd == JCMD_TRANSCEIVE)
		{
			/*TRx is always an endless loop, Initiator and Target must set STARTSEND.*/
			RcModifyReg(JREG_BITFRAMING, 1, JBIT_STARTSEND);
		}
		else
		{
			getRegVal = RcGetReg(JREG_COMMAND);
			RcSetReg(JREG_COMMAND, (getRegVal & ~JMASK_COMMAND) | cmd);
		}

		/*polling mode*/
		getRegVal = 0;
		setRegVal = 0;
		counter = 0; /*Just for debug*/
		while(!(waitForComm ? (waitForComm & setRegVal) : 1) ||
			!(waitForDiv ? (waitForDiv & getRegVal) :1))
		{
			setRegVal = RcGetReg(JREG_COMMIRQ);
			getRegVal = RcGetReg(JREG_DIVIRQ);
			counter ++;
			if(counter > 0x0100)
				break;
		}
		/*store IRQ bits for clearance afterwards*/
		waitForComm = (unsigned char)(waitForComm & setRegVal);
		waitForDiv  = (unsigned char)(waitForDiv & getRegVal);

		/*set status to Timer Interrupt occurence*/
		if (setRegVal & JBIT_TIMERI)
		{
			istatus = STATUS_IO_TIMEOUT;
		}
	}
	else
	{
		return status;
	}

	/*disable all interrupt sources*/
	RcModifyReg(JREG_COMMIEN, 0, commIrqEn);
	RcModifyReg(JREG_DIVIEN, 0, divIrqEn);

	if(doReceive && (istatus == STATUS_SUCCESS))
	{
		/*read number of bytes received (used for error check and correct transaction*/
		MpIsrInfo->nBytesReceived = RcGetReg(JREG_FIFOLEVEL);
		nbytes = MpIsrInfo->nBytesReceived;

		getRegVal = RcGetReg(JREG_CONTROL);
		MpIsrInfo->nBitsReceived = (unsigned char)(getRegVal & 0x07);
		nbits = (unsigned char) MpIsrInfo->nBitsReceived;

		getRegVal = RcGetReg(JREG_ERROR);

		/*set status information if error occured*/
		if(getRegVal)
		{
			if(getRegVal & JBIT_COLLERR)
				istatus = STATUS_COLLISION_ERROR;         /* Collision Error */
			else if(getRegVal & JBIT_PARITYERR)
				istatus = STATUS_PARITY_ERROR;            /* Parity Error */
			if(getRegVal & JBIT_PROTERR)
				istatus = STATUS_PROTOCOL_ERROR;          /* Protocoll Error */
			else if(getRegVal & JBIT_BUFFEROVFL)
				istatus = STATUS_BUFFER_OVERFLOW;         /* BufferOverflow Error */
			else if(getRegVal & JBIT_CRCERR)
			{   /* CRC Error */
				if(MpIsrInfo->nBytesReceived == 0x01 &&
					(MpIsrInfo->nBitsReceived == 0x04 ||
					MpIsrInfo->nBitsReceived == 0x00))
				{   /* CRC Error and only one byte received might be a Mifare (N)ACK */
					ExchangeBuf[0] = RcGetReg(JREG_FIFODATA);
					MpIsrInfo->nBytesReceived = 1;

					istatus = STATUS_ACK_SUPPOSED;        /* (N)ACK supposed */
				}
				else
					istatus = STATUS_CRC_ERROR;           /* CRC Error */
			}
			else if(getRegVal & JBIT_TEMPERR)
				istatus = STATUS_JOINER_TEMP_ERROR;       /* Temperature Error */
			if(getRegVal & JBIT_WRERR)
				istatus = STATUS_FIFO_WRITE_ERROR;        /* Error Writing to FIFO */
			if(istatus == STATUS_SUCCESS)
				istatus = STATUS_ERROR_NY_IMPLEMENTED;    /* Error not yet implemented, shall never occur! */

			/* if an error occured, clear error register before IRQ register */
			RcSetReg(JREG_ERROR, 0);
		}

		/*read data from FIFO and set response parameter*/
		if(istatus != STATUS_ACK_SUPPOSED)
		{
			for(i=0; i<MpIsrInfo->nBytesReceived; i++)
			{
				ExchangeBuf[i] = RcGetReg(JREG_FIFODATA);
			}

			/*in case of incomplete last byte reduce number of complete bytes by 1*/
			if(MpIsrInfo->nBitsReceived && MpIsrInfo->nBytesReceived)
				MpIsrInfo->nBytesReceived --;
		}

	}
	RcSetReg(JREG_COMMIRQ, waitForComm);
	RcSetReg(JREG_DIVIRQ, waitForDiv);
	RcSetReg(JREG_FIFOLEVEL, JBIT_FLUSHBUFFER);
	RcSetReg(JREG_COMMIRQ, JBIT_TIMERI);
	RcSetReg(JREG_BITFRAMING, 0);

	return istatus;
}

/*************************************************
Function:       Request
Description:
	REQA, request to see if have a ISO14443A card in the field
Parameter:
	req_code   command code(ISO14443_3_REQALL or ISO14443_3_REQIDL)
	atq        the buffer to save the answer to request from the card
				0x4400 = Mifare_UltraLight
				0x0400 = Mifare_One(S50)
				0x0200 = Mifare_One(S70)
				0x0800 = Mifare_Pro(X)
				0x4403 = Mifare_DESFire
Return:
	short      status of implement
**************************************************/
short Request(unsigned char req_code, unsigned char *atq)
{
	short  status = STATUS_SUCCESS;

	/************* initialize *****************/
	RcModifyReg(JREG_STATUS2, 0, JBIT_CRYPTO1ON);  /* disable Crypto if activated before */
	RcSetReg(JREG_COLL, JBIT_VALUESAFTERCOLL);  //active values after coll
	RcModifyReg(JREG_TXMODE, 0, JBIT_CRCEN);  //disable TxCRC and RxCRC

	RcModifyReg(JREG_RXMODE, 0, JBIT_CRCEN);
	RcSetReg(JREG_BITFRAMING, REQUEST_BITS);

	/* set necessary parameters for transmission */
	ResetInfo(MInfo);
	SerBuffer[0] = req_code;

	MInfo.nBytesToSend   = 1;

	/* Set timeout for REQA, ANTICOLL, SELECT*/
	SetTimeOut(300);

	status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);
	if (status)      // error occured
	{
		*atq = 0;
	}
	else
	{
		if (MInfo.nBytesReceived != 2) // 2 bytes expected
		{
			*atq = 0;
			status = STATUS_BITCOUNT_ERROR;
		}
		else
		{
			status = STATUS_SUCCESS;
			//memcpy(atq,SerBuffer,2);
			*atq = SerBuffer[0];
			*(atq + 1) = SerBuffer[1];
		}
	}

	return status;
}

/*************************************************
Function:       CascAnticoll
Description:
Functions to split anticollission and select internally.
NOTE: this founction is used internal only, and cannot call by application program

Parameter:
sel_code   command code
bitcount   the bit counter of known UID
snr        the UID have known
Return:
short      status of implement
**************************************************/
short CascAnticoll(unsigned char sel_code, unsigned char bitcount, unsigned char *snr)
{
	short status  = STATUS_SUCCESS;
	short istatus = STATUS_SUCCESS;

	unsigned char  i;
	unsigned char  complete = 0; /* signs end of anticollission loop */
	unsigned char  rbits    = 0; /* number of total received bits */
	unsigned char  nbits    = 0; /* */
	unsigned char  nbytes   = 0; /* */
	unsigned char  byteOffset;   /* stores offset for ID copy if uncomplete last byte was sent */

	/* initialise relvant bytes in internal buffer */
	for(i=2;i<7;i++)
		SerBuffer[i] = 0x00;

	/* disable TxCRC and RxCRC */

	RcModifyReg(JREG_TXMODE, 0, JBIT_CRCEN);
	RcModifyReg(JREG_RXMODE, 0, JBIT_CRCEN);

	/* activate deletion of bits after coll */
	RcSetReg(JREG_COLL, 0);

	/* init parameters for anticollision */

	while(!complete && (istatus == STATUS_SUCCESS))
	{
		/* if there is a communication problem on the RF interface, bcnt
		   could be larger than 32 - folowing loops will be defective. */
		if(bitcount > SINGLE_UID_LENGTH)
		{
			istatus = STATUS_INVALID_PARAMETER;
			continue;
		}

		/* prepare data length */

		nbits = (unsigned char)(bitcount > BITS_PER_BYTE);
		nbytes = (unsigned char)(bitcount / BITS_PER_BYTE);
		if(nbits)
			nbytes++;

		/* prepare data buffer */

		SerBuffer[0] = sel_code;
		SerBuffer[1] = (unsigned char)(NVB_MIN_PARAMETER + ((bitcount / BITS_PER_BYTE) << UPPER_NIBBLE_SHIFT) + nbits);
		for(i=0;i<nbytes;i++)
			SerBuffer[2+i] = snr[i];   /* copy serial number to tranmit buffer */


		/* set TxLastBits and RxAlign to number of bits sent */
		RcSetReg(JREG_BITFRAMING, (unsigned char)((nbits << UPPER_NIBBLE_SHIFT) | nbits));

		/* prepare data for common transceive */
		ResetInfo(MInfo);
		MInfo.nBytesToSend   = (unsigned char)(nbytes + 2);

		SetTimeOut(2000);
		status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

		if(istatus == STATUS_COLLISION_ERROR || istatus == STATUS_SUCCESS)
		{
			/* store number of received data bits and bytes internaly */
			rbits = (unsigned char)(MInfo.nBitsReceived + (MInfo.nBytesReceived << 3) - nbits);

			if((rbits + bitcount) > COMPLETE_UID_BITS)
			{
				istatus = STATUS_BITCOUNT_ERROR;
				continue;
			}

			/* increment number of bytes received if also some bits received */
			if(MInfo.nBitsReceived)
				MInfo.nBytesReceived++;

			/* reset offset for data copying */
			byteOffset = 0;
			/* if number of bits sent are not 0, write first received byte in last of sent */
			if(nbits)
			{   /* last byte transmitted and first byte received are the same */
				snr[nbytes - 1] |= SerBuffer[0];
				byteOffset++;
			}

			for(i=0;i<(4-nbytes);i++)
				snr[nbytes + i] = SerBuffer[i + byteOffset];

			if(istatus == STATUS_COLLISION_ERROR)
			{
				/* calculate new bitcount value */
				bitcount = (unsigned char)(bitcount + rbits);
				istatus = STATUS_SUCCESS;
			}
			else
			{
				if((snr[0] ^ snr[1] ^ snr[2] ^ snr[3]) != SerBuffer[i + byteOffset])
				{
					istatus = STATUS_WRONG_UID_CHECKBYTE;
					continue;
				}
				complete=1;
			}
		}
	}


	/* clear RxAlign and TxLastbits */
	RcSetReg(JREG_BITFRAMING, 0);

	/* activate values after coll */
	RcSetReg(JREG_COLL, JBIT_VALUESAFTERCOLL);
	return istatus;
}

/*************************************************
Function:       Select
Description:
	selecte a card to response the following command
	NOTE: this founction is used internal only, and cannot call by application program
Parameter:
	sel_code   command code
	snr        buffer to store the card UID
	sak        the byte to save the ACK from card
Return:
	short      status of implement
**************************************************/
short Select(unsigned char sel_code, unsigned char *snr, unsigned char *sak)
{
	short status = STATUS_SUCCESS;
	/* define local variables */
	unsigned char i;
	/* activate CRC */
	RcModifyReg(JREG_TXMODE, 1, JBIT_CRCEN);
	RcModifyReg(JREG_RXMODE, 1, JBIT_CRCEN);

	/* prepare data stream */
	SerBuffer[0] = sel_code;   /* command code */
	SerBuffer[1] = NVB_MAX_PARAMETER;       /* parameter */
	for(i=0;i<4;i++)
		SerBuffer[2+i] = snr[i];   /* serial numbner bytes 1 to 4 */
	SerBuffer[6] = (unsigned char)(snr[0] ^ snr[1] ^ snr[2] ^ snr[3]);   /* serial number check byte */

	/* prepare data for common transceive */
	ResetInfo(MInfo);
	MInfo.nBytesToSend   = 0x07;

	SetTimeOut(2000);
	status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

	if(status == STATUS_SUCCESS)
	{
		if(MInfo.nBytesReceived == SAK_LENGTH && MInfo.nBitsReceived == 0)
			*sak = SerBuffer[0];
		else
			status = STATUS_BITCOUNT_ERROR;
	}
	return status;
}


/*************************************************
Function:       AnticollSelect
Description:
	do anti-collision and selecte a card in one command
Parameter:
	bcnt	   already receive buffer size (not sure, comment by robertliu)
	snr        buffer to store the card UID
Return:
	short      status of implement
**************************************************/
short AnticollSelect(unsigned char bcnt, unsigned char *snr)
{
	unsigned char i;
	short status=STATUS_SUCCESS;
	unsigned char length, casc_code, length_in,sak,tmpSnr[12];

	length_in = bcnt;
	/* do loop for max. cascade level */
	for(i=0;i<MAX_CASCADE_LEVELS;i++)
	{
		if(length_in)
		{
			if(length_in > SINGLE_UID_LENGTH)
			{
				length = SINGLE_UID_LENGTH;
				length_in -= SINGLE_UID_LENGTH;
			}
			else
			{
				length = length_in;
				length_in = 0;
			}
		}
		else
		{
			length = 0;
		}

		switch(i)
		{
		case 1:  casc_code = SELECT_CASCADE_LEVEL_2;
			memcpy(snr, tmpSnr+1,3);
			break;
		case 2:  casc_code = SELECT_CASCADE_LEVEL_3;
			break;
		default: casc_code = SELECT_CASCADE_LEVEL_1;
			break;
		}

		if(length != SINGLE_UID_LENGTH && status == STATUS_SUCCESS)
			/* do anticollission with selected level */
			status = CascAnticoll(casc_code, length, tmpSnr + i * 4);

		if(status == STATUS_SUCCESS)
		{
			/* select 1st cascade level uid */
			status = Select(casc_code, tmpSnr + i * 4, &sak);

			/* check if further cascade level is used */
			if(status == STATUS_SUCCESS)
			{
				/* increase number of received bits in parameter */
				bcnt = (unsigned char)(SINGLE_UID_LENGTH * (i + 1)); //the actually length of the UID, you can return it.

				/* check if cascade bit is set */
				if(!(sak & CASCADE_BIT))
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	switch(i)
	{
	case 1:  memcpy(snr+3, tmpSnr+4,4);     //copy UID to snr buffer without CT(0x88)
		break;
	case 2:  memcpy(snr+6, tmpSnr+4,4);
		break;
	default: memcpy(snr, tmpSnr,4);
		break;
	}
	return status;
}

/*************************************************
Function:       HaltA
Description:
	halt the current selected card
Parameter:
	NONE
Return:
	short      status of implement
**************************************************/
short HaltA(void)
{
	short  status = STATUS_SUCCESS;

	/* initialise data buffer */
	SerBuffer[0] = HALTA_CMD;
	SerBuffer[1] = HALTA_PARAM;

	ResetInfo(MInfo);
	MInfo.nBytesToSend   = HALTA_CMD_LENGTH;

	SetTimeOut(200);
	status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

	if(status == STATUS_IO_TIMEOUT)
		status = STATUS_SUCCESS;

	return status;
}

void DeSelect()
{
	/* initialise data buffer */
	SerBuffer[0] = 0xca;
	SerBuffer[1] = 0x00;

	ResetInfo(MInfo);
	MInfo.nBytesToSend   = HALTA_CMD_LENGTH;
	
	RcModifyReg(JREG_TXMODE, 1, JBIT_CRCEN);
	RcModifyReg(JREG_RXMODE, 1, JBIT_CRCEN);

	SetTimeOut(2000);

	M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

	return;
}

/*************************************************
Function:       Authentication
Description:
	authentication the password for a sector of mifare card
Parameter:
	auth_mode  specify key A or key B -- MIFARE_AUTHENT_A or MIFARE_AUTHENT_A
	key        the buffer stored the key(6 bytes)
	snr        the buffer stored the selected card's UID
	addr       the block address of a sector
Return:
	short      status of implement
*************************************************
short Authentication(unsigned char auth_mode, const unsigned char *key, const unsigned char *snr, unsigned char addr)
{
	short status;
	unsigned char i = 0, RegVal;

	ResetInfo(MInfo);

	SerBuffer[0] = auth_mode;      //key A or key B
	SerBuffer[1] = addr;           //address to authentication
	memcpy(SerBuffer+2,key,6);     //6 bytes key
	memcpy(SerBuffer+8,snr,4);     //4 bytes UID

	MInfo.nBytesToSend = 12;       //length
	SetTimeOut(2000);
	status = M522PcdCmd(JCMD_AUTHENT, SerBuffer, &MInfo);
	if(status == STATUS_SUCCESS)
	{
		RegVal = RcGetReg(JREG_STATUS2);

		if((RegVal & 0x0f) != 0x08)
			status = STATUS_AUTHENT_ERROR;
	}
	return status;
}
*/

/*************************************************
Function:       Read
Description:
	read 16 bytes data from a block
Parameter:
	addr       the address of the block
	_data      the buffer to save the 16 bytes data

Return:
	short      status of implement
**************************************************
short Read(unsigned char addr, unsigned char *_data)
{
	short status = STATUS_SUCCESS;
	char tmp    = 0;

	ResetInfo(MInfo);
	SerBuffer[0] = MIFARE_READ;
	SerBuffer[1] = addr;
	MInfo.nBytesToSend   = 2;
	SetTimeOut(5000);

	status = M522PcdCmd(JCMD_TRANSCEIVE,
		SerBuffer,
		&MInfo);

	if (status != STATUS_SUCCESS)
	{
		if (status != STATUS_IO_TIMEOUT )     // no timeout occured
		{
			if (MInfo.nBitsReceived == 4)
			{
				SerBuffer[0] &= 0x0f;
				if ((SerBuffer[0] & 0x0a) == 0)

				{
					status = STATUS_AUTHENT_ERROR;
				}
				else
				{
					status = STATUS_INVALID_FORMAT;

				}
			}
		}
		memset(_data,0,16);
	}
	else   // Response Processing

	{
		if (MInfo.nBytesReceived != 16)
		{
			status = STATUS_ACCESS_DENIED;
			memset(_data,0,16);
		}

		else
		{
			memcpy(_data,SerBuffer,16);
		}
	}
	return status;

}
*/

/*************************************************
Function:       Write
Description:
	write 16 bytes data to a block
Parameter:
	addr       the address of the block
	_data      the data to write
Return:
	short      status of implement
**************************************************
short Write( unsigned char addr, unsigned char *_data)
{
	short status = STATUS_SUCCESS;

	ResetInfo(MInfo);
	SerBuffer[0] = MIFARE_WRITE;
	SerBuffer[1] = addr;
	MInfo.nBytesToSend   = 2;
	SetTimeOut(10000);
	status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

	if (status != STATUS_IO_TIMEOUT)
	{
		if (MInfo.nBitsReceived != 4)
		{
			status = STATUS_BITCOUNT_ERROR;
		}
		else
		{
			SerBuffer[0] &= 0x0f;

			if ((SerBuffer[0] & 0x0a) == 0)
			{
				status = STATUS_AUTHENT_ERROR;
			}
			else
			{
				if (SerBuffer[0] == 0x0a)
				{
					status = STATUS_SUCCESS;
				}
				else
				{
					status = STATUS_INVALID_FORMAT;
				}
			}
		}
	}


	if ( status == STATUS_SUCCESS)
	{
		SetTimeOut(5000);
		ResetInfo(MInfo);

		memcpy(SerBuffer,_data,16);
		MInfo.nBytesToSend   = 16;
		status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

		if (status & 0x80)
		{
			status = STATUS_IO_TIMEOUT;
		}
		else
		{
			if (MInfo.nBitsReceived != 4)
			{
				status = STATUS_BITCOUNT_ERROR;
			}
			else
			{
				SerBuffer[0] &= 0x0f;
				if ((SerBuffer[0] & 0x0a) == 0)
				{
					status = STATUS_ACCESS_DENIED;

				}
				else
				{
					if (SerBuffer[0] == 0x0a)
					{
						status = STATUS_SUCCESS;
					}
					else
					{
						status = STATUS_INVALID_FORMAT;
					}
				}
			}
		}
	}
	return status;
}
*/
/*************************************************
Function:       ValueOper
Description:
	block value operation function, increment or decrement the block value
	and transfer to a block
Parameter:
	OperMode   MIFARE_INCREMENT or MIFARE_DECREMENT
	addr       the address of the block
	value      the value to be increment or decrement
	trans_addr the address to save the resulet of increment or decrement
Return:
	short      status of implement
*************************************************
short ValueOper(unsigned char OperMode, unsigned char addr, unsigned char *value, unsigned char trans_addr)
{
	short status = STATUS_SUCCESS;

	ResetInfo(MInfo);
	SerBuffer[0] = OperMode;
	SerBuffer[1] = addr;
	MInfo.nBytesToSend   = 2;
	SetTimeOut(5000);
	status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

	if (status != STATUS_IO_TIMEOUT)
	{
		if (MInfo.nBitsReceived != 4)
		{
			status = STATUS_BITCOUNT_ERROR;
		}
		else
		{
			SerBuffer[0] &= 0x0f;
			switch(SerBuffer[0])
			{
			case 0x00:
				status = STATUS_AUTHENT_ERROR;
				break;
			case 0x0a:
				status = STATUS_SUCCESS;
				break;
			case 0x01:
				status = STATUS_INVALID_FORMAT;
				break;
			default:
				status = STATUS_OTHER_ERROR;
				break;
			}
		}
	}

	if ( status == STATUS_SUCCESS)
	{
		SetTimeOut(10000);
		ResetInfo(MInfo);
		memcpy(SerBuffer,value,4);
		MInfo.nBytesToSend   = 4;
		status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

		if (status == STATUS_IO_TIMEOUT||(OperMode == MIFARE_DECREMENT && OperMode == MIFARE_DECREMENT))
		{
			status = STATUS_SUCCESS;
		}
		else
		{
			status = STATUS_OTHER_ERROR;
		}
	}

	if ( status == STATUS_SUCCESS)
	{
		ResetInfo(MInfo);
		SerBuffer[0] = MIFARE_TRANSFER;
		SerBuffer[1] = trans_addr;
		MInfo.nBytesToSend   = 2;
		status = M522PcdCmd(JCMD_TRANSCEIVE, SerBuffer, &MInfo);

		if (status != STATUS_SUCCESS && (SerBuffer[0] & MIFARE_ACK_MASK))
		{
			status = STATUS_SUCCESS;
		}
		else
		{
			status = STATUS_OTHER_ERROR;
		}
	}

	return status;
}
*/
/*************************************************
Function:       InitBlock
Description:
	initialize a block value
Parameter:
	addr       the address of the block
	value      the value to be initialized, 4 bytes buffer
Return:
	short      status of implement
*************************************************
short InitBlock(unsigned char addr,unsigned char *value)
{

	unsigned char tmp[16],i;
	short status = STATUS_SUCCESS;
	for(i=0;i<4;i++)
	{
		tmp[i]=value[i];
		tmp[i+4]=255-value[i];

		tmp[i+8]=value[i];
	}
	tmp[12]=addr;
	tmp[13]=255-addr;
	tmp[14]=tmp[12];
	tmp[15]=tmp[13];

	status=Write(addr,tmp);
	return status;
}

*/
unsigned short ExchangeBlock(
			unsigned char *pDataBuf, 
			unsigned char ucSendLen,
			unsigned char *pDataOut, 
			unsigned char *pRcvLen,
			unsigned int delay
			)
{
	short status = STATUS_SUCCESS;
	/* define local variables */
	unsigned char i;
	/* activate CRC */
	RcModifyReg(JREG_TXMODE, 1, JBIT_CRCEN);
	RcModifyReg(JREG_RXMODE, 1, JBIT_CRCEN);

	
	/* prepare data for common transceive */
	ResetInfo(MInfo);
	MInfo.nBytesToSend = ucSendLen;

	SetTimeOut(delay);
	status = M522PcdCmd(JCMD_TRANSCEIVE, pDataBuf, &MInfo);

	if(status == STATUS_SUCCESS)
	{
		*pRcvLen = MInfo.nBytesReceived;
		memcpy(pDataOut, pDataBuf,MInfo.nBytesReceived);
	}
	else
	{
		*pRcvLen = 0;
	}
	
	return status;
}

unsigned char ISO14443A_Start_Cpu(void)
{
	unsigned char Result;
	unsigned char RFID_Data_buf[100];
	unsigned char RFID_Length;
	
	RFID_Data_buf[0] = 0xe0;
	RFID_Data_buf[1] = 0x50;
	RFID_Data_buf[2] = 0x00;
	RFID_Data_buf[3] = 0x00;
	RFID_Data_buf[4] = 0x00;
	RFID_Length = 0xff;

	Result = ExchangeBlock(RFID_Data_buf,2,RFID_Data_buf, &RFID_Length, 5000);

	if(Result == STATUS_SUCCESS)
	{
	 	RFID_Data_buf[0] = 0xd0;
		RFID_Data_buf[1] = 0x11;
		RFID_Data_buf[2] = 0x00;
		RFID_Data_buf[3] = 0x00;
		RFID_Data_buf[4] = 0x00;
		RFID_Length = 0xff;
	
		Result = ExchangeBlock(RFID_Data_buf, 3 ,RFID_Data_buf,&RFID_Length, 5000);

		if(Result == STATUS_SUCCESS)
		{
								   	
		}
		else
		{
		 	
		}
	}
	else
	{
			
	}

	Prev_block = 0x0a;
	
	return Result;



}


short test_SIM_Data(APDU *pT0APDU, unsigned int delay)
{   
	unsigned char RFID_Data_buf[100];
	unsigned char RFID_Length;
	unsigned short  uintResult = 0;
	unsigned char tmp;
	unsigned short tmplen =0;

	pT0APDU->RLE = 0;

	RFID_Data_buf[0] = Prev_block;
	RFID_Data_buf[1] = 0x00;
	RFID_Data_buf[2] = pT0APDU->CLA;			 
	RFID_Data_buf[3] = pT0APDU->INS;
	RFID_Data_buf[4] = pT0APDU->PP1;
	RFID_Data_buf[5] = pT0APDU->PP2;
	RFID_Data_buf[6] = pT0APDU->PP3;
	RFID_Length = 0xff;
	memcpy(RFID_Data_buf + 7, pT0APDU->pInData, pT0APDU->LC);

	uintResult = ExchangeBlock(RFID_Data_buf, ((pT0APDU->LC)+7),RFID_Data_buf, &RFID_Length, delay);

	if(uintResult)
	{
		return uintResult;
	}

reChain:

	if(RFID_Length != 0)
	{
		if((RFID_Data_buf[0] & 0x10) == 0x10)
		{
			//处理链接 根据iso 14443-4 和pboc2.0 -8协议规定

			//保存已接收到的数据
			memcpy(pT0APDU->pOutData, RFID_Data_buf + 2, RFID_Length - 2 - 2);		   
			pT0APDU->RLE += (RFID_Length - 2);

			//step 1. 发送R-块

			tmp = 0xa0|(RFID_Data_buf[0] & 0x0f);

			tmp = tmp | ((~(tmp & 0x01)) & 0x01);
			
			RFID_Data_buf[0] = tmp;
			RFID_Length = 0xff;

			uintResult = ExchangeBlock(RFID_Data_buf, 2,RFID_Data_buf, &RFID_Length, delay);

			goto reChain;
		}
		else
		{
			if((RFID_Data_buf[0] == Prev_block))
			{
				Prev_block = 0x0a | ((~(Prev_block & 0x01)) & 0x01);
			}
			
			pT0APDU->SW1 = RFID_Data_buf[RFID_Length - 2];
			pT0APDU->SW2 = RFID_Data_buf[RFID_Length - 1];
			
			memcpy(pT0APDU->pOutData+pT0APDU->RLE, RFID_Data_buf + 2, RFID_Length - 2 - 2);		   
			pT0APDU->RLE += (RFID_Length - 2 - 2);

		}
	}
	else
	{
		uintResult = STATUS_OTHER_ERROR;
	}
	


	return uintResult;
}


//脱机查询电子现金余额
//Select PSE
unsigned short test_SelectPSE(unsigned char *uchDirSfi,APDU *t0_apdu)
{
    unsigned char *ptr = NULL;
	unsigned short ret = 0;

	//向卡片发送Select PSE命令		00a404000e0e00
	t0_apdu->CLA = 0x00;			 
	t0_apdu->INS = 0xA4;
	t0_apdu->PP1 = 0x04;
	t0_apdu->PP2 = 0x00;
	t0_apdu->PP3 = 0x0E;
	t0_apdu->LC = 0x0E;
	t0_apdu->LE = 0x00;
	t0_apdu->RLE = 0x00;
	memcpy(t0_apdu->pInData, "1PAY.SYS.DDF01", t0_apdu->LC);
	//ret = test_SIM_Data(t0_apdu, 100000);
	ret = test_SIM_Data(t0_apdu, 1000000);

	 if(ret != STATUS_SUCCESS)
	 {
		return ret;
	 }

//	*uchDirSfi = 1;
	if((0x6a == t0_apdu->SW1) || (0x81 == t0_apdu->SW2))	  //交易终止
	{
		return SW_FUNCTION_NOTSUPPORT;
	} 
	else if((0x90 != t0_apdu->SW1) || (0x00 != t0_apdu->SW2))
	{
		//使用AID列表选择
		return t0_apdu->SW1*256+t0_apdu->SW2;
	}
	else
	{	
		ptr = SearchTag(t0_apdu->pOutData, t0_apdu->RLE, 0x88);	//查找DIR短文件标识符数据对象

		if(NULL == ptr)
		{
			//使用AID列表选择

			return SW_FILE_NOTFOUND;
		}
		else
		{
			*uchDirSfi = *(ptr+2);
		}
	} 

	return SW_OPERATION_SUCCESS; 
}
//Read Directory
unsigned short test_RecordDir(unsigned char *uchDirSfi,APDU *t0_apdu)
{	
	unsigned char *ptr = NULL;
	
	//读目录文件		
	t0_apdu->CLA = 0x00;
	t0_apdu->INS = 0xB2;
	t0_apdu->PP1 = 0x01;
	t0_apdu->PP2 = (*uchDirSfi<<3) | 0x04;
	t0_apdu->PP3 = 0x00;
	t0_apdu->LC = 0x00;
	t0_apdu->LE = 0x00;
	t0_apdu->RLE = 0x00;


	//test_SIM_Data(t0_apdu, 100000);
	test_SIM_Data(t0_apdu, 1000000);


	if((0x90 != t0_apdu->SW1) || (0x00 != t0_apdu->SW2))
	{
		//使用AID列表选择
		return t0_apdu->SW1*256+t0_apdu->SW2;
	}
	else
	{
		ptr =  SearchTag(t0_apdu->pOutData, t0_apdu->RLE, 0x4f);	//查找AID数据对象
		if(NULL == ptr)
		{
			//使用AID列表选择
			return SW_FILE_NOTFOUND+1;
		}
		else
		{
			uchAFL[0] = *(ptr+1);					//保存AID长度
			memcpy(uchAFL+1, ptr+2, uchAFL[0]); 	//保存AID
		}
	}
	return SW_OPERATION_SUCCESS;
}

//Select ADF
unsigned short test_SelectADF(APDU *t0_apdu)
{	
	unsigned char *ptr = NULL;
	unsigned short ret = 0;
	
	//选择指定的ADF		
	t0_apdu->CLA = 0x00;
	t0_apdu->INS = 0xA4;
	t0_apdu->PP1 = 0x04;
	t0_apdu->PP2 = 0x00;
	t0_apdu->PP3 = uchAFL[0];
	t0_apdu->LC = t0_apdu->PP3;
	t0_apdu->LE = 0x00;
	t0_apdu->RLE = 0x00;
	memcpy(t0_apdu->pInData, uchAFL+1, t0_apdu->LC);

	//ret = test_SIM_Data(t0_apdu,100000);
	ret = test_SIM_Data(t0_apdu,1000000);


	 if(ret != STATUS_SUCCESS)
	 {
		return ret;
	 }

	if((0x90 != t0_apdu->SW1) || (0x00 != t0_apdu->SW2))
	{
		//选择其他ADF
		return t0_apdu->SW1*256+t0_apdu->SW2;
		return SW_FILE_NOTFOUND;
	}
	else
	{
		ptr = SearchTag(t0_apdu->pOutData, *(t0_apdu->pOutData+1)+2, 0x9f38);	//查找PDOL数据对象
		if(NULL == ptr)
		{
			*(t0_apdu->pInData + 1) = 0;
		}
		else
		{
			memcpy(uchAFL, ptr+3, *(ptr+2));

			//终端填充PDOL数据
			*(t0_apdu->pInData + 1) = CollectDol(uchAFL, *(ptr+2), t0_apdu->pInData + 2);
		}

		*t0_apdu->pInData = 0x83;	//设置PDOL数据对象Tag
	}
	return SW_OPERATION_SUCCESS;
}

//Get Cash
unsigned short test_GetCash(APDU *t0_apdu)
{	
	unsigned char *ptr = NULL;
	unsigned char  disp,i,j,nozero,disp_a;
	
	//读取Tag 0x9f79，电子现金余额		
	t0_apdu->CLA = 0x80;
	t0_apdu->INS = 0xca;
	t0_apdu->PP1 = 0x9f;
	t0_apdu->PP2 = 0x79;
	t0_apdu->PP3 = 0;
	t0_apdu->LC = t0_apdu->PP3;
	t0_apdu->LE = 0x00;
	t0_apdu->RLE = 0x00;
		
	test_SIM_Data(t0_apdu, 1000000);

	if((0x90 != t0_apdu->SW1) || (0x00 != t0_apdu->SW2))
	{	
		return t0_apdu->SW1*256+t0_apdu->SW2;
	}
	else
	{
		ptr = SearchTag(t0_apdu->pOutData, t0_apdu->RLE, 0x9f79);
		if(NULL == ptr)
		{
			return SW_FILE_NOTFOUND;
		}
		else
		{
			//if(1 != uchKeyStatus)					//没有按下功能键，上位机发送指令进行脱机余额查询
			{
				g_APDU.RLE =  t0_apdu->RLE-3;
				memcpy(g_APDU.pOutData, t0_apdu->pOutData+3, t0_apdu->RLE-3);
			
				if(0 == g_APDU.PP1)					//终端屏幕上显示电子现金余额
				{
		    		MID_DISP_SmallPic(SMALLPIC_DIANZIYUE1);
					j = 0;
					nozero = 0;
					for(i=0;i<8;i++)
					{
					 	disp = *(t0_apdu->pOutData+3+1+(i/2));
						if(i%2 == 0)
						{
							disp_a = disp/16+0x30;
						}
						else
						{
							disp_a = disp%16+0x30;
						}

						if(disp_a != 0x30)
						{
							nozero = 1;
						}

						if (nozero == 1)
						{
							MID_DISP_ShowChar16X8(4,16+8*j,disp_a,ASCIIchardot);
							j++;
						}
					}
					nozero = 0;
					disp_a = '.';
	
					if (j == 0)
					{
						MID_DISP_ShowChar16X8(4,16+8*j,'0',ASCIIchardot);
						j++;
					}
						
					MID_DISP_ShowChar16X8(4,16+8*j,disp_a,ASCIIchardot);
					j++;
					disp = *(g_APDU.pOutData+5);
					disp_a = disp/16+0x30;
					MID_DISP_ShowChar16X8(4,16+8*j,disp_a,ASCIIchardot);
					j++;
					disp_a = disp%16+0x30;
					MID_DISP_ShowChar16X8(4,16+8*j,disp_a,ASCIIchardot);
	
					MID_DISP_ShowChar16X8(4,70,'(',ASCIIchardot);
					MID_DISP_ShowHZ16X16(4,79,yuan);
					MID_DISP_ShowChar16X8(4,96,')',ASCIIchardot);
				}
			}
/*			else									//功能键按下，进行脱机余额查询 
			{
				MID_DISP_SmallPic(SMALLPIC_YUE);
				j = 0;
				nozero = 0;
				for(i=0;i<8;i++)
				{
				 	disp = *(t0_apdu->pOutData+3+1+(i/2));
					if(i%2 == 0)
					{
						disp_a = disp/16+0x30;
					}
					else
					{
						disp_a = disp%16+0x30;
					}
					if(disp_a != 0x30)
						nozero = 1;
					if (nozero == 1)
					{
						MID_DISP_ShowChar16X8(2,1+8*j,disp_a,ASCIIchardot);
						j++;
					}
				}
				nozero = 0;
				disp_a = '.';

				if (j == 0)
				{
					MID_DISP_ShowChar16X8(2,1+8*j,'0',ASCIIchardot);
					j++;
				}
	
				MID_DISP_ShowChar16X8(2,1+8*j,disp_a,ASCIIchardot);
				j++;
				disp = *(t0_apdu->pOutData+3+5);
				disp_a = disp/16+0x30;
				MID_DISP_ShowChar16X8(2,1+8*j,disp_a,ASCIIchardot);
				j++;
				disp_a = disp%16+0x30;
				MID_DISP_ShowChar16X8(2,1+8*j,disp_a,ASCIIchardot);
				MID_DISP_ShowChar16X8(2,73,'(',ASCIIchardot);
				MID_DISP_ShowHZ16X16(2,79,yuan);
				MID_DISP_ShowChar16X8(2,90,')',ASCIIchardot);
			}
*/
		}
	}
	return SW_OPERATION_SUCCESS;
}

unsigned short test_APP_POS_PAY_CDGetCashBalance(void)
{
	APDU  test_t0_apdu;
	unsigned char  uchDirSfi = 0;
	unsigned int   uintResult = 0;
	unsigned char tmp;

//	APP_POS_PAY_CardOn();
//	Delay_ms(300);

	/*-----------1. Select PSE--------------------*/
	uintResult = test_SelectPSE(&uchDirSfi,&test_t0_apdu);

	if (SW_OPERATION_SUCCESS != uintResult)
	{
		//MID_DISP_SmallPic(SMALLPIC_CHAXUNSHIBAI);
		//MID_DISP_ShowChar16X8(4,8,'1',(unsigned char *)ASCIIchardot);
		return	uintResult; 
	}

	/*-----------2. Read Record 读DIR文件记录-----*/ 
	uintResult = test_RecordDir(&uchDirSfi,&test_t0_apdu);

	if (SW_OPERATION_SUCCESS != uintResult)
	{
		//MID_DISP_SmallPic(SMALLPIC_CHAXUNSHIBAI);	
		return	uintResult; 
	}


	/*-----------3. Select ADF------------*/
	uintResult = test_SelectADF(&test_t0_apdu);
	if (SW_OPERATION_SUCCESS != uintResult)
	{
		//MID_DISP_SmallPic(SMALLPIC_CHAXUNSHIBAI);
		//MID_DISP_ShowChar16X8(4,8,'3',(unsigned char *)ASCIIchardot);
		return	uintResult; 
	}
	
	/*-----------4. Get Cash------------*/
	uintResult = test_GetCash(&test_t0_apdu);
	if (SW_OPERATION_SUCCESS != uintResult)
	{
		//MID_DISP_SmallPic(SMALLPIC_CHAXUNSHIBAI);
		//MID_DISP_ShowChar16X8(4,8,'4',(unsigned char *)ASCIIchardot);
		return	uintResult; 
	}
	return 	 SW_OPERATION_SUCCESS;
}

