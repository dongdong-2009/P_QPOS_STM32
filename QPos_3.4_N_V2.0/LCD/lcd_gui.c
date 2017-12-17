//***************************************************************************************
//*  File Name		roc_gui.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			03/23/2015 
//*  Description	
//*
//***************************************************************************************
#include "string.h"
#include "FontFonction.h"
#include "ASCII.h"
#include "lcd_gui.h"
#include "Display.h"
#include "msd.h"

unsigned char g_ucLCDGuiBuf[ROC_GUI_PAGE_MAX * ROC_GUI_COL_MAX];
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_GUI_DrawLCD(void)
{
	unsigned char ucPage;
	unsigned char ucCol;
	
	ucPage = 0;
	while(ucPage < ROC_GUI_PAGE_MAX) 
	{
		HAL_DISP_SetXY(ucPage,0);	//set page
		ucCol = 0;
		while(ucCol < ROC_GUI_COL_MAX)
		{
			HAL_DISP_WriteByte(g_ucLCDGuiBuf[ucPage * 128 + ucCol],0x02);	//write col
			ucCol++;	
		}
		ucPage++;	
	}	
}

void vLCD_GUI_Update(unsigned char uiFromRow,unsigned char uiFromCol,   unsigned char uiCol)
{
	unsigned char i,j;
	unsigned char row;
	
	if(uiFromRow%8>5)
	{
		row=3;
	}
	else
	{
		row=2;
	}
	uiFromRow>>=3;
	for(i=uiFromRow;i<uiFromRow+row;i++)
	{
		HAL_DISP_SetXY(i,uiFromCol);	//set page
		for(j=uiFromCol;j<uiCol;j++)
		{
			HAL_DISP_WriteByte(g_ucLCDGuiBuf[i*128+j],2);
		}
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_DrawDot(unsigned int uiRow, unsigned int uiCol, unsigned char ucPixel)//
{
	if((uiRow >= (ROC_GUI_PAGE_MAX * 8)) || (uiCol >= ROC_GUI_COL_MAX))
	{
		return;
	}	
	else{}

	if(ucPixel == 0)
	{
		g_ucLCDGuiBuf[(uiRow / 8) * 128 + uiCol] &= ~(0x01 << (uiRow % 8));
	}
	else
	{
		g_ucLCDGuiBuf[(uiRow / 8) * 128 + uiCol] |= (0x01 << (uiRow % 8));
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLVD_DrawOneCol(unsigned int uiRow, unsigned int uiCol, unsigned char ucData)
{
	unsigned char ucLoop;
	unsigned char ucPixel;

	ucLoop = 0;
	while(ucLoop < 8)
	{
		ucPixel = 0x01 << ucLoop;
		ucPixel &= ucData;
		vLCD_DrawDot(uiRow + ucLoop, uiCol, ucPixel);
		ucLoop++;	
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_DrawHalfCol(unsigned int uiRow, unsigned int uiCol, unsigned char ucData)
{
	unsigned char ucLoop;
	unsigned char ucPixel;

	ucLoop = 0;
	while(ucLoop < 4)
	{
		ucPixel = 0x01 << ucLoop;
		ucPixel &= ucData;
		vLCD_DrawDot(uiRow + ucLoop, uiCol, ucPixel);
		ucLoop++;	
	}
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Draw12X12(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData)
{
	unsigned char ucLoop;

	ucLoop = 0;
	while(ucLoop < 12)
	{
		vLVD_DrawOneCol(uiRow, uiCol + ucLoop, pucData[ucLoop]);
		vLCD_DrawHalfCol(uiRow + 8, uiCol + ucLoop, pucData[12 + ucLoop]);
		ucLoop++;	
	}			
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Draw6X12(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData)
{
	unsigned char ucLoop;

	ucLoop = 0;
	while(ucLoop < 6)
	{
		vLVD_DrawOneCol(uiRow, uiCol + ucLoop, pucData[ucLoop]);
		vLCD_DrawHalfCol(uiRow + 8, uiCol + ucLoop, pucData[6 + ucLoop]);
		ucLoop++;	
	}			
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Draw20X8(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData)
{
	unsigned char ucLoop;

	ucLoop = 0;
	while(ucLoop < 20)
	{
		vLVD_DrawOneCol(uiRow, uiCol + ucLoop, pucData[ucLoop]);
		ucLoop++;	
	} 
	vLCD_GUI_Update(0,90,128);
}

//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Draw8X8(unsigned int uiRow, unsigned int uiCol, const unsigned char * pucData)
{
	unsigned char ucLoop;

	ucLoop = 0;
	while(ucLoop < 8)
	{
		vLVD_DrawOneCol(uiRow, uiCol + ucLoop, pucData[ucLoop]);
		ucLoop++;	
	} 
	vLCD_GUI_Update(uiRow,uiCol,uiCol+8);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Clear(unsigned int uiFromRow, unsigned int uiToRow, unsigned int uiFromCol, unsigned int uiToCol)
{
	unsigned int uiRow;
   	unsigned int uiCol;
	uiRow = uiFromRow;
	while(uiRow <= uiToRow)
	{
		uiCol = uiFromCol;
		while(uiCol <= uiToCol)
		{
			vLCD_DrawDot(uiRow, uiCol, 0);
			uiCol++;	
		}
		uiRow++;
	}	
}

//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_ClearWorkPlace2345(void)
{
	vLCD_Clear(13, 63, 0, 127);	
	vLCD_GUI_DrawLCD();
}


void vLCD_ClearWorkPage(unsigned char ucWorkPage)
{
	unsigned char row;
	if(ucWorkPage<1||ucWorkPage>5)
		return;
	row = (ucWorkPage - 1) * 13;
	vLCD_Clear(row,row+13,0,127);
	//更新显示
	vLCD_GUI_Update(row,0,127);
}

void vLCD_ClearALL(void)
{
	vLCD_Clear(0, 63, 0, 127);	
	vLCD_GUI_DrawLCD();
}

//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_Set(unsigned int uiFromRow, unsigned int uiToRow, unsigned int uiFromCol, unsigned int uiToCol)
{
	unsigned int uiRow;
   	unsigned int uiCol;
	uiRow = uiFromRow;
	while(uiRow <= uiToRow)
	{
		uiCol = uiFromCol;
		while(uiCol <= uiToCol)
		{
			vLCD_DrawDot(uiRow, uiCol, 1);
			uiCol++;	
		}
		uiRow++;
	}
}
void vLCD_ShowString( unsigned char* pucData, unsigned char ucLen,unsigned char ucWorkPage, unsigned char ucColumn)
{	
	unsigned int uiOffset = 0;
	unsigned char ucLoop = 0;
	unsigned char ucbBuf[32];
	unsigned char ucRow;

	unsigned char row_start,col_start;	
	
	if(ucWorkPage < 1)
	{
		return; 
	}
	else{}

	ucRow = (ucWorkPage - 1) * 13;

	row_start=ucRow;
	col_start=ucColumn;

	for(ucLoop = 0; ucLoop < 32; ucLoop++)
	{
		ucbBuf[ucLoop] = 0;	
	}

	ucLoop = 0;
	while(ucLoop < ucLen)
	{
		if( pucData[ucLoop] & 0x80)
		{
			get_hanzi_mu(pucData+ucLoop,ucbBuf);
			vLCD_Draw12X12(ucRow, ucColumn, ucbBuf);
			ucColumn += 12;
			ucLoop += 2; 	
		}
		else
		{
			//byte
			uiOffset = (pucData[ucLoop] -0x20)* 12;
			memcpy(ucbBuf,ASCIIchardot12x6+uiOffset,12); 
			vLCD_Draw6X12(ucRow, ucColumn, ucbBuf);
			ucColumn += 6; 
			ucLoop++;
		}
	}
	//更新显示
	vLCD_GUI_Update(row_start,col_start,ucColumn);
}
//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************
void vLCD_GUI_ShowMid12X12( unsigned char* pucData, unsigned char ucWorkPage)
{	
	unsigned char ucLength = 0;
	unsigned char ucLoop = 0;
	unsigned char ucColumn;
	const unsigned char* pucWord_Copy = pucData;
	while(*pucWord_Copy)
	{
		ucLoop++;
		pucWord_Copy++;				
	}
	ucLength= ucLoop;	//多少个字节
	ucColumn=(128-ucLength*6)/2;
	vLCD_ShowString(pucData,ucLength,ucWorkPage,ucColumn);		
}


//***************************************************************************************
//* Function Name  : 
//* Description    : 
//*
//* Input          :   			  
//* Output         : none       
//* Return         : data or ignore
//***************************************************************************************

void vLCD_GUI_Show12X12_Ex( unsigned char* pucData, unsigned char ucLen, unsigned char ucWorkPage, unsigned char ucColumn)
{
	vLCD_ShowString(pucData,ucLen,ucWorkPage,ucColumn);
}

void vLCD_GUI_Show12X12_Col( unsigned char* pucData, unsigned char ucWorkPage, unsigned char ucColumn)
{
	unsigned char len;	
	len=strlen(pucData);
	vLCD_ShowString(pucData,len,ucWorkPage,ucColumn);
}
//************************************end of file****************************************
