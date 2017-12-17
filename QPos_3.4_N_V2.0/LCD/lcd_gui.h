//***************************************************************************************
//*  File Name		roc_gui.h
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			03/23/2015 
//*  Description	
//*
//***************************************************************************************
#ifndef __VLCD_GUI_H__
#define __VLCD_GUI_H__

#define ROC_GUI_PAGE_MAX	8
#define ROC_GUI_COL_MAX		128
#define ROC_GUI_12X12_SIZE	24

extern unsigned char g_ucRocGuiBuf[ROC_GUI_PAGE_MAX * ROC_GUI_COL_MAX];

void vLCD_GUI_DrawLCD(void);

void vLCD_DrawOnePixel(unsigned int uiRow, unsigned int uiCol, unsigned char ucPixel);

void vLCD_DrawOneCol(unsigned int uiRow, unsigned int uiCol, unsigned char ucData);

void vLCD_DrawHalfCol(unsigned int uiRow, unsigned int uiCol, unsigned char ucData);

void vLCD_Draw12X12(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData);

void vLCD_Draw6X12(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData);

void vLCD_Draw20X8(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData);

void vLCD_Draw8X8(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData);

void vLCD_Clear(unsigned int uiFromRow, unsigned int uiToRow, unsigned int uiFromCol, unsigned int uiToCol);

void vLCD_ClearWorkPlace2345(void);

void vLCD_ClearWorkPage(unsigned char ucWorkPage);

void vLCD_ClearALL(void);

void vLCD_Set(unsigned int uiFromRow, unsigned int uiToRow, unsigned int uiFromCol, unsigned int uiToCol);

void vLCD_GUI_ShowMid12X12( unsigned char* pucData, unsigned char ucWorkPage);

void vLCD_GUI_Show12X12_Ex( unsigned char* pucData, unsigned char ucLen, unsigned char ucWorkPage, unsigned char ucColumn);

void vLCD_GUI_Show12X12_Col( unsigned char* pucData, unsigned char ucWorkPage, unsigned char ucColumn);
#endif
//************************************end of file****************************************
