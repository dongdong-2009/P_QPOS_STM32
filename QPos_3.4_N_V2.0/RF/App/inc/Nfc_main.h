
#ifndef __NFCMAIN_H__
#define __NFCMAIN_H__

typedef unsigned char       uint8_t;
 typedef unsigned short      uint16_t;

//EMVCo Test
#define   TYPE_A_CARD 				0x01
#define   TYPE_B_CARD 				0x02
#define   NO_CARD 					0x00


#define MAX_BUFFSIZE		260
#define HALTONDETECT		0
#define NOHALTONDETECT 	1


void Reset_RC523_device(void);

uint16_t RC523_Init(void);

uint16_t Polling_TypeA(uint8_t NoHalt);

uint16_t Polling_TypeB(void);

uint16_t SendWupA(uint8_t *pAtqa);

uint16_t SendWupB(void);

void  SendHaltA(void);

uint16_t SendRats(void);

uint16_t Activation_TypeA(void);

uint16_t Activation_TypeB(void);

uint16_t SendCarrier(void);

uint16_t PollPICC(uint8_t TypeMask);

uint16_t ResetPICC(void);

uint16_t NFC_SendAPDU(uint8_t *com_buffer, uint8_t cmdsize, uint8_t *resp_buffer);

uint16_t NFC_Reset(void);

void NFC_RemovalProcess(uint8_t bType);

uint16_t NFC_SendWUPA(void);

uint16_t NFC_SendWUPB(void);

uint16_t NFC_SendRATS(void);

uint16_t NFC_SendAttrib(void);

uint16_t NFC_SendLoopback(void);

void NFC_FieldOff(void);

void NFC_FieldOn(void);

#endif 
