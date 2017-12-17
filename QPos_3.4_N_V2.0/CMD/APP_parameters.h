#ifndef _APP_PARAMETER_H_
#define _APP_PARAMETER_H_

//------------常量定义------------------------------------
#define	FLASH_DEVICE_VERSION_INFO	0x803F001
#define PARAMETER_INIT_FLAG   		0x12345678
//------------结构体定义---------------------------------
//  注意 程序代码如果已经发布并进行商用，添加参数只允许在后面增加
typedef struct
{
	unsigned int paramInitFlag;//初始化标志

	unsigned char SecurityAuthSerial[6];//安全认证编码
	unsigned char mPOSEntrySerial[5];//MPOS入网编号
	unsigned char DeviceType[8];//设备型号
	unsigned char DeviceVersion[20];
	unsigned char FirmwareVersion[20];
	unsigned char SoftVersionSource[2];
	unsigned char	TermianalAPPVersion[6];
	unsigned char DeviceSerialNO[24];
	unsigned char BatchUploadFlag;
	unsigned char	DeviceModelState[8];	
}DEVICE_VERSION_INFO;

//----------------------------外部接口---------------------------
extern DEVICE_VERSION_INFO *pdevice_version_info;
extern const DEVICE_VERSION_INFO device_info_init;
extern const unsigned char g_cucbDevicePara[];

void Update_Parameters(unsigned char *psrc,unsigned char *pdest,unsigned char len);

void Input_DeviceSerialNO(void);

unsigned char GET_APPVersion(unsigned char *pIn);

unsigned char GET_DeviceSerialNO(unsigned char *pIn);

void Update_DeviceSerialNO(unsigned char *pIN);

void Update_APPVersion(void);

void Device_Init(void);

#endif
