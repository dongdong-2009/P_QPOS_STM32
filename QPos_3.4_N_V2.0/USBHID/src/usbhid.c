#include "stm32f10x.h"
#include "usbhid.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_conf.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_istr.h"

#include "VectorTable.h"

USBHID_MESSAGE g_usbhid_message;
extern uint8_t USB_Receive_Buffer[];
extern uint8_t USB_Send_Buffer[];
extern volatile uint8_t USB_Received_Flag;
extern uint8_t USB_Received_Len;

void USB_hid_send(unsigned char *pdata,unsigned short len)//unsigned char *pdata,
{

	unsigned short i=0;

	while(len>REPORT_COUNT)
	{
		UserToPMABufferCopy(pdata+i, ENDP2_TXADDR, REPORT_COUNT);
    		SetEPTxCount(ENDP2, REPORT_COUNT);
   		 SetEPTxValid(ENDP2); 
		 i+=REPORT_COUNT;
		 len-=REPORT_COUNT;
		 while(GetEPTxStatus(ENDP2)!=EP_TX_NAK);
	}
	if(len>0)
	{
		UserToPMABufferCopy(pdata+i, ENDP2_TXADDR, len);
    		SetEPTxCount(ENDP2, len);
   		 SetEPTxValid(ENDP2); 
		 while(GetEPTxStatus(ENDP2)!=EP_TX_NAK);
	}
	
}

void HID_CMD_ANSWER(unsigned char * pucBuf, unsigned char ret)
{
	memset(pucBuf+9,0,11);
	if(ret)
	{	
		pucBuf[2]=0x45;
		pucBuf[4]=0x03;
		pucBuf[8]=ret;
		pucBuf[10]=0xcc;
		pucBuf[11]=0x33;
		USB_hid_send(pucBuf,20);
	}
	else
	{
		pucBuf[2]=0x41;
		pucBuf[4]=0x02;
		pucBuf[9]=0xcc;
		pucBuf[10]=0x33;
		USB_hid_send(pucBuf,20);
	}
	
}

unsigned char HID_CMD(void)
{
	unsigned char iRET;
	unsigned short cmd,len;
	if(USB_Received_Flag)
	{
		g_uiTime100ms_LCDOff_PowerOff = Time_PowerOFF;
		USB_Received_Flag=0;

		if(USB_Receive_Buffer[0]!=0x55||USB_Receive_Buffer[1]!=0xaa)
		return 1;
	
		len=USB_Receive_Buffer[5]<<8|USB_Receive_Buffer[4];
		cmd=USB_Receive_Buffer[6]<<8|USB_Receive_Buffer[7];

		switch(cmd)
		{
			case 0x7803:
				vLCD_GUI_ShowMid12X12("ÅäÖÃÖÐ...",4);
				iRET=iCMD_Factory_DeviceInit(USB_Receive_Buffer+8,len);	
			break;
			case 0x7805:				
				iRET=IVT_Update(USB_Receive_Buffer+8,len-2);	
			break;
			default:
				iRET=1;
			break;			
		}
		HID_CMD_ANSWER(USB_Receive_Buffer,iRET);
		if(cmd==0x7805&&iRET==0)
		{
			iRET=0xff;
		}
		USB_Received_Len=0;
		return iRET;
	}
	return 0xff;
}
