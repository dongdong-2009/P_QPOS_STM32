/*
    this file is the headerfile for bluetooch.
*/
#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__


#define BT_Port_StatusInd GPIOC
#define BT_PIN_StatusInd1 GPIO_Pin_11
#define BT_PIN_StatusInd2 GPIO_Pin_12

#define BT_Port_RST 	GPIOC
#define BT_PIN_RST GPIO_Pin_1

//bm77
#define BT_Port_MoodSelect 	GPIOD
#define BT_PIN_MoodSelect	 GPIO_Pin_2
//IVT 改变连接状态
#define BT_Port_ConnectRev 	GPIOD
#define BT_PIN_ConnectRev	 GPIO_Pin_2

#define BT_Port_RxInd	GPIOC
#define BT_PIN_RxInd	 GPIO_Pin_10

#define BT_Port_WakeUP	GPIOB
#define BT_PIN_WakeUP	 GPIO_Pin_8

#define BT_Port_BOOT	GPIOB
#define BT_PIN_BOOT	 GPIO_Pin_9

#define BT_Port_UART		USART2
#define BT_Port_UARTGPIO		GPIOA
#define BT_UART_RxPin	GPIO_Pin_3
#define BT_UART_TxPin	GPIO_Pin_2

typedef enum
{
	BLUETOOTH_RECV_SUCEESCE = 0,
    BLUETOOTH_RECV_NO_DATA = 0x10,
    BLUETOOTH_RECV_HEADER_ERROR = 0x11,
    BLUETOOTH_RECV_LENGTH_ERROR = 0x12,
    BLUETOOTH_RECV_XOR_ERROR = 0x13,	
	BLUETOOTH_RECV_TAIL_ERROR = 0x14,
	BLUETOOTH_ANSWER_CMD = 0x15,
	BLUETOOTH_SEND_CMD = 0x16,
	
	BLUETOOTH_RECV_FRAME_SUCCESS = 0x20,
	BLUETOOTH_RECV_ACK_SUCCESS = 0x21,
	BLUETOOTH_RECV_NAK_SUCCESS = 0x22,
	BLUETOOTH_SEND_ANSWER = 0x23,

	BLUETOOTH_CMD_OK=0x30
}BLUETOOTH_STATUS;

typedef enum
{
	BLUETOOTH_SHUT_DOWN,
	BLUETOOTH_ACCESS,
	BLUETOOTH_LINK_WITH,
	BLUETOOTH_LINK_WO
}BLUETOOTH_STATE;

typedef enum  
{
	BLUETOOTH_APP,
	BLUETOOTH_BOOT,
	BLUETOOTH_EEPROM_CONFIG
}BLUETOOTH_MODE;

typedef struct
{
	unsigned char ucbHead[2];
	unsigned char ucbFrameIndex;
	unsigned char ucbFrameLen[2];
	unsigned char ucType;
	unsigned char ucIndex;
	unsigned char ucbLen[2];
	unsigned char ucbCmd[2];
	unsigned int uiLen;//帧长度
	unsigned int uibaglen;//包总长
	unsigned char ucallbagdata[3072];
	unsigned char * pucInData;
	unsigned char * pucOutData;
	unsigned char ucXOR;
	unsigned char ucbTail[2];
	unsigned char ucbStatus;
}BLUETOOTH_MESSAGE; 


//IVT_update data

#define BT_LICENSE_LEN	32
#define BT_BAUD_LEN		4
#define BT_MODE_LEN		4
#define DEV_NAME_LEN	33
#define PIN_CODE_LEN	17
#define MAC_ADDR_LEN    12

struct IVT_UserSettings_t
{
	unsigned char license[BT_LICENSE_LEN];
	unsigned long baudrate;
	unsigned long mode_flag;
	unsigned long cts_flag;
	unsigned long ana_ftrim;
};

struct IVT_StackSettings_t
{
	unsigned long idle_timer;
	char local_name[DEV_NAME_LEN];
	char pin_code[PIN_CODE_LEN];
	unsigned long cod;
	unsigned char io_cap;
	unsigned char ssp_mode;
};

struct IVT_Settings_t
{
	struct IVT_UserSettings_t user_settings;
	struct IVT_StackSettings_t stack_settings;
};


/***************************/

extern BLUETOOTH_STATUS g_eBluetoothRecvStatus;

extern BLUETOOTH_MESSAGE g_sBluetoothMessage;

extern unsigned int g_uiBluetoothRcvLen;

//extern unsigned char g_ucbBluetoothRcvBuf[1200];

//extern unsigned char g_ucbBluetoothSendBuf[1200];

extern unsigned char g_ucbBluetoothBuf[1280];

extern BLUETOOTH_STATE g_BluetoothState;

extern unsigned char g_BluetoothDateIN;

extern unsigned int g_uiTime100ms_BTRecvTimeout;


void vBluetooth_GPIO_Config(void);

void vBluetooth_USART_Config(void);

void vBluetooth_UART_SendData(unsigned char* pucBuf, unsigned int uiLen);

BLUETOOTH_STATE eBluetooth_GetBluetoothState(void);

void vBluetooth_SetBluetoothMode(BLUETOOTH_MODE eMode);

BLUETOOTH_MODE eBluetooth_GetBluetoothMode(void);

unsigned char vBluetooth_Force_BT_Into_WeakUp(void);

void vBluetooth_Init(void);

void vBluetooth_LinkDrop(void);

void vBluetooth_SendData(unsigned char* pucBuf,unsigned int uiLen);

int iBluetooth_RecvData(unsigned char* pucBuf,unsigned int uiLen);

int iBluetooth_Update_EEPROM(unsigned char *pucSendBuf, unsigned int uiSendLen,
							unsigned char *pucRevBuf,unsigned int uiRevLen);

int iBluetooth_Read_EEPROM(unsigned char *pucTexbuf, unsigned char *pucAddrbuf, unsigned char uclen);

int iBluetooth_Write_EEPROM(unsigned char *pucTexbuf, unsigned char *pucAddrbuf, unsigned char uclen);

int iBluetooth_ChangeBTName(unsigned char *pucBuf, unsigned char ucLen);

int iBluetooth_GETBTVersion(unsigned char *buf);

BLUETOOTH_STATUS eBluetooth_RecvAndAnalyze(unsigned char * pucBuf);

void vBluetooth_Send_ACKorNAK(unsigned char * pucBuf);

int vBluetooth_UI_Init(void);

void vBluetooth_Reset(void);

char vBluetooth_IVT_Update(void);

char IVT_Update(unsigned char *pIN,unsigned short ulen);

	
#endif //__BLUETOOTH_H__
/*******************************end of  file**********************************************/

