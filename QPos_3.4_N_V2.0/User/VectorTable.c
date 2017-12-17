//***************************************************************************************
//*  File Name		VectorTable_PBOC.c
//*  Author     	laughing       
//*	 Version		v1.0
//*  Date			05/20/2014 
//*  Description	
//*
//***************************************************************************************
#include "VectorTable.h"
#include "Display.h"
#include "Bluetooth.h"
#include "Roc_PersonalFont.h"
#include "AN_ADC.h"
#include "powercheck.h"
#include "UP_MPOS_CMD.h"
#include "Roc_LittleTool.h"
#include "APP_PBOC.h"
#include "PBOC_Var.h"
#include "string.h"
#include "Sercrity.h"
#include "Key.h"
#include "MSCard_PA1100.h"
#include "rtc.h"
#include "FontFonction.h"
#include "SysTick_delay.h"
#include "IROM.h"
#include "Nfc_main.h"
#include "lcd_gui.h"

unsigned int g_uiTime100ms_LCDOff_PowerOff = 0;
unsigned int g_uiTime100ms_LCDShow = 0;
unsigned int g_uiTime100ms_Timeout = 0;
unsigned int g_uiTime100ms_LCD_BackLightOff = 0;

unsigned char g_LCDShowInfo=0;

unsigned char g_LCDRefeshTimeFlag=1;

//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vVectorTable_Device(void)
{	
	unsigned char cell_level;
	
	BLUETOOTH_STATE BTstate;

	if(g_UpdateFlag)
	{
		while(1);
	}
	BTstate = eBluetooth_GetBluetoothState();
	if( ((BTstate== BLUETOOTH_ACCESS)||(BTstate== BLUETOOTH_SHUT_DOWN))&&(g_BluetoothDateIN))//应用关闭
	{		
		g_BluetoothDateIN=0;
		//if(g_sCardReaderInfo.ucCardStatus != 0xff)
		{
			vLCD_ClearWorkPlace2345();
			vLCD_GUI_ShowMid12X12("蓝牙连接断开",3);
		}		
		g_uiTime100ms_LCDShow=30;//刷新主界面
			
		g_sCardReaderInfo.ucCardStatus = 0xff;
	}

	vLCD_ShowBluetoothIcon();
	if(g_LCDRefeshTimeFlag)
	{
		vLCD_ShowTime();
	}
	
	if(0 == g_uiTime100ms_LCDShow)//刷新主界面
	{
		if(g_sCardReaderInfo.ucCardStatus == 0xff)
		{
			g_uiTime100ms_LCDShow = Time_LCD_MainRresh;	
			g_LCDRefeshTimeFlag=1;
			vLCD_MainUI();
		}	
		else 
			g_uiTime100ms_LCDShow = Time_LCD_BackMainRresh;	
	}
	if( g_uiTime100ms_LCDShow%3)//刷新主界面
	{
		cell_level=AN_ADC_GetCellVoltageAndElectric();
		DispalyPower(cell_level); //电量
		while(cell_level==0)
		{
			vLCD_ClearWorkPlace2345();
			vLCD_GUI_ShowMid12X12("电量低,请充电",3);
			Delay_ms(1000);
			cell_level=AN_ADC_GetCellVoltageAndElectric();
			g_eBluetoothRecvStatus=BLUETOOTH_RECV_NO_DATA;
			g_uiTime100ms_LCDShow=0;
		}
		
	}

	if(g_uiTime100ms_LCDOff_PowerOff < 540)//60s 关闭屏幕
	{
		if(g_sCardReaderInfo.ucCardStatus == 0xff)
			BackLightCmd(DISABLE);
	}		
	if(0 == g_uiTime100ms_LCDOff_PowerOff)
	{
		Power_OFF(0);
	}

}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vVectorTable_CMD()
{
	int iRet;
	unsigned int uiCMD;
	unsigned char status;
	unsigned int uiLen;
	
	unsigned char *pucInData=g_sBluetoothMessage.ucallbagdata+2;
	unsigned char *pucOutData=g_sBluetoothMessage.ucallbagdata+2;
	
	mPOS_FrameAnalyse();
	
	if(g_eBluetoothRecvStatus == BLUETOOTH_CMD_OK)
	{
		uiCMD = (g_sBluetoothMessage.ucbCmd[0] << 8) + g_sBluetoothMessage.ucbCmd[1];
		uiLen=g_sBluetoothMessage.uibaglen-2;
		switch(uiCMD)
		{
			case 0x7700:
				iRet = iCMD7700_UToP_Device_Boot();
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0101:
				iRet = iCMD0101_UToP_GetDeviceVersion(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0107:
				iCMD0107_UToP_ResetDevice();
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				uiLen = 0;
				break;				
			case 0x0108:
				iRet = iCMD0108_UToP_GetRTC(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0109:
				iRet = iCMD0109_UToP_SetRTC(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x010A:
				iRet = iCMD010A__GetDeviceParameter(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x010c:
				g_LCDRefeshTimeFlag=0;
				iRet = iCMD010C_UToP_UpdateDeviceFirmware(pucInData, &uiLen, pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0202:
				iRet = iCMD0202_UToP_UpdateDeviceWorkKey(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0204:
				iRet = iCMD0204_UToP_VerifyMAC(pucInData, &uiLen, pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0207:
				iRet = iCMD0207_UToP_EncriptData(pucInData, &uiLen, pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0304:
				g_LCDRefeshTimeFlag=0;
				iRet = iCMD0304_UToP_OpenCardReader(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_RECV_NO_DATA;
				break;
			case 0x0305:
				iRet = iCMD0305_GetCardNo(pucInData, &uiLen,pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0306:
				iRet = iCMD0306__GetCardTrack(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0404:
				iRet = iCMD0404_UToP_OpenKeyboard(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_RECV_NO_DATA;
				break;
			case 0x0405:
				iRet = iCMD0405_UToP_GetPIN(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0501:
				iRet = iCMD0501_UpdateRSAPublicKey(pucInData, &uiLen,pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0502:
				iRet = iCMD0502_UpdateAID(pucInData, &uiLen,pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0503:
				iRet = iCMD0503_UToP_StartPBOC(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_RECV_NO_DATA;
				break;
			case 0x0505:
				g_sBluetoothMessage.uiLen = 0;
				iRet = iCMD0505_UToP_GetPBOC(pucOutData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0506:
				iRet = iCMD0506_UToP_StartPBOCIssuers(pucInData, &uiLen,pucOutData);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0510:
				iRet = iCMD0510_UToP_ClosedEMV(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x0701:
				g_LCDRefeshTimeFlag=0;
				iRet = iCMD0701_UToP_LCDShow(pucInData, &uiLen);
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			default:
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				uiLen=1;
				iRet = CMD_ERROR_PARAM;
				break;
		}
			g_sBluetoothMessage.uibaglen=uiLen+2;
			g_sBluetoothMessage.ucbStatus=iRet;
	}
	else{}

		// do something
	if(!vVectorTable_Card())
	{
		g_uiTime100ms_LCDShow = Time_LCD_BackMainRresh;	
	}//send new cmd
	
	if(g_eBluetoothRecvStatus == BLUETOOTH_ANSWER_CMD)
	{
		iBluetooth_AnalyzeAndSend();
		g_eBluetoothRecvStatus = BLUETOOTH_RECV_NO_DATA;
	}
	else{}
#if 1

	vVectorTable_Key();

#endif
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char  vVectorTable_Card(void)
{
	int iRet;
	static unsigned int counter=0;
	if(g_sCardReaderInfo.ucCardStatus == 0x01)
	{
	
		iRet=vLCD_ShowResTime(g_uiTime100ms_Timeout);
		if(iRet == 1)
		{
			iCMD0304_PToU_SendCardReaderStatus(0x03);
			g_sCardReaderInfo.ucCardStatus = 0xff;					
			return;
		}	
	

		if(g_sCardReaderInfo.ucReaderType&0x80)
		{
			#if 0
			if(counter==0)
			{
				NFC_FieldOff();
				Delay_ms(5);//关场后延时
			}
			counter++;
			#endif
			iRet = iPayThroughMSC(g_sCardReaderInfo.ucbCardNo, &g_sCardReaderInfo.ucCardNoLen);
			if(0 == iRet)
			{
				iRet = iMSCardIsICCard(MSR_ucTrack2Buf, MSR_ucTrack2Len);
				if(0 == iRet)
				{
					//show ICC
					vLCD_SwipeICCardOrChange();
				}
				else
				{
					g_sCardReaderInfo.ucCardStatus = 0x09;
					return 0;						
				}
			}
			else if(2 == iRet)
			{
				vLCD_SwipeMSCError();
			}
		}

		if(g_sCardReaderInfo.ucReaderType&0x40)
		{
			vPBOC_Init();			
			CheckCardState();						
			if(G_CardStats==1)
			{
				vLCD_ShowWaitForICC();
				iCMD0304_PToU_SendCardReaderStatus(0x05);
				g_sCardReaderInfo.ucCardStatus = 0xff;	
				return 0;
			}
			else	if(G_CardStats==2)
			{
				G_CardStats=0;
				vLCD_RetryICC();
			}
				
			else{}
		}
		if(g_sCardReaderInfo.ucReaderType&0x20)
		{
			if(NFC_USE==Device_NFC_State())
			{
				if(!G_CardStats)
				{
					#if 0
					if(counter==0x300)
					{
						counter=0;
						NFC_FieldOn();
						Delay_ms(10);
					}
					#else
						NFC_FieldOn();
						Delay_ms(10);					
					#endif
					if(g_NFC_State)
					{
						if(NFC_CheckCard()==0)
						{}
						else
							g_NFC_State=0;
					}
					else
					{
						if(0x9000 == NFC_SendWUPA())
						{
							if(0==Activation_TypeA())
							{	
								g_NFC_State= 1;														
							}
						}
					}
				}
			}
			if(g_NFC_State)
			{
				vLCD_Show_NFC_WaitForICC();				
				iCMD0304_PToU_SendCardReaderStatus(0x07);
				g_sCardReaderInfo.ucCardStatus = 0xff;				
				return 0;
			}			
		}
	}	

	else if(g_sCardReaderInfo.ucCardStatus == 0x02)
	{
		memcpy(g_sCardReaderInfo.ucbField55InDataBuf, g_sCardReaderInfo.ucbTradeAmount, 6);	   	
		memcpy(g_sCardReaderInfo.ucbField55InDataBuf + 6, g_sCardReaderInfo.ucTimestamp + 1, 6);	
		vCriticalSection(0);
		g_BluetoothDateIN=2;//检测读卡期间蓝牙是否收到数据	
		
		if(g_sCardReaderInfo.ucPBOCType == 0x01)	//标准的授权过程
		{
					
			iRet = iPayThroughICC(g_sCardReaderInfo.ucbField55InDataBuf, g_sCardReaderInfo.ucTradeType, 
									g_sCardReaderInfo.ucbField55OutDataBuf, &g_sCardReaderInfo.uiField55OutDataLen);		
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x02)		//qPBOC流程
		{			
			iRet = iPayQPOCFlow(g_sCardReaderInfo.ucbField55InDataBuf, g_sCardReaderInfo.ucTradeType, 
									g_sCardReaderInfo.ucbField55OutDataBuf, &g_sCardReaderInfo.uiField55OutDataLen);
			
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x21)		//电子现金指定账户圈存
		{
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x22)		//电子现金非指定账户圈存
		{
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x23)		//电子现金余额查询
		{
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x24)		//IC卡交易查询
		{
		}
		else if(g_sCardReaderInfo.ucPBOCType == 0x25)		//电子现金消费,强制脱机
		{
		}

		vCriticalSection(1); 
		if(g_BluetoothDateIN==1)
		{
			vLCD_MainUI();
			g_sCardReaderInfo.ucCardStatus = 0xff;
			g_eBluetoothRecvStatus=BLUETOOTH_RECV_NO_DATA;
			return 0;
		}
		if(0 == iRet)
		{
			//show card no
			g_sCardReaderInfo.ucCardNoLen = uchPan[0];
			iHexToASCII(uchPan + 1, &g_sCardReaderInfo.ucCardNoLen, g_sCardReaderInfo.ucbCardNo);
			if(0x3F == g_sCardReaderInfo.ucbCardNo[g_sCardReaderInfo.ucCardNoLen - 1])
			{
				g_sCardReaderInfo.ucCardNoLen -= 1;
			}
			else{}
			g_sCardReaderInfo.ucCardStatus = 0x09;
			return 0;	
		}
		else if(2 == iRet)//读卡失败
		{			
			g_sCardReaderInfo.ucCardStatus = 0xff;
			vLCD_ReadICCFail();
			iCMD0503_PToU_UploadPBOCData(0x07);
		}
	}
	else if(g_sCardReaderInfo.ucCardStatus == 0x06)
	{
		//pin
		iRet=iCMD_InputPIN();
		vLCD_ClearResTime();
		if(iRet==0||iRet==1)
		{		
			vLCD_ShowWait();
		}
		else if(iRet==2)
		{
			vLCD_ShowESC();
		}
		else if(iRet==3)
		{
			vLCD_ShowTimeout();
		}
		if(g_sBluetoothMessage.ucbCmd[0]==0x05&&g_sBluetoothMessage.ucbCmd[1]==0x03)
		{
			iCMD0503_PToU_UploadPBOCData(0x03);
		}
		else if(g_sBluetoothMessage.ucbCmd[0]==0x04&&g_sBluetoothMessage.ucbCmd[1]==0x04)
		{
			iRet=iCMD0404_PToU_UpLoadKeyboardStatus(g_sBluetoothMessage.ucallbagdata, &g_sBluetoothMessage.uibaglen);		
			
		}
		
		g_sKeyboardInfo.ucTimeout = 0;
		g_sCardReaderInfo.ucCardStatus = 0xff;
		return 0;
	}
	
	else if(g_sCardReaderInfo.ucCardStatus == 0x08)
	{
		//money
		iRet = iAmountAddPoint(g_sCardReaderInfo.ucbTradeAmount, &g_sCardReaderInfo.ucDisTradeAmountLen,  g_sCardReaderInfo.ucbDisTradeAmount);
		vLCD_SwipeMSOrICCardOrInputCardNo();
		MSR_vClearCardData();
		vClearKeyBuf();
		g_sCardReaderInfo.ucCardStatus = 0x01;
		return 0;
	}
	else if(g_sCardReaderInfo.ucCardStatus == 0x09)
	{			
		
		if(g_sCardReaderInfo.ucResultsReportType==1)
		{
			if(g_sCardReaderInfo.ucTimeout != 0)
			{
				g_uiTime100ms_Timeout = g_sCardReaderInfo.ucTimeout  * 10;
			}
			else
			{
				g_uiTime100ms_Timeout = 600;	
			}
			vLCD_CardNoAndOK(g_sCardReaderInfo.ucbCardNo, g_sCardReaderInfo.ucCardNoLen);
			iRet = iWaitForESCOrOK();
			vLCD_ClearResTime();
		}
		else
		{
			iRet=0;
		}
		if(0 == iRet)
		{
			vLCD_ShowWait();
			if((g_sBluetoothMessage.ucbCmd[0] == 0x03) && (g_sBluetoothMessage.ucbCmd[1] == 0x04))
			{
				iCMD0304_PToU_SendCardReaderStatus(0x00);
				g_sCardReaderInfo.ucCardStatus = 0xff;
			}
			else if((g_sBluetoothMessage.ucbCmd[0] == 0x05) && (g_sBluetoothMessage.ucbCmd[1] == 0x03))
			{
				if(g_sCardReaderInfo.ucPBOCType == 0x01)	//标准的授权过程
				{	
					g_sCardReaderInfo.ucCardStatus = 0x06;
				}
			}
			 
			return 0;
		}
		else if(1 == iRet) 
		{
			//redo
			g_sCardReaderInfo.ucCardStatus = 0xff;
			vLCD_ShowESC();
			if((g_sBluetoothMessage.ucbCmd[0] == 0x03) && (g_sBluetoothMessage.ucbCmd[1] == 0x04))
			{
				iCMD0304_PToU_SendCardReaderStatus(0x04);
			}
			else if((g_sBluetoothMessage.ucbCmd[0] == 0x05) && (g_sBluetoothMessage.ucbCmd[1] == 0x03))
			{
				iCMD0503_PToU_UploadPBOCData(0x07);
			}
			else{}
			
			return 0;
		}
		else if(2 == iRet) 
		{
			//timeout
			if((g_sBluetoothMessage.ucbCmd[0] == 0x03) && (g_sBluetoothMessage.ucbCmd[1] == 0x04))
			{
				iCMD0304_PToU_SendCardReaderStatus(0x03);
			}
			else if((g_sBluetoothMessage.ucbCmd[0] == 0x05) && (g_sBluetoothMessage.ucbCmd[1] == 0x03))
			{
				iCMD0503_PToU_UploadPBOCData(0x07);	
			}
			else{}
			vLCD_ShowTimeout();
			g_sCardReaderInfo.ucCardStatus = 0xff;
			return 0;
		}
		else
		{
			g_sCardReaderInfo.ucCardStatus = 0xff;
			return 0;
		}		
	}
	else
	{
		return 1;
	}

}

void vVectorTable_Key(void)
{
	unsigned char ucKey;
	static unsigned char menu_state=0;
	static unsigned char cmd_state=0;
	static unsigned char num=0;
	int iRet;

	ucKey = ucKey_Scan(1);
	if(ucKey==0xff)
	{
		if(g_uiTime100ms_LCDShow<5)
		{
			num=0;
			menu_state=0;
		}
		return;
	}
	if(g_sCardReaderInfo.ucCardStatus != 0xff)//交易状态
	{
		if(ucKey == 'E')
		{
			g_sCardReaderInfo.ucCardStatus = 0xff;
			vLCD_ShowESC();
			
			if((g_sBluetoothMessage.ucbCmd[0] == 0x03) && (g_sBluetoothMessage.ucbCmd[1] == 0x04))
			{
				iCMD0304_PToU_SendCardReaderStatus(0x04);
			}
			else if((g_sBluetoothMessage.ucbCmd[0] == 0x05) && (g_sBluetoothMessage.ucbCmd[1] == 0x03))
			{
				iCMD0503_PToU_UploadPBOCData(0x07);	
			}

		}
		else if((ucKey >= '0') && (ucKey <= '9'))
		{
		 	//input card no
			if((g_sCardReaderInfo.ucReaderType)&0x10)
			{
				iRet = iTaskGetAndDisplayInputPin(ucKey, g_sCardReaderInfo.ucbCardNo, &g_sCardReaderInfo.ucCardNoLen);
				if(0 == iRet)
				{
					vLCD_ShowWait();
					iCMD0304_PToU_SendCardReaderStatus(0x06);
				}
				else if(1 == iRet)
				{
					vLCD_ShowESC();
					iCMD0304_PToU_SendCardReaderStatus(0x04);
				}
				else if(2 == iRet)
				{
					//timeout
					iCMD0304_PToU_SendCardReaderStatus(0x03);
				}
				g_sCardReaderInfo.ucCardStatus=0xff;
			}
			else{}
		}
		
	}
	else
	{
		if((ucKey=='O')&&(g_LCDShowInfo))
		{
			g_uiTime100ms_LCDShow=0;
			g_LCDShowInfo=0;
		}
		if((ucKey=='#')&&(menu_state==0))
		{
			unsigned char i=0;
			while(1)
			{	
				ucKey= ucKey_Scan(0);
				if(ucKey=='#')
				{
					i++;
					if(i==2)
					{
						vLCD_ShowMenu();
						menu_state=1;	
						g_LCDRefeshTimeFlag=0;
						g_uiTime100ms_LCDShow = 100;
						break;
					}
				}
				else
					break;
			}
		}
		else
		{
			if(menu_state==1)
			{
				if(ucKey=='1')
				{
					vLCD_ShowVersion();
					g_uiTime100ms_LCDShow = 50;
					menu_state=0;
				}
				else if(ucKey=='2')
				{
					menu_state=2;
					vLCD_ShowSelectMode();
					g_uiTime100ms_LCDShow = 100;
				}
				else if(ucKey=='3')//||ucKey=='4'
				{
					cmd_state=ucKey;
					vLCD_ShowPassword();
					g_uiTime100ms_LCDShow = 100;
					menu_state=3;
				}
				else if(ucKey=='4')//||ucKey=='4'
				{
					vBluetooth_LinkDrop();
					g_uiTime100ms_LCDShow=0;
					menu_state=0;
				}
				else if(ucKey=='5')//||ucKey=='4'
				{
					Input_DeviceSerialNO();
					g_uiTime100ms_LCDShow=20;
					menu_state=0;
				}
				if(ucKey=='E')
				{
					g_uiTime100ms_LCDShow = 0;
					num=0;
					menu_state=0;
				}
				
			}
			else if(menu_state==2)
			{
				if(ucKey=='1'||ucKey=='2')//ucKey=='1'||
				{
					cmd_state=ucKey;
					vLCD_ShowPassword();
					g_uiTime100ms_LCDShow = 100;
					menu_state=3;
				}
			}
			else if(menu_state==3)
			{
				static unsigned char password[6];
				static unsigned char password1[6]="111111";
				if((ucKey >= '0') && (ucKey <= '9'))
				{
					password[num]=ucKey;
					vLCD_GUI_Show12X12_Ex("*",1,3, 40+6*num);
					num++;
				}
			
				if(num==6)
				{
					unsigned char flag;
					ClearLines(0,7);
					if(memcmp(password,password1,6))
					{
						vLCD_GUI_ShowMid12X12("密码错误",3);
						g_uiTime100ms_LCDShow = 30;
					}
					else
					{
						vLCD_GUI_ShowMid12X12("切换中,请稍等",4);
						if(cmd_state=='1')
						{
							flag=0x55;
							IROM_Write(FLASH_ADDR_FACTORY_INIT, &flag, 1);		
						}
						else if(cmd_state=='2')
						{
							flag=0xaa;
							IROM_Write(FLASH_ADDR_FACTORY_INIT, &flag, 1);		
						}
						else if(cmd_state=='3')
						{
							flag=0xff;
							IROM_Write(BOOTLOADER_FLAG_ADDR, &flag, 1);		
						}
						else if(cmd_state=='5')
						{
							flag=0xff;
							IROM_Write(FLASH_ADDR_FACTORY_TEST, &flag, 1);					
						}
						NVIC_SystemReset();
					}
					
					num=0;
					menu_state=0;
				}	
				if(ucKey=='E')
				{
					vLCD_MainUI();
					g_uiTime100ms_LCDShow = Time_LCD_MainRresh;
					num=0;
					menu_state=0;
				}
			}
			
		}		
		
	}
	
}
unsigned char vVectorTableFactory_CMD(unsigned char * pucBuf)
{
	int iRet;
	unsigned int uiCMD;
	unsigned char flag;
	g_sBluetoothMessage.pucOutData = pucBuf + 8;
	//recv cmd
	g_sBluetoothMessage.uiLen=0;
	if(g_eBluetoothRecvStatus == BLUETOOTH_RECV_SUCEESCE)
	{
		uiCMD = (g_sBluetoothMessage.ucbCmd[0] << 8) + g_sBluetoothMessage.ucbCmd[1];
		switch(uiCMD)
		{
			case 0x7801:
				flag=0x55;
				IROM_Write(FLASH_ADDR_FACTORY_TEST, &flag, 1);			
				iRet = 0;				
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;
			case 0x7802:
				iRet = 0;
				flag=0xaa;
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				break;	
			default:
				g_eBluetoothRecvStatus = BLUETOOTH_ANSWER_CMD;
				iRet = CMD_ERROR_PARAM;
				break;
		}
	}

	if(g_eBluetoothRecvStatus == BLUETOOTH_ANSWER_CMD)
	{
		iBluetooth_AnalyzeAndSend(iRet, g_sBluetoothMessage.uiLen);
		g_eBluetoothRecvStatus = BLUETOOTH_RECV_NO_DATA;
	}

	if(flag==0xaa)
	{
		return 1;
	}
	else
		return 0;
}
void vFactory_Test(unsigned char flag)

{
	unsigned int iRet;
	unsigned char buf[20],bufasc[5],i,len;
	unsigned char ucbBuf[16] = "UP-Test-013456";
	unsigned char ucbBuf1[16] = "UP-013-00000000";

#if 1
	vLCD_ClearWorkPlace2345();
	vLCD_GUI_ShowMid12X12("安全芯片检测...",2);
	vLCD_GUI_ShowMid12X12("请稍等",3);	

	if(GetChallenge(buf,4)!=0)
	{
		vLCD_GUI_ShowMid12X12("检测失败",3);	
		while(1);
	}
	memcpy((unsigned char *)&iRet,buf,4);
	for(i=0;i<5;i++)
	{
		bufasc[4-i]=(iRet%10)+0x30;
		iRet/= 10;    
	}

	if(flag==0)
	{
		vLCD_ClearWorkPlace2345();
		vLCD_GUI_ShowMid12X12("蓝牙模块检测",2);
		vLCD_GUI_ShowMid12X12("蓝牙初始化中...",3);
		
		if(vBluetooth_UI_Init())//初始化失败
		{
			vLCD_GUI_ShowMid12X12("蓝牙测试失败",3);
			while(1);
		}
		
		memcpy(ucbBuf+9,bufasc,5);
		if(iBluetooth_ChangeBTName(ucbBuf,14))
		{
			vLCD_GUI_ShowMid12X12("蓝牙测试失败",3);
			while(1);
		}
	}
	
	HAL_DISP_SET();   
	vKeyWaitOK();
	
	HAL_DISP_CLS();   
	DispalyPower(AN_ADC_GetCellVoltageAndElectric()); 
	if(flag==0)
	{
		vLCD_GUI_Show12X12_Ex("1",1,1,20);
	}
	else if(flag==1)
	{
		vLCD_GUI_Show12X12_Ex("2",1,1,20);
	}
	vLCD_ClearWorkPlace2345();
	
	vLCD_GUI_ShowMid12X12("开始检测...",2);
	vLCD_GUI_ShowMid12X12("确认背光已亮起",3);
	
	
	vKeyWaitOK();

	vLCD_ClearWorkPlace2345();
	vLCD_GUI_ShowMid12X12("按键检测...",2);
	
retest_KEY:
	if(vKeyTest())
	{
		vLCD_ClearWorkPage(3);
		//vLCD_ShowMiddle("重新输入",3);
		goto retest_KEY;
	}

	vLCD_ClearWorkPlace2345();
	vLCD_GUI_ShowMid12X12("磁条卡检测...",2);
	vLCD_GUI_ShowMid12X12("请刷磁条卡",3);

	while(iPayThroughMSC(g_sCardReaderInfo.ucbCardNo, &g_sCardReaderInfo.ucCardNoLen)) 
	{
	}
	
	vLCD_ClearWorkPlace2345();
	vLCD_GUI_ShowMid12X12("IC卡检测...",2);
	vLCD_GUI_ShowMid12X12("插入IC卡",3);

retest_IC:
	SC_PowerCmd(0);
	while(!SC_Detect())//检测卡插入
	{
	}
	if(HAL_7816_PowerOn(buf, &len)!=0)
	{
		vLCD_GUI_ShowMid12X12("失败,重新插入",3);
		goto retest_IC;
	}

	if(NFC_USE==Device_NFC_State())
	{
		vLCD_ClearWorkPlace2345();
		vLCD_GUI_ShowMid12X12("非接模块检测...",2);
		vRC523_Init();
		Delay_ms(10);
		vLCD_GUI_ShowMid12X12("请放置卡片",3);	
		
		while(!g_NFC_State)
		{
			if(0x9000 == NFC_SendWUPA())
			{
				if(0==Activation_TypeA())
				{	
					g_NFC_State= 1;	
					break;
				}
			}
		}
		g_NFC_State=0;
		NFC_FieldOff();
	}
	
#endif

	vLCD_ClearWorkPlace2345();
	vLCD_GUI_ShowMid12X12("初始化时钟",2);

	if(RTC_Init())//外部晶振未起振
	{
		vLCD_GUI_ShowMid12X12("请检测RTC晶振",3);
		while(1);
	}
	else 
	{
		//ClearLines(3,4);
		//vLCD_ShowMiddle("时钟初始成功",3);
		//Delay_ms(1000);
	}
	vLCD_ShowTime();//时间

	vLCD_ClearWorkPlace2345();
	DispalyPower(AN_ADC_GetCellVoltageAndElectric()); //电量
	vLCD_GUI_ShowMid12X12("所有测试完成",2);	
	vLCD_GUI_ShowMid12X12(ucbBuf,3);//设备
	vLCD_ShowTime();//时间
	Delay_ms(1000);
	if(flag==0)
	{
#if 1
	i=0x55;
	IROM_Write(FLASH_ADDR_FACTORY_TEST, &i, 1);	
	NVIC_SystemReset();
#else
	while(1)
	{
		if(vVectorTableFactory_CMD(g_ucbBluetoothBuf))
		{
			HAL_DISP_CLS();
			vLCD_ShowMiddle("正在关机...",2);
			vBluetooth_Reset();			
			iBluetooth_ChangeBTName(ucbBuf1,16);
			NVIC_SystemReset();	
		}
	}
#endif	
	}
	else//第二次测试完成 
	{
		i=0xaa;
		IROM_Write(FLASH_ADDR_FACTORY_TEST, &i, 1);	
		//iBluetooth_ChangeBTName(ucbBuf1,16);

	}
}

void vFactory_Init(unsigned char flag)
{
	unsigned char tmp,key;
	vLCD_ClearWorkPlace2345();
	if(flag==0)
	{
		vLCD_GUI_ShowMid12X12("请连接母POS",2);	
	}
	else if(flag==1)
	{
		vLCD_GUI_ShowMid12X12("请连接电脑",2);	
		vLCD_GUI_ShowMid12X12("按取消退出",3);
			
	}
	while(1)
	{
		key = ucKey_Scan(1);
		if(key=='E')
		{
			tmp=0xff;
			IROM_Write(FLASH_ADDR_FACTORY_INIT, &tmp, 1);
			NVIC_SystemReset();
		}
		if(flag==0)
		{
			tmp=GP_UART_CMD();
		}
		else if(flag==1)
		{
			tmp=HID_CMD();	
		}
		
		if(tmp==0)
		{
			vLCD_GUI_ShowMid12X12("配置成功,请重启",4);	
		}
		else if(tmp!=0xff)
		{
			vLCD_GUI_ShowMid12X12("配置失败",4);	
		}
	}
	
		
}


//************************************end of file**********************************


