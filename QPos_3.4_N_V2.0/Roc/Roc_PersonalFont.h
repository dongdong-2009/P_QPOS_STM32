//***************************************************************************************
//*  File Name		Roc_PersonalFont.h
//*  Author     	Roc       
//*	 Version		v1.0
//*  Date			04/05/2014 
//*  Description	
//*
//***************************************************************************************
#ifndef __ROC_PERSONAL_FONT_H__
#define __ROC_PERSONAL_FONT_H__

extern const unsigned char cucTuBiao[];

extern const unsigned char cucCell[];

extern const unsigned char cuc16X8ASCII[];

extern const unsigned char cuc24X24WordHYSYZFT[];

extern const unsigned char cuc24X24WordHYSY[];

extern const unsigned char cuc24X12QPOS[];

extern const unsigned char cuc16X16WordQCKHSK[];

extern const unsigned char cuc16X16WordQCKHHK[];

extern const unsigned char cuc16X16WordQCK[];

extern const unsigned char cuc16X16Money[];

extern const unsigned char cuc16X16WordQD[];

extern const unsigned char cuc16X16WordQQRNDKH[];

extern const unsigned char cuc16X16WordWMZJDJQR[];

extern const unsigned char cuc16X8Num[];

extern const unsigned char cuc16X16WordQSRMM[];

extern const unsigned char cucWordQuXiao[];

extern const unsigned char cucWordQingChu[];

extern const unsigned char cucWordQueRen[];

extern const unsigned char cucWordYuE[];

extern const unsigned char cucWordShangFan[];

extern const unsigned char cucWordXiaFan[];

void vLCD_PowerON(void);

void vLCD_MainUI(void);

void vLCD_SwipeMSOrICCardOrInputCardNo(void);

void vLCD_SwipeICCardOrChange(void);

void vLCD_SwipeMSCError(void);

void vLCD_CardNoAndOK(unsigned char * pucCardNo, unsigned char ucCardNoLen);

void vLCD_ShowTimeout(void);

unsigned char  vLCD_ShowResTime(unsigned int ucTimeout_ms);

void  vLCD_ClearResTime(void);

void vLCD_ShowWait(void);

void vLCD_ShowWaitForICC(void);

void vLCD_Show_NFC_WaitForICC(void);

void vLCD_RetryICC(void);

void vLCD_ReadICCFail(void);

void vLCD_ScriptFail(void);

void vLCD_ShowESC(void);

void vLCD_ShowWaitForIssuersScript(void);

void vLCD_InputPin(void);

void vLCD_ShowBluetoothIcon(void);

void vLCD_ShowMenu(void);

void vLCD_ShowVersion(void);

void vLCD_ShowPassword(void);

void vLCD_ShowSelectMode(void);

void vLCD_ShowFirmwareUpdateBegin(void);

void vLCD_ShowFirmwareUpdateProgress(unsigned int uiCurrentLen, unsigned int uiTotalLen);

void vLCD_ShowFirmwareUpdateEnd(void);

void vLCD_ShowFirmwareUpdateFail(void);

#endif
//*****************************end of file***********************************************
