/****************************************************************************
 *   $Id:: i2c_main.c 4785 2010-09-03 22:39:27Z nxp21346                    $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains main entry.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
/* Definitions of which drivers we
 * want to have enabled.
 * Just set to zero you want to
 * have disabled. */
typedef unsigned char       uint8_t;
 typedef unsigned short      uint16_t;
 
#include "phbalReg.h"
#include "phhalHw.h"

#include "phcsBflRegCtl.h"
#include "ph_TypeDefs.h"
#include "phhalHw_Rc523.h"
#include "ph_RefDefs.h"       //test

 
/* Configuration Headers */
/* Controls build behavior of components */
#include "ph_NxpBuild.h"
/* Status code definitions */
#include "ph_Status.h"
#include "phpalI14443p3a.h"
#include "phpalI14443p4.h"
#include "phpalI14443p4a.h"
#include "phalMfc.h"
//#include "phalMful.h"

#include "phpalI14443p3b.h"

#include "RegCtl_SpiHw.h"
#include "Nfc_main.h"

#include "phpalI14443p4_Sw_Int.h"

#include "SysTick_delay.h"
#include "HAL_7816.h"


/* External variables */

uint8_t bHalBufferReader[MAX_BUFFSIZE];

/* Bfl data parameter storage */
phbalReg_Stub_DataParams_t balReader;
phhalHw_Rc523_DataParams_t halReader;
phpalI14443p3a_Sw_DataParams_t I14443p3a;
phpalI14443p4a_Sw_DataParams_t I14443p4a;
phpalI14443p3b_Sw_DataParams_t I14443p3b;
phpalI14443p4_Sw_DataParams_t I14443p4;

phpalMifare_Sw_DataParams_t palMifare;
phalMfc_Sw_DataParams_t alMfc;

uint8_t  bSak[1];
uint8_t  bUid[10];
uint8_t  bMoreCardsAvailable;
uint8_t  bLength;
void      *pHal;
uint8_t  Atqa[2];
uint8_t  Atqb[16];
uint8_t  AtqbLen;
uint16_t wRxLength;
uint8_t  bAfi=0;
uint8_t  bAtqbLen=0; 
uint8_t  bNumSlots = 0;
uint8_t  reg_data;

uint8_t bType_A = 0;
uint8_t bType_B = 0;

void NFC_Delay_us(unsigned short nus)
{
	Delay_us(nus);
}
void NFC_Delay_ms(unsigned short nms)
{
	Delay_us(nms);
}
uint16_t RC523_Init(void)
{
	phStatus_t status;	
	//Global Variables initialization
	bSak[0] = 0;
	bMoreCardsAvailable = 0;
	bLength = 0;
	pHal = NULL;
	AtqbLen = 0;
	wRxLength = 0;
	bAfi=0;
	bAtqbLen=0; 
	bNumSlots = 0;
	reg_data = 0;
	bType_A = NO_CARD;
	bType_B	= NO_CARD;
	memset(Atqa,0,sizeof(Atqa));
	memset(Atqb,0,sizeof(Atqb));
	memset(bUid,0,sizeof(bUid));

    /* Initialise the Reader BAL component */
	phbalReg_Stub_Init(&balReader, sizeof(phbalReg_Stub_DataParams_t));

    /* init the Reader HAL component */
    status = phhalHw_Rc523_Init(
        &halReader,
        sizeof(phhalHw_Rc523_DataParams_t),
        &balReader,
        0,
        bHalBufferReader,
        sizeof(bHalBufferReader),
        bHalBufferReader,
        sizeof(bHalBufferReader));


    /* Use SPI interface */
    halReader.bBalConnectionType = PHHAL_HW_BAL_CONNECTION_SPI;

    /* Set the generic pointer */
    pHal = &halReader;

    /************ TYPE A TECHNOLOGY ********************/
    /* init the 14443-3A pal component */
    PH_CHECK_SUCCESS_FCT(status, phpalI14443p3a_Sw_Init(&I14443p3a, sizeof(phpalI14443p3a_Sw_DataParams_t), pHal));

    /* init the 14443-4A pal component */
    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4a_Sw_Init(&I14443p4a, sizeof(phpalI14443p4a_Sw_DataParams_t), pHal));

    /* init the 14443-4 pal component */
    PH_CHECK_SUCCESS_FCT(status, phpalI14443p4_Sw_Init(&I14443p4, sizeof(phpalI14443p4_Sw_DataParams_t), pHal));
#if 0
	/* Initialize the Mifare PAL component */
	 PH_CHECK_SUCCESS_FCT(status, phpalMifare_Sw_Init(&palMifare,
						 sizeof(phpalMifare_Sw_DataParams_t), pHal, &I14443p4));
	
	  /* Initialize the Mifare (R) Classic AL component - set NULL because
	   * the keys are loaded in E2 by the function */
	/* phKeyStore_SetKey */
	PH_CHECK_SUCCESS_FCT(status, phalMfc_Sw_Init(&alMfc,
			sizeof(phalMfc_Sw_DataParams_t), &palMifare, /*&Rc663keyStore*/ NULL));
#endif	
	

    /************ TYPE B TECHNOLOGY ********************/
    /* init the 14443-3B pal component */
    //PH_CHECK_SUCCESS_FCT(status, phpalI14443p3b_Sw_Init(&I14443p3b, sizeof(phpalI14443p3b_Sw_DataParams_t), &halReader));

    /* reset the IC  */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_Rc523_Cmd_SoftReset(pHal));

    /* read version IC */
	PH_CHECK_SUCCESS_FCT(status, phhalHw_ReadRegister(&halReader, PHHAL_HW_RC523_REG_VERSION, &reg_data));
	
	phhalHw_Rc523_FieldReset(pHal);
	return status;
}

uint16_t Polling_TypeA(uint8_t NoHalt)
{
    phStatus_t status;
	
	status = 0xFFFF;
	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);

	status = SendWupA(Atqa);
	if (PH_ERR_IO_TIMEOUT != (status & PH_ERR_MASK))
	{
		/* Put Picc in HALT A state */
		NFC_Delay_us(100);
		if (NoHalt != NOHALTONDETECT)
		{
		    /* Switch On Tx CRC */
    		//phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_TXCRC, PH_ON);
			SendHaltA();
		}
		return 0x01;
	}
	return 0;
}

uint16_t Polling_TypeB(void)
{
	phStatus_t status;
	
	status = 0xFFFF;
	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443B);
	
	status = SendWupB();	
	if (PH_ERR_IO_TIMEOUT != (status & PH_ERR_MASK))
	{
		return 0x01;
	}

	return 0;
}

uint16_t SendWupA(uint8_t *pAtqa)
{
    phStatus_t status;
    status = phpalI14443p3a_WakeUpA((void *)(&I14443p3a), pAtqa);
	return status;
}

uint16_t SendWupB(void)
{
    phStatus_t status;
    AtqbLen = 0;
	status = phpalI14443p3b_WakeUpB(&I14443p3b, 0, 0, I14443p3b.bExtAtqb, Atqb, &AtqbLen);
	return status;
}

void SendHaltA(void)
{
    phpalI14443p3a_HaltA((void *)(&I14443p3a));
}

uint16_t SendRats(void)
{
    phStatus_t status;

    /* no Cid - EMCCo Pvd TB000 */
    I14443p4a.bCid = 0x00;
    /* Fsd = 256 bytes - EMCCo Pvd TB000 */
    I14443p4a.bFsdi = 0x08;

	status = phpalI14443p4a_Rats(&I14443p4a, I14443p4a.bFsdi, I14443p4a.bCid, bHalBufferReader);

	if (PH_ERR_SUCCESS == status)
	{
		phpalI14443p4_SetProtocol(&I14443p4, PH_OFF, I14443p4a.bCid, PH_OFF, PH_OFF, I14443p4a.bFwi, I14443p4a.bFsdi, I14443p4a.bFsci);
	}
	return status;
}

unsigned short Select_TypeA()
{
	phStatus_t status;

	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
	/* reset additional info before activation */
	halReader.wAdditionalInfo = 0;
	/* Activate Layer 3 card - EMVCp Pcd processing */
	status = phpalI14443p3a_ActivateCard(&I14443p3a,
			                             NULL,
			                             0,	/* to send a Wakeup A */
			                             bUid,
			                             &bLength,
			                             bSak,
			                             &bMoreCardsAvailable);

	if (PH_ERR_SUCCESS == status)
	{
		return 0x9000;
	}
	return status;
}
uint16_t Activation_TypeA(void)
{
	phStatus_t status;

	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
	/* reset additional info before activation */
	halReader.wAdditionalInfo = 0;
	/* Activate Layer 3 card - EMVCp Pcd processing */
	status = phpalI14443p3a_ActivateCard(&I14443p3a,
			                             NULL,
			                             0,	/* to send a Wakeup A */
			                             bUid,
			                             &bLength,
			                             bSak,
			                             &bMoreCardsAvailable);

	if (PH_ERR_SUCCESS == status)
	{
		if (0x20 == (*bSak & 0x20))
		{
			status = SendRats();
		
#if 1   		//Added By Li Shuang, 2012.12.05
			/* TA307_X JLL 02/12/2011 EMVCo Pcd */
			if ( (PH_ERR_IO_TIMEOUT == (status & PH_ERR_MASK)) ||
			    (PH_ERR_IO_TIMEOUT_INT_ERROR == (status & PH_ERR_MASK)) ||
				(PH_ERR_NOISE_ERROR == (status & PH_ERR_MASK)) /*||
				(PH_ERR_INTEGRITY_ERROR == (status & PH_ERR_MASK)) ||
				(PH_ERR_PROTOCOL_ERROR == (status & PH_ERR_MASK)) */)
#else
			if (PH_ERR_IO_TIMEOUT == (status & PH_ERR_MASK))
#endif
			{
				/* if no noise, do not send a second RATS again */
				if (0 == halReader.wAdditionalInfo)
				{
					status = SendRats();
				}
			}
		}
		else if (0x08 == (*bSak & 0x08))
		{
			//mifare card
			return status;
		}	 

	}

	return status;
}

uint16_t Activation_TypeB(void)
{
	phStatus_t status;
    uint8_t Mbli;

	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443B);

	AtqbLen = 0;
	I14443p3b.bFsdi = 0x08; 	/* 256 bytes */
	I14443p3b.bCid =  0x00;		/* Cid not used */

	status = phpalI14443p3b_ActivateCard(&I14443p3b,
			I14443p3b.pPupi,
		    0x04,				/* to send a Wakeup B */
		    0,
		    0,
		    I14443p3b.bExtAtqb,
		    I14443p3b.bFsdi,
		    I14443p3b.bCid,
		    I14443p3b.bDri,
		    I14443p3b.bDsi,
		    Atqb,
		    &AtqbLen,
		    &Mbli,
		    &bMoreCardsAvailable
		    );

	if (PH_ERR_SUCCESS == status)
	{
	    phpalI14443p4_SetProtocol(&I14443p4, PH_OFF, I14443p3b.bCid, PH_OFF, PH_OFF, I14443p3b.bFwi, I14443p3b.bFsdi, I14443p3b.bFsci);
	}
	return status;
}

uint16_t SendCarrier(void)
{
	uint16_t status;

	status = phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);								

	if(0 == status )
		status = 0x9000;
	
	return status;
}

uint16_t PollPICC(uint8_t TypeMask)
{
	uint8_t result = NO_CARD;

	unsigned char test;

	
	bType_A = 0;
	bType_B = 0;

	/* Polling procedure */
	//while((!bType_A)&&(!bType_B))
	{
		// Here always send a HALT on detect to handle TB303 in EMVCo test.
		// The correct behavior must be: after card answer to WUPB and WUPA (in that order), a HALT is expected, followed by a reset, and restart polling
		if(TypeMask & TYPE_A_CARD)
		{
			bType_A = Polling_TypeA(HALTONDETECT);			
			NFC_Delay_ms(3);
		}
		if(TypeMask & TYPE_B_CARD)
		{
			bType_B = Polling_TypeB();			
			NFC_Delay_ms(3);
			if (!bType_A && bType_B)
			{
				if(TypeMask & TYPE_A_CARD) 
					bType_A = Polling_TypeA(HALTONDETECT);		// TB303
			}	
			
		}
	}

	/* End of Polling procedure */
	
	/* Collision detection and Activation procedures */
	if (bType_A ^ bType_B)
	{		
	
		NFC_Delay_ms(3);
		if (bType_A)
		{
			if(PH_ERR_SUCCESS == Activation_TypeA())
			{
				result = TYPE_A_CARD;
			}
		}
		else
		{
			if(PH_ERR_SUCCESS == Activation_TypeB())
			{
				result = TYPE_B_CARD;
			}
		}

		/* Activation failed - Reset RF field with delay */
		if (NO_CARD == result)
		{
			NFC_Reset();
			/* some delay to satisfy tp - TA304 - Requirements 9.2 */
			NFC_Delay_ms(3);		// Modified in FIME 06/04/2012
		}
	}
	/* End of Collision detection and Activation procedures */

	/* Type A & type B detected both ? */
	if (bType_A && bType_B)
	{
		NFC_Delay_ms(4);
		NFC_Reset();
		/* some delay to satisfy tp - Requirements 9.2 */
		NFC_Delay_ms(3);		// Modified in FIME 06/04/2012
	}	

	if((TYPE_A_CARD == result)||(TYPE_B_CARD == result))
		return 0x9000;
	else
		return 0x1111;
}

uint16_t ResetPICC(void)
{
	phStatus_t status;
	uint16_t  counter = 0;

#if 0
	status = phhalHw_FieldOn(pHal);
    NFC_Delay_ms(3);
    status = phhalHw_FieldOff(pHal);
    NFC_Delay_ms(5);
#endif

//	while(counter < 20)
	{
		RC523_Init();
		status = 0xFFFF;

		status = PollPICC(TYPE_A_CARD|TYPE_B_CARD);
		
	    status = phhalHw_FieldOff(pHal);
	    NFC_Delay_ms(6);
		status = phhalHw_FieldOn(pHal);
		NFC_Delay_ms(30);
		counter++;
	}
	if(0 == status)
		status = 0x9000;									

	return status;
}



uint16_t NFC_SendAPDU(uint8_t *com_buffer, uint8_t cmdsize, uint8_t *resp_buffer)
{

    phStatus_t status;
    uint8_t **ppRxBuffer = NULL;

    /* Layer 4 EMVCo Pcd processing */

	/* Added By Li Shuang 2012.12.04, Set Activation timeout to MAX */
/*    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
        pHal,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P4_SW_FWT_MAX_US));
*/

    /* In this function ppRxBuffer is a double output pointer which indicates the start address of the received data array */
    status = phpalI14443p4_Exchange(&I14443p4, PH_EXCHANGE_DEFAULT, com_buffer, cmdsize, ppRxBuffer, &wRxLength);
	if (PH_ERR_SUCCESS == status)
	{
		// copy the received data to the array response_buffer
		memcpy(resp_buffer, &bHalBufferReader[2], wRxLength);
	}
	else
	{
		/* Exchange not successful, reset the number of rxd bytes */
		//HAL_USB_BulkIn(2, &status, 2);
		wRxLength = 0x00;
	}

	return (uint16_t)wRxLength;
}

phStatus_t NFC_Card_APDU(APDU *pAPDU)
{   
   	unsigned char NFCBuf[260] = {0};
	unsigned char NFCSendlen;
   	unsigned char NFCRevBuf[260] = {0};
	unsigned char NFCRevlen=0;

	memset(NFCRevBuf,0,260);
	
	NFCBuf[0] = pAPDU->CLA;
	NFCBuf[1] = pAPDU->INS;
	NFCBuf[2] = pAPDU->PP1;
	NFCBuf[3] = pAPDU->PP2;
	NFCBuf[4] = pAPDU->PP3;

	if(pAPDU->LC)
	{
		memcpy(NFCBuf+5, pAPDU->pInData, pAPDU->LC);
	}

	NFCSendlen = 5+pAPDU->LC;
	NFCRevlen = NFC_SendAPDU(NFCBuf, NFCSendlen,NFCRevBuf);

	if(NFCRevlen == 0)
	{
		pAPDU->RLE = 0;
		pAPDU->SW1 =0x6a;
		pAPDU->SW2 = 0xaa;
		return 1;
	}	
	if(NFCRevlen>2)
	{
		memcpy(pAPDU->pOutData, NFCRevBuf, NFCRevlen-2);
	}

	
	pAPDU->RLE = NFCRevlen-2;
	pAPDU->SW1 = NFCRevBuf[NFCRevlen-2];
	pAPDU->SW2 = NFCRevBuf[NFCRevlen-1];
		
	return 0;
}

uint16_t NFC_Reset(void)
{
	phStatus_t status;

    status = phhalHw_FieldOff(pHal);
    NFC_Delay_ms(6);
	status = phhalHw_FieldOn(pHal);

	return status;

}

uint16_t NFC_Deselect()
{
	return phpalI14443p4_Deselect(&I14443p4);
}
uint16_t NFC_CheckCard()
{
	return phpalI14443p4_PresCheck(&I14443p4);
}

void NFC_RemovalProcess(uint8_t bType)
{
	phStatus_t status;
	uint8_t PollNoResultCpt = 0;

	NFC_Reset();

	if (TYPE_A_CARD== bType)
	{
		phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
		while(PollNoResultCpt < 3)
		{
			/* some delay to satisfy TA002 tp - Requirements 9.2 */
			NFC_Delay_ms(3);
			status = SendWupA(Atqa);
			if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
			{
				/* put Picc in HALT A state */
				/* Switch On Tx CRC */
    			phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_TXCRC, PH_ON);
				SendHaltA();
				PollNoResultCpt = 0;
				phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
			}
			else
			{
				PollNoResultCpt++;
			}
		}
	}
	else /* CARD_B == bType */
	{
		phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443B);
		while(PollNoResultCpt < 3)
		{
			/* some delay to satisfy TB003 tp - Requirements 9.2 */
			NFC_Delay_ms(5);
			status = SendWupB();
			if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
			{
				PollNoResultCpt = 0;
			}
			else
			{
				PollNoResultCpt++;
			}
		}
	}

	NFC_Reset();
}

uint16_t NFC_SendWUPA(void)
{
	phStatus_t status = 0;
	uint8_t pollATQ[2];


	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
	status = SendWupA(pollATQ);
	if(0 == status)
	{
		SendHaltA();
		status = 0x9000;
		
	}
	else
	{
	}

	return status;
}

uint16_t NFC_SendWUPB(void)
{
    phStatus_t status;
	

	phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443B);
	status = SendWupB();

	NFC_Delay_ms(50);		// delay of 50 ms		

	if(0 == status)
		status = 0x9000;									

	return status;
}

uint16_t NFC_SendRATS(void)
{
    phStatus_t 	status = 0xFFFF;
	uint8_t     bAtqa[2];
    uint8_t     bUid[4];
	uint8_t     bCascadeLevel;
	uint8_t     bNvbUid;
	uint8_t     counter = 0;

	while(counter < 500)  
	{
		phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443A);
		NFC_Delay_ms(3);

		status = 0xFFFF;
		
		while (0 != status)
	    {
	    	status = phpalI14443p3a_Sw_WakeUpA(&I14443p3a, bAtqa);
			NFC_Delay_ms(20);
			//HAL_USB_BulkIn(2,&status,2);
	    }
			

		if(0 == status)
		{	
			bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_1;
			bNvbUid = 0;
			/* Clear Uid */
	    	memset(bUid, 0x00, 4);  /* PRQA S 3200 */

			status = phpalI14443p3a_Sw_Anticollision(&I14443p3a, bCascadeLevel, bUid, bNvbUid, bUid, &bNvbUid);

			bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_1;
			bNvbUid = 0;
			/* Clear Uid */
	    	memset(bUid, 0x00, 4);  /* PRQA S 3200 */

			status = phpalI14443p3a_Sw_Anticollision(&I14443p3a, bCascadeLevel, bUid, bNvbUid, bUid, &bNvbUid);

			bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_1;
			bNvbUid = 0;
			/* Clear Uid */
	    	memset(bUid, 0x00, 4);  /* PRQA S 3200 */

			status = phpalI14443p3a_Sw_Anticollision(&I14443p3a, bCascadeLevel, bUid, bNvbUid, bUid, &bNvbUid);

	    }
		
	   	NFC_Reset();
			
		counter++;
	}
	

	if(PH_ERR_SUCCESS == status)
	{
		status = 0x9000;	
	}
	else
	{
	}
	

	
	return status;
}

uint16_t NFC_SendAttrib(void)
{
#if 0
	uint16_t status = 0;
	while(status != 0x9000)
	{
    status = Activation_TypeB();

	NFC_Delay_ms(5);		// delay of 5 ms

	if(0 == status)
		status = 0x9000;									
	}
#endif
	phStatus_t status = 0xFFFF;
	uint8_t Mbli;
	uint8_t counter = 0;

    phStatus_t   statusTmp;
    phStatus_t   statusFirstError;
    uint8_t      bSlot;
    uint8_t      bPICCsFound;
    uint8_t      pLocalAtqb[13];
    uint8_t      bLocalAtqbLen;
    uint8_t      *pLocalPupi;

	while(counter < 500)
	{
		phhalHw_ApplyProtocolSettings(pHal, PHHAL_HW_CARDTYPE_ISO14443B);

		AtqbLen = 0;
		I14443p3b.bFsdi = 0x08; 	/* 256 bytes */
		I14443p3b.bCid =  0x00; 	/* Cid not used */
		bNumSlots = 0;
		bAfi = 0;
    	bLocalAtqbLen = 0x00;
    	pLocalPupi = &pLocalAtqb[1];

		status = 0xFFFF;

		while(0 != status)
		{
			status = phpalI14443p3b_WakeUpB(&I14443p3b, bNumSlots, bAfi, I14443p3b.bExtAtqb, pLocalAtqb, &bLocalAtqbLen);
			NFC_Delay_ms(20);
		}

		if(0 == status)
		{	
			/* AttriB: activate the first PICC found */
			status = phpalI14443p3b_Attrib(&I14443p3b, pLocalAtqb, bLocalAtqbLen, I14443p3b.bFsdi,I14443p3b.bCid,I14443p3b.bDri,I14443p3b.bDsi,&Mbli);

			
			status = phpalI14443p3b_Attrib(&I14443p3b, pLocalAtqb, bLocalAtqbLen, I14443p3b.bFsdi,I14443p3b.bCid,I14443p3b.bDri,I14443p3b.bDsi,&Mbli);

			
			status = phpalI14443p3b_Attrib(&I14443p3b, pLocalAtqb, bLocalAtqbLen, I14443p3b.bFsdi,I14443p3b.bCid,I14443p3b.bDri,I14443p3b.bDsi,&Mbli);

	    }
		
	   	NFC_Reset();

		counter++;
#if 0
	status = phpalI14443p3b_ActivateCard(&I14443p3b,
			I14443p3b.pPupi,
			0x04,				/* to send a Wakeup B */
			0,
			0,
			I14443p3b.bExtAtqb,
			I14443p3b.bFsdi,
			I14443p3b.bCid,
			I14443p3b.bDri,
			I14443p3b.bDsi,
			Atqb,
			&AtqbLen,
			&Mbli,
			&bMoreCardsAvailable
			);
#endif
	}
	return status;
}



void NFC_FieldOff(void)
{
	phhalHw_FieldOff(pHal);

	return;
}

void NFC_FieldOn(void)
{
	phhalHw_FieldOn(pHal);

	return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
 
	
