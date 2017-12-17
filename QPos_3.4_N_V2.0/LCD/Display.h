/*
    this file is the header file for display

*/
#ifndef _DISPALY_H_
#define _DISPALY_H_
void MID_DISP_Show8X20(unsigned char page,unsigned char column,const unsigned char *pASCIIchar);

void Display_Init(void);
void MID_DISP_ShowChar16X6(unsigned char page,unsigned char column,unsigned char Order,const unsigned char *pASCIIchar);
void MID_DISP_ShowChar16X8(unsigned char page,unsigned char column,unsigned char Order,const unsigned char *pASCIIchar);

void MID_DISP_ShowHZ16X16(unsigned char line,unsigned char column, const unsigned char *pt);

void MID_DISP_ShowHZ24X24(unsigned char page,unsigned char column, const unsigned char *pt);

void MID_DISP_ShowChar24X12(unsigned char page,unsigned char column,const unsigned char *pASCIIchar);

void MID_DISP_ShowCharStr(unsigned char page,unsigned char column,unsigned char *pStr,unsigned char len);

void MID_DISP_HexNumber(unsigned char page,unsigned char column,unsigned char *pHexNumBuf,unsigned char len);

void MID_DISP_ShowHZStr(unsigned char page,unsigned char column,const unsigned char *pStr,unsigned char len);

void HAL_DISP_CLS(void);

void HAL_DISP_WriteString(const unsigned char *uDisplayData, unsigned char uDataNum);

void ClearALLPage(unsigned char page,unsigned char column);

void ClearLines(unsigned char page1,unsigned char page2);

void MID_DISP_Show8X8(unsigned char page,unsigned char column,const unsigned char *pASCIIchar);

void DispalyPower(unsigned char value);

unsigned char dispaly_key(unsigned char buffer[],unsigned char time);

void MID_DISP_ShowCharStr6(unsigned char page,unsigned char column,unsigned char *pStr,unsigned char len);

void HAL_DISP_SetXY(unsigned char page, unsigned char column);

void BuleToothState(void);

void PowerCurrentState(void);

void PowerChangeState(void);

void BackLightCmd(unsigned char NewSta);

void ST7567_Power_OFF_QY(void);

void ST7567_Power_On_QY(void);

void HAL_DISP_SET(void);
#endif //_DISPALY_H_





