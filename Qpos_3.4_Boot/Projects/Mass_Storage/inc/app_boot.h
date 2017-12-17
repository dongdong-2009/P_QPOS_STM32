#ifndef __APP_BOOT_H
#define __APP_BOOT_H

#define BOOT_TEST	0

#define NULL	0

#define BOOT_VERSION	"1.5.7.14"


#define BOOT_DATA_BASE_ADDR		0x08005E00

#define BOOT_KEY_ADDR				BOOT_DATA_BASE_ADDR
#define BOOT_KEY_LEN				16


#define BOOT_FLAG_ADDR				BOOT_KEY_ADDR+BOOT_KEY_LEN
#define BOOT_FLAG_LEN				1

#define BOOTLOADER_LEN_OF_FILE_ADDR 	(BOOT_FLAG_ADDR + BOOT_FLAG_LEN)	//3
#define BOOTLOADER_LEN_OF_FILE_LEN		4	//3

#define DEVICE_INFO_DEV_VER_ADDR	0x08005E00+128
#define DEVICE_INFO_DEV_VER_ADDR_LEN	10

#define APP_BASE_ADDR				0x08006000

#define SPI_FONT_BASE_ADDR		0x00000000
#define SPI_FLASH_FIRMWARE_ADDR	   	0x00100000		//外接SPI FLASH写入地址

#define FILE_MAX_LEN	0x34000	//208K  256K-boot-设备信息(0x8003a00)

typedef struct{
	unsigned char CLA;
	unsigned char INS;
	unsigned char PP1;
	unsigned char PP2;
	unsigned char PP3;
	unsigned char PP4;
	unsigned char PP5;
	unsigned int  RLE;		   	//实际要返回的数据长度
	unsigned char SW1;
	unsigned char SW2;
	unsigned char *pInData;		//除去命令之外的数据区指针
	unsigned char *pOutData;	//要返回的数据区的指针
}APDU;

extern APDU	G_APDU;



#define	SW_OPERATION_SUCCESS		0x9000
#define	SW_P1P2_ERROR				0x6a86
#define   SW_VERIFY_CRC_ERROR		0X7000 //CRC错误


//用到的命令码
#define CMD_BOOT_VERSION			0x00
#define CMD_BOOT_DNLOAD_KEY		0x01	   //下载或读取密钥
#define CMD_DNLOAD_DATA_DES		0x02	   //3DES密文烧写
#define CMD_BOOT_CRC				0x03	   //校验CRC
#define CMD_SPIFLASH_GETID			0x04
#define CMD_SPIFLASH_READ			0x05
#define CMD_SPIFLASH_WRITE		0x06	   //校验CRC

#endif

