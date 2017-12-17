/*******************************************************************************
*                                                            
*                   MAG-TEK PROPRIETARY                       
*                                                             
*  THIS DOCUMENT CONTAINS PROPRIETARY INFORMATION AND EXCEPT  
*  WITH WRITTEN PERMISSION OF MAG-TEK, INC., SUCH INFORMATION 
*  SHALL NOT BE PUBLISHED OR DISCLOSED TO OTHERS OR USED FOR  
*  ANY PURPOSE OTHER THAN FOR THE OPERATION AND MAINTENANCE   
*  OF THE EQUIPMENT AND SOFTWARE WITH WHICH IT WAS PROCURED,  
*  AND THE DOCUMENT SHALL NOT BE COPIED IN WHOLE OR IN PART.  
*                                                             
*  Copyright (c) 2004                                         
*  MAG-TEK, Inc.                                              
*  Unpublished Work, All Rights Reserved                      
********************************************************************************
*  File: MSRead.h (MSR)     Created: 10/8/04		By:  Ted Logan 
********************************************************************************
*  Description:                                               
*  	This header file defines the exported objects of the magnetic stripe read
*		module.
*             
*  Revision History:                                          
*  Programmer       Date           Description                
*******************************************************************************/
#ifndef MSREAD_H
#define MSREAD_H

#ifndef NULL
	#define NULL    ((void *)0)
#endif

#ifndef FALSE
	#define FALSE   (0)
#endif

#ifndef TRUE
	#define TRUE    (1)
#endif

#define MSR_ASIC_PREAMBLE_LEN_IN_BITS 16
#define MSR_ASIC_PREAMBLE_LEN_IN_BYTES (MSR_ASIC_PREAMBLE_LEN_IN_BITS / 8)

#define MSR_ASIC_TK_LEN_IN_BITS 704
#define MSR_ASIC_TK_LEN_IN_BYTES (MSR_ASIC_TK_LEN_IN_BITS / 8)

#define MSR_ASIC_PREAMBLE_AND_ALL_3_TKACKS_LEN_IN_BITS (MSR_ASIC_PREAMBLE_LEN_IN_BITS + (3 * MSR_ASIC_TK_LEN_IN_BITS))
#define MSR_ASIC_PREAMBLE_AND_ALL_3_TKACKS_LEN_IN_BYTES (MSR_ASIC_PREAMBLE_AND_ALL_3_TKACKS_LEN_IN_BITS / 8)

#define MSR_TRACK_MAX_LENGTH	(MSR_ASIC_TK_LEN_IN_BITS / 5 + 1)	

#define MSR_CARD_NO_MAX_LENGTH		(30)	

extern unsigned char MSR_fCardDataReady;
extern volatile unsigned int MSR_ub100msTickDownCntTimeToReceiveBufferReadySignal;

/*add by roc*/
extern unsigned char MSR_ucTrack1Buf[MSR_TRACK_MAX_LENGTH];
extern unsigned char MSR_ucTrack1Len;
extern unsigned char MSR_ucTrack2Buf[MSR_TRACK_MAX_LENGTH];
extern unsigned char MSR_ucTrack2Len;
extern unsigned char MSR_ucTrack3Buf[MSR_TRACK_MAX_LENGTH];
extern unsigned char MSR_ucTrack3Len;
extern unsigned char MSR_ucCardNo[MSR_CARD_NO_MAX_LENGTH];
extern unsigned char MSR_ucCardNoLen;
/*if user swiping card, MSR_ucGetTrackData |= 0x80;
if user swiping card backward, MSR_ucGetTrackData |= 0x40;
if get track 1 data, MSR_ucGetTrackData |= 0x01;
if get track 2 data, MSR_ucGetTrackData |= 0x02;
if get track 3 data, MSR_ucGetTrackData |= 0x04.*/
extern unsigned char MSR_ucGetTrackData;

void MSR_vInit(void);

void MSR_vCardDataAcquisitionManager(void);

void MSR_vClearCardPresent(void);

void MSR_vGetData(unsigned char * pryubTkData, unsigned short uwCntInBits);

void MSR_vReset(void);

void MSR_vArm(void);

void MSR_vClearCardData(void);

int MSR_iGetData_CheckMagTek(void);

void MSR_vToASCII_Track2Or3(unsigned char * pucTrackData, unsigned char ucTrackLen);

int MSR_iMSCardTask(void);

int MSR_iGetCardNum(unsigned char * pucData, unsigned char * pucDataLen);

int MSR_iGetData_Track2Or3(unsigned char * pucTkData, unsigned char *pucTkDataLen);

int MSR_iGetData_Track1(unsigned char * pucTkData, unsigned char *pucTkDataLen);

int MSR_iAnalyzingData_Track1Or2Or3(unsigned char * pucData, unsigned char * pucDataLen, unsigned char ucTrackNo);

int MSR_iMSCardTask(void);
#endif
 

