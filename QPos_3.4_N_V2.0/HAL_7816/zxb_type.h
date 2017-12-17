#ifndef __ZXB_TYPE_H__
#define __ZXB_TYPE_H__

//typedef unsigned long       u32;
//typedef unsigned short      u16;
//typedef unsigned char       u8;
/*
typedef struct{
	unsigned char type;
	unsigned char length[4];
	unsigned char slot;
	unsigned char seq;
	unsigned char param[3];
	unsigned char dat[1];
}CCID_HDR;

typedef struct{

	unsigned char *pCommBuff;	//输入输出数据缓冲区BUF指针，用于通信

	unsigned char CLA;
	unsigned char INS;
	unsigned char PP1;
	unsigned char PP2;
	unsigned char  PP3;
	unsigned char  LC;
	unsigned char  LE;
	unsigned char *pInData;		//除去命令之外的数据区指针

	unsigned int  RLE;		   	//实际要返回的数据长度
	unsigned char *pOutData;	//要返回的数据区的指针
	unsigned char SW1;
	unsigned char SW2;
}APDU;

*/


typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
/* ADPU-Header command structure ---------------------------------------------*/
typedef struct
{
  uint8_t CLA;  /* Command class */
  uint8_t INS;  /* Operation code */
  uint8_t P1;   /* Selection Mode */
  uint8_t P2;   /* Selection Option */
} SC_Header;

/* ADPU-Body command structure -----------------------------------------------*/
typedef struct 
{
  uint8_t LC;           /* Data field length */
  uint8_t Data[0xff];  /* Command parameters */
  uint8_t LE;           /* Expected length of data to be returned */
} SC_Body;

/* ADPU Command structure ----------------------------------------------------*/
typedef struct
{
  SC_Header Header;
  SC_Body Body;
} SC_ADPU_Commands;

/* SC response structure -----------------------------------------------------*/
typedef struct
{
  uint8_t Data[0xff];  /* Data returned from the card */
  uint8_t SW1;          /* Command Processing status */
  uint8_t SW2;          /* Command Processing qualification */
} SC_ADPU_Responce;



#endif

