#include "string.h"
#include	"APP_parameters.h"
#include "UP_MPOS_CMD.h"
#include "Bluetooth.h"
#include "IROM.h"
#include "Sercrity.h"
#include "Key.h"
#include "Buzzer.h"
#include "sercrity.h"
#include	"IC_interface.h"
#include	"lcd_gui.h"

DEVICE_VERSION_INFO *pdevice_version_info=(DEVICE_VERSION_INFO*)FLASH_DEVICE_VERSION_INFO;

const DEVICE_VERSION_INFO device_info_init=
{
	0x12345678,	
	//安全认证编号	6 PP3342	
	"PP3342",
	//银联入网许可号  5	MP003
	"MP013",
	//设备型号	8  PSC-043-A									
	"PSC043A ",
	//硬件版本号   20	V000_000_001		
 	"V03                 ",
	//固件版本号   20	V000_000_024
	"1.1                 ",
	//终端应用版本来源码  2	  32
	"32",	
	//终端应用版本号  6	 410201											
	"410201",					

	"013000000000000000000000",
	//批上送指示	1
	0, 
	//硬件模块状态	8 	
	{0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00}
};
const unsigned char g_cucbDevicePara[FLASH_ADDR_DEVICE_PARA_MAX]= 
{
	//主控芯片总线宽度	0x00：8位.0x01：16位.0x02：32位.0x03：64位
	0xDF, 0x60, 0x01, 0x02,
	//主控芯片片上非易失性存储大小 
	0xDF, 0x61, 0x04,0x00,0x00, 0x01, 0x00, 
	//主控芯片片上易失性存储大小
	0xDF, 0x62, 0x04, 0x00,0x00, 0x00, 0x30,  
	//存储器大小
//	0xDF, 0x63, 0x04, 0x00, 0x01, 0x00, 0x00,   
	//蓝牙版本
	0xDF, 0x64, 0x01, 0x02,  
	//支持的通讯方式
	0xDF, 0x65, 0x01, 0x03, 
	//接触式IC卡支持的卡片电压
	0xDF, 0x66, 0x01, 0x03, 
	//接触式IC卡支持情况
	0xDF, 0x67, 0x02, 0x00,0x01,
	//非接触式IC卡支持情况
	0xDF, 0x68, 0x01, 0x03, 
	//手写签名支持情况
	0xDF, 0x69, 0x01, 0x00, 
	//打印机支持情况
	0xDF, 0x70, 0x01, 0x00, 
	//磁条卡
	//0xDF, 0x71, 0x01, 0x0f,
	//屏幕类型
	0xDF, 0x72, 0x01, 0x00,
	//屏幕分辨率
	0xDF, 0x73, 0x04, 0x01, 0x28, 0x00, 0x64,
	//显示行列
	0xDF, 0x74, 0x02, 0x05, 0x20, 
	//字库支持
	0xDF, 0x75, 0x01, 0x01, 
	//安全芯片支持的算法
	0xDF, 0x76, 0x02, 0x07, 0x00,
	//RSA算法密钥长度
	0xDF, 0x77, 0x02, 0x10,0x24,
	//SHA-1算法密钥长度
	0xDF, 0x78, 0x02, 0x10,0x24,
	
};
//****************将psrc的数据写到pdest指向的内存
void Update_Parameters(unsigned char *psrc,unsigned char *pdest,unsigned char len)
{
	IROM_Write((unsigned int)pdest,psrc,len);
}
unsigned char GET_APPVersion(unsigned char *pIn)
{
	unsigned char len;
	len=sizeof(pdevice_version_info->TermianalAPPVersion);
	memcpy(pIn,pdevice_version_info->TermianalAPPVersion,len);
	return len;
}

unsigned char GET_DeviceSerialNO(unsigned char *pIn)
{
	unsigned char len;
	len=sizeof(pdevice_version_info->DeviceSerialNO);
	memcpy(pIn,pdevice_version_info->DeviceSerialNO,len);
	return len;
}

void Update_DeviceSerialNO(unsigned char *pIN)
{	
	unsigned char ucbBuf[16] = "UP-013-00000000";
	
	Update_Parameters(pIN,pdevice_version_info->DeviceSerialNO,24);
	memcpy(ucbBuf + 8,pdevice_version_info->DeviceSerialNO+16, 8);	
	iBluetooth_ChangeBTName(ucbBuf, 16);
	
}
void Update_APPVersion(void)
{
	if(strcmp(device_info_init.TermianalAPPVersion,pdevice_version_info->TermianalAPPVersion)!=0)
	{
		Update_Parameters((unsigned char *)device_info_init.TermianalAPPVersion,pdevice_version_info->TermianalAPPVersion,6);
	}
}

void Input_DeviceSerialNO(void)
{
	unsigned char buf[24];
	unsigned char ucKey;
	unsigned char num;

	num=0;
	vLCD_ClearWorkPlace2345();

	vLCD_GUI_ShowMid12X12("输入序列号(测试机)",2);
	while(1)
	{
		ucKey=ucKey_Scan(1);

		if((ucKey >= '0') && (ucKey <= '9')&&(num<24))
		{
			buf[num]=ucKey;
			if(num<20)
				vLCD_GUI_Show12X12_Ex(&ucKey,1,3,num*6);
			else
				vLCD_GUI_Show12X12_Ex(&ucKey,1,4,(num-20)*6);
			num++;
		}
		else if(ucKey=='E')
		{
			vLCD_GUI_ShowMid12X12("取消输入",5);
			return ;
		}
		else if(ucKey=='C')
		{	
			if(num>0)
			{
				num--;
				if(num<20)
				{
					vLCD_GUI_Show12X12_Ex(" ",1,3,num*6);					
				}
				else
				{
					vLCD_GUI_Show12X12_Ex(" ",1,4,(num-20)*6);
				}				
			}						
		}
		else if(ucKey=='O')
		{
			if(num==24)
			{
				vLCD_GUI_ShowMid12X12("配置中",5);
				Update_DeviceSerialNO(buf);				
				vLCD_ClearWorkPage(5);
				vLCD_GUI_ShowMid12X12("设置成功",5);
				return;
			}
			else
			{
				vLCD_GUI_ShowMid12X12("长度错误",5);
			}
		}

	}
}
void Device_Init(void)
{
	unsigned char ucbTMK[20] = 
	{
		0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x82
	};
	unsigned char ucbBuf[16] = "UP-013-00000000";
	unsigned char ucRet;
	
	if(pdevice_version_info->paramInitFlag!=PARAMETER_INIT_FLAG)	//未初始化
	{
		IROM_Write(FLASH_DEVICE_VERSION_INFO,(unsigned char *)&device_info_init.paramInitFlag,sizeof(DEVICE_VERSION_INFO));

		QPOSFileStruct();
		createPublicFile(0x0006);

		ucRet = seleteBinaryFile(0x0006);	
		if(ucRet)
		{
			return ;
		}
		else{}

		ucRet = updateBinaryFile(UN_TMK_ADDR, ucbTMK, 0x10);	
		if( ucRet)
		{
			return ;
		}
		memcpy(ucbBuf + 8,pdevice_version_info->DeviceSerialNO+16, 8);	
		iBluetooth_ChangeBTName(ucbBuf, 16);	
	}
	Update_APPVersion();

	return ;
	
}

