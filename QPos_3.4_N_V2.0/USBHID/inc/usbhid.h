#ifndef __USB_HID_H_
#define	__USB_HID_H_

typedef struct
{
	unsigned char ucbHead[2];
	unsigned char ucType;
	unsigned char ucIndex;
	unsigned char ucbLen[2];
	unsigned char ucbCmd[2];
	unsigned int uiLen;
	unsigned char * pucInData;
	unsigned char * pucOutData;
	unsigned char ucXOR;
	unsigned char ucbTail[2];
}USBHID_MESSAGE; 

void USB_hid_send(unsigned char *pdata,unsigned short len);//unsigned char *pdata,
void HID_CMD_ANSWER(unsigned char * pucBuf, unsigned char ret);
unsigned char HID_CMD(void);

#endif
