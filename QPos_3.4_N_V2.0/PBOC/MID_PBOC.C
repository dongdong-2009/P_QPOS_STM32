#include "stm32f10x.h"
#include "MID_PBOC.h"
#include "string.h"
#include "CCID_VALUE.h"
//#include "bee.h"
#include "APP_PBOC.h"
//#include "AU9541.h"
#include "PBOC_Var.h"

#include "IROM.h"
#include "SysTick_delay.h"
#include "HAL_7816.h"
#include "Z8D256.h"
#include "sercrity.h"
#include "UP_MPOS_CMD.h"
//extern APDU g_APDU;
//Private Variables
#if 0
unsigned char AID0[7] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10};	  					//Visa AID 	
unsigned char AID1[7] = {0xA0,0x00,0x00,0x03,0x33,0x01,0x01};	 						//银联 
unsigned char AID2[8] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x03};	  		//ANO3 	
unsigned char AID3[8] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x04};	  		//ANO4 		   	
unsigned char AID4[8] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x05};	  		//ANO5 		   	
unsigned char AID5[8] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x06};	  		//ANO6 		   
unsigned char AID6[8] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x07};	  		//ANO7 				
unsigned char AID7[7] = {0xA0,0x00,0x00,0x00,0x99,0x90,0x90};							//Non-EMV AID		
unsigned char AID8[5] = {0xA0,0x00,0x00,0x99,0x99};		
unsigned char AID9[7] = {0xA0,0x00,0x00,0x00,0x04,0x10,0x10};							//Mastercard AID	
unsigned char AID10[7] = {0xA0,0x00,0x00,0x00,0x65,0x10,0x10};						//JCB AID
unsigned char AID11[5] = {0xA1,0x22,0x33,0x44,0x55};											//ANOD	
unsigned char AID12[8] = {0xA0,0x00,0x00,0x00,0x25,0x01,0x05,0x01};				//AMEX AID 
unsigned char AID13[16] = {0xA0,0x00,0x00,0x00,0x03,0x10,0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};		//ANOE
unsigned char AID14[6] = {0xA0,0x00,0x00,0x99,0x99,0x01};									//Test

#else

const unsigned char cucAIDCnt;

const unsigned char cucAIDBuf[15 * 128] = 
{
	//1 Visa AID 	
	0x9f,0x06,0x07,0xA0,0x00,0x00,0x00,0x03,0x10,0x10, 
	0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//2 银联
	0x9f,0x06,0x07,0xA0,0x00,0x00,0x03,0x33,0x01,0x01,
	0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//3 ANO3 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x03,0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//4 ANO4 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x04,0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//5 ANO5 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x05,0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//6 ANO6 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x06,0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//7 ANO7 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x07,0xdf,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//8 Non-EMV AID	
	0x9f,0x06,0x07,0xA0,0x00,0x00,0x00,0x99,0x90,0x90,
	0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//9
	0x9f,0x06,0x05,0xA0,0x00,0x00,0x99,0x99,0xdf,0x01,
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//10 Mastercard AID
	0x9f,0x06,0x07,0xA0,0x00,0x00,0x00,0x04,0x10,0x10,
	0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//11 JCB AID
	0x9f,0x06,0x07,0xA0,0x00,0x00,0x00,0x65,0x10,0x10,
	0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//12 ANOD
	0x9f,0x06,0x05,0xA1,0x22,0x33,0x44,0x55,0xdf,0x01,
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//13 AMEX AID 
	0x9f,0x06,0x08,0xA0,0x00,0x00,0x00,0x25,0x01,0x05,
	0x01,0xdf,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//14 ANOE
	0x9f,0x06,0x10,0xA0,0x00,0x00,0x00,0x03,0x10,0x10,
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xdf,
	0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	//15 Test
	0x9f,0x06,0x06,0xA0,0x00,0x00,0x99,0x99,0x01,0xdf,
	0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
#endif
unsigned char DO_TYPE_LIST[] = {
					 //Reader DO LIST
		0x9F, 0x7A, BINARY_TAG, 							  	//电子现金终端指示器			 		
		0x5F, 0x2A, NUMERIC_TAG, 	 							//交易货币代码
		0x9F, 0x1A, NUMERIC_TAG, 								//终端国家代码
		0x9F, 0x33, BINARY_TAG, 									//终端性能
		0x9F, 0x40, BINARY_TAG,   	  						//终端附加性能,编码表未指定
		0x9F, 0x1E, AN_TAG, 											//终端IFD序号
		0x9F, 0x35, NUMERIC_TAG, 		 						//终端类型
		0x9F, 0x1C, AN_TAG, 											//终端标识
		0x5F, 0x57, NUMERIC_TAG, 								//账户类型:默认-未指定
		0x9F, 0x01, NUMERIC_TAG, 								//收单行标识
		0x9F, 0x4E, ANS_TAG, 										//商户名称
		0x9F, 0x16, ANS_TAG,											//Merchant Identifier
		0x9F, 0x15, NUMERIC_TAG,									//Merchant Category Code
		0x9F, 0x1D,AN_TAG,												//Terminal Risk Management Data
		0x9F, 0x02, NUMERIC_TAG, 								//授权金额
		0x9F, 0x03, NUMERIC_TAG, 								//其他金额
		0x95, BINARY_TAG,            						//TVR
		0x9A, NUMERIC_TAG, 											//交易日期
		0x9C, NUMERIC_TAG, 	            				//交易类型
		0x9F, 0x37, BINARY_TAG, 									//不可预知数
		0x9F, 0x21, NUMERIC_TAG,  								//交易时间
		0x9B, BINARY_TAG,               					//TSI
		0x9F, 0x09, BINARY_TAG, 		  						//终端应用版本号
		0x9F, 0x34, BINARY_TAG,     							//CVM验证结果
		0x8A, AN_TAG, 	                					//授权响应码			  ???Issuer Data
		0x9F, 0x1B, NUMERIC_TAG,     						//终端最低限额	 		???
 		0x91, BINARY_TAG,  											//发卡行认证数据    ??? Issuer Data
		0x5F, 0x36, NUMERIC_TAG,									//Transaction Currency Exponent
		0x9F, 0x3A, BINARY_TAG,									//Amount,Reference Currency (Binary)
		0x81, BINARY_TAG, 												//授权金额 (Binary)
		0x9F, 0x04, NUMERIC_TAG,									//第二其他金额	
		0x9F, 0x3C, NUMERIC_TAG,  								//Transaction Reference Currency Code
		0x9F, 0x3D, NUMERIC_TAG, 								//Transaction Reference Currency Exponent
		0x9F, 0x41, NUMERIC_TAG, 							  //Transaction Sequence Counter
		0x99, BINARY_TAG, 		  									//PIN		
		0x98, BINARY_TAG, 												//TC Hash Value
		//Card DO LIST
		0x9f, 0x26, BINARY_TAG, 									//AC
		0x9f, 0x27, BINARY_TAG,  								//CID
		0x9f, 0x10, BINARY_TAG,  								//发卡行应用数据
		0x9f, 0x36, BINARY_TAG,  								//ATC
		0x82, BINARY_TAG,  											//AIP
		0x84, BINARY_TAG,  											//AID
		0x9f, 0x08, BINARY_TAG,  								//应用版本
		0x9f, 0x74, BINARY_TAG,  								//电子现金发卡行授权码		???
		0x57, BINARY_TAG,  											//磁条2等效数据	 max.19
		0x5f, 0x20, ANS_TAG,  										//持卡人姓名 2~26   
		0x9f, 0x61, NUMERIC_TAG,  								//持卡人证件号 1~40		???
		0x5f, 0x25, NUMERIC_TAG,  								//应用生效期
		0x5f, 0x24, NUMERIC_TAG,  								//应用失效期
		0x5a, COMPRESS_NUMERIC_TAG,  						//PAN, max. 10
		0x5f, 0x34, NUMERIC_TAG,  								//PAN序列号
		0x9f, 0x07, BINARY_TAG,  								//AUC应用用途控制
		0x8e, BINARY_TAG,  											//CVM,8~256,type.32
		0x9f, 0x0d, BINARY_TAG,  								//发卡行行为码-default
		0x9f, 0x0e, BINARY_TAG,  								//发卡行行为码-denial
		0x9f, 0x0f, BINARY_TAG,  								//发卡行行为码-online
		0x5f, 0x28, NUMERIC_TAG,  								//发卡行国家代码
		0x5f, 0x30, NUMERIC_TAG,  								//服务码
		0x8c, BINARY_TAG,  											//CDOL1, max252
		0x8d, BINARY_TAG,  											//CDOL2, max252
		0x9f, 0x42, NUMERIC_TAG,  								//应用货币代码
		0X9f, 0x1f, ANS_TAG,  										//Track 1 Data
		0x8f, BINARY_TAG,     										//CA Key Index
		0x9F, 0x44, NUMERIC_TAG,  								//Application Currency Exponent
		0x9F, 0x05, BINARY_TAG, 									//Application Discretionary Data
		0x94, BINARY_TAG,  											//AFL
		0x4F, BINARY_TAG,   											//ADF Name
		0x50, ANS_TAG,  													//AppLabel
		0x87, BINARY_TAG,  											//API
		0x9F, 0x3B, NUMERIC_TAG,  								//Application Reference Currency
		0x9F, 0x43, NUMERIC_TAG,   							//Application Reference Currency Exponent
		0x5F, 0x54, BINARY_TAG,  								//Bank Identifier Code
		0x9F, 0x0B, ANS_TAG,   									//Cardholder Name Extended
		0x9F, 0x45, BINARY_TAG,  								//Data Authentication Code
		0x9F, 0x49, BINARY_TAG,  								//DDOL
		0x9D, BINARY_TAG,   											//DDF Name
		0xBF, 0x0C, ANS_TAG,   									//FCI Issuer Discretionary Data
		0x9F, 0x4C, BINARY_TAG,   								//ICC Dynamic Number
		0x9F, 0x2D, BINARY_TAG,  								//ICC PIN Encipherment Public Key Certificate
		0x9F, 0x2E, BINARY_TAG,  								//ICC PIN Encipherment Public Key Exponent
		0x9F, 0x2F, BINARY_TAG,  								//ICC PIN Encipherment Public Key Reminder
		0x9F, 0x46, BINARY_TAG,  								//ICC Public Key Certificate
		0x9F, 0x47, BINARY_TAG,  								//ICC Public Key Exponent
		0x9F, 0x48, BINARY_TAG,  								//ICC Public Key Reminder
		0x5F, 0x53, BINARY_TAG, 									//International Bank Account Number
		0x9F, 0x11, NUMERIC_TAG, 								//Issuer Code Table Index
		0x5F, 0x55, ALPHABETIC_TAG, 							//Issuer Country Code (alpha2 format)
		0x5F, 0x56, ALPHABETIC_TAG, 							//Issuer Country Code (alpha3 format)
		0x42, NUMERIC_TAG,  											//Issuer Identification Number (IIN)
		0x90, BINARY_TAG,  											//Issuer Public Key Certificate
		0x9F, 0x32, BINARY_TAG,  								//Issuer Public Key Exponent
		0x92, BINARY_TAG,  											//Issuer Public Key Remainder
		0x5F, 0x50, ANS_TAG, 										//Issuer URL
		0x5F, 0x2D, AN_TAG, 											//language Preference
		0x9F, 0x13, BINARY_TAG, 									//Last ATC
		0x9F, 0x4D, BINARY_TAG, 									//Log Entry
		0x9F, 0x4F, BINARY_TAG, 									//Log Format
		0x9F, 0x14, BINARY_TAG, 									//LCOL
		0x9F, 0x17, BINARY_TAG, 									//PIN Try Counter
		0x88, BINARY_TAG, 												//SFI 	
		0x9F, 0x4B,  														//SDA Data
		0x93, BINARY_TAG, 												//SSA Data
		0x9F, 0x4A, BINARY_TAG, 									//SDA Tag List
		0x9F, 0x20, COMPRESS_NUMERIC_TAG, 				//Track 2 Discretionary Data
		0x9F, 0x23, BINARY_TAG,  								//UCOL
		0x97, BINARY_TAG,  											//TDOL
		0x9F, 0x12, ANS_TAG,  										//App Prefered Name
		0x9F, 0x38, BINARY_TAG,  								//PDOL
};		


void TIM_Configuration(void)
{ 
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=255;           //2000;//自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler=(36000 - 1); // 时钟预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; // 采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down;//TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除溢出中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);// / 开启时钟
}

// void MID_ALG_GenRand(unsigned char *buf, unsigned int len)
// {
// 	unsigned int i = 0;
// 	unsigned int j = 0;
// 	unsigned char	uintRandom_Seed = 0;
// 	for(i = 0; i < len; i++)
// 	{
// 		uintRandom_Seed = TIM_GetCounter(TIM2);	
// 		buf[i] = uintRandom_Seed;
// 		for(j = 0; j<0x3ff99;j++);
// //		Delay_ms(1);
// 	}
// }

//void MID_ALG_GenRand(unsigned char *pucBuf, unsigned int uiLen)
//{
//	unsigned int uiCount;
//	unsigned char pucTemp[16];
//
//	if(pucBuf == NULL || uiLen == 0)
//	{
//		return;	
//	}
//	else{}
//
//	if(uiLen > 0)
//	{
//		uiCount = TIM_GetCounter(TIM1);
//		pucTemp[0] = uiCount % 10;
//		pucTemp[1] = (uiCount % 100) / 10;
//		pucTemp[2] = (uiCount % 1000) / 100;
//		pucTemp[3] = (uiCount % 10000) / 1000;
//		pucTemp[4] = (uiCount % 100000) / 10000;
//	}
//	else{}
//	if(uiLen > 4)
//	{
//		uiCount = TIM_GetCounter(TIM1);
//		pucTemp[5] = uiCount % 10;
//		pucTemp[6] = (uiCount % 100) / 10;
//		pucTemp[7] = (uiCount % 1000) / 100;
//		pucTemp[8] = (uiCount % 10000) / 1000;
//		pucTemp[9] = (uiCount % 100000) / 10000;
//	}
//	else{}
//	if(uiLen > 8)
//	{
//		uiCount = TIM_GetCounter(TIM1);
//		pucTemp[10] = uiCount % 10;
//		pucTemp[11] = (uiCount % 100) / 10;
//		pucTemp[12] = (uiCount % 1000) / 100;
//		pucTemp[13] = (uiCount % 10000) / 1000;
//		pucTemp[14] = (uiCount % 100000) / 10000;
//		pucTemp[15] = '\0';
//	}
//	else{}
//
//	if(uiLen < 16)
//	{	 
//		makekey((char *)pucTemp, pucTemp);
//	}
//	else
//	{
//		make2key((char *)pucTemp, pucTemp);
//	}
//
//	memcpy(pucBuf, pucTemp, uiLen);
//	return;
//}

unsigned int MID_NVMEM_Read(unsigned int StartAddr, unsigned int ReadLen, unsigned char * puchDataBuff)
{  
//	BOOT_NVMEM_Read(StartAddr, ReadLen, puchDataBuff);
	IROM_Read(StartAddr, puchDataBuff, ReadLen);
	return 0x9000;
}

unsigned int MID_NVMEM_Write(unsigned int StartAddr, unsigned int WriteLen, unsigned char* puchDataBuff)
{  
//	BOOT_NVMEM_Write(StartAddr, WriteLen, puchDataBuff);
	IROM_Write(StartAddr, puchDataBuff, WriteLen);
	return 0x9000;
}

//在指定的TL列表中查找指定Tag是否存在
unsigned char IsTagExist(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag)
{
	unsigned char  uchTag[2];
	unsigned char  uchTagLen;
	unsigned short i;
	unsigned char  *p;

	if(TLVString == NULL)
		return NULL;
	if(ushTag == 0)	//指定的Tag不能为0
		return NULL;

	p = TLVString;
	i = 0;
	uchTag[0] = (unsigned char)((ushTag & 0xff00) >> 8);
	uchTag[1] = (unsigned char)(ushTag	& 0xff);

	if(uchTag[0] == 0)	//如果高字节为0，说明Tag是单字节的
	{
		uchTagLen = 1;	   //设定Tag长度
		uchTag[0] = uchTag[1];	
	}
	else if((uchTag[0] & 0x1f) == 0x1f)	 //如果Tag高字节不为0，检查高字节的低5位是否全1
	{
		uchTagLen = 2;	//设定Tag长度
	}
	else   //如果Tag高字节的低5位非全1，则Tag格式错误
	{
		return NULL;
	}

	while(memcmp(p, uchTag, uchTagLen) && (i < ushStringLen))	 //在TL字符串中查找Tag
	{								
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				p += 3;
				i += 3;
			}
			else
			{
				p += 2;
				i += 2;				
			}
	}

	if(i < ushStringLen)
		return EMV_TRUE;
	else
		return EMV_FALSE;	
}

//在DO_TYPE_LIST列表中查找指定Tag的数据类型,
unsigned char SearchTagType(unsigned short ushTag)
{
	unsigned char  uchTag[2];
	unsigned char  uchTagLen;
	unsigned short i;
	const unsigned char  *p;

	if(ushTag == 0)	//指定的Tag不能为0
		return NULL;

	p = DO_TYPE_LIST;
	i = 0;
	uchTag[0] = (unsigned char)((ushTag & 0xff00) >> 8);
	uchTag[1] = (unsigned char)(ushTag	& 0xff);

	if(uchTag[0] == 0)	//如果高字节为0，说明Tag是单字节的
	{
		uchTagLen = 1;	   //设定Tag长度
		uchTag[0] = uchTag[1];	
	}
	else if((uchTag[0] & 0x1f) == 0x1f)	 //如果Tag高字节不为0，检查高字节的低5位是否全1
	{
		uchTagLen = 2;	//设定Tag长度
	}
	else   //如果Tag高字节的低5位非全1，则Tag格式错误
	{
		return NULL;
	}

	while(memcmp(p, uchTag, uchTagLen) && (i < sizeof(DO_TYPE_LIST)))	 //在TL字符串中查找Tag
	{								
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				p += 3;
				i += 3;
			}
			else
			{
				p += 2;
				i += 2;				
			}
	}

	if(i < sizeof(DO_TYPE_LIST))
		return *(p+uchTagLen);
	else
		return TYPE_NOT_FOUND;	
}

//在TLV结构的字符串中查找指定Tag的数据对象
unsigned char *SearchTag(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag)
{
	unsigned char  uchTag[2];
	unsigned char  uchTagLen;
	unsigned char  uchLen;
	unsigned short i;
	unsigned char  *p;

	if(TLVString == NULL)
		return NULL;
	if(ushTag == 0)	//指定的Tag不能为0
		return NULL;

	p = TLVString;
	i = 0;
	uchTag[0] = (unsigned char)((ushTag & 0xff00) >> 8);
	uchTag[1] = (unsigned char)(ushTag	& 0xff);

	if(uchTag[0] == 0)	//如果高字节为0，说明Tag是单字节的
	{
		uchTagLen = 1;	   //设定Tag长度
		uchTag[0] = uchTag[1];	
	}
	else if((uchTag[0] & 0x1f) == 0x1f)	 //如果Tag高字节不为0，检查高字节的低5位是否全1
	{
		uchTagLen = 2;	//设定Tag长度
	}
	else   //如果Tag高字节的低5位非全1，则Tag格式错误
	{
		return NULL;
	}

	while(memcmp(p, uchTag, uchTagLen) && (i < ushStringLen))	 //在TLV字符串中查找Tag
	{		
		if(((*p == 0x00) || (*p == 0xFF)) && (i < ushStringLen))
		{
				p++;
				i++;
			  continue;
		}
	
		if((*p & 0x20))	 //如果某Tag的第6位是1，表示该数据对象是复合型的，下次查找是在该数据对象内部
		{
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				if(*(p+2) < 0x80)
				{ 
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+2); //获取数据对象长度
								p += 3+uchLen;
								i += 3+uchLen;
						}
						else
						{
								p += 3;
								i += 3;
						}
				}
				else if(*(p+2) == 0x81)
				{
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+3); //获取数据对象长度
								p += 4+uchLen;
								i += 4+uchLen;
						}
						else
						{
								p += 4;
								i += 4;
						}
				}
				else{return NULL;}
			}
			else
			{
				if(*(p+1) < 0x80)
				{ 
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+1); //获取数据对象长度
								p += 2+uchLen;
								i += 2+uchLen;
						}		
						else
						{
								p += 2;
								i += 2;
						}
				}
				else if(*(p+1) == 0x81)
				{
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+2); //获取数据对象长度
								p += 3+uchLen;
								i += 3+uchLen;
						}
						else
						{
								p += 3;
								i += 3;
						}
				}
				else{return NULL;}	  				
			}
		}
		else //如果某Tag的第6位是0，表示该数据对象是基本型的，下次查找是从下一个数据对象开始
		{
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				if(*(p+2) < 0x80)
				{
					uchLen = *(p+2); //获取数据对象长度
					p += 3+uchLen;
					i += 3+uchLen;
				}
				else if(*(p+2) == 0x81)
				{
					uchLen = *(p+3); //获取数据对象长度
					p += 4+uchLen;
					i += 4+uchLen;
				}
				else{return NULL;}
			}
			else
			{				
				if(*(p+1) < 0x80)
				{
					uchLen = *(p+1); //获取数据对象长度
					if(*p == 0x86)
					{
						if(*(p+1) != *(p+6)+5)	
						{
							uchLen = *(p+6)+5; //获取数据对象长度								
						}
						else
						{
							 uchLen = *(p+1); //获取数据对象长度
						}
					}		
					
					p += 2+uchLen;
					i += 2+uchLen;
				}
				else if(*(p+1) == 0x81)
				{
					uchLen = *(p+2); //获取数据对象长度
					p += 3+uchLen;
					i += 3+uchLen;
				}
				else{return NULL;} 				
			}
		}
	}

	if(i<ushStringLen)
		return p;
	else
		return NULL;	
}

//在TLV结构的字符串中查找指定Tag的数据对象
unsigned char *SearchTagforScript(unsigned char *TLVString, unsigned short ushStringLen, unsigned short ushTag)
{
	unsigned char  uchTag[2];
	unsigned char  uchTagLen;
	unsigned char  uchLen;
	unsigned short i;
	unsigned char  *p;

	if(TLVString == NULL)
		return NULL;
	if(ushTag == 0)	//指定的Tag不能为0
		return NULL;

	p = TLVString;
	i = 0;
	uchTag[0] = (unsigned char)((ushTag & 0xff00) >> 8);
	uchTag[1] = (unsigned char)(ushTag	& 0xff);

	if(uchTag[0] == 0)	//如果高字节为0，说明Tag是单字节的
	{
		uchTagLen = 1;	   //设定Tag长度
		uchTag[0] = uchTag[1];	
	}
	else if((uchTag[0] & 0x1f) == 0x1f)	 //如果Tag高字节不为0，检查高字节的低5位是否全1
	{
		uchTagLen = 2;	//设定Tag长度
	}
	else   //如果Tag高字节的低5位非全1，则Tag格式错误
	{
		return NULL;
	}

	while(memcmp(p, uchTag, uchTagLen) && (i < ushStringLen))	 //在TLV字符串中查找Tag
	{		
		if(((*p == 0x00) || (*p == 0xFF)) && (i < ushStringLen))
		{
				p++;
				i++;
			  continue;
		}
	
		if((*p & 0x20))	 //如果某Tag的第6位是1，表示该数据对象是复合型的，下次查找是在该数据对象内部
		{
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				if(*(p+2) < 0x80)
				{ 
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+2); //获取数据对象长度
								p += 3+uchLen;
								i += 3+uchLen;
						}
						else
						{
								p += 3;
								i += 3;
						}
				}
				else if(*(p+2) == 0x81)
				{
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+3); //获取数据对象长度
								p += 4+uchLen;
								i += 4+uchLen;
						}
						else
						{
								p += 4;
								i += 4;
						}
				}
				else{return NULL;}
			}
			else
			{
				if(*(p+1) < 0x80)
				{ 
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+1); //获取数据对象长度
								p += 2+uchLen;
								i += 2+uchLen;
						}		
						else
						{
								p += 2;
								i += 2;
						}
				}
				else if(*(p+1) == 0x81)
				{
						if(TLVString == Card_Do_List)
						{
								uchLen = *(p+2); //获取数据对象长度
								p += 3+uchLen;
								i += 3+uchLen;
						}
						else
						{
								p += 3;
								i += 3;
						}
				}
				else{return NULL;}	  				
			}
		}
		else //如果某Tag的第6位是0，表示该数据对象是基本型的，下次查找是从下一个数据对象开始
		{
			if((*p & 0x1f) == 0x1f)	  //判断某Tag是单字节还是双字节
			{
				if(*(p+2) < 0x80)
				{
					uchLen = *(p+2); //获取数据对象长度
					p += 3+uchLen;
					i += 3+uchLen;
				}
				else if(*(p+2) == 0x81)
				{
					uchLen = *(p+3); //获取数据对象长度
					p += 4+uchLen;
					i += 4+uchLen;
				}
				else{return NULL;}
			}
			else
			{				
				if(*(p+1) < 0x80)
				{
					uchLen = *(p+1); //获取数据对象长度
					if(*p == 0x86)
					{
						if(*(p+1) != *(p+6)+5)	
						{
							uchLen = *(p+6)+5; //获取数据对象长度								
						}
						else
						{
							 uchLen = *(p+1); //获取数据对象长度
						}
					}
					else if(*p == 0x84)
					{
							uchLen = 3+*(p+4);
					}
					p += 2+uchLen;
					i += 2+uchLen;
				}
				else if(*(p+1) == 0x81)
				{
					uchLen = *(p+2); //获取数据对象长度
					p += 3+uchLen;
					i += 3+uchLen;
				}
				else{return NULL;} 				
			}
		}
	}

	if(i<ushStringLen)
		return p;
	else
		return NULL;	
}


//提取DOL中指定的数据对象
unsigned char CollectDol(unsigned char *uchDol, unsigned char uchDolLen, unsigned char *uchOutput,unsigned char uchDolType)
{
	unsigned char  uchOutLen;
	unsigned char  i, uchTagLen;
	unsigned short ushTag;
	unsigned char  *p, *ptr;
	unsigned char  uchTagType;
	unsigned char  uchLenLen;
	unsigned int 	 uintAddr;

	if((uchDol == NULL) || (uchOutput == NULL) || (uchDolLen < 2))
	{
		return 0;
	}

	ushTag = 0;
	uchOutLen = 0;
	uchLenLen = 0;
	uchTagType = TYPE_NOT_FOUND;
	i = 0;		
	p = uchDol;

	while(i < uchDolLen)
	{
		if((*p & 0x1f) == 0x1f)
		{
			ushTag = *p*256+*(p+1);
			uchTagLen = 2;	 								
		}
		else
		{
			ushTag = *p;
			uchTagLen = 1;	 			
		}
		
		ptr = SearchTag(Reader_VarDo_List, sizeof(Reader_VarDo_List), ushTag);
		if(ptr == NULL)
		{
			ptr = SearchTag(Reader_ConstantDo_List, sizeof(Reader_ConstantDo_List), ushTag);
			if(ptr == NULL)
			{
					if(uchDolType != PDOL_FLAG)
					{
						ptr = SearchTag(Card_Do_List, sizeof(Card_Do_List), ushTag);;
						if(ptr == NULL)
						{
							if(*(p+uchTagLen) < 0x80)
							{
								memset(uchOutput+uchOutLen, 0, *(p+uchTagLen));
								uchOutLen += *(p+uchTagLen);
							}
							else if(*(p+uchTagLen) == 0x81)
							{
								memset(uchOutput+uchOutLen, 0, *(p+uchTagLen+1));
								uchOutLen += *(p+uchTagLen+1);
							}
							else{}
						}
						else
						{
							uchTagType = SearchTagType(ushTag);

							memcpy((unsigned char *)&uintAddr, ptr+uchTagLen+1, 4);	  //获取该DO在Ram中的存储地址
							if(*(p+uchTagLen) < 0x80)
							{
								uchLenLen = 0;
							}
							else if(*(p+uchTagLen) == 0x81)
							{
								uchLenLen = 1;	
							}
							else
							{
							}	
							
							if(*(p+uchTagLen+uchLenLen) < *((unsigned char *)uintAddr+uchLenLen))
							{
									if(NUMERIC_TAG == uchTagType)
									{		
											//leftmost bytes of the data element are truncated							
											memcpy(uchOutput+uchOutLen,(unsigned char *)uintAddr+uchLenLen+*((unsigned char *)uintAddr+uchLenLen)-*(p+uchTagLen+uchLenLen)+1, *(p+uchTagLen+uchLenLen));	 //保存该DO值和长度
									}
									else
									{
											//rightmost bytes of the data element are truncated
											memcpy(uchOutput+uchOutLen,(unsigned char *)uintAddr+uchLenLen+1, *(p+uchTagLen+uchLenLen));	 //保存该DO值和长度
									}
							}
							else if(*(p+uchTagLen+uchLenLen) > *((unsigned char *)uintAddr+uchLenLen))
							{
									if(NUMERIC_TAG == uchTagType)
									{		
											//padded with leading hexadecimal zeroes
											memset(uchOutput+uchOutLen, 0x00,*(p+uchTagLen+uchLenLen)-*((unsigned char *)uintAddr+uchLenLen));
											memcpy(uchOutput+uchOutLen+*(p+uchTagLen+uchLenLen)-*((unsigned char *)uintAddr+uchLenLen), (unsigned char *)uintAddr+uchLenLen+1, *((unsigned char *)uintAddr+uchLenLen));										
									}
									else if(COMPRESS_NUMERIC_TAG == uchTagType)
									{
											//padded with trailing hexadecimal FF's
											memcpy(uchOutput+uchOutLen, (unsigned char *)uintAddr+uchLenLen+1, *((unsigned char *)uintAddr+uchLenLen));
											memset(uchOutput+uchOutLen+*((unsigned char *)uintAddr+uchLenLen), 0xFF, *(p+uchTagLen+uchLenLen)-*((unsigned char *)uintAddr+uchLenLen));
									}
									else
									{
											//padded with trailing hexadecimal zeroes
												memcpy(uchOutput+uchOutLen, (unsigned char *)uintAddr+uchLenLen+1, *((unsigned char *)uintAddr+uchLenLen));
												memset(uchOutput+uchOutLen+*((unsigned char *)uintAddr+uchLenLen), 0x00, *(p+uchTagLen+uchLenLen)-*((unsigned char *)uintAddr+uchLenLen));
									}								
							}
							else
							{
									memcpy(uchOutput+uchOutLen,(unsigned char *)uintAddr+uchLenLen+1, *(p+uchTagLen+uchLenLen));	 //保存该DO值和长度						
							}
							uchOutLen += *(p+uchTagLen+uchLenLen);
					 }
				}
				else
				{
						if(*(p+uchTagLen) < 0x80)
						{
							memset(uchOutput+uchOutLen, 0, *(p+uchTagLen));
							uchOutLen += *(p+uchTagLen);
						}
						else if(*(p+uchTagLen) == 0x81)
						{
							memset(uchOutput+uchOutLen, 0, *(p+uchTagLen+1));
							uchOutLen += *(p+uchTagLen+1);
						}
						else{}
				}
			}
			else
			{
				uchTagType = SearchTagType(ushTag);
				
				if(*(p+uchTagLen) < 0x80)
				{
					uchLenLen = 0;
				}
				else if(*(p+uchTagLen) == 0x81)
				{
					uchLenLen = 1;		
				}
				else{}
								
				if(*(p+uchTagLen+uchLenLen) < *(ptr+uchTagLen+uchLenLen))
				{
						if(NUMERIC_TAG == uchTagType)
						{		
								//leftmost bytes of the data element are truncated							
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+*(ptr+uchTagLen+uchLenLen)-*(p+uchTagLen+uchLenLen)+1, *(p+uchTagLen+uchLenLen));
						}
						else
						{
								//rightmost bytes of the data element are truncated
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(p+uchTagLen+uchLenLen));
						}
				}
				else if(*(p+uchTagLen+uchLenLen) > *(ptr+uchTagLen+uchLenLen))
				{
						if(NUMERIC_TAG == uchTagType)
						{		
								//padded with leading hexadecimal zeroes
								memset(uchOutput+uchOutLen, 0x00,*(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
								memcpy(uchOutput+uchOutLen+*(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen), ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
						}
						else if(COMPRESS_NUMERIC_TAG == uchTagType)
						{
								//padded with trailing hexadecimal FF's
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
								memset(uchOutput+uchOutLen+*(ptr+uchTagLen+uchLenLen), 0xFF, *(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
						}
						else
						{
								//padded with trailing hexadecimal zeroes
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
								memset(uchOutput+uchOutLen+*(ptr+uchTagLen+uchLenLen), 0x00, *(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
						}
				}
				else
				{
						memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(p+uchTagLen+uchLenLen));
				}
				uchOutLen += *(p+uchTagLen+uchLenLen);
			}
		}
		else
		{
			uchTagType = SearchTagType(ushTag);
				
				if(*(p+uchTagLen) < 0x80)
				{
					uchLenLen = 0;
				}
				else if(*(p+uchTagLen) == 0x81)
				{
					uchLenLen = 1;		
				}
				else{}
								
				if(*(p+uchTagLen+uchLenLen) < *(ptr+uchTagLen+uchLenLen))
				{
						if(NUMERIC_TAG == uchTagType)
						{		
								//leftmost bytes of the data element are truncated							
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+*(ptr+uchTagLen+uchLenLen)-*(p+uchTagLen+uchLenLen)+1, *(p+uchTagLen+uchLenLen));
						}
						else
						{
								//rightmost bytes of the data element are truncated
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(p+uchTagLen+uchLenLen));
						}
				}
				else if(*(p+uchTagLen+uchLenLen) > *(ptr+uchTagLen+uchLenLen))
				{
						if(NUMERIC_TAG == uchTagType)
						{		
								//padded with leading hexadecimal zeroes
								memset(uchOutput+uchOutLen, 0x00,*(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
								memcpy(uchOutput+uchOutLen+*(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen), ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
						}
						else if(COMPRESS_NUMERIC_TAG == uchTagType)
						{
								//padded with trailing hexadecimal FF's
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
								memset(uchOutput+uchOutLen+*(ptr+uchTagLen+uchLenLen), 0xFF, *(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
						}
						else
						{
								//padded with trailing hexadecimal zeroes
								memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(ptr+uchTagLen+uchLenLen));
								memset(uchOutput+uchOutLen+*(ptr+uchTagLen+uchLenLen), 0x00, *(p+uchTagLen+uchLenLen)-*(ptr+uchTagLen+uchLenLen));
						}
				}
				else
				{
						memcpy(uchOutput+uchOutLen, ptr+uchTagLen+uchLenLen+1, *(p+uchTagLen+uchLenLen));
				}
				uchOutLen += *(p+uchTagLen+uchLenLen);
		}
		
		if(*(p+uchTagLen) < 0x80)
		{
			i += uchTagLen+1;
			p += uchTagLen+1;
		}
		else if(*(p+uchTagLen) == 0x81)
		{
			i += uchTagLen+2;
			p += uchTagLen+2;
		}
		else{}		
	}	
	return uchOutLen;
}

//从卡片读出的记录中搜集有用的数据对象，保存到读写器Ram中
unsigned short SaveCardDo(unsigned char *puchRecord, unsigned short ushRecLen)
{
	unsigned short i;
	unsigned char  *p, *ptr;
	unsigned short ushTag;
	unsigned char  uchTagLen;
	unsigned int   uintAddr;
	unsigned char  Tag70_Len;

	if((puchRecord == NULL) || (ushRecLen == 0))
		return SW_OPERATION_SUCCESS;
	p = puchRecord;
	i = 0;
	Tag70_Len = 0;
					
	if(*(p+1) < 0x80)
	{
		Tag70_Len = *(p+1)+2; 
	}
	else if(*(p+1) == 0x81)
	{
		Tag70_Len = *(p+2)+3; 
	}
	else
	{
		Nok_Reason = DATA_ERROR;
		return NOK;
	}				
				
	while(i < ushRecLen)
	{
		if((*p & 0x1f) == 0x1f)		//判断Tag是单字节还是双字节
		{
			ushTag = *p*256+*(p+1);
			uchTagLen = 2;	
			if((ushTag == 0x00) || (ushTag == 0xFFFF))
			{
					p += 2;
					i += 2;
					Tag70_Len -= 2;
					continue;
			}
		}
		else
		{
			ushTag = *p;
			uchTagLen = 1;	
			if((ushTag == 0x00) || (ushTag == 0xFF))
			{
					p++;
					i++;
					Tag70_Len--;
					continue;
			}
		}
		
		if(*p & 0x20)	//如果某Tag的第6位是1，表示该数据对象是复合型的
		{
			//复合型数据对象的TL直接跳过
			if(*(p+uchTagLen) < 0x80) //L是单字节
			{			
				p += (uchTagLen + 1);
				i += (uchTagLen + 1);
				Tag70_Len -= (uchTagLen + 1);
			}
			else if(*(p+uchTagLen) == 0x81)	  //L是双字节
			{
				p += (uchTagLen + 2);
				i += (uchTagLen + 2);
				Tag70_Len -= (uchTagLen + 2);
			}
			else
			{
			}		
		}
		else
		{
			ptr = SearchTag(Card_Do_List, sizeof(Card_Do_List), ushTag);	//在读写器中预设的卡片DO列表中查找指定DO
			if(ptr == NULL)	//如果在DO列表中没找到指定DO，则说明读写器不需要该DO，因此可忽略该DO值
			{
				if(*(p+uchTagLen) < 0x80)
				{
					Tag70_Len -= (uchTagLen + 1 + *(p+uchTagLen));
					i += (uchTagLen + 1 + *(p+uchTagLen));
					p += (uchTagLen + 1 + *(p+uchTagLen));					
				}
				else if(*(p+uchTagLen) == 0x81)
				{
					Tag70_Len -= (uchTagLen + 2 + *(p+uchTagLen+1));
					i += (uchTagLen + 2 + *(p+uchTagLen+1));
					p += (uchTagLen + 2 + *(p+uchTagLen+1));
				}
				else
				{
				}
			}
			else  //找到DO
			{
				memcpy((unsigned char *)&uintAddr, ptr+uchTagLen+1, 4);	  //获取该DO在Ram中的存储地址
				if(*(p+uchTagLen) < 0x80)
				{
					memcpy((unsigned char *)uintAddr, p+uchTagLen, 1+*(p+uchTagLen));	 //保存该DO值和长度
					Tag70_Len -= (uchTagLen + 1 + *(p+uchTagLen));
					i += (uchTagLen + 1 + *(p+uchTagLen));
					p += (uchTagLen + 1 + *(p+uchTagLen));
				}
				else if(*(p+uchTagLen) == 0x81)
				{
					memcpy((unsigned char *)uintAddr, p+uchTagLen+1, 1+*(p+uchTagLen+1));	 //保存该DO值和长度
					Tag70_Len -= (uchTagLen + 2 + *(p+uchTagLen+1));
					i += (uchTagLen + 2 + *(p+uchTagLen+1));
					p += (uchTagLen + 2 + *(p+uchTagLen+1));
				}
				else
				{
				}		
			}
		}
	}
	
	if(Tag70_Len > 0)
	{
		Nok_Reason = DATA_ERROR;
		return NOK;
	}
	
	return SW_OPERATION_SUCCESS;
}




/********************     EMV Processing Interface     ********************/
unsigned short EMV_Show_Candidate_List(unsigned char Len)
{
//	unsigned short i,j;
//	unsigned char  AscllKey = 0xff;
//	unsigned char  column=0;
//	unsigned short keyValue = 0;
//	unsigned char  currentPage;
//	unsigned char  currentLine;
//	unsigned char  AppNameNibble;
//	
//	if(0 == Len)
//		return 	NOK;
//
//	currentLine = 0;
//	currentPage = 0;
//
//	if(1 == Len)
//	{
//		HAL_DISP_CLS();
//		column = 0;
//		MID_DISP_ShowChar16X8(0,column,'1',ASCIIchardot);
//		column += 14;
//
//		if((Candidate_List[0].App_Preferred_Name_Len > 0)&&(0x01 == Candidate_List[0].IssuerCodeTableIndex))
//		{
//			for(i=0;i<Candidate_List[0].App_Preferred_Name_Len; i++)
//			{
//				MID_DISP_ShowChar16X8(0,column,Candidate_List[0].App_Preferred_Name[i],ASCIIchardot);
//				column += 7;
//			}
//		}
//		else if (Candidate_List[0].App_Label_Len > 0)
//		{
//			for(i=0;i<Candidate_List[0].App_Label_Len;i++)
//			{
//				MID_DISP_ShowChar16X8(0,column,Candidate_List[0].App_Label[i],ASCIIchardot);
//				column += 7;
//			}
//		}
//		else
//		{
//			for(i=0;i<Candidate_List[0].App_Name_Len;i++)
//			{
//				AppNameNibble = (Candidate_List[0].App_Name[i]&0xF0)>>4;
//				if(AppNameNibble >9)
//					AppNameNibble = 0x40+AppNameNibble-9;	
//				else
//					AppNameNibble = 0x30+AppNameNibble;
//				
//				MID_DISP_ShowChar16X8(0,column,AppNameNibble,ASCIIchardot);
//				column += 7;
//				
//				AppNameNibble = Candidate_List[0].App_Name[i]&0x0F;
//				if(AppNameNibble >9)
//					AppNameNibble = 0x40+AppNameNibble-9;	
//				else
//					AppNameNibble = 0x30+AppNameNibble;
//				
//				MID_DISP_ShowChar16X8(0,column,AppNameNibble,ASCIIchardot);
//				column += 7;
//			}
//		}
//		goto Key_Check;
//	}
//
//ShowAID:
//	column = 0;
//	HAL_DISP_CLS();
//	for(j=0;j<4;j++)
//	{			   		
//		currentLine = 4*currentPage+j;
//		if(currentLine >9)
//		{
//			column = 0;
//			if(1 == Candidate_List[currentLine].ADF_Valid)
//			{
////				MID_DISP_ShowChar16X8(2*j,column,((currentLine+1)/10)+0x30,ASCIIchardot);
//				column += 7;
//				MID_DISP_ShowChar16X8(2*j,column,((currentLine+1)%10)+0x30,ASCIIchardot);
//				column += 14;
//			}
//		}
//		else
//		{
//			column = 0;
//			if(1 == Candidate_List[currentLine].ADF_Valid)
//			{
//				MID_DISP_ShowChar16X8(2*j,column,currentLine+1+0x30,ASCIIchardot);
//				column += 14;
//			}
//		}
//
//		if((Candidate_List[currentLine].App_Preferred_Name_Len > 0)&&(0x01 == Candidate_List[currentLine].IssuerCodeTableIndex))
//		{
//			for(i=0;i<Candidate_List[currentLine].App_Preferred_Name_Len;i++)
//			{
//				MID_DISP_ShowChar16X8(2*j,column,Candidate_List[currentLine].App_Preferred_Name[i],ASCIIchardot);
//				column += 7;
//			}
//		}
//		else if (Candidate_List[currentLine].App_Label_Len > 0)
//		{
//			for(i=0;i<Candidate_List[currentLine].App_Label_Len;i++)
//			{
//				MID_DISP_ShowChar16X8(2*j,column,Candidate_List[currentLine].App_Label[i],ASCIIchardot);
//				column += 7;
//			}
//		}
//		else
//		{
//			for(i=0;i<Candidate_List[currentLine].App_Name_Len;i++)
//			{
//				AppNameNibble = (Candidate_List[currentLine].App_Name[i]&0xF0)>>4;
//				if(AppNameNibble >9)
//					AppNameNibble = 0x40+AppNameNibble-9;	
//				else
//					AppNameNibble = 0x30+AppNameNibble;
//				
//				MID_DISP_ShowChar16X8(2*j,column,AppNameNibble,ASCIIchardot);
//				column += 7;
//				
//				AppNameNibble = Candidate_List[currentLine].App_Name[i]&0x0F;
//				if(AppNameNibble >9)
//					AppNameNibble = 0x40+AppNameNibble-9;	
//				else
//					AppNameNibble = 0x30+AppNameNibble;
//				
//				MID_DISP_ShowChar16X8(2*j,column,AppNameNibble,ASCIIchardot);
//				column += 7;
//			}
//		}
//	}	
//
//Key_Check:
//	keyValue = 0;
//    while(1)
//	{
//		Key_task();
//		if((rtKey.valid == 1) && Translate_Key_Value(&AscllKey,rtKey.value) == 0)
//		{
//			if(((AscllKey == '0')||(AscllKey == '1')||(AscllKey == '2')||(AscllKey == '3')||(AscllKey == '4')||(AscllKey == '5')||(AscllKey == '6')||(AscllKey == '7')||(AscllKey == '8')||(AscllKey == '9')))
//			{
//				Bee_music(BEE_TPYE_1);
//				keyValue = keyValue*10+AscllKey-'0';
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				break;
//			}
//			else if(AscllKey == 'c')						//取消
//			{
//				Bee_music(BEE_TPYE_1);
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				MID_DISP_SmallPic(SMALLPIC_JIAOYIQUXIAO);	//TBD
//				return NOK;
//			}
//			if((AscllKey == 'u')&&(0 == keyValue))			//up
//			{
//				Bee_music(BEE_TPYE_1);
//				if(currentPage > 0)
//					currentPage--;	
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				goto ShowAID;
//			}
//			if((AscllKey == 'd')&&(0 == keyValue))			//down
//			{
//				Bee_music(BEE_TPYE_1);
//				if(currentPage < Len/4)
//					currentPage++;
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				goto ShowAID;
//			}
//			else
//			{
//			}
//			rtKey.valid = 0;
//			rtKey.value = 0;
//			AscllKey = 0xff; 
//		}
//	}
//
//	MID_DISP_SmallPic(SMALLPIC_CHULIZHONG);
//
//
//	if(keyValue > Candidate_Number)
//	{
//		return NOK;
//	}
//	return keyValue;
return 1;
}



//选择卡片ADF
unsigned short	EMV_SelectADF(unsigned char* AID,unsigned char AID_len,unsigned char select_param,unsigned char Match_Criteria)
{
	unsigned char *ptr = NULL;
	unsigned int  Tag6F_Len = 0;
	unsigned int  TagA5_Len = 0;
	unsigned char *pTag = NULL;
	unsigned char *pIn = NULL;
	unsigned char *pA5 = NULL;
	unsigned char totalLen = 0;
	unsigned int  nLen  = 0;
	unsigned char i,temp;

	if((NULL == AID) || (AID_len>16) || (AID_len<5))
	{
		Nok_Reason = NONE;
		return NOK;
	}

  g_APDU.CLA = 0x00;//*(g_APDU.pCommBuff+0) = 0x00;
	g_APDU.INS = 0xA4;//*(g_APDU.pCommBuff+1) = 0xA4;
	g_APDU.PP1 = 0x04;//*(g_APDU.pCommBuff+2) = 0x04;
	g_APDU.PP2 = select_param;//*(g_APDU.pCommBuff+3) = select_param;	//选择第一个还是下一个
	g_APDU.PP3 = AID_len;//*(g_APDU.pCommBuff+4) = AID_len;
	g_APDU.LC = AID_len;
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;
	memcpy(g_APDU.pInData, AID, AID_len);

//	AU9541_ICCRAD_TransCeive(&g_APDU);
//	EMV_ExchangeAPDU(ICC_Protocol,5+AID_len);
	HAL_Card_APDU(&g_APDU,5+g_APDU.LC);

	if((0x6A == g_APDU.SW1) && (0x81 == g_APDU.SW2))	  //Card is blocked or SELECT command is not supported, terminate the session
	{
		Nok_Reason = CARD_BLOCKED;
		return NOK;
	} 
	else if((0x62 == g_APDU.SW1) && (0x83 == g_APDU.SW2))  //Application is blocked (0x6283)
	{
		Nok_Reason = APPLICATION_BLOCKED;
		return NOK;
	}
	else if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))  
	{
		Nok_Reason = NONE;
		return NOK;
	}
	else   		//9000
	{	
		//保存当前选中的终端AID名称
		*(puchTerminalAID-1) = AID_len;
		memcpy(puchTerminalAID,AID,AID_len);   
		
		//查找FCI模板 (M)
		if(EMV_TAG_006F_FCI_TEMPLATE != g_APDU.pOutData[0])		  
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}

		if(g_APDU.pOutData[1] <0x80)
		{
			Tag6F_Len = g_APDU.pOutData[1];
			pIn = g_APDU.pOutData+2;
			totalLen = g_APDU.pOutData[1];
			if(g_APDU.RLE-Tag6F_Len-2 != 0)
			{
					Nok_Reason = DATA_ERROR;
				  return NOK;
			}		
		}
		else if (0x81 == g_APDU.pOutData[1])
		{
			Tag6F_Len = g_APDU.pOutData[2];
			pIn = g_APDU.pOutData+3;
			totalLen = g_APDU.pOutData[2];
			if(g_APDU.RLE-Tag6F_Len-3 != 0)
			{
					Nok_Reason = DATA_ERROR;
				  return NOK;
			}	
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		i=0;
		while((Tag6F_Len>0)&&((pIn[i]==0x00)||(pIn[i]==0xFF)))
		{
				i++;
				Tag6F_Len--;
		}
		//查找DF名 (M)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_0084_DF_NAME);	 
		if(NULL == ptr)
		{
				Nok_Reason = MISSING_DATA;
// 				*puchTvr |= 0x20;		//设置IC卡数据缺失
				return NOK;
		}
		else
		{
			if(Match_Criteria != 0)		//Exact Match
			{
				if((*(ptr+1) != AID_len)||(0 != memcmp(ptr+2,AID,AID_len)))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
			}
			else
			{
				if((0 != memcmp(ptr+2,AID,AID_len)) || (*(ptr+1)>16) || (*(ptr+1)<5))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
			}
			uchAid[0] = *(ptr+1);
			memcpy(uchAid+1, ptr+2, uchAid[0]); 	//保存DF名
			
			Tag6F_Len -= 2+uchAid[0];
			i=0;
			temp = *(ptr+1);
			while(((ptr+2+temp+i-pIn)<totalLen)&&(Tag6F_Len>0)&&((*(ptr+2+temp+i)==0x00)||(*(ptr+2+temp+i)==0xFF)))
			{
					i++;
					Tag6F_Len--;
			}
		}

		//查找FCI数据专用模板 (M)
		ptr =  SearchTag(pIn, totalLen,EMV_TAG_00A5_FCI_PROPRIETARY_TEMPLATE);	
		if(NULL == ptr)
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}

		if(*(ptr+1) <0x80)
		{
			nLen = TagA5_Len = *(ptr+1);
			pTag = ptr+2;
			Tag6F_Len -= 2;
			pA5 = ptr;
										
			i=0;
			temp = TagA5_Len;
			while((Tag6F_Len-TagA5_Len>0)&&((*(ptr+2+temp+i)==0x00)||(*(ptr+2+temp+i)==0xFF)))
			{
					i++;
					Tag6F_Len--;
			}
		}
		else if (0x81 == *(ptr+1))
		{
			nLen = TagA5_Len = *(ptr+2);
			pTag = ptr+3;
			Tag6F_Len -= 3;
			pA5 = ptr;
			
			i=0;
			temp = TagA5_Len;
			while((Tag6F_Len-TagA5_Len>0)&&((*(ptr+3+temp+i)==0x00)||(*(ptr+3+temp+i)==0xFF)))
			{
					i++;
					Tag6F_Len--;
			}
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		//查找应用优先指示符Tag (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_0087_API);	
		if((NULL != ptr)&&(*(ptr+1) == 1))   //Length =1
		{
				uchAPI[0] = 1;
				uchAPI[1] = *(ptr+2); 
		}
		else if(NULL == ptr)
		{
				uchAPI[0] = 1;
				uchAPI[1] = 0;
		}
		else
		{				
			  Nok_Reason = DATA_ERROR;
 			  return NOK;
		}

		if(NULL != ptr)
		{
			TagA5_Len -= 2+*(ptr+1);
			Tag6F_Len -= 2+*(ptr+1);	
		}

		//查找PDOL数据对象 (O)
		ptr = SearchTag(pTag, nLen, EMV_TAG_9F38_PDOL);	
		if(NULL == ptr) 
		{
			if(NULL != SearchTag(pIn, totalLen, EMV_TAG_9F38_PDOL))
			{
					Nok_Reason = DATA_ERROR;
					return NOK;
			}
			uchPDOL[1] = 0x00;    //设置PDOL数据对象长度
		}
		else if(0 == *(ptr+2))
		{
		  uchPDOL[1] = 0x00;    //设置PDOL数据对象长度	
			TagA5_Len -= 3;
			Tag6F_Len -= 3;	
		}
		else if(ptr < pA5)
		{
				Nok_Reason = DATA_ERROR;
				return NOK;
		}
		else
		{
			if(*(ptr+2) < 0x80)
			{				
				//终端填充PDOL数据
				uchPDOL[1] = CollectDol(ptr+3, *(ptr+2), &uchPDOL[2],PDOL_FLAG);
				if(uchPDOL[1] >= 0x80)
				{
						for(i=uchPDOL[1]+1;i>0;i--)
							uchPDOL[i+1] = uchPDOL[i];
					  uchPDOL[1] = 0x81;
				}
				TagA5_Len -= 3+*(ptr+2);
				Tag6F_Len -= 3+*(ptr+2);
			}
			else if (*(ptr+2) == 0x81)
			{
				//终端填充PDOL数据
				uchPDOL[2] = CollectDol(ptr+4, *(ptr+3), &uchPDOL[3],PDOL_FLAG);
				uchPDOL[1] = 0x81;
				TagA5_Len -= 4+*(ptr+3);
				Tag6F_Len -= 4+*(ptr+3);
			}
		}
		uchPDOL[0] = 0x83;	//设置PDOL数据对象Tag
 
		//查找语言选择Tag (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_5F2D_LANGUAGE_PREFERENCE);	
		if((NULL != ptr)&&(*(ptr+2)>1)&&(*(ptr+2)<9))   //Length = 2 ~ 8 Byte
		{
			memcpy(uchLangPref+1,ptr+3,*(ptr+2));
			uchLangPref[0] = *(ptr+2);			
		}
		else
		{
			memset(uchLangPref+1,0x00,8);
			uchLangPref[0] = 0;
		}

		if(NULL != ptr)
		{
			TagA5_Len -= 3+*(ptr+2);
			Tag6F_Len -= 3+*(ptr+2);
		}
 
		//查找发卡行代码表索引Tag (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_9F11_ISSUER_CODE_TABLE_INDEX);	
		if((NULL != ptr)&&(*(ptr+2) == 1))   //Length =1
		{
			uchIssuerCodeTableIndex[0] = 1;
			uchIssuerCodeTableIndex[1] = *(ptr+3);
		}
		else
		{
			uchIssuerCodeTableIndex[0] = 0;
			uchIssuerCodeTableIndex[1] = 0;
		}

		if(NULL != ptr)
		{
			TagA5_Len -= 3+*(ptr+2);
			Tag6F_Len -= 3+*(ptr+2);
		}

		//查找应用优先名称 (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_9F12_APPLICATION_PREFERRED_NAME);	
		if((NULL != ptr)&&(*(ptr+2)>0)&&(*(ptr+2)<17))   //Length =1~16
		{
				memcpy(uchAppPrefName+1,ptr+3,*(ptr+2));
				uchAppPrefName[0] = *(ptr+2);
		}
		else
		{
				memset(uchAppPrefName+1,0,16);
				uchAppPrefName[0] = 0;
		}

		if(NULL != ptr)
		{
			TagA5_Len -= 3+*(ptr+2);
			Tag6F_Len -= 3+*(ptr+2);
		}

		//查找发卡行自定义数据Tag (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_BF0C_FCI_ISSUER_DISCRETIONARY_DATA);	
		if(NULL != ptr)   //Length = 0~222 Byte
		{
				if(*(ptr+2) < 0x80)
				{
					uchFCI[0] = *(ptr+2);
				  memcpy(uchFCI+1,ptr+3,*(ptr+2)); 
					TagA5_Len -= 3+uchFCI[0] ;
					Tag6F_Len -= 3+uchFCI[0] ;
				}
				else if ((*(ptr+2) == 0x81)&&(*(ptr+3)<= 222))
				{
					uchFCI[0] = *(ptr+2);
					uchFCI[1] = *(ptr+3);
				  memcpy(uchFCI+2,ptr+4,*(ptr+3)); 	
					TagA5_Len -= 4+*(ptr+3) ;
					Tag6F_Len -= 4+*(ptr+3) ;
				}
				else
				{			
				}	
		}	
		else
		{
				uchFCI[0] = 0;
				memset(uchFCI+1,0x00,222);
		}
		
		//查找应用标签 (M)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_0050_APPLICATION_LABEL);	
		if(NULL == ptr)
		{
			uchAppLabel[0] = 0;
			if(APP_FINAL_SELECTION == internal_state)
			{
					for(i=0;i<Candidate_Number;i++)
					{
							if((0 == memcmp(Candidate_List[i].App_Name,AID,AID_len)) && (Candidate_List[i].App_Label_Len > 0))
									break;					
					}
							
					if(i == Candidate_Number)
					{
// 						*puchTvr |= 0x20;		//设置IC卡数据缺失
					}		
			}
		}
		else if((*(ptr+1)>0)&&(*(ptr+1)<17))   //Application Label 1~16字节
		{
			memcpy(uchAppLabel+1,ptr+2,*(ptr+1));
			uchAppLabel[0] = *(ptr+1);

			TagA5_Len -= 2+uchAppLabel[0] ;
			Tag6F_Len -= 2+uchAppLabel[0] ;
			
			i = 0;
			while((TagA5_Len>0) && ((*(ptr+uchAppLabel[0]+2+i) == 0x00) || (*(ptr+uchAppLabel[0]+2+i) == 0xFF)))
			{
					TagA5_Len--;
					Tag6F_Len--;
					i++;
			}
		}
		else
		{
				memcpy(uchAppLabel+1,ptr+2,*(ptr+1));
				uchAppLabel[0] = *(ptr+1);
			
				TagA5_Len -= 2+uchAppLabel[0] ; 
			  Tag6F_Len -= 2+uchAppLabel[0] ;
			
				i = 0;
				while((TagA5_Len>0) && ((*(ptr+uchAppLabel[0]+2+i) == 0x00) || (*(ptr+uchAppLabel[0]+2+i) == 0xFF)))
				{
						TagA5_Len--;
						Tag6F_Len--;
						i++;
				}
				*puchTvr |= 0x20;		//设置IC卡数据缺失
		}		

		if(TagA5_Len != 0) 
		{
				if(*(pA5+1) < 0x80)
				{
						while((*(pA5+*(pA5+1)+2-TagA5_Len) == 0xFF) || (*(pA5+*(pA5+1)+2-TagA5_Len) == 0x00)&& (TagA5_Len>0))
						{
								TagA5_Len--;
								Tag6F_Len--;
						}
				}
				else if(*(pA5+1) == 0x81)
				{
						while((*(pA5+*(pA5+2)+3-TagA5_Len) == 0xFF) || (*(pA5+*(pA5+2)+3-TagA5_Len) == 0x00) && (TagA5_Len>0))
						{
								TagA5_Len--;
								Tag6F_Len--;
						}
				}
				else
				{}
		}		
		
		if((Tag6F_Len != 0)||(TagA5_Len != 0)) 
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
	} 
		
	return SW_OPERATION_SUCCESS;
}

//选择卡片PSE或DDF
unsigned short	EMV_SelectDDF(unsigned char* DF_Name,unsigned char DF_nameLen)
{
	unsigned char *ptr = NULL;
	unsigned char *pIn = NULL;
//	unsigned char *pDFName = NULL;
	unsigned char *pA5 =NULL;
	unsigned char nLen,totalLen;
	unsigned int  Tag6F_Len = 0;
	unsigned int  TagA5_Len = 0;
	unsigned int  Tag84_Len = 0;
//	unsigned int  tempLen = 0;
	nLen = DF_nameLen;

	if((NULL == DF_Name) || (nLen > 16) || (nLen < 5))
	{
		Nok_Reason = NONE;
		return NOK;
	}
    g_APDU.CLA =0x00;//*(g_APDU.pCommBuff+0) = 0x00;
	g_APDU.INS =0xA4;//*(g_APDU.pCommBuff+1) = 0xA4;
	g_APDU.PP1 =0x04;//*(g_APDU.pCommBuff+2) = 0x04;
	g_APDU.PP2 =0x00;// *(g_APDU.pCommBuff+3) = 0x00;
	g_APDU.PP3 =nLen;// *(g_APDU.pCommBuff+4) = nLen;
	g_APDU.LC = nLen;
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;
	memcpy(g_APDU.pInData, DF_Name, nLen);

//	AU9541_ICCRAD_TransCeive(&g_APDU);
//	EMV_ExchangeAPDU(ICC_Protocol,5+nLen);
	 HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);

	if((0x6A == g_APDU.SW1) && (0x81 == g_APDU.SW2))	  //Card is blocked or SELECT command is not supported, terminate the session
	{
		Nok_Reason = CARD_BLOCKED;
		return NOK;
	} 
	else if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))  //PSE is blocked (0x6283) or PSE is not found(0x6A82)
	{
		Nok_Reason = NONE;
		return NOK;
	}
	else 				//9000
	{	
		 //查找FCI模板 (M)
		if(EMV_TAG_006F_FCI_TEMPLATE != g_APDU.pOutData[0])		  
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}

		if(g_APDU.pOutData[1] <0x80)
		{
			Tag6F_Len = g_APDU.pOutData[1];
			pIn = g_APDU.pOutData+2;
			totalLen = g_APDU.pOutData[1];
		}
		else if (0x81 == g_APDU.pOutData[1])
		{
			Tag6F_Len = g_APDU.pOutData[2];
			pIn = g_APDU.pOutData+3;
			totalLen = g_APDU.pOutData[2];
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		//查找DF名 (M) 
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_0084_DF_NAME);	
		if(NULL == ptr)
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}
		else
		{
			if((*(ptr+1) != nLen)||(0 != memcmp(ptr+2,DF_Name,nLen)))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			Tag84_Len = *(ptr+1)+2;
			Tag6F_Len -= 2+nLen;
//			pDFName = ptr;
		}

		//查找FCI数据专用模板 (M)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_00A5_FCI_PROPRIETARY_TEMPLATE);	
		if(NULL == ptr)
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}
		else if(*(ptr+1) < 0x80)
		{
			TagA5_Len = *(ptr+1);//	tempLen = TagA5_Len = *(ptr+1);			
			Tag6F_Len -= 2;
			pA5 = ptr;
		}
		else if(0x81 == *(ptr+1))
		{
			TagA5_Len = *(ptr+1);//tempLen = TagA5_Len = *(ptr+2);
			Tag6F_Len -= 3;
			pA5 = ptr;
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
		
		if(pA5 > pIn+Tag84_Len)
		{				
				ptr =  SearchTag(pIn, pA5-pIn, 0x42);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 2+ptr[1];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x54);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 2+ptr[1];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x56);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 2+ptr[1];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x5F53);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 3+ptr[2];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x5F54);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 3+ptr[2];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x5F55);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 3+ptr[2];
				}
				
				ptr =  SearchTag(pIn, pA5-pIn, 0x5F56);	
				if(NULL != ptr)
				{
					Tag6F_Len -= 3+ptr[2];
				}
		}			
		
		//查找目录基本文件的SFI (M)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_0088_SFI);	
		if(NULL == ptr)
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}
		else if((*(ptr+1) == 1)&&(*(ptr+2)>0)&&(*(ptr+2)<11))   //SFI =1~10
		{
			uchSFI[0] = 1;
			uchSFI[1] = *(ptr+2);
			Tag6F_Len -= 3;
			if(ptr>pA5)
			{		
				TagA5_Len -= 3; 
			}
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		//查找语言选择Tag (O)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_5F2D_LANGUAGE_PREFERENCE);	
		if((NULL != ptr)&&(*(ptr+2)>1)&&(*(ptr+2)<9))   //Length = 2 ~ 8 Byte
		{
			memcpy(uchLangPref+1,ptr+3,*(ptr+2));
			uchLangPref[0] = *(ptr+2);
			
			Tag6F_Len -= 3+*(ptr+2);
			if(ptr>pA5)
			{		
				TagA5_Len -= 3+*(ptr+2); 
			}
		}
		else
		{
				memset(uchLangPref+1,0x00,8);
			  uchLangPref[0] = 0;
		}
 
		//查找发卡行代码表索引Tag (O)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_9F11_ISSUER_CODE_TABLE_INDEX);	
		if((NULL != ptr)&&(*(ptr+2) == 1))   //Length =1
		{
			uchIssuerCodeTableIndex[0] = 1;
			uchIssuerCodeTableIndex[1] = *(ptr+3);

			Tag6F_Len -= 4;
			if(ptr>pA5)
			{		
				TagA5_Len -= 4; 
			}
		}
		else
		{
			uchIssuerCodeTableIndex[0] = 0;
			uchIssuerCodeTableIndex[1] = 0;
		}

		//查找发卡行自定义数据Tag (O)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_BF0C_FCI_ISSUER_DISCRETIONARY_DATA);	
		if(NULL != ptr)  
		{
				if(*(ptr+2) <0x80)
				{
						uchFCI[0] = *(ptr+2);
						memcpy(uchFCI+1,ptr+3,*(ptr+2)); 
						
						Tag6F_Len -= 3+*(ptr+2);
						if(ptr>pA5)
						{					
							TagA5_Len -= 3+*(ptr+2); 
						}
				}
				else if((*(ptr+2) == 0x81) && (*(ptr+3) < 222))  //Length = 0~222 Byte
				{
						uchFCI[0] = *(ptr+2);
					  uchFCI[1] = *(ptr+3);
						memcpy(uchFCI+2,ptr+4,uchFCI[1]); 
						
						Tag6F_Len -= 4+*(ptr+3);
						if(ptr>pA5)
						{		
							TagA5_Len -= 4+*(ptr+3); 	
						}
				}
				else
				{}					
		}	
		else
		{
				uchFCI[0] = 0;
				memset(uchFCI+1,0x00,222); 
		}	
		
		//查找应用标签 (M)
		ptr = SearchTag(pIn, totalLen, EMV_TAG_0050_APPLICATION_LABEL);	
		if(NULL != ptr)
		{
			Tag6F_Len -=  2+*(ptr+1);
			if(ptr>pA5)
			{
				TagA5_Len -=  2+*(ptr+1);
			}
		}

		if((Tag6F_Len != 0) || (TagA5_Len != 0))
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
	} 
	return SW_OPERATION_SUCCESS;
}

//Application Initialization
unsigned short EMV_AppInitialization(void)
{
	unsigned short ret = 0;
	const unsigned char   maxTSC[4] = {0x99,0x99,0x99,0x99};	// 

	EMV_InitAIDList();
	
	internal_state = WAIT_FOR_TRANSACTION;
	
	//Transaction Sequence Counter++
	{
		MID_NVMEM_Read(TRANS_SEQ_COUNTER,4,puchTSC);

		if (0 == memcmp(puchTSC,maxTSC,4))
		{
			memset(puchTSC,0x00,3);
			puchTSC[3] = 0x01;
		}
		else
		{
			puchTSC[3]++;
			if(puchTSC[3] > 0x99)
			{
					puchTSC[3] = 0;
					puchTSC[2]++;
			}	
			else if((puchTSC[3]&0x0F) >= 0x0A)
			{
				  puchTSC[3] += 0x10;				  				
				  puchTSC[3] &= 0xF0;
			}
			
			if(puchTSC[2] > 0x99)
			{
					puchTSC[2] = 0;
					puchTSC[1]++;
			}	
			else if((puchTSC[2]&0x0F) >= 0x0A)
			{
				  puchTSC[2] += 0x10;				  				
				  puchTSC[2] &= 0xF0;
			}
			
			if(puchTSC[1] > 0x99)
			{
					puchTSC[1] = 0;
					puchTSC[0]++;
			}	
			else if((puchTSC[1]&0x0F) >= 0x0A)
			{
				  puchTSC[1] += 0x10;				  				
				  puchTSC[1] &= 0xF0;
			}
       
			if(puchTSC[0] > 0x99)
			{
					puchTSC[0] = 0;
			}				
		}
		MID_NVMEM_Write(TRANS_SEQ_COUNTER,4,puchTSC);   
	}
	//Transaction Sequence Counter++

	ret = EMV_BuildCLbyPSE();	 //通过选择PSE构建Candidate List
	if(NOK == ret)
	{
		if(CARD_BLOCKED == Nok_Reason)
		{
			return NOK;
		}
		else 								//if(NONE == Nok_Reason)
		{
			ret = EMV_BuildCLbyAID();   	//使用AID列表构建Candidate List
			if(NOK == ret)
			{
				Nok_Reason = NONE;
				return NOK;
			}
			Nok_Reason = NONE;
		}
	}

	while(1) 
	{
		ret = EMV_FinalSelection();
		if((NOK == ret)&&((Nok_Reason == NOT_ACCEPTED)||(Nok_Reason == EMPTY_LIST)))
		{
			return NOK;
		}

		ret = EMV_GetProcessOn();
		if(NOK == ret)
		{
			if((GPO6985 == Nok_Reason))
			{
//				  MID_DISP_SmallPic(SMALLPIC_QINGCHONGSHI);
			    Delay_ms(1000);
				  continue;
			}
			else
			{
					Nok_Reason = GPO_ERROR;
					return NOK;
			}
		}
		else //9000
		{
			return SW_OPERATION_SUCCESS;
		}
	}
}

//建立候选应用列表 PSE方式
unsigned short  EMV_BuildCLbyPSE(void)
{
	unsigned char 	*ptr = NULL;
	unsigned char 	*pTag = NULL;
	unsigned char 	Tag_len = 0;
	unsigned char 	Rest_Taglen = 0;
	unsigned short 	ret = 0;
	unsigned char 	i;
	unsigned char 	name[16]={0};
	unsigned char 	name_len = 0;
	unsigned char 	label[16]={0};
	unsigned char 	label_len = 0;
	unsigned char   ddf_len;
	unsigned char 	prefer_name[16]={0};
	unsigned char 	prefer_name_len = 0;
	unsigned char 	api = 0;
	unsigned char 	ddf_name[16] = {0};
	unsigned char 	stack_index;
	unsigned int 		Tag61_Len = 0;

	EMV_ClearCandidateList();

	Candidate_Number =	0;							//Candidate List起始索引号
	stack_index = DDF_STACK_EMPTY;  		//设置DDF Stack为空

	//Select PSE
	memcpy(ddf_name, CONTACT_PSE_NAME,14);
	ddf_len = 14;
	
//	internal_state = SELECT_BY_PSE;

Select_DDF:
	ret = EMV_SelectDDF(ddf_name,ddf_len);						 
	if(SW_OPERATION_SUCCESS != ret) 
	{
		if(DDF_STACK_EMPTY == stack_index)      //DDF Stack为空
		{
			if(0 == Candidate_Number)          		//Candidate List为空
			{
				return NOK;
			}
			else
			{
				return SW_OPERATION_SUCCESS;
			}
		}
		else
		{
			//保存的前一个DDF信息出栈
			stack_index--;
			uchSFI[0] = 1;
			uchSFI[1] = ddf_stack[stack_index].Sfi;	
			i = ddf_stack[stack_index].Record_Number;
			Rest_Taglen = Tag_len = ddf_stack[stack_index].Rest_Len;
			pTag = ddf_stack[stack_index].Rest_Data;
			goto Search_Tag61;
		}
	}

	i = 1;												//起始记录号
	while(1)											//读记录
	{
	 	g_APDU.CLA = 0x00;//*(g_APDU.pCommBuff+0) = 0x00;
		g_APDU.INS = 0xb2;//*(g_APDU.pCommBuff+1) = 0xb2;
		g_APDU.PP1 = i;//*(g_APDU.pCommBuff+2) = i;
		g_APDU.PP2 = (uchSFI[1]<<3) | 0x04;//*(g_APDU.pCommBuff+3) = (uchSFI[1]<<3) | 0x04;
		g_APDU.PP3 = 0x00;//*(g_APDU.pCommBuff+4) = 0x00;
		g_APDU.LC = 0x00;
		g_APDU.LE = 0x00;
		g_APDU.RLE = 0x00;
//		AU9541_ICCRAD_TransCeive(&g_APDU);
//		EMV_ExchangeAPDU(ICC_Protocol,5);
		HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
		if((0x6A == g_APDU.SW1) && (0x83 == g_APDU.SW2))  //没找到记录
		{
			if(DDF_STACK_EMPTY == stack_index)
			{
				if (0 == Candidate_Number)			  //Candidate List为空
				{
					Nok_Reason = NONE;
					return NOK;
				}
				else
				{
					return SW_OPERATION_SUCCESS;
				}
			}
			else     //DDF Stack不为空
			{
				//保存的前一个DDF信息出栈
				stack_index--;
				uchSFI[1]	= ddf_stack[stack_index].Sfi;			
				i = ddf_stack[stack_index].Record_Number;
				Rest_Taglen = Tag_len = ddf_stack[stack_index].Rest_Len;
				pTag = ddf_stack[stack_index].Rest_Data;
				
				goto Search_Tag61;
			}
		}
		else if	((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))
		{
				EMV_ClearCandidateList();
				Nok_Reason = NONE;
				return NOK;
		}
		else if((0x90 == g_APDU.SW1) && (0x00 == g_APDU.SW2))
		{
			//查找标签AEF数据模板
			if (EMV_TAG_0070_AEF_DATA_TEMPLATE != g_APDU.pOutData[0])	
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if(*(g_APDU.pOutData+1) < 0x80)
			{
				Rest_Taglen = Tag_len = *(g_APDU.pOutData+1);
				pTag = g_APDU.pOutData+2;     //Bypass Tag70 and 70_Len
			}
			else if (*(g_APDU.pOutData+1) == 0x81)
			{
				Rest_Taglen = Tag_len = *(g_APDU.pOutData+2);
				pTag = g_APDU.pOutData+3;     //Bypass Tag70 and 70_Len
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}		

Search_Tag61:
			while(((*pTag == 0x00)||(*pTag == 0xFF))&&(Rest_Taglen>0))
			{
					pTag++;
					Rest_Taglen--;
			}
			
			if (0 == Rest_Taglen)
			{
				i++;
				continue;
			}

			//查找应用模板61
			ptr =  SearchTag(pTag, Tag_len, EMV_TAG_0061_APPLICATION_TEMPLATE);	
			if(NULL == ptr) 
			{
				if (Rest_Taglen != 0)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				i++;	
				continue;
			}
			else if((ptr+*(ptr+1)-pTag) >Rest_Taglen)
			{
				if (Rest_Taglen != 0)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				i++;	
				continue;
			}

			pTag = ptr;	

			if(*(ptr+1)<0x80)	
			{				
				Tag_len = *(ptr+1)+2;
				Rest_Taglen -= Tag_len;
				Tag61_Len = *(ptr+1);
			}
			else if(*(ptr+1) == 0x81)
			{				
				Tag_len = *(ptr+2)+3;
				Rest_Taglen -= Tag_len;
				Tag61_Len = *(ptr+2);
			}
				
				   			
			//查找ADF名称 (M)
			ptr =  SearchTag(pTag, Tag_len, EMV_TAG_004F_AID);	
			if(NULL == ptr)
			{
				//查找DDF名称
        ptr =  SearchTag(pTag,Tag_len, EMV_TAG_009D_DDF_NAME);	 
				if(NULL == ptr)
				{
					Nok_Reason = MISSING_DATA;	
					*puchTvr |= 0x20;		//设置IC卡数据缺失
					return NOK;
				}
				else if((NULL != ptr)&&(*(ptr+1)>4)&&(*(ptr+1)<17))  //DDF名称5~16 Bytes
				{
					ddf_len = *(ptr+1);
					memcpy(ddf_name,ptr+2,ddf_len);

					Tag61_Len -=  2+ddf_len;
					if(Tag61_Len != 0)
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}

					//当前记录位置及信息入栈
					stack_index++;
					ddf_stack[stack_index-1].Sfi = uchSFI[1] ;			  //保存当前的SFI 
					ddf_stack[stack_index-1].Record_Number = i;			  //保存当前处理的记录号
					ddf_stack[stack_index-1].Rest_Len = Rest_Taglen;      //保存剩余的数据长度
					memcpy(ddf_stack[stack_index-1].Rest_Data,pTag+Tag_len,Rest_Taglen); //保存剩余的数据
					goto Select_DDF;
				}
				
				pTag += Tag_len;
				Tag_len = Rest_Taglen;
				goto Search_Tag61;
			}
			else
			{
				if((*(ptr+1) <17)&&(*(ptr+1)>4))			//AID 5~16 Bytes
				{
					memcpy(name,ptr+2,*(ptr+1));
					name_len = *(ptr+1);

					Tag61_Len -=  name_len+2;
				}
				else
				{
					Nok_Reason = DATA_ERROR;
					pTag += Tag_len;
					Tag_len = Rest_Taglen;
					goto Search_Tag61;
				}
	
				//查找应用标签 (M)
				ptr = SearchTag(pTag, Tag_len, EMV_TAG_0050_APPLICATION_LABEL);	
				if(NULL == ptr)
				{
					label_len = 0;
// 					Nok_Reason = MISSING_DATA;	
					*puchTvr |= 0x20;		//设置IC卡数据缺失
					return NOK;
				}
				else
				{
					if((*(ptr+1) <17)&&(*(ptr+1)>0))		 //应用标签1~16Bytes
					{
						memcpy(label,ptr+2,*(ptr+1));
						label_len = *(ptr+1);

						Tag61_Len -=  2+label_len;
					}
					else
					{
						Nok_Reason = DATA_ERROR;	
						pTag += Tag_len;
						Tag_len = Rest_Taglen;
						goto Search_Tag61;
					}
				}
	
				//查找应用优先名称 (O)
				ptr = SearchTag(pTag, Tag_len, EMV_TAG_9F12_APPLICATION_PREFERRED_NAME);	
				if((NULL == ptr)&&((*(ptr+1) <253)&&(*(ptr+1)>0)))	     //Length = 1~252 Bytes
				{
					memset(prefer_name,0x00,16);			//无应用优先名称
					prefer_name_len = 0;
				}
				else
				{
//					if((*(ptr+2) <17)&&(*(ptr+2)>0))		 //应用优先名称1~16Bytes
					if(*(ptr+2) <17)		 //应用优先名称1~16Bytes
					{
						memcpy(prefer_name,ptr+3,*(ptr+2));
						prefer_name_len = *(ptr+2);

						Tag61_Len -= prefer_name_len+3;
					}
				}
				
				//查找应用优先权标识符 O）
				ptr = SearchTag(pTag, Tag_len, EMV_TAG_0087_API);	
				if(NULL == ptr)
				{
					api = 0;				//无应用优先权标识符
				}
				else
				{
					if(*(ptr+1) == 1)		 //应用优先权标识符 1Byte
					{
						api = *(ptr+2);	   
						
						Tag61_Len -= 3;
					}
				}
			
				//查找目录自定义模板 O）
				ptr = SearchTag(pTag, Tag_len, EMV_TAG_0073_DIRECTORY_DISCRETIONARY_TEMPLATE);	
				if(NULL == ptr)
				{
					memset(uchTag73,0x00,253);
					uchTag73[0] = 0;				
				}
				else
				{
					if(*(ptr+1) < 0x80)
					{					
						uchTag73[0] = *(ptr+1);
						memcpy(uchTag73+1,ptr+2,uchTag73[0]);
						Tag61_Len -= 2+*(ptr+1);	
					}
					else if (*(ptr+1) == 0x81)
					{
						uchTag73[0] = *(ptr+2);
						memcpy(uchTag73+1,ptr+3,uchTag73[0]);
						Tag61_Len -= 3+*(ptr+2);	
					}
					else
					{}
				}
				
				if(((Rest_Taglen!=0)&&(Rest_Taglen<3))||(Tag61_Len>0))
				{
						Nok_Reason = DATA_ERROR;
						return NOK;
				}
	
				ret = EMV_SearchAID(name,name_len);
				if (NOK != ret)
				{
					Candidate_List[Candidate_Number].ADF_Valid = 1;		//0 - Invalid, 1 - Valid 
					memcpy(Candidate_List[Candidate_Number].App_Name,name,name_len); 
					memcpy(Candidate_List[Candidate_Number].App_Label,label,label_len); 
					memcpy(Candidate_List[Candidate_Number].App_Preferred_Name,prefer_name,prefer_name_len); 
					Candidate_List[Candidate_Number].App_Name_Len = name_len;
					Candidate_List[Candidate_Number].App_Label_Len = label_len;
					Candidate_List[Candidate_Number].App_Preferred_Name_Len = prefer_name_len;
					if(uchIssuerCodeTableIndex[0] == 0x01)
					{
						Candidate_List[Candidate_Number].IssuerCodeTableIndex = uchIssuerCodeTableIndex[1];
					}
					else
					{
						Candidate_List[Candidate_Number].IssuerCodeTableIndex = 0;	
					}
					Candidate_List[Candidate_Number].API = api;	
					Candidate_List[Candidate_Number].ASI = AID_List[ret].ASI;
					Candidate_Number++;
				}

				pTag += Tag_len;
				Tag_len = Rest_Taglen;
				goto Search_Tag61;
			}
		}
	}

}

//建立候选应用列表 List of AID方式
unsigned short  EMV_BuildCLbyAID(void)
{
	unsigned char  select_param;
	unsigned short ret = 0;
	unsigned char  i;


 	EMV_ClearCandidateList();

	Candidate_Number = 0;
	select_param = SELECT_FIRST; 

  internal_state = SELECT_BY_AID;	
	
	for(i=0;i<MAX_AID_NUMBER;)
  {
		if(0 == AID_List[i].AID_len)
		{
			i++;
			select_param = SELECT_FIRST; 
			continue;
		}

		uchAid[0] = 0;
		uchAppLabel[0] = 0;
		uchAppPrefName[0] = 0;
		ret = EMV_SelectADF( AID_List[i].AID,AID_List[i].AID_len,select_param,AID_List[i].ASI);
		if(NOK == ret)
		{
			if((CARD_BLOCK == Nok_Reason)&&(select_param == SELECT_FIRST))
			{
				return NOK;
			}
			else if (APPLICATION_BLOCKED == Nok_Reason)
			{
				select_param = SELECT_NEXT; 
				continue;
			}
			else
			{
				i++;
				select_param = SELECT_FIRST; 
				continue;
			}
		}
		else		//SW_OPERATION_SUCCESS or Application is Blocked
		{
			if(uchAid[0] >= AID_List[i].AID_len)
			{
				if (0 == memcmp(&uchAid[1],AID_List[i].AID,AID_List[i].AID_len))
				{
					if(AID_List[i].ASI == 0)  //Partial Match OK
					{
						if((NOK == ret) && (APPLICATION_BLOCKED == Nok_Reason))
						{
							i++;
							select_param = SELECT_FIRST;
							continue;
						}
						else
						{
							Candidate_List[Candidate_Number].ADF_Valid = 1;		//0 - Invalid, 1 - Valid 
							memcpy(Candidate_List[Candidate_Number].App_Name,&uchAid[1],uchAid[0]); 
							memcpy(Candidate_List[Candidate_Number].App_Label,uchAppLabel+1,uchAppLabel[0]); 
							memcpy(Candidate_List[Candidate_Number].App_Preferred_Name,uchAppPrefName+1,uchAppPrefName[0]); 
							Candidate_List[Candidate_Number].App_Name_Len = uchAid[0];
							Candidate_List[Candidate_Number].App_Label_Len = uchAppLabel[0];
							Candidate_List[Candidate_Number].App_Preferred_Name_Len = uchAppPrefName[0];
							if(uchIssuerCodeTableIndex[0] == 0x01)
							{
								Candidate_List[Candidate_Number].IssuerCodeTableIndex = uchIssuerCodeTableIndex[1];
							}
							else
							{
							  Candidate_List[Candidate_Number].IssuerCodeTableIndex = 0;	
							}
							Candidate_List[Candidate_Number].API = uchAPI[1];	
							Candidate_List[Candidate_Number].ASI = AID_List[i].ASI;
							Candidate_Number++;
							if(uchAid[0] > AID_List[i].AID_len)
							{
								select_param = SELECT_NEXT;
							}
							else
							{
								i++;
								select_param = SELECT_FIRST;
							}
							continue;
						}
					}
					else if(uchAid[0] == AID_List[i].AID_len)	//Exact Match
					{
						Candidate_List[Candidate_Number].ADF_Valid = 1;		//0 - Invalid, 1 - Valid 
						memcpy(Candidate_List[Candidate_Number].App_Name,&uchAid[1],uchAid[0]); 
						memcpy(Candidate_List[Candidate_Number].App_Label,&uchAppLabel[1],uchAppLabel[0]); 
						memcpy(Candidate_List[Candidate_Number].App_Preferred_Name,&uchAppPrefName[1],uchAppPrefName[0]); 
						Candidate_List[Candidate_Number].App_Name_Len = uchAid[0];
						Candidate_List[Candidate_Number].App_Label_Len = uchAppLabel[0];
						Candidate_List[Candidate_Number].App_Preferred_Name_Len = uchAppPrefName[0];
						if(uchIssuerCodeTableIndex[0] == 0x01)
						{
							Candidate_List[Candidate_Number].IssuerCodeTableIndex = uchIssuerCodeTableIndex[1];
						}
						else
						{
							Candidate_List[Candidate_Number].IssuerCodeTableIndex = 0;	
						}
						Candidate_List[Candidate_Number].API = uchAPI[1];	
						Candidate_List[Candidate_Number].ASI = AID_List[i].ASI;
						Candidate_Number++;
						i++;
						select_param = SELECT_FIRST; 
						continue;
					}
					else
					{
						i++;
						select_param = SELECT_FIRST; 
						continue;
					}
				}
				else
				{	
					i++;
					select_param = SELECT_FIRST; 
					continue;
				}
			}
			else
			{
				i++;
				select_param = SELECT_FIRST; 
				continue;
			}
		}
	}	

	if (0 == Candidate_Number)
	{
		Nok_Reason = NONE;
		return NOK;
	}
	return SW_OPERATION_SUCCESS;
}

//最终选择
unsigned short	EMV_FinalSelection(void)
{
	unsigned short ret = 0;

	while(1)
	{
		if (GPO6985 == Nok_Reason)
		{
			Nok_Reason = NONE;
			if((Current_DF_Len != 0) && (SW_OPERATION_SUCCESS != EMV_RemoveACandidate(Current_DF_Name,Current_DF_Len)))
			{
				 return NOK;
			}
		}

		if (DATA_ERROR == Nok_Reason)
		{
			Nok_Reason = NOT_ACCEPTED;
			return NOK;
		}

		if(0 == Candidate_Number)
		{
			Nok_Reason = EMPTY_LIST;
			return NOK;
		}

		EMV_SortCandidateList();

		if(1 == Candidate_Number)
		{
#if 0
//			if(Candidate_List[0].API & 0x80)
//			{
//				ret = EMV_Show_Candidate_List(Candidate_Number);	   //返回用户选择的应用的序号
//				if(1 != ret)
//				{
//					Nok_Reason = NOT_ACCEPTED;
//					return NOK;
//				}
//
//			}	
#endif
			Current_DF_Len = Candidate_List[0].App_Name_Len;
			memcpy(Current_DF_Name,Candidate_List[0].App_Name,Current_DF_Len);
			goto Final_Selection;
		}
		else
		{
			  //当Candidate多于1个时，必须显示Candidate List，让用户选择应用
				ret = EMV_Show_Candidate_List(Candidate_Number);	   //返回用户选择的应用的序号
				if(NOK == ret)
				{
					Nok_Reason = NOT_ACCEPTED;
					return NOK;
				}
				Current_DF_Len = Candidate_List[ret-1].App_Name_Len;
				memcpy(Current_DF_Name,Candidate_List[ret-1].App_Name,Current_DF_Len);
		}

Final_Selection:	
	  internal_state = APP_FINAL_SELECTION;
		ret = EMV_SelectADF(Current_DF_Name,Current_DF_Len,SELECT_FIRST,0x01);	//Exact Match
		if(SW_OPERATION_SUCCESS != ret)
		{
			if(SW_OPERATION_SUCCESS != EMV_RemoveACandidate(Current_DF_Name,Current_DF_Len))
			{
				 Nok_Reason = NONE;
				 return NOK;
			}
			
			Nok_Reason = NONE;
//			MID_DISP_SmallPic(SMALLPIC_QINGCHONGSHI);
			Delay_ms(1000);
			continue;
		}
		else
		{
			return SW_OPERATION_SUCCESS;
		}
	}
}

//Search Matched AID
unsigned short EMV_SearchAID(unsigned char *AID, unsigned char len)
{
	unsigned short i;

	if((len < 5) || (len > 16) || (NULL == AID))
		return NOK;

	for (i=0; i< MAX_AID_NUMBER; i++)
	{
		if(AID_List[i].ASI == 1)	   //Exact Match
		{
			if((len == AID_List[i].AID_len)	&&(AID_List[i].AID_len != 0))
			{
				if (0 == memcmp(AID,AID_List[i].AID,len))
					return i;
			}
			else
			{
				continue;
			}
		}
		else if(AID_List[i].ASI == 0)  //Partial Match OK
		{
			if((len >= AID_List[i].AID_len)	&&(AID_List[i].AID_len != 0))
			{
				if (0 == memcmp(AID,AID_List[i].AID,AID_List[i].AID_len))
					return i;
			}
			else
			{
				continue;
			}
		}
	}
	return NOK;
}

//Clear Candidate List
void EMV_ClearCandidateList(void)
{
	unsigned char i;

	for(i=0;i<MAX_CANDIDATE_NUMBER;i++)
	{
		Candidate_List[i].ADF_Valid = 0;		//0 - Invalid, 1 - Valid 
		memset(Candidate_List[i].App_Name,0x00,16); 
		memset(Candidate_List[i].App_Label,0x00,16); 
		memset(Candidate_List[i].App_Preferred_Name,0x00,16); 
		Candidate_List[i].App_Name_Len = 0;
		Candidate_List[i].App_Label_Len = 0;
		Candidate_List[i].App_Preferred_Name_Len = 0;
	  Candidate_List[i].IssuerCodeTableIndex = 0;
		Candidate_List[i].API = 0;	
		Candidate_List[i].ASI = 0;
	}
	Candidate_Number = 0;
	Current_DF_Len = 0;
	memset(Current_DF_Name,0xFF,16);
	return;
}
#if 0
//Initialize AID List;
void EMV_InitAIDList(void)
{
	unsigned char i;
	
	Nok_Reason = NONE;
	memset(Current_DF_Name,0x00,16);
	Current_DF_Len = 0;
	Candidate_Number = 0;
	
	memcpy(AID_List[0].AID,AID0,7);
	AID_List[0].AID_len	= 7;
	AID_List[0].ASI = 0; 

	memcpy(AID_List[1].AID,AID1,7);
	AID_List[1].AID_len	= 7;
	AID_List[1].ASI = 0; 

	memcpy(AID_List[2].AID,AID2,8);
	AID_List[2].AID_len	= 8;
	AID_List[2].ASI = 0; 

	memcpy(AID_List[3].AID,AID3,8);
	AID_List[3].AID_len	= 8;
	AID_List[3].ASI = 0; 

	memcpy(AID_List[4].AID,AID4,8);
	AID_List[4].AID_len	= 8;
	AID_List[4].ASI = 0; 

	memcpy(AID_List[5].AID,AID5,8);
	AID_List[5].AID_len	= 8;
	AID_List[5].ASI = 0; 

	memcpy(AID_List[6].AID,AID6,8);
	AID_List[6].AID_len	= 8;
	AID_List[6].ASI = 1; 

	memcpy(AID_List[7].AID,AID7,7);
	AID_List[7].AID_len	= 7;
	AID_List[7].ASI = 0; 

	memcpy(AID_List[8].AID,AID8,5);
	AID_List[8].AID_len	= 5;
	AID_List[8].ASI = 0; 

	memcpy(AID_List[9].AID,AID9,7);
	AID_List[9].AID_len	= 7;
	AID_List[9].ASI = 0; 
	
	memcpy(AID_List[10].AID,AID10,7);
	AID_List[10].AID_len	= 7;
	AID_List[10].ASI = 0; 
	
	memcpy(AID_List[11].AID,AID11,5);
	AID_List[11].AID_len	= 5;
	AID_List[11].ASI = 0; 
	
	memcpy(AID_List[12].AID,AID12,8);
	AID_List[12].AID_len	= 8;
	AID_List[12].ASI = 0; 
	
	memcpy(AID_List[13].AID,AID13,16);
	AID_List[13].AID_len	= 16;
	AID_List[13].ASI = 0; 
	
	memcpy(AID_List[14].AID,AID14,6);
	AID_List[14].AID_len	= 6;
	AID_List[14].ASI = 0; 
	
	for(i=15;i<MAX_AID_NUMBER;i++)
	{
		memset(AID_List[i].AID,0x00,16);
		AID_List[i].AID_len	= 0;
		AID_List[i].ASI = 0; 
	}

	return;
}
#else
//Initialize AID List;
void EMV_InitAIDList(void)
{
	unsigned char ucLoop;
	unsigned char * pucBuf;
	unsigned char * pucTemp;
	unsigned char ucCnt;

	Nok_Reason = NONE;
	memset(Current_DF_Name,0x00,16);
	Current_DF_Len = 0;
	Candidate_Number = 0;

	pucBuf = (unsigned char *)cucAIDBuf;
	for(ucLoop = 0;ucLoop < 15; ucLoop++)
	{
		pucBuf += ucLoop * 128;
		pucTemp = SearchTagforScript(pucBuf, 128, 0xdf01);
		AID_List[ucLoop].ASI = *(pucTemp + 3);
		pucTemp = SearchTagforScript(pucBuf, 128, 0x9f06);
		AID_List[ucLoop].AID_len = *(pucTemp + 2);
		if(AID_List[ucLoop].AID_len > 16)
		{
			AID_List[ucLoop].AID_len = 16;	
		} 
		else{}
		memcpy(AID_List[ucLoop].AID, pucTemp + 3, AID_List[ucLoop].AID_len);
	}

	IROM_Read(FLASH_ADDR_PBOC_AID_CNT, &ucCnt, 1);
	if(ucCnt > 20)
	{
		ucCnt = 20;

	}
	else{}

	pucBuf = (unsigned char *)FLASH_ADDR_PBOC_AID;
	for(ucLoop = 0;ucLoop < ucCnt; ucLoop++)
	{
		pucBuf += ucLoop * 128;
		pucTemp = SearchTagforScript(pucBuf, 128, 0xdf01);
		AID_List[ucLoop + 15].ASI = *(pucTemp + 3);
		pucTemp = SearchTagforScript(pucBuf, 128, 0x9f06);
		AID_List[ucLoop + 15].AID_len = *(pucTemp + 2);
		if(AID_List[ucLoop + 15].AID_len > 16)
		{
			AID_List[ucLoop + 15].AID_len = 16;	
		} 
		else{}
		memcpy(AID_List[ucLoop + 15].AID, pucTemp + 3, AID_List[ucLoop + 15].AID_len);
	}
}
#endif
//Initialize DDF Stack;
void  EMV_InitDDFStack(void)
{
	unsigned char i;

	for(i=0;i<DDF_STACK_DEPTH;i++)
	{
		ddf_stack[i].Sfi = 0;
		ddf_stack[i].Record_Number = 0;
		ddf_stack[i].Rest_Len = 0;
		memset(ddf_stack[i].Rest_Data, 0x00,255);

	}
	return;
}

//Remove a Candidate from the Candidate List
unsigned short EMV_RemoveACandidate(unsigned char *AID, unsigned char len)
{
	unsigned char i,j;

	if((len < 5) || (len > 16) || (0 == Candidate_Number))
		return NOK;

	for(i=0;i<Candidate_Number;i++)
	{
		if((1 == Candidate_List[i].ADF_Valid)&&(len == Candidate_List[i].App_Name_Len))
		{
			if (0 == memcmp(AID,Candidate_List[i].App_Name,len))
			{
				if (i == Candidate_Number-1)
				{
					Candidate_Number--;	
					Current_DF_Len = 0;
					memset(Current_DF_Name,0x00,16);

					Candidate_List[i].ADF_Valid = 0;		//0 - Invalid, 1 - Valid 
					memset(Candidate_List[i].App_Name,0x00,16); 
					memset(Candidate_List[i].App_Label,0x00,16); 
					memset(Candidate_List[i].App_Preferred_Name,0x00,16); 
					Candidate_List[i].App_Name_Len = 0;
					Candidate_List[i].App_Label_Len = 0;
					Candidate_List[i].App_Preferred_Name_Len = 0;
					Candidate_List[i].API = 0;	
					Candidate_List[i].ASI = 0;	
					return SW_OPERATION_SUCCESS;
				}

				for(j=i;j<Candidate_Number;j++)
				{
				 	Candidate_List[j].ADF_Valid = Candidate_List[j+1].ADF_Valid;
					memcpy(Candidate_List[j].App_Name,Candidate_List[j+1].App_Name,Candidate_List[j+1].App_Name_Len); 
					memcpy(Candidate_List[j].App_Label,Candidate_List[j+1].App_Label,Candidate_List[j+1].App_Label_Len); 
					memcpy(Candidate_List[j].App_Preferred_Name,Candidate_List[j+1].App_Preferred_Name,Candidate_List[j+1].App_Preferred_Name_Len); 
					Candidate_List[j].App_Name_Len = Candidate_List[j+1].App_Name_Len;
					Candidate_List[j].App_Label_Len = Candidate_List[j+1].App_Label_Len;
					Candidate_List[j].App_Preferred_Name_Len = Candidate_List[j+1].App_Preferred_Name_Len;
					Candidate_List[j].API = Candidate_List[j+1].API;
					Candidate_List[j].ASI = Candidate_List[j+1].ASI;
				}

				Candidate_List[Candidate_Number-1].ADF_Valid = 0;		//0 - Invalid, 1 - Valid 
				memset(Candidate_List[Candidate_Number-1].App_Name,0x00,16); 
				memset(Candidate_List[Candidate_Number-1].App_Label,0x00,16); 
				memset(Candidate_List[Candidate_Number-1].App_Preferred_Name,0x00,16); 
				Candidate_List[Candidate_Number-1].App_Name_Len = 0;
				Candidate_List[Candidate_Number-1].App_Label_Len = 0;
				Candidate_List[Candidate_Number-1].App_Preferred_Name_Len = 0;
				Candidate_List[Candidate_Number-1].API = 0;	
				Candidate_List[Candidate_Number-1].ASI = 0;	
				
				Candidate_Number--;	
				Current_DF_Len = 0;
				memset(Current_DF_Name,0x00,16);
				return SW_OPERATION_SUCCESS;	
			}
		}
		else
		{
			continue;
		}
	}
	return NOK;
}

//Sort Candidate List
void EMV_SortCandidateList(void)
{
	unsigned char i,j;
	unsigned char index;
	CANDIDATE_TYPE Candidate;

	index = Candidate_Number;
	for(j=0;j<index;j++)
	{
		for(i=0;i<index-j;i++)
		{
			if(i == index-j-1)
				break;

			if((Candidate_List[i].API != 0) && ((Candidate_List[i].API & 0x0F) > (Candidate_List[i+1].API & 0x0F)))	  //优先级比较小的元素沉底
			{
			 	Candidate.ADF_Valid = Candidate_List[i].ADF_Valid;
				memcpy(Candidate.App_Name,Candidate_List[i].App_Name,Candidate_List[i].App_Name_Len); 
				memcpy(Candidate.App_Label,Candidate_List[i].App_Label,Candidate_List[i].App_Label_Len); 
				memcpy(Candidate.App_Preferred_Name,Candidate_List[i].App_Preferred_Name,Candidate_List[i].App_Preferred_Name_Len); 
				Candidate.App_Name_Len = Candidate_List[i].App_Name_Len;
				Candidate.App_Label_Len = Candidate_List[i].App_Label_Len;
				Candidate.App_Preferred_Name_Len = Candidate_List[i].App_Preferred_Name_Len;
				Candidate.API = Candidate_List[i].API;
				Candidate.ASI = Candidate_List[i].ASI;
				
				Candidate_List[i].ADF_Valid = Candidate_List[i+1].ADF_Valid;
				memcpy(Candidate_List[i].App_Name,Candidate_List[i+1].App_Name,Candidate_List[i+1].App_Name_Len); 
				memcpy(Candidate_List[i].App_Label,Candidate_List[i+1].App_Label,Candidate_List[i+1].App_Label_Len); 
				memcpy(Candidate_List[i].App_Preferred_Name,Candidate_List[i+1].App_Preferred_Name,Candidate_List[i+1].App_Preferred_Name_Len); 
				Candidate_List[i].App_Name_Len = Candidate_List[i+1].App_Name_Len;
				Candidate_List[i].App_Label_Len = Candidate_List[i+1].App_Label_Len;
				Candidate_List[i].App_Preferred_Name_Len = Candidate_List[i+1].App_Preferred_Name_Len;
				Candidate_List[i].API = Candidate_List[i+1].API;
				Candidate_List[i].ASI = Candidate_List[i+1].ASI;
	
				Candidate_List[i+1].ADF_Valid = Candidate.ADF_Valid;
				memcpy(Candidate_List[i+1].App_Name,Candidate.App_Name,Candidate.App_Name_Len); 
				memcpy(Candidate_List[i+1].App_Label,Candidate.App_Label,Candidate.App_Label_Len); 
				memcpy(Candidate_List[i+1].App_Preferred_Name,Candidate.App_Preferred_Name,Candidate.App_Preferred_Name_Len); 
				Candidate_List[i+1].App_Name_Len = Candidate.App_Name_Len;
				Candidate_List[i+1].App_Label_Len = Candidate.App_Label_Len;
				Candidate_List[i+1].App_Preferred_Name_Len = Candidate.App_Preferred_Name_Len;
				Candidate_List[i+1].API = Candidate.API;
				Candidate_List[i+1].ASI = Candidate.ASI;
			}
		}
	}	
	
	return;
}

//Get Process On
unsigned short EMV_GetProcessOn(void)
{
	unsigned char *ptr = NULL;
	unsigned char nLen = 0;
	unsigned char nPos = 0;
	unsigned char *pTag77 = NULL;
	unsigned char Tag77_Len = 0;
	unsigned char tempLen = 0;
	unsigned char i,j;
	
#if 1
//	MID_ALG_GenRand(puchUnpreData,4);	//不可预知数，应该用随机数
	GetChallenge(puchUnpreData,4);
#else
	puchUnpreData[0] = 0x11;
	puchUnpreData[1] = 0x22;
	puchUnpreData[2] = 0x33;
	puchUnpreData[3] = 0x44;
#endif	

	internal_state = GPO_START;
	
	//Get Process On		
	g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
	g_APDU.INS = 0xa8;//*(g_APDU.pCommBuff+1) = 0xa8;
	g_APDU.PP1 = 0x00;//*(g_APDU.pCommBuff+2) = 0x00;
	g_APDU.PP2 = 0x00;//*(g_APDU.pCommBuff+3) = 0x00;

	if(uchPDOL[1] == 0x81)
	{
		g_APDU.PP3 =uchPDOL[2]+3;//*(g_APDU.pCommBuff+4) = uchPDOL[2]+3;
		g_APDU.LC = uchPDOL[2]+3;
		
//		memcpy(g_APDU.pCommBuff+5,uchPDOL,uchPDOL[2]+3); 
		memcpy(g_APDU.pInData, uchPDOL,uchPDOL[2]+3);
	}
	else
	{
		g_APDU.PP3 =uchPDOL[1]+2;//*(g_APDU.pCommBuff+4) = uchPDOL[1]+2;
		g_APDU.LC = uchPDOL[1]+2;
		
//		memcpy(g_APDU.pCommBuff+5,uchPDOL,uchPDOL[1]+2);  
		memcpy(g_APDU.pInData, uchPDOL,uchPDOL[1]+2);
	}  
	
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;
//	AU9541_ICCRAD_TransCeive(&g_APDU);
//	EMV_ExchangeAPDU(ICC_Protocol,g_APDU.LC+5);	
	HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
	
	if((0x69 == g_APDU.SW1) && (0x85 == g_APDU.SW2))
	{
			Nok_Reason = GPO6985;
			return NOK;
	}
	else if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))	 
	{
			Nok_Reason = NOT_ACCEPTED;
			return NOK;
	}
	else	//9000
	{	
		if(g_APDU.pOutData[1] < 0x80)
		{
			nLen = g_APDU.pOutData[1];
			nPos = 2;
		}
		else if(g_APDU.pOutData[1] == 0x81)
		{
			nLen = g_APDU.pOutData[2];
			nPos = 3;
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
			    
		if(*g_APDU.pOutData == EMV_TAG_0080_RESPONSE_MESSAGE_TEMPLATE_FORMAT_1)			//格式1
		{
			if((nLen<6) || ((nLen>6) && (nLen-6)%4))		//AFL长度不是4的倍数
			{
				Nok_Reason = DATA_ERROR;
				return NOK;	  						
			}
			else
			{
				uchAip[0] = 2;
				memcpy(uchAip+1,g_APDU.pOutData+nPos,2);
				if(0 == g_APDU.pOutData[nPos+2])
				{
					Nok_Reason = DATA_ERROR;
					return NOK;	
				}
				uchAFL[0] = nLen-2;	
				memcpy(uchAFL+1,g_APDU.pOutData+nPos+2,uchAFL[0]);
			}
		}
		else if (*g_APDU.pOutData == EMV_TAG_0077_RESPONSE_MESSAGE_TEMPLATE_FORMAT_2)			//格式2
		{
			if(g_APDU.pOutData[1]<0x80)
			{
				tempLen = Tag77_Len = g_APDU.pOutData[1];
				pTag77 = g_APDU.pOutData+2;
			}
			else if(g_APDU.pOutData[1] == 0x81)
			{
				tempLen = Tag77_Len = g_APDU.pOutData[2];
				pTag77 = g_APDU.pOutData+3;
			}
			else
			{}
				
			while((Tag77_Len>0)&&((*pTag77 == 0x00)||(*pTag77 == 0xFF)))
			{
					pTag77++;
				  Tag77_Len--;
			}
			
			ptr = SearchTag(pTag77, tempLen, EMV_TAG_0082_AIP);	//查找AIP数据对象
			if(NULL == ptr)
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if(ptr[1]<0x80)
			{
				nPos = 2;
				Tag77_Len -= 2+ptr[1];
				if(ptr[1] != 2)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}				
				j = ptr[1];
				i = 0;
				while((Tag77_Len>0) && ((ptr[2+j+i]==0x00)||(ptr[2+j+i]==0xFF)))
				{
						i++;
						Tag77_Len--;
				}
			}
			else if(ptr[1] == 0x81)
			{
				nPos = 3;
				Tag77_Len -= 3+ptr[2];
				if(ptr[2] != 2)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				j = ptr[2];
				i = 0;
				while((Tag77_Len>0) && ((ptr[3+j+i]==0x00)||(ptr[3+j+i]==0xFF)))
				{
						i++;
						Tag77_Len--;
				}
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			uchAip[0] = 2;
			memcpy(uchAip+1, ptr+nPos, 2);			

			ptr = SearchTag(pTag77, tempLen, EMV_TAG_0094_AFL);	//查找AFL数据对象
			if(NULL == ptr)
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if(ptr[1]<0x80)
			{
				nLen = ptr[1];
				nPos = 2;
				Tag77_Len -= 2+ptr[1];
				j = ptr[1];
				i = 0;
				while((Tag77_Len>0) && ((ptr[2+j+i]==0x00)||(ptr[2+j+i]==0xFF)))
				{
						i++;
						Tag77_Len--;
				}
			}
			else if(ptr[1] == 0x81)
			{
				nLen = ptr[2];
				nPos = 3;
				Tag77_Len -= 3+ptr[2];
				j = ptr[2];
				i = 0;
				while((Tag77_Len>0) && ((ptr[3+j+i]==0x00)||(ptr[3+j+i]==0xFF)))
				{
						i++;
						Tag77_Len--;
				}
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if((nLen<4) || ((nLen>4) && (nLen-4)%4))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			
			uchAFL[0] = nLen;
			memcpy(uchAFL+1, ptr+nPos, nLen); 	
			ptr = SearchTag(pTag77, tempLen, 0x9F30);	//查找reserved EMV data
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
						Tag77_Len-= 3+*(ptr+2);
				}
				else if(*(ptr+2)==0x81)
				{
						Tag77_Len-= 4+*(ptr+3);
				}	
			}
			
			ptr = SearchTag(pTag77, tempLen, 0x9F24);	//查找reserved EMV data
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
						Tag77_Len-= 3+*(ptr+2);
				}
				else if(*(ptr+2)==0x81)
				{
						Tag77_Len-= 4+*(ptr+3);
				}	
			}
			
			ptr = SearchTag(pTag77, tempLen, 0x9F28);	//查找reserved EMV data
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
						Tag77_Len-= 3+*(ptr+2);
				}
				else if(*(ptr+2)==0x81)
				{
						Tag77_Len-= 4+*(ptr+3);
				}	
			}
			
			if(Tag77_Len != 0)
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
		}
		else
		{
			Nok_Reason = DATA_ERROR;		
			return NOK;		
		}
	}
	return SW_OPERATION_SUCCESS;
}


//Read Application Data
unsigned short EMV_ReadAppData(void)
{
	unsigned char i,j;
	unsigned char sfi;
	unsigned char start_record,end_record;
	unsigned char *ptr = NULL;
	unsigned char Flag_5F24 = 0;
	unsigned char Flag_9F32 = 0;
	unsigned char Flag_9F07 = 0;
	unsigned char Flag_5A = 0;
	unsigned char Flag_8C = 0;
	unsigned char Flag_8D = 0;

	
	internal_state = READ_APP_DATA;

//	AFL entry (4 Bytes):
//	Byte 1 - SFI Value 
//	Byte 2 - First Record Number
//	Byte 3 - Last Record Number
//	Byte 4 - Number of Records in Static Data to be Authenticated
	for(i=0;i<uchAFL[0]/4;i++) //根据AFL循环读取指定记录
	{

		sfi = uchAFL[i*4+1]>>3;
		start_record = uchAFL[i*4+2];
		end_record = uchAFL[i*4+3];

		if((sfi<1) || (sfi>30) || (start_record ==0) || (start_record > end_record) || (uchAFL[i*4+4] > (end_record-start_record+1)))		 //SFI 取值范围1~30, Record Number >0
		{
			Nok_Reason = DATA_ERROR;			 
			return NOK;	
		}
		
		for(j=start_record;j<=end_record;j++)  //根据AFL项中指定的SFI和记录号范围读取记录
		{
			g_APDU.CLA = 0x00;//*(g_APDU.pCommBuff+0) = 0x00;
			g_APDU.INS = 0xB2;//*(g_APDU.pCommBuff+1) = 0xB2;
			g_APDU.PP1 = j;//*(g_APDU.pCommBuff+2) = j;
			g_APDU.PP2 = uchAFL[i*4+1] | 0x04;//*(g_APDU.pCommBuff+3) = uchAFL[i*4+1] | 0x04;
			g_APDU.PP3 = 0x00;//*(g_APDU.pCommBuff+4) = 0x00;
			g_APDU.LC = 0x00;
			g_APDU.LE = 0x00;
			g_APDU.RLE = 0x00;
//			AU9541_ICCRAD_TransCeive(&g_APDU);
//			EMV_ExchangeAPDU(ICC_Protocol,5);
			HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);

			if((g_APDU.SW1 != 0x90) || (g_APDU.SW2 != 0x00))	//交易终止
			{
				Nok_Reason = IC_ERROR;			 
				return NOK;	
			}
			else
			{  
				//查找标签AEF数据模板
				if (EMV_TAG_0070_AEF_DATA_TEMPLATE != g_APDU.pOutData[0])	
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				if(g_APDU.pOutData[1]<3)
					continue;

				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_9F07_AUC);	//查找AUC数据对象
				if((NULL != ptr) && (*(ptr+2)>0))
				{
					if(Flag_9F07 == 0)
					{
						Flag_9F07 = 1;
					}
					else
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
				}
				
				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_9F32_ISSUER_PUBLIC_KEY_EXPONENT);	
				if((NULL != ptr) && (*(ptr+2)>0))
				{
					if(Flag_9F32 == 0)
					{
						Flag_9F32 = 1;
					}
					else
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
				}
				
				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_5F24_APPLICATION_EXPIRATION_DATE);//查找应用失效日期数据对象
				if((NULL != ptr) && (*(ptr+2)>0))
				{
						if(Flag_5F24 == 0)
						{
							Flag_5F24 = 1;
						}
						else
						{
								Nok_Reason = DATA_ERROR;
								return NOK;
						}
				}
					
				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_005A_APPLICATION_PAN);	//查找PAN数据对象
				if((NULL != ptr) && (*(ptr+1)>0))
				{
					if(Flag_5A == 0)
					{
							Flag_5A = 1;
					}
					else
					{
							Nok_Reason = DATA_ERROR;
							return NOK;
					}
				}

				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_008C_CDOL_1);	//查找CDOL1数据对象
				if((NULL != ptr) && ((*(ptr+1)<0x80) && (*(ptr+1)>0)) || ((0x81 == *(ptr+1)) && (*(ptr+2)>0)))
				{				
					if(Flag_8C == 0)
					{
							Flag_8C = 1;
					}
					else
					{
								Nok_Reason = DATA_ERROR;
								return NOK;
					}
				}

				ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, EMV_TAG_008D_CDOL_2);	//查找CDOL2数据对象
				if((NULL != ptr) && ((*(ptr+1)<0x80) && (*(ptr+1)>0)) || ((0x81 == *(ptr+1)) && (*(ptr+2)>0)))
				{
					if(Flag_8D == 0)
					{
						Flag_8D = 1;
					}
					else
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
				}
				
				if(NOK == SaveCardDo(g_APDU.pOutData, g_APDU.RLE))
				{
						Nok_Reason = DATA_ERROR;
						return NOK;
				}
			}
		}
	}

	if(Flag_5F24 && Flag_5A && Flag_8C && Flag_8D)			 //All Mandatory Data are found
	{
		return 	SW_OPERATION_SUCCESS;
	}
	else
	{
		Nok_Reason = MISSING_DATA;			 
		*puchTvr |= 0x20;		//设置IC卡数据缺失
		return NOK;		
	}

}

// 处理限制
/* Application Version Number
 * Dates
 * Application Usage Control
*/
unsigned short EMV_ProcessingRestriction(void)
{
	unsigned char *ptr = NULL;
	unsigned int  year_var = 0;
	
	internal_state = PROCESSING_RESTRICTION;
	
	*puchTvr |= 0x80;			//No SDA performed	

	/*--------------6.1. 应用版本检查----------------------*/
	if(uchAppVer[0] == 2)  //卡片应用版本存在
	{
		if((uchAppVer[1] != *puchVer) || (uchAppVer[2] != *(puchVer+1)))
		{
			*(puchTvr+1) |= 0x80;	//设置卡片和终端应用版本不一致	
		}
	}
		
	/*--------------6.2. 应用用途控制检查----------------------*/
	if(uchAuc[0] == 2)	//AUC数据对象存在
	{
		if((uchAuc[1] & 0x01) == 0)	//检查是否在非ATM终端上有效
		{
			*(puchTvr+1) |= 0x10;	//设置卡片不允许所请求的服务
		}
		
		ptr = SearchTag(Reader_ConstantDo_List, sizeof(Reader_ConstantDo_List), EMV_TAG_9F1A_TERMINAL_COUNTRY_CODE); //查找终端国家代码数据对象
		if((NULL != ptr) && (uchCountryCode[0] == 2))	//如果读写器和卡片中有终端国家代码数据对象
		{
			if((uchCountryCode[1] == *(ptr+3)) && (uchCountryCode[2] == *(ptr+4)))	 //发卡行国家代码与终端相同，则检查国内消费交易是否有效
			{
				if(((uchAuc[1] & 0x28) == 0x00) && (GOODS_PURCHASE == *puchTranType))
				{
					*(puchTvr+1) |= 0x10;	//设置卡片不允许所请求的服务
				}
			}
			else  //发卡行国家代码与终端不同，则检查国际消费交易是否有效
			{
				if(((uchAuc[1] & 0x14) == 0x00) && (GOODS_PURCHASE == *puchTranType))
				{
					*(puchTvr+1) |= 0x10;	//设置卡片不允许所请求的服务
				}
			} 	
		}					
	}
		
	/*--------------6.3. 应用生效日期检查----------------------*/
	if(3 == uchEffectiveDate[0])  //如果卡片生效日期数据对象存在
	{
		if((0 == uchEffectiveDate[2]) || (uchEffectiveDate[2]>0x12))
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		switch(uchEffectiveDate[2])
		{
			case 0x01:
			case 0x03:
			case 0x05:
			case 0x07:
			case 0x08:
			case 0x10:
			case 0x12:
			{
				if((uchEffectiveDate[3]>0x31)||(uchEffectiveDate[3] == 0x00))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				break;
			}
			case 2:
			{
				if(uchEffectiveDate[3] == 0x00)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				if(uchEffectiveDate[1] < 0x50)  //20xx
				{
						year_var = 2000+(uchEffectiveDate[1]/16)*10+(uchEffectiveDate[1]%16);
				}					
				else if(uchEffectiveDate[1] >= 0x50)  //19xx
				{
					  year_var = 1900+(uchEffectiveDate[1]/16)*10+(uchEffectiveDate[1]%16);
				}
				
				if(((0 == (year_var%4))&&(0 != (year_var%100)))||(0 == (year_var%400)))
				{
						if(uchEffectiveDate[3]>0x29)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						}
				}
				else
				{
						if(uchEffectiveDate[3]>0x28)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						}
				}
				
				break;
			}
			case 0x04:
			case 0x06:
			case 0x09:
			case 0x11:
			{
				if((uchEffectiveDate[3]>0x30)||(uchEffectiveDate[3] == 0x00))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				break;
			}
			case 0x00:
			default:			
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
		}

		if(uchEffectiveDate[1] > *puchDate )
		{
			if(uchEffectiveDate[1] < 0x50)
			{	
				*(puchTvr+1) |= 0x20;	//设置应用尚未生效
			}
		}
		else if(uchEffectiveDate[1] == *puchDate) 
		{
			if(uchEffectiveDate[2] > *(puchDate+1))
			{
				*(puchTvr+1) |= 0x20;	//设置应用尚未生效
			}
			else if(uchEffectiveDate[2] == *(puchDate+1))
			{
		 		if(uchEffectiveDate[3] > *(puchDate+2))
				{
					*(puchTvr+1) |= 0x20;	//设置应用尚未生效
				}
			}
			else
			{
			}
		}
		else
		{
		}
	}	
	
	/*--------------6.4. 应用失效日期检查----------------------*/
    if(3 == uchExpirationDate[0])  //如果卡片失效日期数据对象存在
	 {
		if((0 == uchExpirationDate[2]) || (uchExpirationDate[2]>0x12))
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
		
		year_var = 0;

		switch(uchExpirationDate[2])
		{
			case 0x01:
			case 0x03:
			case 0x05:
			case 0x07:
			case 0x08:
			case 0x10:
			case 0x12:
			{
				if((uchExpirationDate[3]>0x31)||(uchExpirationDate[3]==0x00))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				break;
			}
			case 0x02:
			{				
				if(uchExpirationDate[3] == 0x00)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				if(uchExpirationDate[1] < 0x50)  //20xx
				{
						year_var = 2000+(uchExpirationDate[1]/16)*10+(uchExpirationDate[1]%16);
				}					
				else if(uchExpirationDate[1] >= 0x50)  //19xx
				{
					  year_var = 1900+(uchExpirationDate[1]/16)*10+(uchExpirationDate[1]%16);
				}
				
				if(((0 == (year_var%4))&&(0 != (year_var%100)))||(0 == (year_var%400)))
				{
						if(uchExpirationDate[3]>0x29)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						}
				}
				else
				{
						if(uchExpirationDate[3]>0x28)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						}
				}
				
				break;
			}
			case 0x04:
			case 0x06:
			case 0x09:
			case 0x11:
			{
				if((uchExpirationDate[3]>0x30)||(uchExpirationDate[3]==0x00))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				break;
			}
			case 0x00:
			default:			
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
		}

		if(uchExpirationDate[1] > *puchDate )
		{
			if(uchExpirationDate[1] >= 0x50)
			{
					*(puchTvr+1) |= 0x40;	//设置应用已过期
			}
		}
		else if(uchExpirationDate[1] < *puchDate)
		{
			*(puchTvr+1) |= 0x40;	//设置应用已过期
		}
		else if(uchExpirationDate[1] == *puchDate) 
		{
			if(uchExpirationDate[2] < *(puchDate+1))
			{
				*(puchTvr+1) |= 0x40;	//设置应用已过期
			}
			else if(uchExpirationDate[2] == *(puchDate+1))
			{
		 		if(uchExpirationDate[3] < *(puchDate+2))
				{
					*(puchTvr+1) |= 0x40;	//设置应用已过期
				}
			}
			else
			{
			}
		}
		else
		{
		}
	}

	return SW_OPERATION_SUCCESS;
}

//持卡人验证
unsigned short EMV_CardholderVerification(void)
{
	unsigned char  i=0;
	unsigned char  *ptr = NULL;
	unsigned char  cvmLen = 0;
	unsigned char  cvmIndex = 0;
	unsigned int   x_transaction_amount = 0;
	unsigned int   y_transaction_amount = 0;
//	unsigned short rv;

	puchCvmRes[0] = 0x3F;
	puchCvmRes[1] = 0x00;
	puchCvmRes[2] = 0x00;

	if((0 == uchAip[0]) || 0 == (uchAip[1] & 0x10))	  //无AIP或卡片不支持持卡人验证，则跳过该过程
	{
		return SW_OPERATION_SUCCESS;
	}
		
	if(uchCvm[0] < 0x80)
	{
		cvmLen = uchCvm[0];
		cvmIndex = 0;
	}
	else if (uchCvm[0] == 0x81)
	{
		cvmLen = uchCvm[1];
	    cvmIndex = 1;
	}
			
	if((0x10 == (uchAip[1] & 0x10)) && (cvmLen < 8))
	{
		Nok_Reason = MISSING_DATA;
		*puchTvr |= 0x20;		//设置IC卡数据缺失
		return SW_OPERATION_SUCCESS;  						
	}
	
	if((0x10 == (uchAip[1] & 0x10)) && (cvmLen%2 == 1))
	{
		Nok_Reason = DATA_ERROR;
		return NOK;	   						
	}
			
	if((0x10 == (uchAip[1] & 0x10)) &&(cvmLen == 8))
	{
	
		*puchTvr |= 0x20;		//设置IC卡数据缺失
		return SW_OPERATION_SUCCESS;  
	}	
	
	for(i=1;i<5;i++)
	{
		x_transaction_amount <<= 8;
		x_transaction_amount += uchCvm[i];
	}

	for(i=5;i<9;i++)
	{			
		y_transaction_amount <<= 8;
		y_transaction_amount += uchCvm[i];
	}
	
	*puchTsi |= 0x40;	//设置TSI中CVM已执行



	*(puchTvr+2) |= 0x04;  //设置Online PIN Entered	//////////////暂设置所有卡都是需要输入密码的卡
//	PIN_Input = 0;
	
	//循环处理CVM列表, 前八个字节为Amount X,Amount Y
	//第9个字节开始每两个字节为一个CVM入口，其中第一个字节表示持卡人验证方法(CVM Rule)，第二个字节表示CVM条件(CVM COndition Code)
	for(i=9+cvmIndex;i<cvmLen+1;i=i+2) 
	{
		switch(uchCvm[i+1])	   //CVM条件
		{
			case ALWAYS:	//CVM条件：总是执行
			{
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
				{
					//Set CVM Results to ‘Failed’
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];	 							
					*(puchCvmRes+2)	= 0x01;
					*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
					goto cvm_finished;
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];								
#if 0					
					/*处理外部输入的联机加密PIN，并设置相应的TVR*/
					if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
					{
						if(rv == SW_UNKOWN_ERROR)
						{
							//交易取消
							return rv;
							
						}
						else
						{
							if(0 == (uchCvm[i] & 0x40))	 								
							{
								*(puchCvmRes+2)	= 0x01;
								*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
								goto cvm_finished;		
							}
							continue;
						}
					
					
					}				
					
#endif	
//					PIN_Input = 0;
					*(puchCvmRes+2)	= 0x00;	//Unknow
					*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
					goto cvm_finished;	
				}
				else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
				{			
					*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
					if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;	
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}	
					continue;
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{ 
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
					*(puchCvmRes+1)	= uchCvm[i+1];						
					*(puchCvmRes+2)	= 0x02;
					goto cvm_finished;
				}
				else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
				{					
					*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
					if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;	
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}	
					continue;
				}
				else if(OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)) 
				{
					*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
					if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  
				}
				else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
				{
				  if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  	
				}
				else 
				{
					*(puchTvr+2) |= 0x40;  //设置未知的CVM
					if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
						goto cvm_finished;
					}
					continue;
				}
			}
			case SUPPORT_CVM:	//CVM条件：如果终端支持该CVM
			{
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM失败处理
				{	
					//Set CVM Results to ‘Failed’
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1]; 							
					*(puchCvmRes+2)	= 0x01;
					*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
					goto cvm_finished;
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{				
					ptr = SearchTag(Reader_ConstantDo_List, sizeof(Reader_ConstantDo_List), EMV_TAG_9F33_TERMINAL_CAPABILITIES); 
					if(*(ptr+4) & 0x40)	//终端性能中支持该CVM
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1];								
#if 0						
// 						/*处理外部输入的联机加密PIN，并设置相应的TVR*/
// 						if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
// 						{
// 							if(rv == SW_UNKOWN_ERROR)
// 							{
// 								//交易取消
// 								return rv;
// 								
// 							}
// 							else
// 							{
// 								if(0 == (uchCvm[i] & 0x40))	 								
// 								{
// 									*(puchCvmRes+2)	= 0x01;
// 									*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
// 									goto cvm_finished;		
// 								}
// 								continue;
// 							}
// 						}	
						
#endif					
//						PIN_Input = 0;
						*(puchCvmRes+2)	= 0x00;	//Unknow
						*(puchTvr+2) |= 0x04;  //设置Online PIN Entered					
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{
					ptr = SearchTag(Reader_ConstantDo_List, sizeof(Reader_ConstantDo_List), EMV_TAG_9F33_TERMINAL_CAPABILITIES);
	
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];	
	
					if(*(ptr+4) & 0x08)	//终端性能中支持该CVM
					{ 
						*(puchCvmRes+2)	= 2;
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if((OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f))|| (PAPER_SIGNATURE == (uchCvm[i] & 0x3f)))
				{				
					continue;  	
				}
				else
				{
					*(puchTvr+2) |= 0x40;  //设置未知的CVM
					//Set CVM Results to ‘Failed’ 
					*puchCvmRes	= 0x3F;  //设置CVM验证结果
					*(puchCvmRes+1)	= 0x00; 
					puchCvmRes[2] = 0x01;
					continue;
				}
			}
			case UNDRE_X:	//CVM条件：如果Transaction Currency < X
			{				
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount < x_transaction_amount) && (0 == (uchCvm[i] & 0x40)))
					{
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1]; 							
						*(puchCvmRes+2)	= 0x01;
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
						goto cvm_finished;
					}
					else
					{
						continue;
					}
					
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];
					
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount < x_transaction_amount))
					{
#if 0					
// 						/*处理外部输入的联机加密PIN，并设置相应的TVR*/
// 						if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
// 						{
// 							if(rv == SW_UNKOWN_ERROR)
// 							{
// 								//交易取消
// 								return rv;
// 								
// 							}
// 							else
// 							{
// 								if(0 == (uchCvm[i] & 0x40))	 								
// 								{
// 									*(puchCvmRes+2)	= 0x01;
// 									*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
// 									goto cvm_finished;		
// 								}
// 								continue;
// 							}
// 						}
						
#endif
//						PIN_Input = 0;
						*(puchCvmRes+2)	= 0x00;	//Unknow
						*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
						goto cvm_finished;
					}
					else
					{
						continue;
					}								
				}
				else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
				{ 	
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount < x_transaction_amount))
					{
						*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue;
					}
						continue;
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{ 
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount < x_transaction_amount))
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
						*(puchCvmRes+1)	= uchCvm[i+1];						
						*(puchCvmRes+2)	= 0x02;
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
				{
					 *(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue;
				}
				else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
				{
				  if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  	
				}
				else 
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount < x_transaction_amount))
					{
						*(puchTvr+2) |= 0x40;  //设置未知的CVM
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= 0x3F;  //设置CVM验证结果
						*(puchCvmRes+1)	= 0x00; 
						puchCvmRes[2] = 0x01;
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
							goto cvm_finished;
						}
						continue;
					}
					else
					{
						continue;
					}
					
				}
			}
			case OVER_X:	//CVM条件：如果Transaction Currency > X
			{				
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount > x_transaction_amount) && (0 == (uchCvm[i] & 0x40)))
					{
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1];	 							
						*(puchCvmRes+2)	= 0x01;
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
						goto cvm_finished;
					}
					else
					{
						continue;
					}
					
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];
					
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount > x_transaction_amount))
					{
#if 0					
						/*处理外部输入的联机加密PIN，并设置相应的TVR*/
						if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
						{
							if(rv == SW_UNKOWN_ERROR)
							{
								//交易取消
								return rv;
								
							}
							else
							{
								if(0 == (uchCvm[i] & 0x40))	 								
								{
									*(puchCvmRes+2)	= 0x01;
									*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
									goto cvm_finished;		
								}
								continue;
							}
						}	
						
#endif					
//						PIN_Input = 0;
						*(puchCvmRes+2)	= 0x00;	//Unknow
						*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
						goto cvm_finished;
					}
					else
					{
						continue;
					}								
				}
				else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
				{ 	
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount > x_transaction_amount))
					{
						*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
					}
						continue;
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{ 
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount > x_transaction_amount))
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
						*(puchCvmRes+1)	= uchCvm[i+1];						
						*(puchCvmRes+2)	= 0x02;
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
				{
					  *(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
				}
				else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
				{
				  if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  	
				}
				else 
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != x_transaction_amount) && (current_transaction_amount > x_transaction_amount))
					{
						*(puchTvr+2) |= 0x40;  //设置未知的CVM
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= 0x3F;  //设置CVM验证结果
						*(puchCvmRes+1)	= 0x00; 
						puchCvmRes[2] = 0x01;
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
							goto cvm_finished;
						}
						continue;
					}
					else
					{
						continue;
					}
				}
			}
			case UNDRE_Y:	//CVM条件：如果Transaction Currency < Y
			{				
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount < y_transaction_amount) && (0 == (uchCvm[i] & 0x40)))
					{
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1];	 							
						*(puchCvmRes+2)	= 0x01;
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
						goto cvm_finished;
					}
					else
					{
						continue;
					}
					
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];
					
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount < y_transaction_amount))
					{
#if 0					
						/*处理外部输入的联机加密PIN，并设置相应的TVR*/
						if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
						{
							if(rv == SW_UNKOWN_ERROR)
							{
								//交易取消
								return rv;
								
							}
							else
							{
								if(0 == (uchCvm[i] & 0x40))	 								
								{
									*(puchCvmRes+2)	= 0x01;
									*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
									goto cvm_finished;		
								}
								continue;
							}
						}			
						
#endif			
//						PIN_Input = 0;
						*(puchCvmRes+2)	= 0x00;	//Unknow
						*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
						goto cvm_finished;
					}
					else
					{
						continue;
					}								
				}
				else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount < y_transaction_amount))
					{
						*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
					}
						continue;
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{ 
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount < y_transaction_amount))
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
						*(puchCvmRes+1)	= uchCvm[i+1];						
						*(puchCvmRes+2)	= 0x02;
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
				{
					  *(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
				}
				else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
				{
				  if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  	
				}
				else 
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount < y_transaction_amount))
					{
						*(puchTvr+2) |= 0x40;  //设置未知的CVM
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= 0x3F;  //设置CVM验证结果
						*(puchCvmRes+1)	= 0x00; 
						puchCvmRes[2] = 0x01;
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
							goto cvm_finished;
						}
						continue;
					}
					else
					{
						continue;
					}
				}
			}
			case OVER_Y:	//CVM条件：如果Transaction Currency > Y
			{
				if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount > y_transaction_amount) && (0 == (uchCvm[i] & 0x40)))
					{
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1];	 							
						*(puchCvmRes+2)	= 0x01;
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
						goto cvm_finished;
					}
					else
					{
						continue;
					}
					
				}
				else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
				{
					*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
					*(puchCvmRes+1)	= uchCvm[i+1];
					
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount > y_transaction_amount))
					{
#if 0					
						/*处理外部输入的联机加密PIN，并设置相应的TVR*/
						if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
						{
							if(rv == SW_UNKOWN_ERROR)
							{
								//交易取消
								return rv;
								
							}
							else
							{
								if(0 == (uchCvm[i] & 0x40))	 								
								{
									*(puchCvmRes+2)	= 0x01;
									*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
									goto cvm_finished;		
								}
								continue;
							}
						}	
						
#endif					
//						PIN_Input = 0;
						*(puchCvmRes+2)	= 0x00;	//Unknow
						*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
						goto cvm_finished;
					}
					else
					{
						continue;
					}								
				}
				else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
				{ 	
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount > y_transaction_amount))
					{
						*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
					}
						continue;
				}
				else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
				{ 
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount > y_transaction_amount))
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
						*(puchCvmRes+1)	= uchCvm[i+1];						
						*(puchCvmRes+2)	= 0x02;
						goto cvm_finished;
					}
					else
					{
						continue;
					}
				}
				else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
				{
					  *(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
				}
				else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
				{
				  if(0 == (uchCvm[i] & 0x40))	 								
					{
						//Set CVM Results to ‘Failed’
						puchCvmRes[0] = 0x3F;
						puchCvmRes[1] = 0;
						puchCvmRes[2] = 0x01;		
						*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
					  goto cvm_finished;		
					}						
					continue;  	
				}
				else 
				{
					if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)) && (0 != y_transaction_amount) && (current_transaction_amount > y_transaction_amount))
					{
						*(puchTvr+2) |= 0x40;  //设置未知的CVM
						//Set CVM Results to ‘Failed’
						*puchCvmRes	= 0x3F;  //设置CVM验证结果
						*(puchCvmRes+1)	= 0x00; 
						puchCvmRes[2] = 0x01;
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
							goto cvm_finished;
						}
						continue;
					}
					else
					{
						continue;
					}
					
				}
			}			
			case NOT_UNATTENED_CASH:
			{				
					if(FAIL_CVM_PROCESSING == (uchCvm[i] & 0x3f))		//CVM = Fail CVM
					{
						if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)))
						{
							//Set CVM Results to ‘Failed’
							*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
							*(puchCvmRes+1)	= uchCvm[i+1];	 							
							*(puchCvmRes+2)	= 0x01;
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	 								
							goto cvm_finished;
						}
						else
						{
							continue;
						}
						
					}
					else if(ONLINE_ENCIPERED_PIN == (uchCvm[i] & 0x3f))	//联机加密PIN验证
					{
						*puchCvmRes	= uchCvm[i];  //设置CVM验证结果
						*(puchCvmRes+1)	= uchCvm[i+1];
						
						if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)))
						{
#if 0						
							/*处理外部输入的联机加密PIN，并设置相应的TVR*/
							if( SW_OPERATION_SUCCESS != (rv = EMV_OnlineEncryptedPinProcessing()))
							{
								if(rv == SW_UNKOWN_ERROR)
								{
									//交易取消
									return rv;
									
								}
								else
								{
									if(0 == (uchCvm[i] & 0x40))	 								
									{
										*(puchCvmRes+2)	= 0x01;
										*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
										goto cvm_finished;		
									}
									continue;
								}
							}	
							
#endif						
//							PIN_Input = 0;
							*(puchCvmRes+2)	= 0x00;	//Unknow
							*(puchTvr+2) |= 0x04;  //设置Online PIN Entered
							goto cvm_finished;
						}
						else
						{
							continue;
						}								
					}
					else if(OFFLINE_PLAIN_PIN == (uchCvm[i] & 0x3f))	//脱机明文PIN验证
					{
						if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)))
						{
							*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
							if(0 == (uchCvm[i] & 0x40))	 								
							{
								//Set CVM Results to ‘Failed’
								puchCvmRes[0] = 0x3F;
								puchCvmRes[1] = 0;
								puchCvmRes[2] = 0x01;	
								*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
								goto cvm_finished;		
							}	
							continue; 
						}
							continue;
					}
					else if(NO_CVM_REQUIRED == (uchCvm[i] & 0x3f))	//无需CVM
					{ 
						if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)))
						{
							*puchCvmRes	= uchCvm[i];  //设置CVM验证结果Success
							*(puchCvmRes+1)	= uchCvm[i+1];						
							*(puchCvmRes+2)	= 0x02;
							goto cvm_finished;
						}
						else
						{
							continue;
						}
					}
					else if((OFFLINE_ENCIPEREDPIN == (uchCvm[i] & 0x3f)) || (OFFLINE_ENCIPEREDPIN_SIGNATURE == (uchCvm[i] & 0x3f)) || (OFFLINE_PLAINPIN_SIGNATURE == (uchCvm[i] & 0x3f)))
					{
						*(puchTvr+2) |= 0x10;      //密码键盘不存在或工作不正常
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;	
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}	
						continue; 
					}
					else if (PAPER_SIGNATURE == (uchCvm[i] & 0x3f))
					{
						if(0 == (uchCvm[i] & 0x40))	 								
						{
							//Set CVM Results to ‘Failed’
							puchCvmRes[0] = 0x3F;
							puchCvmRes[1] = 0;
							puchCvmRes[2] = 0x01;		
							*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
							goto cvm_finished;		
						}						
						continue;  	
					}
					else 
					{
						if((2 == uchAppCurrencyCode[0]) && (0 == memcmp(puchCurrencyCode,&uchAppCurrencyCode[1],2)))
						{
							*(puchTvr+2) |= 0x40;  //设置未知的CVM
							//Set CVM Results to ‘Failed’
							*puchCvmRes	= 0x3F;  //设置CVM验证结果
							*(puchCvmRes+1)	= 0x00; 
							puchCvmRes[2] = 0x01;
							if(0 == (uchCvm[i] & 0x40))	 								
							{
								*(puchTvr+2) |= 0x80;  //设置持卡人验证失败	  								
								goto cvm_finished;
							}
							continue;
						}
						else
						{
							continue;
						}
						
					}
			}
			case MANUAL_CASH:
			case UNATTENED_CASH:
			case PURCHASE_WITH_CASHBACK:
			{
					continue;
			}
			default:    //Not Recognized CVM Condition
			{
				*(puchTvr+2) |= 0x40;  //设置未知的CVM
				
				puchCvmRes[0] = 0x3F;
				puchCvmRes[1] = 0;
				puchCvmRes[2] = 0x01;
				continue;
			}
		}
	}

	if(i == cvmLen+1)  //CVM列表检查完毕，没有验证成功的CVM
	{
		*(puchTvr+2) |= 0x80;  //设置持卡人验证失败
		//Set CVM Result == Failed
		puchCvmRes[0] = 0x3F;
		puchCvmRes[1] = 0;
		puchCvmRes[2] = 0x01;
	}

cvm_finished:
	
	return 	SW_OPERATION_SUCCESS;	
}

//联机密文PIN处理
unsigned short EMV_OnlineEncryptedPinProcessing(void)
{
	
//	unsigned char i,line,column,AscllKey,temp;
//
//	MID_DISP_SmallPic(SMALLPIC_MIWEN); 
//	memset(g_b_plain_pin,0x00,PIN_BUF_LENGTH);
//
//	i=0;
//  while(1)
//	{
//		Key_task();
//		if((rtKey.valid == 1) && Translate_Key_Value(&AscllKey,rtKey.value) == 0)
//		{
//			if((i<MAX_PIN_LENGTH)&&((AscllKey == '0')||(AscllKey == '1')||(AscllKey == '2')||(AscllKey == '3')||(AscllKey == '4')||(AscllKey == '5')||(AscllKey == '6')||(AscllKey == '7')||(AscllKey == '8')||(AscllKey == '9')))
//			{
//				Bee_music(BEE_TPYE_1);
//	      column= (i%ONE_PAGE_WORD_SIZE)*8+1;
//				MID_DISP_ShowChar16X8(2,column,0x2A,ASCIIchardot);
//				if (i%2)
//				{
//					g_b_plain_pin[1+(i/2)] += AscllKey-0x30;
//				}
//				else
//				{
//					g_b_plain_pin[1+(i/2)] += (AscllKey-0x30)*16;
//				}
//				i++;			
//			}
//			else if((i>3) && (AscllKey == 'a'))
//			{
//				
//				g_b_plain_pin[0] = i;
//				i++;
//				Bee_music(BEE_TPYE_1);
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				MID_DISP_SmallPic(SMALLPIC_CHULIZHONG);
//				break;
//			}
//			else if(AscllKey == 'c')
//			{
//				i = MAX_PIN_LENGTH+1;
//				Bee_music(BEE_TPYE_1);
//				MID_DISP_SmallPic(SMALLPIC_JIAOYIQUXIAO);
//				rtKey.valid = 0;
//				rtKey.value = 0;
//				AscllKey = 0xff; 
//				memset(g_b_plain_pin,0x00,PIN_BUF_LENGTH);
//				return SW_UNKOWN_ERROR;
//			}
//			else if((AscllKey == 'e')&&(i>0))
//			{
//				i--;
//				Bee_music(BEE_TPYE_1);
//				if (i%2)
//				{
//					g_b_plain_pin[1+(i/2)] &= 0xf0;
//				}
//				else
//				{
//					g_b_plain_pin[1+(i/2)] &= 0x0f;
//				}
//	            line = 2;
//	            column= (i%ONE_PAGE_WORD_SIZE)*8+1;
//	            MID_DISP_ShowChar16X8(line,column,0x5f,ASCIIchardot);
//			}
//			else
//			{
//			}
//			rtKey.valid = 0;
//			rtKey.value = 0;
//			AscllKey = 0xff; 
//		}
//	}
//
//	*(puchTvr+2) |= 0x04;  //设置Online PIN entered	
//
//	memset(encrypt_buf,0x00,600);	
//
//	temp = g_b_plain_pin[0];
//  if(temp%2)
//	{
//			memset(g_b_plain_pin+2+(temp/2),0xFF,6-(temp/2));
//		  g_b_plain_pin[1+(temp/2)] = (g_b_plain_pin[1+(temp/2)]&0xF0) + 0x0F;
//	}
//	else
//	{
//			memset(g_b_plain_pin+1+(temp/2),0xFF,7-(temp/2));
//	}
//
//	
//	if (SW_OPERATION_SUCCESS != PIN_Encrypt(encrypt_buf))		
//		return NOK;	
//		
// 	MID_DISP_SmallPic(SMALLPIC_ZHONGGUOYINLIAN);		   
	return 	SW_OPERATION_SUCCESS;
}









//终端风险管理
void EMV_TerminalRiskManagement(void)
{
	unsigned char  rand[4] = {0};
	unsigned char  *ptr = NULL;
	unsigned int   transaction_target_percent;
	unsigned int   floor_limit;

	internal_state = TERMINAL_RISK_MANAGEMENT;
	
/*  EMV supports two different formats for Amount, Authorised and terminals must support both formats for the amount.  Amount, Authorised (Binary), tag '81' is a 4-byte binary format number 
	while Amount, Authorised (Numeric), tag '9F02' is a 6-byte numeric format number. Both represent the amount that is being authorised. The recommended set of data elements to be included in
	Application Cryptogram generation includes Amount, Authorised (Numeric) so the card is likely to request this tag in the CDOL1/CDOL2. However, nothing prevents the card from requesting 
	Amount, Authorised (Binary) in the PDOL, DDOL, CDOL1, CDOL2 so EMV terminals must support both the binary and numeric formats of the amount.

	Terminal Floor Limit, Amount Reference Currency, and CVM List values 'X' and 'Y' are expressed as 4-byte binary numbers. When the terminal is using these values for internal comparisons to 
	the authorised amount, the terminal needs to use a binary representation of the amount field.

	The maximum amount that can be expressed using Amount, Authorised (Numeric) is 999,999,999,999 (9,999,999.99 with two decimal positions) while the maximum amount that can be expressed using 
	Amount, Authorised (Binary) is smaller, 4,294,967,295 (42,949,672.95 with two decimal positions).  Either data element is considered sufficient as an authorisation amount. 
*/
	floor_limit = ((puchTerminalFloorLimit[0]*256+puchTerminalFloorLimit[1])*256+puchTerminalFloorLimit[2])*256+puchTerminalFloorLimit[3];;
	if((ThresholdforBiasedRandomSelection > floor_limit) || (PercentforRandomSelection > 99) || (MaxPercentforBiasedRandomSelection < PercentforRandomSelection))
	{
		goto Velocity_Checking;
	}

	while(1)
	{

//		MID_ALG_GenRand(&rand,1);
		GetChallenge(rand,4);
			 
	 	if((rand[0]>0) && (rand[0]<100))
		{
			break;
		}
	}

	//屏幕显示终端风险管理，随机数 	MID_DISP_ShowChar16X8(2,1+7*8,0x30,ASCIIchardot);
// 	MID_DISP_SmallPic(SMALLPIC_SUIJISHU);	
// 	MID_DISP_ShowChar16X8(1,4*17,(rand/10)+0x30,ASCIIchardot);
// 	MID_DISP_ShowChar16X8(1,4*17+8,(rand%10)+0x30,ASCIIchardot);
// 	Delay_ms(3000);
	
	
	if(current_transaction_amount >= floor_limit)
	{
		*(puchTvr+3) |= 0x80;  //设置交易超过最低限额
	}	
	else if(current_transaction_amount < ThresholdforBiasedRandomSelection)
	{
	 	if(rand[0] <= PercentforRandomSelection)
		{
			*(puchTvr+3) |= 0x10;  //设置交易被随机选择联机处理
		}
	}
	else if((current_transaction_amount >= ThresholdforBiasedRandomSelection) && (current_transaction_amount <= floor_limit))
	{
		transaction_target_percent = ((MaxPercentforBiasedRandomSelection-PercentforRandomSelection)*(current_transaction_amount-ThresholdforBiasedRandomSelection)/(floor_limit-ThresholdforBiasedRandomSelection))+PercentforRandomSelection;	
		if(rand[0] <= transaction_target_percent)
		{
			*(puchTvr+3) |= 0x10;  //设置交易被随机选择联机处理
		}
	}

	//Velocity checking
Velocity_Checking:
	memset(uchAtc, 0x00, 3);
	memset(uchLATC, 0x00, 3);
	
	//Get LCOL and UCOL
	if ((uchLCOL[0] > 0) && (uchUCOL[0] > 0))
  {
	 	//Get Last ATC
		g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
		g_APDU.INS = 0xCA;//*(g_APDU.pCommBuff+1) = 0xCA;
		g_APDU.PP1 = 0x9F;//*(g_APDU.pCommBuff+2) = 0x9F;
		g_APDU.PP2 = 0x13; //*(g_APDU.pCommBuff+3) = 0x13;  //Last ATC
		g_APDU.PP3 = 0x00;//*(g_APDU.pCommBuff+4) = 0x00;
		g_APDU.LC = 0x00;
		g_APDU.LE = 0x00;
		g_APDU.RLE = 0x00;
//		AU9541_ICCRAD_TransCeive(&g_APDU);
//		EMV_ExchangeAPDU(ICC_Protocol,5);
		HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
		if(((g_APDU.SW1 == 0x6A) && (g_APDU.SW2 == 0x81)) || ((g_APDU.SW1 ==0x6A) && (g_APDU.SW2 == 0x88)))
		{
			*puchTvr |= 0x20;	   //设置IC卡数据缺失
			*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
			goto trm_Finished;
		}
		else if((g_APDU.SW1 == 0x90) && (g_APDU.SW2 == 0x00))
		{
			ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, 0x9F13);
			if(NULL != ptr)
			{
				uchLATC[0] = 2;
				memcpy(uchLATC+1, ptr+3,2);
			}
			else
			{
				*puchTvr |= 0x20;	   //设置IC卡数据缺失
				*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
				goto trm_Finished;
			}
		}
		else
		{
			*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
		}
		
		if((0 == uchLATC[1]) && (0 == uchLATC[2]))
		{
			*(puchTvr+1) |= 0x08;  //设置新卡
		}
	
		//Get ATC
		g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
		g_APDU.INS = 0xCA;//*(g_APDU.pCommBuff+1) = 0xCA;
		g_APDU.PP1 = 0x9F;//*(g_APDU.pCommBuff+2) = 0x9F;
		g_APDU.PP2 = 0x36; //*(g_APDU.pCommBuff+3) = 0x36;  //ATC
		g_APDU.PP3 = 0x00;//*(g_APDU.pCommBuff+4) = 0x00;
		g_APDU.LC = 0x00;
		g_APDU.LE = 0x00;
		g_APDU.RLE = 0x00;
//		AU9541_ICCRAD_TransCeive(&g_APDU);
//		EMV_ExchangeAPDU(ICC_Protocol,5);
		HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
		if(((g_APDU.SW1 == 0x6A) && (g_APDU.SW2 == 0x81)) || ((g_APDU.SW1 ==0x6A) && (g_APDU.SW2 == 0x88)))
		{
			*puchTvr |= 0x20;	   //设置IC卡数据缺失
			*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
			goto trm_Finished;
		}
		else if((g_APDU.SW1 == 0x90) && (g_APDU.SW2 == 0x00))
		{
			ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, 0x9F36);
			if(NULL != ptr)
			{
				uchAtc[0] = 2;
				memcpy(uchAtc+1, ptr+3,2);
			}
			else
			{
				*puchTvr |= 0x20;	   //设置IC卡数据缺失
				*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
				goto trm_Finished;
			}
		}
		else
		{
			*(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
		}
	
		if((uchAtc[1] < uchLATC[1]) || ((uchAtc[1] == uchLATC[1])&&(uchAtc[2] <= uchLATC[2])))
		{
		   *(puchTvr+3) |= 0x60;  //设置超过连续脱机交易下限和上限
		}
	
		if(((uchAtc[1]-uchLATC[1])*256+(uchAtc[2]-uchLATC[2])) > uchLCOL[1])
		{
		   *(puchTvr+3) |= 0x40;  //设置超过连续脱机交易下限 
		}
		else
		{
			goto trm_Finished;
		}
	
		if(((uchAtc[1]-uchLATC[1])*256+(uchAtc[2]-uchLATC[2])) > uchUCOL[1])
		{
		   *(puchTvr+3) |= 0x20;  //设置超过连续脱机交易上限 
		}
	 }

trm_Finished:
	*puchTsi |= 0x08;	//设置TSI中Terminal Risk Management已执行
	return;
}

//终端行为分析
unsigned short EMV_TerminalActionAnalysis(unsigned char revFlag)
{
	unsigned char i,j,tempLen;//,cvmLen,cvmIndex;
	unsigned char *ptr = NULL;
	unsigned char *pTag = NULL;
	unsigned int  Tag77_Len = 0;
	unsigned int  Tag80_Len = 0;
	unsigned int  nLen = 0;
  	unsigned char uchReq = 0;
	unsigned char uchOnline;
//	unsigned char temp_buf[8] = {0};

	unsigned short n = 0;
	
	uchOnline = 1;
	uchReq = 0xFF;
	
//	if((0 == PIN_Input)&&((0 == revFlag)||(1 == revFlag)))
//	{
//		if( SW_OPERATION_SUCCESS != EMV_OnlineEncryptedPinProcessing())
//		{
//			Nok_Reason = DATA_ERROR;
//			return NOK;
//		}
//		
//		PIN_Input = 1;
//	}
	*(puchTvr+2) |= 0x04;  //设置Online PIN Entered	 //		 后添


	if(0 == uchIacDenial[0])	//If the Issuer Action Code - Denial does not exist, a default value with all bits set to 0 is to be used.
	{
		memset(uchIacDenial+1, 0, 5);
	}  

	/* **************************************************
	 * Offline approved 					Y1
	 * Offline declined 						Z1
	 * Unable to go online, offline approved 	Y3
	 * Unable to go online, offline declined 	Z3
	 ****************************************************/

	for(i=0;i<5;i++)  //检查是否脱机拒绝
	{
		if(0 == *(puchTvr+i))
		{
			continue;
		}
		else
		{
			for(j=0;j<8;j++)
			{
				if((((uchIacDenial[i+1]>>j)&0x01) || ((uchTacDenial[i]>>j)&0x01)) && ((*(puchTvr+i)>>j)&0x01))
				{
					memcpy(puchArc, "Z1", 2);
					uchReq = AAC_REQ;	//AAC
					break;
				}
			}
			if(8 != j)
			{
				break;
			}
		}
	}
	
	if((5 == i) && (8 == j))	   //if it has not already decided to reject the transaction as procedure described above, it shall continue transaction processing online
	{
		if((uchOnline)&&(uchReq != AAC_REQ)) 	//检查是否联机					
		{
			uchReq = ARQC_REQ;	//ARQC
		}
		else
		{ 
			memcpy(puchArc, "Z3", 2);		  //Unable to go online, offline declined
			uchReq = AAC_REQ;  //AAC
		}
	}
	
	*puchTvr |= 0x80;	//Offline data authentication was not performed

	 G_B_GACResult = 0;    //初始化

//	//第一次Generate AC
//	g_APDU.CLA = 0x80;
//	g_APDU.INS = 0xAE;
//	g_APDU.PP1 = uchReq;
//	g_APDU.PP2 = 0x00;
	//第一次Generate AC
	g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
	g_APDU.INS = 0xAE;//*(g_APDU.pCommBuff+1) = 0xAE;
	g_APDU.PP1 = uchReq;//*(g_APDU.pCommBuff+2) = uchReq;
	g_APDU.PP2 = 0x00;//*(g_APDU.pCommBuff+3) = 0x00;
	
	if(uchCdol1[0] < 0x80)
	{		
//		g_APDU.PP3 = *(g_APDU.pCommBuff+4) = CollectDol(uchCdol1+1, uchCdol1[0], g_APDU.pCommBuff+5,CDOL1_FLAG);
			g_APDU.PP3 = CollectDol(uchCdol1+1, uchCdol1[0], g_APDU.pInData,CDOL1_FLAG);

	}
	else if(uchCdol1[0] == 0x81)
	{
//		g_APDU.PP3 = *(g_APDU.pCommBuff+4) = CollectDol(uchCdol1+2, uchCdol1[1], g_APDU.pCommBuff+5,CDOL1_FLAG);	
		g_APDU.PP3 = CollectDol(uchCdol1+2, uchCdol1[1], g_APDU.pInData,CDOL1_FLAG);
	}
	else
	{}
	memcpy(g_APDU.pInData,g_APDU.pInData,g_APDU.PP3);	
	g_APDU.LC = g_APDU.PP3;//*(g_APDU.pCommBuff+4);
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;

//	AU9541_ICCRAD_TransCeive(&g_APDU);
//	HAL_Card_APDU(&g_APDU,5+g_APDU.LC);
		HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);

	if(0x90 != (g_APDU.SW1) || 0x00 != (g_APDU.SW2))	//交易终止
	{
		Nok_Reason = IC_ERROR;
		return NOK;
	}
	else
	{
		*puchTsi |= 0x20;	//卡片风险管理已执行

		/* Bit2~Bit0 of CID
		 *  	x x x Reason/advice code
		 *  	0 0 0 No information given
		 *  	0 0 1 Service not allowed
		 *  	0 1 0 PIN Try Limit exceeded
		 *  	0 1 1 Issuer authentication failed
		 *  	1 x x Other values RFU
		*/

		if(EMV_TAG_0080_RESPONSE_MESSAGE_TEMPLATE_FORMAT_1 == *g_APDU.pOutData)  //格式1
		{
			if(*(g_APDU.pOutData+1) < 0x80)
			{
					Tag80_Len = *(g_APDU.pOutData+1);
					if((Tag80_Len > 43) || (Tag80_Len < 11) || (Tag80_Len != g_APDU.RLE-2))   //Miminum and Maximum Lenth for Format 1
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					
					G_B_GACResult = 0x01;
					if((ARQC_REQ || TC_REQ) == (*(g_APDU.pOutData+2) & 0xC0))
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					else if(TC_REQ == (*(g_APDU.pOutData+2) & 0xC0))
					{
						if((uchReq == ARQC_REQ) || (uchReq == AAC_REQ))
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						} 
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+2);
					}
					else if(ARQC_REQ == (*(g_APDU.pOutData+2) & 0xC0))
					{
						if(uchReq == AAC_REQ)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						} 
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+2);

						G_B_GACResult |= 0x10;//同意联机
						memcpy(puchArc, "Y1", 2);
					}
					else if(AAC_REQ == (*(g_APDU.pOutData+2) & 0xC0))
					{
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+2);

						*g_APDU.pOutData = 0x01;
						g_APDU.RLE = 0x01;
						memcpy(puchArc, "Z1", 2);
					}
					else
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					
					if((NOT_ALLOWED == (*(g_APDU.pOutData+2) & 0x0B)) || ( NOT_ALLOWED_ADVICE== (*(g_APDU.pOutData+2) & 0x0B)))
					{				   				
						*g_APDU.pOutData = 0x01;
						g_APDU.RLE = 0x01;
//						MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
						return SW_OPERATION_SUCCESS;
					}
												
					uchAtc[0] = 2;						  				//M
					memcpy(uchAtc+1,g_APDU.pOutData+3,2);

					uchAc[0] = 8;												//M
					memcpy(uchAc+1,g_APDU.pOutData+5,8);

					if(*(g_APDU.pOutData+1)>11)							//O
					{
						uchIad[0] = *(g_APDU.pOutData+1)-11;				
						memcpy(uchIad+1,g_APDU.pOutData+13,uchIad[0]);
					}
					else
					{
						uchIad[0] = 0;
					}
			}
			else if(*(g_APDU.pOutData+1) == 0x81)
			{
					Tag80_Len = *(g_APDU.pOutData+2);
					if((Tag80_Len > 43) || (Tag80_Len < 11) || (Tag80_Len != g_APDU.RLE-3))   //Miminum and Maximum Lenth for Format 1
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					
					G_B_GACResult = 0x01;
					if((ARQC_REQ || TC_REQ) == (*(g_APDU.pOutData+3) & 0xC0))
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					else if(TC_REQ == (*(g_APDU.pOutData+3) & 0xC0))
					{
						if((uchReq == ARQC_REQ) || (uchReq == AAC_REQ))
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						} 
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+3);
					}
					else if(ARQC_REQ == (*(g_APDU.pOutData+3) & 0xC0))
					{
						if(uchReq == AAC_REQ)
						{
							Nok_Reason = DATA_ERROR;
							return NOK;
						} 
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+3);

						G_B_GACResult |= 0x10;//同意联机
						memcpy(puchArc, "Y1", 2);
					}
					else if(AAC_REQ == (*(g_APDU.pOutData+3) & 0xC0))
					{
						uchCid[0] = 1;
						uchCid[1] = *(g_APDU.pOutData+3);

// 						*g_APDU.pOutData = 0x01; //脱机拒绝
						*g_APDU.pOutData = 0x01;
						g_APDU.RLE = 0x01;
						memcpy(puchArc, "Z1", 2);
					}
					else
					{
						Nok_Reason = DATA_ERROR;
						return NOK;
					}
					
					if((NOT_ALLOWED == (*(g_APDU.pOutData+3) & 0x0B)) || ( NOT_ALLOWED_ADVICE== (*(g_APDU.pOutData+3) & 0x0B)))
					{				   				
						*g_APDU.pOutData = 0x01;
						g_APDU.RLE = 0x01;
//						MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
						return SW_OPERATION_SUCCESS;
					}
												
					uchAtc[0] = 2;						  				//M
					memcpy(uchAtc+1,g_APDU.pOutData+4,2);

					uchAc[0] = 8;												//M
					memcpy(uchAc+1,g_APDU.pOutData+6,8);

					if(*(g_APDU.pOutData+2)>11)							//O
					{
						uchIad[0] = *(g_APDU.pOutData+2)-11;				
						memcpy(uchIad+1,g_APDU.pOutData+14,uchIad[0]);
					}
					else
					{
						uchIad[0] = 0;
					}
			}
			else
			{} 
		}
		else if (*g_APDU.pOutData == EMV_TAG_0077_RESPONSE_MESSAGE_TEMPLATE_FORMAT_2)			//格式2
		{
			G_B_GACResult = 0x02;

			if(*(g_APDU.pOutData+1) < 0x80)
			{
				Tag77_Len = *(g_APDU.pOutData+1); 
				pTag = 	g_APDU.pOutData+2;
			}
			else if(*(g_APDU.pOutData+1) == 0x81)
			{
				Tag77_Len = *(g_APDU.pOutData+2); 
				pTag = 	g_APDU.pOutData+3;
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			
			nLen = Tag77_Len;

			i=0;
			while((Tag77_Len>0)&&((pTag[i] == 0x00)||(pTag[i] == 0xFF)))
			{
				Tag77_Len--;
				i++;
			}
			
			ptr = SearchTag(pTag, nLen, 0x54);	//Tag 54
			if((NULL != ptr))
			{
				Tag77_Len -= 2+*(ptr+1); 
			}
			
			ptr = SearchTag(pTag, nLen, EMV_TAG_9F27_CRYPTOGRAM_INFO_DATA);	//查找CID数据对象(M)
			if((NULL == ptr)||(ptr[2] != 1))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else if((ARQC_REQ || TC_REQ)  == (ptr[3] & 0xC0))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else if(TC_REQ == (ptr[3] & 0xC0))
			{
				if((uchReq == ARQC_REQ) || (uchReq == AAC_REQ))
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				} 
				uchCid[0] = 1;
				uchCid[1] = ptr[3];

				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			else if(ARQC_REQ == (ptr[3] & 0xC0))
			{
				if(uchReq == AAC_REQ)
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				} 
				uchCid[0] = 1;
				uchCid[1] = ptr[3];

				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
				G_B_GACResult |= 0x10;//同意联机
				memcpy(puchArc, "Y1", 2);
			}
			else if(AAC_REQ == (ptr[3] & 0xC0))
			{
				uchCid[0] = 1;
				uchCid[1] = ptr[3];

				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
				*g_APDU.pOutData = 0x01;
				g_APDU.RLE = 0x01;
				memcpy(puchArc, "Z1", 2);
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if((NOT_ALLOWED == (ptr[3] & 0x0B)) || ( NOT_ALLOWED_ADVICE== (ptr[3] & 0x0B)))
			{				   				
				*g_APDU.pOutData = 0x01;
				g_APDU.RLE = 0x01;
//				MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
				return SW_OPERATION_SUCCESS;				
			}

			ptr = SearchTag(pTag, nLen, EMV_TAG_9F36_ATC);	//查找ATC数据对象(M)
			if((NULL == ptr)||(ptr[2] != 2))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else
			{
				uchAtc[0] = 2;
				memcpy(uchAtc+1,ptr+3,2);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}

			ptr = SearchTag(pTag, nLen, EMV_TAG_9F26_APPLICATION_CRYPTOGRAM);	//查找AC数据对象(M)
			if((NULL == ptr)||(ptr[2] != 8))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else
			{
				uchAc[0] = 8;
				memcpy(uchAc+1,ptr+3,8);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}

			ptr = SearchTag(pTag, nLen, EMV_TAG_9F10_ISSUER_APPLICATION_DATA);	//查找发卡行自定义数据对象(O)
			if((NULL != ptr) && (ptr[2] <= 32))
			{
				uchIad[0] = ptr[2];
				memcpy(uchIad+1,ptr+3,ptr[2]);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			
			ptr = SearchTag(pTag, nLen, EMV_TAG_DF24_ISSUER_PROPRIETARY_DATA);	//查找发卡行自定义数据对象(O)
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
					Tag77_Len -= 3+*(ptr+2);
					i=0;
					tempLen = ptr[2];
					while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
					{
						Tag77_Len--;
						i++;
					}
				}
				else if(*(ptr+2)==0x81)
				{
					Tag77_Len -= 4+*(ptr+3);
					i=0;
					tempLen = ptr[3];
					while((Tag77_Len>0)&&((ptr[4+tempLen+i] == 0x00)||(ptr[4+tempLen+i] == 0xFF)))
					{
						Tag77_Len--;
						i++;
					}
				}
				else
				{}
				
			}
			
			ptr = SearchTag(pTag, nLen, EMV_TAG_DF06_ISSUER_PROPRIETARY_DATA);	//查找发卡行自定义数据对象(O)
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
					Tag77_Len -= 3+*(ptr+2);
					i=0;
					tempLen = ptr[2];
					while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
					{
						Tag77_Len--;
						i++;
					}
				}
				else if(*(ptr+2)==0x81)
				{
					Tag77_Len -= 4+*(ptr+3);
					i=0;
					tempLen = ptr[3];
					while((Tag77_Len>0)&&((ptr[4+tempLen+i] == 0x00)||(ptr[4+tempLen+i] == 0xFF)))
					{
						Tag77_Len--;
						i++;
					}
				}
				else
				{}
			}
			
			if(Tag77_Len>0)
			{
				Nok_Reason = DATA_ERROR;
				return NOK;				
			}
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}
	}

// Response_Data:
	if(0x00 == (G_B_GACResult & 0xf0))	//脱机拒绝
	{
		*g_APDU.pOutData = 0x01;
		g_APDU.RLE = 0x01;
//		MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);
	}
	else								//同意联机
	{
//		MID_DISP_SmallPic(SMALLPIC_CHULIZHONG);

		ptr = g_APDU.pOutData;
		n = 0;
#if 0		
		if ((1 == revFlag)||(2 == revFlag))				//圈存交易
		{
			ptr[n++] = 0;
			ptr[n++] = 0;
			ptr[n++] = 0;
			ptr[n++] = puchAmount[3];
			ptr[n++] = puchAmount[4];
			ptr[n++] = puchAmount[5];
//			ptr[n++] = (((money1/1000)<<4)&0xf0) + (((money1%1000)/100)&0x0f);		
//			ptr[n++]  = ((((money1%100)/10)<<4)&0xf0) + ((money1%10)&0x0f);
//			ptr[n++] = (((money2/10)<<4)&0xf0) + ((money2%10)&0x0f);
		}		
		ptr[n++] = 0x02;	//联机请求	
#endif	
#if 0		
		if ((2 == revFlag)||(3 == revFlag))
		{
			ptr[n++] = 0x5a;
			ptr[n++] = uchPan[0];
			memcpy(ptr+n, uchPan+1, uchPan[0]);
			n += uchPan[0];
			
			ptr[n++] = 0x5f;
			ptr[n++] = 0x34;
			ptr[n++] = 0x01;
			ptr[n++] = uchPanNum[1];
//			memcpy(ptr+n, uchPanNum+1, 1);
			n++;

			ptr[n++] = 0x57;
			ptr[n++] = uchMstrip2[0];
			memcpy(ptr+n, uchMstrip2+1, uchMstrip2[0]);
			n += uchMstrip2[0];
		}
#endif	
		//AC	应用密文
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x26;
		ptr[n++] = 0x08;
		memcpy(ptr+n, uchAc+1, 8);
		n += 8;
		//CID	密文信息数据		
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x27;
		ptr[n++] = 0x01;
		ptr[n++] = uchCid[1];
		//IAD 发卡行应用数据
		if(uchIad[0]>0)
		{
			ptr[n++] = 0x9f;	 				  
			ptr[n++] = 0x10;
			ptr[n++] = uchIad[0];
			memcpy(ptr+n, &uchIad[1], uchIad[0]);
			n += uchIad[0];
		}
		//不可预知数
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x37;
		ptr[n++] = 0x04;
		memcpy(ptr+n, puchUnpreData, 4);
		n += 4;
		//ATC 应用交易计数器
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x36;
		ptr[n++] = 0x02;
		memcpy(ptr+n, uchAtc+1, 2);					
		n += 2;
		//TVR 终端验证结果
		ptr[n++] = 0x95;  
		ptr[n++] = 0x05;
		memcpy(ptr+n, puchTvr, 5);
		n += 5;
		//交易日期
		ptr[n++] = 0x9a;	  
		ptr[n++] = 0x03;
		memcpy(ptr + n, puchDate, 3);
		n += 3;
		//交易类型
		ptr[n++] = 0x9C;	  
		ptr[n++] = 0x01;
		ptr[n++] = puchTranType[0];
		//授权金额
		ptr[n++] = 0x9f;
		ptr[n++] = 0x02;
		ptr[n++] = 0x06;
		ptr[n++] = puchAmount[0];
		ptr[n++] = puchAmount[1];
		ptr[n++] = puchAmount[2];
		ptr[n++] = puchAmount[3];
		ptr[n++] = puchAmount[4];
		ptr[n++] = puchAmount[5];
		//交易货币代码
		ptr[n++] = 0x5f;	  
		ptr[n++] = 0x2a;
		ptr[n++] = 0x02;
		memcpy(ptr+n, puchCurrencyCode, 2);					
		n += 2;
		//AIP 应用交互特征
		ptr[n++] = 0x82;	  
		ptr[n++] = 2;
		memcpy(ptr+n, uchAip+1, 2);
		n += 2;
		//终端国家代码
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x1a;
		ptr[n++] = 0x02;
		memcpy(ptr+n, puchTermCountryCode, 2);					
		n += 2;
		//其它金额
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x03;
		ptr[n++] = 0x06;
		memcpy(ptr+n, puchAmountOther, 6);					
		n += 6;
		//终端性能
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x33;
		ptr[n++] = 0x03;
		memcpy(ptr+n, puchTermCapability, 3);					
		n += 3;
		//持卡人验证结果
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x34;
		ptr[n++] = 3;
		memcpy(ptr + n, puchCvmRes, 3);
		n += 3;
		//终端类型
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x35;
		ptr[n++] = 0x01;
		ptr[n++] = puchTermType[0];
		//IFD 接口设备序列号
		ptr[n++] = 0x9f;	  
		ptr[n++] = 0x1e;
		ptr[n++] = 8;
		memcpy(ptr+n, puchIFDSerailNO, 8);
		n += 8;
		//DF AID 专用文件名称
		ptr[n++] = 0x84;	  
		ptr[n++] = uchAid[0];
		memcpy(ptr+n, &uchAid[1], uchAid[0]);
		n += uchAid[0];
		//应用版本号  
		ptr[n++] = 0x9f;	 
		ptr[n++] = 0x09;
		ptr[n++] = 2;
		memcpy(ptr + n, puchVer, 2);
		n += 2;
		//交易序列计数器
		ptr[n++] = 0x9f;
		ptr[n++] = 0x41;
		ptr[n++] = 4;
		MID_NVMEM_Read(TRANS_SEQ_COUNTER, 4, puchTSC);
		memcpy(ptr + n, puchTSC, 4);
		n +=4;
		//PAN序列号
		ptr[n++] = 0x5f;	  
		ptr[n++] = 0x34;
		ptr[n++] = 0x01;
		ptr[n++] = uchPanNum[1];

		//卡片失效日期
		ptr[n++] = 0x5f;	  
		ptr[n++] = 0x24;
		ptr[n++] = 0x04;
		memcpy(ptr + n, uchExpirationDate, 4);
		n += 4;

		//二磁等效数据
		ptr[n++] = 0x57;	  
		ptr[n++] = uchMstrip2[0];
		memcpy(ptr+n, uchMstrip2 + 1, uchMstrip2[0]);
		n += uchMstrip2[0];
		
		//终端id
	   	ptr[n++] = 0x9f;	 
		ptr[n++] = 0x06;
		ptr[n++] = 16;
		memcpy(ptr + n, puchTerminalAID, 16);
		n += 16;

//	   	//???
//	   	ptr[n++] = 0x9f;	 
//		ptr[n++] = 0x63;
//		ptr[n++] = 16;
//		memcpy(ptr + n, "03100000", 8);
//		n += 16;

//		puchCvmRes[0] = 0x1F;
//		puchCvmRes[1] = 0x00;
//		puchCvmRes[2] = 0x02;	

//	   	ptr[n++] = 0x9f;	  //////////////////////////////////持卡人验证结果 改为上传CVM（持卡人验证方法）
//		ptr[n++] = 0x34;
//		ptr[n++] = uchCvm[0];
//		memcpy(ptr+n,uchCvm + 1,uchCvm[0]);
//		n +=uchCvm[0];

//		ptr[n++] = 0x8e;	  //////////////////////////////////CVM
//		ptr[n++] = uchCvm[0];
//		memcpy(ptr+n,&uchCvm[1],uchCvm[0]);
//		n +=uchCvm[0];
	  
//		ptr[n++] = 0x9f;	 
//		ptr[n++] = 0x10;
//		if((G_B_GACResult & 0x0f) == 0x01) //格式1
//		{
//			ptr[n++] = *(g_APDU.pOutData+1)-11;
//			memcpy(ptr+n, g_APDU.pOutData+13, *(g_APDU.pOutData+1)-11);
//			n += *(g_APDU.pOutData+1)-11;
//		}
//		else 								//格式2
//		{
//			ptr[n++] = *(g_APDU.pOutData+24);
//			memcpy(ptr+n, g_APDU.pOutData+25, *(g_APDU.pOutData+24));
//			n += *(g_APDU.pOutData+24);
//		}
	
		if ((1 == revFlag)||(2 == revFlag))					//授权金额
		{		
//			ptr[n++] = (((money1/1000)<<4)&0xf0) + (((money1%1000)/100)&0x0f);
//			ptr[n++]  = ((((money1%100)/10)<<4)&0xf0) + ((money1%10)&0x0f);
//			ptr[n++] = (((money2/10)<<4)&0xf0) + ((money2%10)&0x0f);
		}
//			ptr[n++] = 0x9f;	 				  
//			ptr[n++] = 0x42;
//			ptr[n++] = uchAppCurrencyCode[0];
//			memcpy(ptr+n, &uchAppCurrencyCode[1], uchAppCurrencyCode[0]);
//			n += uchAppCurrencyCode[0];	
		g_APDU.RLE = n;
	}	
	return 	SW_OPERATION_SUCCESS;
}

//联机处理
unsigned short EMV_OnlineProcessing(unsigned char revFlag)
{
	unsigned char i, j, scriptflag,temp;
	unsigned char res[128];
	unsigned char ScriptRes[100];
 	unsigned char *pIAD = NULL;
	unsigned char *pARC = NULL;
	unsigned char *pIS  = NULL;
	unsigned char *ptr  = NULL;
	unsigned char *pTag = NULL;
	unsigned char *pTag9F18 = NULL;
	unsigned char *pTag9F20 = NULL;
	unsigned char *pTag72 = NULL;
	unsigned char Tag72_Len = 0;
	unsigned char *pTag71 = NULL;
	unsigned char Tag71_Len = 0;
	unsigned int  Tag77_Len = 0;
	unsigned int  Tag80_Len = 0;
	unsigned int  tempLen = 0;
	unsigned int  nLen;
	unsigned char Rest_Len = 0;
	unsigned char IS71_Len = 0;
	unsigned char uchReq = 0;
	unsigned char command_num = 0;
	unsigned char script_len = 0;
//	unsigned char message_type = 0;
	
 	if(0 == *(g_APDU.pInData))	//正常联机
	{
		if (0 == revFlag)	//联机余额查询
		{
			pIAD = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_0091_ISSUER_AUTHENTICATION_DATA);
		    pARC = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_008A_AUTH_RESPONSE_CODE);
		}

		if((1 == revFlag)||(2 == revFlag ) ) //消费、圈存
		{	
			pIAD = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_0091_ISSUER_AUTHENTICATION_DATA);
		    pARC = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_008A_AUTH_RESPONSE_CODE);
		}
		
		if (3 == revFlag)	//联机补登余额查询
		{
			pIAD = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_0091_ISSUER_AUTHENTICATION_DATA);
		    pARC = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_008A_AUTH_RESPONSE_CODE);
		}
			
		if(NULL != pIAD)
		{	
			memcpy(T_APDU.pInData, pIAD+2, *(pIAD+1));	
		    memcpy(puchIAD,pIAD+2, *(pIAD+1));
			*(puchIAD-1) = *(pIAD+1);
		}

		if((NULL == pARC)||(*(pARC+1) != 2))
		{
			uchReq = AAC_REQ;	//AAC
			puchArc[0] = 0x5A;
			puchArc[1] = 0x33; 
		}
		else
		{
			memcpy(puchArc,pARC+2,2);
		}

		if ((0x35 == puchArc[0]) && (0x35 == puchArc[1]))			//PIN错误
		{
		 	if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
			{
				//冲正标志位清零
				temp = 0x30;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
			}
			*g_APDU.pOutData = 0x01;	   //交易拒绝
//			*g_APDU.pOutData = 0x11;	   //交易拒绝
			g_APDU.RLE= 1;	 
//			MID_DISP_SmallPic(SMALLPIC_MIMACUOWU);		
//			return SW_OPERATION_SUCCESS;
			return SW_UNKOWN_ERROR;	
		}
		else if ((0x5A == puchArc[0]) && (0x33 == puchArc[1]))		//超时
		{
#if 0		
			if((1 == revFlag) ||(2 == revFlag)||(3 == revFlag))
			{
				//冲正信息保存 
				temp = 0x31;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);		
			}
#endif			
			//冲正标志位清零 
			if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
			{
				temp = 0x30;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);
			}
				
			*g_APDU.pOutData = 0x01;	   //交易拒绝
//			*g_APDU.pOutData = 0x12;	   //交易拒绝
			g_APDU.RLE= 1;	 
//			MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);		
//			return SW_OPERATION_SUCCESS;
			return SW_UNKOWN_ERROR;	
		}
		else if ((0x30 != puchArc[0]) && (0x30 != puchArc[1]))		//错误
		{
			if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
			{
				//冲正标志位清零
				temp = 0x30;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);		
			}
			*g_APDU.pOutData = 0x01;	   //交易拒绝
//			*g_APDU.pOutData = 0x13;	   //交易拒绝
			g_APDU.RLE= 1;	 
//			MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);		
			return SW_OPERATION_SUCCESS;
		}
		
		if((0x04 == (uchAip[1] & 0x04))&&(NULL != pIAD))	//卡片支持发卡行认证
		{
			 //外部认证
		  	T_APDU.CLA = 0x00;//*(T_APDU.pCommBuff+0) = 0x00;
			T_APDU.INS = 0x82;//*(T_APDU.pCommBuff+1)= 0x82;
			T_APDU.PP1 = 0x00;//*(T_APDU.pCommBuff+2)= 0x00;
			T_APDU.PP2 = 0x00;//*(T_APDU.pCommBuff+3)= 0x00;
			T_APDU.PP3 = *(pIAD+1);//*(T_APDU.pCommBuff+4) = *(pIAD+1);
			T_APDU.LC  = *(pIAD+1);
			T_APDU.LE  = 0x00;
			T_APDU.RLE = 0x00;
			memcpy(T_APDU.pInData, pIAD+2, *(pIAD+1));
//			AU9541_ICCRAD_TransCeive(&T_APDU);
			HAL_Card_APDU(&T_APDU,5+T_APDU.LC);
			
			if((0x90 == T_APDU.SW1) && (0x00 == T_APDU.SW2))  	
			{  
				uchReq = TC_REQ;	//TC
			}
			else if ((0x69 == T_APDU.SW1) && (0x85 == T_APDU.SW2))
			{
				uchReq = TC_REQ;	//TC
				*(puchTvr+4) |= 0x40;	//发卡行认证不成功 
			}
			else
			{ 
				*(puchTvr+4) |= 0x40;	//发卡行认证不成功 
				uchReq = AAC_REQ;	//AAC
			}
			*puchTsi |= 0x10;	//发卡行认证已执行
		}
		
		/* **************************************************
		 * Offline approved 						Y1
		 * Offline declined 						Z1
		 * Unable to go online, offline approved 	Y3
		 * Unable to go online, offline declined 	Z3
		 ****************************************************/
		if(0 == memcmp(puchArc,"00",2))
		{
			uchReq = TC_REQ;	//TC
		}
		else 
		{	
			uchReq = AAC_REQ;	//AAC
			if((0 != memcmp(puchArc,"51",2))&&(0 != memcmp(puchArc,"05",2)))
				memcpy(puchArc, "Z3", 2);
		}
	}
	else  	//无法联机
	{	
		for(i=0;i<5;i++)  //检查是否脱机拒绝
		{
			if(0 == *(puchTvr+i))
			{
				continue;
			}
			else
			{
				for(j=0;j<8;j++)
				{
					if((((uchTacDefault[i+1]>>j)&0x01) || ((uchTacDefault[i]>>j)&0x01)) && ((*(puchTvr+i)>>j)&0x01))
					{
						memcpy(puchArc, "Z3", 2);
						uchReq = AAC_REQ;	//AAC
						break;
					}
				}
				if(8 != j)
				{
					break;
				}
			}
		}

		if((5 == i) && (8 == j))	   
		{
			uchReq = TC_REQ;	//TC
		}
	}

	//冲正标志位清零
	if ((TC_REQ == uchReq) && (1 == revFlag))     	//消费
	{
		if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
		{
			temp = 0x30;
			MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
		}
	}
		
	memset(ScriptRes,0x00,100);
//Tag71_Processing:
    if(0 == revFlag)   //联机余额查询
	{
		pTag72 = g_APDU.pInData+7;
//		pTag72 = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_0072_ISSUER_SCRIPT_TEMPLATE_2);
	    Tag71_Len = Tag72_Len = Rest_Len = g_APDU.PP3 - 7;
	}
	else			   //消费，圈存
	{
		pTag72 = g_APDU.pInData+1;
//		pTag72 = SearchTag(g_APDU.pInData+7, g_APDU.PP3-7, EMV_TAG_0072_ISSUER_SCRIPT_TEMPLATE_2);
	    Tag71_Len = Tag72_Len = Rest_Len = g_APDU.PP3 - 1;
	}	

	scriptflag = 0;	//缺省:无脚本，设置脚本执行结果数据对象长度为0	
	script_len = IS71_Len = 0;

	while(Rest_Len>0)
	{
		pTag71 = g_APDU.pInData + g_APDU.PP3 - Rest_Len;		
		pIS =  SearchTagforScript(pTag71, Rest_Len, EMV_TAG_0071_ISSUER_SCRIPT_TEMPLATE_1);	 //查找发卡行脚本
		if(NULL == pIS)
		{
			break;	
		}
		else
		{
			pTag71 = pIS;
			Tag71_Len = *(pTag71+1);	
			script_len += Tag71_Len+2;
		
		    Rest_Len = g_APDU.PP3-(pTag71-g_APDU.pInData)-Tag71_Len-2;
			IS71_Len += Tag71_Len+2;				

			*puchTsi |= 0x04;	//设置TSI，脚本已处理	
			scriptflag += 5;
			pTag9F18 =  SearchTagforScript(pTag71, Tag71_Len+2, EMV_TAG_9F18_ISSUER_SCRIPT_IDENTIFIER);	 //查找Issuer Script Identifier
			if(NULL != pTag9F18)
			{
				if(*(pTag9F18+2) != 4)
				{
					memset(ScriptRes+scriptflag-4, 0x00, 4);	
					i = Tag71_Len-3-*(pTag9F18+2);	//获取脚本实际长度 	
					pTag71 += 3+*(pTag9F18+2); 
				}
				else
				{
					memcpy(ScriptRes+scriptflag-4, pTag9F18+3, 4);
					i = Tag71_Len-7;	//获取脚本实际长度 excluded：'9F18' Tag(2Bytes) Len(1Byte) ScriptID(4Bytes) 
					pTag71 += 9; 	
				}
			}
			else 
			{
				pTag9F20 =  SearchTagforScript(pTag71, Tag71_Len+2, 0x9F20);	 
				if(NULL != pTag9F20)
				{
					memset(ScriptRes+scriptflag-4, 0, 4);	//脚本标识固定设置为0	
					ScriptRes[scriptflag-5] = 0x00; 	
					*(puchTvr+4) |= 0x20; //设置71脚本处理失败标识
 				  continue;
				}
				else 
				{ 
					memset(ScriptRes+scriptflag-4, 0, 4);	//脚本标识固定设置为0					
					ScriptRes[scriptflag-5] = 0x00; 	
					i = Tag71_Len;
				}
			}
			
			if(script_len > 128)
			{
				ScriptRes[scriptflag-5] = 0x00; 				
				*(puchTvr+4) |= 0x20; //设置71脚本处理失败标?
				break;
			}

			pIS = pTag71;
			command_num = 0;
			while(i > 0)
			{	
				pIS =  SearchTagforScript(pIS, i, EMV_TAG_0086_ISSUER_SCRIPT_COMMAND);	 //查找Issuer Script Command Tag '86'
				if(NULL != pIS)
				{
					command_num++;
					if((*(pIS+1)<5) || (*(pIS+1)>i-2) || (*(pIS+1) != *(pIS+6)+5))
					{
						*(puchTvr+4) |= 0x20; //设置71脚本处理失败标识
						ScriptRes[scriptflag-5] = 0x00; 
						break;
					}
				
					i -= (*(pIS+1)+2);
					if((i>0) && (i<7))
					{
						*(puchTvr+4) |= 0x20; //设置71脚本处理失败标识
						ScriptRes[scriptflag-5] = 0x00; 
						break;
					}

					ScriptRes[scriptflag-5] = ScriptRes[scriptflag-5]&0x0F + 0x20;

				  	T_APDU.CLA = *(pIS+2);//*(T_APDU.pCommBuff+0) = *(pIS+2);
					T_APDU.INS = *(pIS+3);//*(T_APDU.pCommBuff+0) = *(pIS+3);
					T_APDU.PP1 = *(pIS+4);//*(T_APDU.pCommBuff+0) = *(pIS+4);
					T_APDU.PP2 = *(pIS+5);//*(T_APDU.pCommBuff+0) = *(pIS+5);
					T_APDU.PP3 = *(pIS+6);//*(T_APDU.pCommBuff+0) = *(pIS+6);
		
					if(*(pIS+1) > 5)
					{
						T_APDU.LC  = T_APDU.PP3;
						T_APDU.LE  = 0x00;
						T_APDU.RLE = 0x00;	
						memcpy(T_APDU.pInData, pIS+7, *(pIS+1)-5);
					}
					else
					{
						T_APDU.LC  = 0;
						T_APDU.LE  = T_APDU.PP3;
						T_APDU.RLE = 0x00;	
					}
//				    AU9541_ICCRAD_TransCeive(&T_APDU);
					HAL_Card_APDU(&T_APDU,5+T_APDU.LC);
				
					if((0x90 == T_APDU.SW1) || (0x62 == T_APDU.SW1) || (0x63 == T_APDU.SW1))
					{
						pIS += *(pIS+1)+2;
						ScriptRes[scriptflag-5] = 0x20;
					}
					else
					{	
						pIS += *(pIS+1)+2;
						*(puchTvr+4) |= 0x20; //设置71脚本处理失败标识
				
						if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
						{
							//冲正标志位置位
							temp = 0x31;
							MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
						}

						if ( 0x0f != (ScriptRes[scriptflag-5] & 0x0f))	  
						{
							ScriptRes[scriptflag-5] = command_num; 
						}
						ScriptRes[scriptflag-5] += 0x10; 
						break;
					}
				}
				else
				{
					*(puchTvr+4) |= 0x20; //设置71脚本处理失败标识
					ScriptRes[scriptflag-5] = 0x00; 
					break;
				}
			}	
		}
 	}
	//第二次Generate AC
    T_APDU.CLA = 0x80;
	T_APDU.INS = 0xae;
	T_APDU.PP1 = uchReq;
	T_APDU.PP2 = 0x00;
	if(uchCdol2[0] < 0x80)
	{			
		T_APDU.PP3 = CollectDol(uchCdol2+1, uchCdol2[0], T_APDU.pInData,CDOL2_FLAG);
	}
	else if(uchCdol2[0] == 0x81)
	{
		T_APDU.PP3 = CollectDol(uchCdol2+2, uchCdol2[1], T_APDU.pInData,CDOL2_FLAG);
	}
	else
	{}
			
	T_APDU.LC  = T_APDU.PP3;
	T_APDU.LE  = 0x00;
	T_APDU.RLE = 0x00;
//	AU9541_ICCRAD_TransCeive(&T_APDU);
	HAL_Card_APDU(&T_APDU,5+T_APDU.LC);
	
	if((T_APDU.SW1 != 0x90) || (T_APDU.SW2 != 0x00))	//交易终止
	{			
		Nok_Reason = IC_ERROR;
		return NOK;
	}
	else
	{
		*puchTsi |= 0x20;	//卡片风险管理已执行

		/* Bit2~Bit0 of CID
		 *  	x x x Reason/advice code
		 *  	0 0 0 No information given
		 *  	0 0 1 Service not allowed
		 *  	0 1 0 PIN Try Limit exceeded
		 *  	0 1 1 Issuer authentication failed
		 *  	1 x x Other values RFU
		*/
		
		if( EMV_TAG_0080_RESPONSE_MESSAGE_TEMPLATE_FORMAT_1 == *T_APDU.pOutData)  //格式1
		{
			if(*(T_APDU.pOutData+1) < 0x80)
			{				
				Tag80_Len = *(T_APDU.pOutData+1);
				if((Tag80_Len > 43) || (Tag80_Len < 11) || (Tag80_Len != T_APDU.RLE-2))   //Miminum and Maximum Lenth for Format 1
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				G_B_GACResult = 0x01;
				if((ARQC_REQ || TC_REQ) == (*(T_APDU.pOutData+2) & 0xC0))
				{
					if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
					{		
						//冲正标志位置位
						temp = 0x31;
						MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
					}
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				else if(ARQC_REQ == (*(T_APDU.pOutData+2) & 0xC0))				//ARQC
				{
					if((uchReq == AAC_REQ) || (uchReq == TC_REQ))
					{
						G_B_GACResult = 0x01;
						*g_APDU.pOutData = 0x01;
//						 *g_APDU.pOutData = 0x01;	  
						if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
						{		
							//冲正标志位置位
							temp = 0x31;
							MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
						}
					} 
				}
				else if(TC_REQ == (*(T_APDU.pOutData+2) & 0xC0))				//TC
				{
					if(uchReq == AAC_REQ)
					{
						G_B_GACResult = 0x01;
//						*g_APDU.pOutData = 0x01;
						*g_APDU.pOutData = 0x01;	 
					} 
					else
					{
						G_B_GACResult |= 0x10;//交易批准
					}
				}
				else if(AAC_REQ == (*(T_APDU.pOutData+2) & 0xC0))			 	//AAC
				{
//					*g_APDU.pOutData = 0x01;
					*g_APDU.pOutData = 0x01;	 
					if(0 == memcmp(puchArc,"00",2))
					{
	//					message_type = REVERSAL_MSG;
					}
				}
				else
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				if((NOT_ALLOWED == (*(T_APDU.pOutData+2) & 0x0B)) || ( NOT_ALLOWED_ADVICE== (*(T_APDU.pOutData+2) & 0x0B)))
				{		
					*g_APDU.pOutData = 0x01;
//					*g_APDU.pOutData = 0x17;
					g_APDU.RLE = 0x01;
//					MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
					
					if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
					{		
						//冲正标志位置位
						temp = 0x31;
						MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
					}

//					return SW_OPERATION_SUCCESS;
					return SW_UNKOWN_ERROR;	
				}
			}
			else if(*(T_APDU.pOutData+1) == 0x81)
			{
				Tag80_Len = *(T_APDU.pOutData+2);
				if((Tag80_Len > 43) || (Tag80_Len < 11) || (Tag80_Len != T_APDU.RLE-3))   //Miminum and Maximum Lenth for Format 1
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				G_B_GACResult = 0x01;
				if((ARQC_REQ || TC_REQ) == (*(T_APDU.pOutData+3) & 0xC0))
				{
					if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
					{		
						//冲正标志位置位
						temp = 0x31;
						MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
					}

					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				else if(ARQC_REQ == (*(T_APDU.pOutData+3) & 0xC0))				//ARQC
				{
					if((uchReq == AAC_REQ) || (uchReq == TC_REQ))
					{
						if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
						{		
							//冲正标志位置位
							temp = 0x31;
							MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
						}

						G_B_GACResult = 0x01;
//						*g_APDU.pOutData = 0x01;
						*g_APDU.pOutData = 0x01;	
					} 
				}
				else if(TC_REQ == (*(T_APDU.pOutData+3) & 0xC0))				//TC
				{
					if(uchReq == AAC_REQ)
					{
						G_B_GACResult = 0x01;
//						*g_APDU.pOutData = 0x01;
						*g_APDU.pOutData = 0x01;	
					} 
					else
					{
						G_B_GACResult |= 0x10;//交易批准
					}
				}
				else if(AAC_REQ == (*(T_APDU.pOutData+3) & 0xC0))			 	//AAC
				{
					*g_APDU.pOutData = 0x01;
//					*g_APDU.pOutData = 0x1a;	   
					if(0 == memcmp(puchArc,"00",2))
					{
//						message_type = REVERSAL_MSG;
					}
				}
				else
				{
					Nok_Reason = DATA_ERROR;
					return NOK;
				}
				
				if((NOT_ALLOWED == (*(T_APDU.pOutData+3) & 0x0B)) || ( NOT_ALLOWED_ADVICE== (*(T_APDU.pOutData+3) & 0x0B)))
				{		
					*g_APDU.pOutData = 0x01;
//					*g_APDU.pOutData = 0x1b;	   
					g_APDU.RLE = 0x01;
					
					if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
					{		
						//冲正标志位置位
						temp = 0x31;
						MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
					}
//					MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
//					return SW_OPERATION_SUCCESS;
					return SW_UNKOWN_ERROR;	
				}					
			}
			else
			{}		
		}
		else if (*T_APDU.pOutData == EMV_TAG_0077_RESPONSE_MESSAGE_TEMPLATE_FORMAT_2)			//格式2
		{
			G_B_GACResult = 0x02;

			if(*(T_APDU.pOutData+1) < 0x80)
			{
				Tag77_Len = *(T_APDU.pOutData+1); 
				pTag = 	T_APDU.pOutData+2;
			}
			else if(*(T_APDU.pOutData+1) == 0x81)
			{
				Tag77_Len = *(T_APDU.pOutData+2); 
				pTag = 	T_APDU.pOutData+3;
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			
			nLen = Tag77_Len;

			i=0;
			while((Tag77_Len>0)&&((pTag[i] == 0x00)||(pTag[i] == 0xFF)))
			{
				Tag77_Len--;
				i++;
			}
			
			ptr = SearchTag(pTag, Tag77_Len, 0x54);	//Tag 54
			if((NULL != ptr))
			{
				Tag77_Len -= 2+*(ptr+1); 
			}

			ptr = SearchTagforScript(pTag, Tag77_Len, EMV_TAG_9F27_CRYPTOGRAM_INFO_DATA);	//查找CID数据对象(M)
			if((NULL == ptr)||(ptr[2] != 1))
			{
				if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
				{		
					//冲正标志位置位
					temp = 0x31;
					MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
				}

				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else if((ARQC_REQ || TC_REQ)  == (ptr[3] & 0xC0))
			{
				if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
				{		
					//冲正标志位置位
					temp = 0x31;
					MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
				}

				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else if(TC_REQ == (ptr[3] & 0xC0))
			{
				if(uchReq == AAC_REQ)
				{
					G_B_GACResult = 0x01;
//					*g_APDU.pOutData = 0x01;
					*g_APDU.pOutData = 0x01;	 
				} 
				else
				{
					G_B_GACResult |= 0x10;//交易批准
				}

				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			else if(ARQC_REQ == (ptr[3] & 0xC0))
			{
				if((uchReq == AAC_REQ) || (uchReq == TC_REQ))
				{
					if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
					{		
						//冲正标志位置位
						temp = 0x31;
						MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
					}

					G_B_GACResult = 0x01;
//					*g_APDU.pOutData = 0x01;
					*g_APDU.pOutData = 0x01;	  
				} 

				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			else if(AAC_REQ == (ptr[3] & 0xC0))
			{
//				*g_APDU.pOutData = 0x01;
				*g_APDU.pOutData = 0x01;	  
				if((0 == memcmp(puchArc,"00",2))&&(uchReq == TC_REQ))
				{
//					message_type = REVERSAL_MSG;
				}
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			else
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}

			if((NOT_ALLOWED == (ptr[3] & 0x0B)) || ( NOT_ALLOWED_ADVICE== (ptr[3] & 0x0B)))
			{		
				*g_APDU.pOutData = 0x01;
//				*g_APDU.pOutData = 0x1f;	  
				g_APDU.RLE = 0x01;

				if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
				{		
					//冲正标志位置位
					temp = 0x31;
					MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
				}

//				MID_DISP_SmallPic(SMALLPIC_BUJIESHOU);
//				return SW_OPERATION_SUCCESS;
				return SW_UNKOWN_ERROR;	
			}

			ptr = SearchTagforScript(pTag, nLen, EMV_TAG_9F36_ATC);	//查找ATC数据对象(M)
			if((NULL == ptr)||(ptr[2] != 2))
			{
				if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
				{		
					//冲正标志位置位
					temp = 0x31;
					MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
				}

				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else
			{
				uchAtc[0] = 2;
				memcpy(uchAtc+1,ptr+3,2);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			ptr = SearchTagforScript(pTag, nLen, EMV_TAG_9F26_APPLICATION_CRYPTOGRAM);	//查找AC数据对象(M)
			if((NULL == ptr)||(ptr[2] != 8))
			{
				if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
				{		
					//冲正标志位置位
					temp = 0x31;
					MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
				}

				Nok_Reason = DATA_ERROR;
				return NOK;
			}
			else
			{
				uchAc[0] = 8;
				memcpy(uchAc+1,ptr+3,8);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}
			ptr = SearchTagforScript(pTag, nLen, EMV_TAG_9F10_ISSUER_APPLICATION_DATA);	//查找发卡行自定义数据对象(O)
			if((NULL != ptr) && (ptr[2] <= 32))
			{
				uchIad[0] = ptr[2];
				memcpy(uchIad+1,ptr+3,ptr[2]);
				Tag77_Len -= 3+ptr[2];
				i=0;
				tempLen = ptr[2];
				while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
				{
					Tag77_Len--;
					i++;
				}
			}		
			ptr = SearchTag(pTag, nLen, EMV_TAG_DF24_ISSUER_PROPRIETARY_DATA);	//查找发卡行自定义数据对象(O)
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
					Tag77_Len -= 3+*(ptr+2);
					i=0;
					tempLen = ptr[2];
					while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
					{
							Tag77_Len--;
							i++;
					}
				}
				else if(*(ptr+2)==0x81)
				{
					Tag77_Len -= 4+*(ptr+3);
					i=0;
					tempLen = ptr[3];
					while((Tag77_Len>0)&&((ptr[4+tempLen+i] == 0x00)||(ptr[4+tempLen+i] == 0xFF)))
					{
							Tag77_Len--;
							i++;
					}
				}
				else
				{}
			}
			
			ptr = SearchTag(pTag, nLen, EMV_TAG_DF06_ISSUER_PROPRIETARY_DATA);	//查找发卡行自定义数据对象(O)
			if(NULL != ptr)
			{
				if(*(ptr+2)<0x80)
				{
					Tag77_Len -= 3+*(ptr+2);
					i=0;
					tempLen = ptr[2];
					while((Tag77_Len>0)&&((ptr[3+tempLen+i] == 0x00)||(ptr[3+tempLen+i] == 0xFF)))
					{
							Tag77_Len--;
							i++;
					}
				}
				else if(*(ptr+2)==0x81)
				{
					Tag77_Len -= 4+*(ptr+3);
					i=0;
					tempLen = ptr[3];
					while((Tag77_Len>0)&&((ptr[4+tempLen+i] == 0x00)||(ptr[4+tempLen+i] == 0xFF)))
					{
							Tag77_Len--;
							i++;
					}
				}
				else
				{}
			}
			
			if(Tag77_Len>0)
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}
		}
	}
		
	memcpy(res, T_APDU.pOutData, T_APDU.RLE);
	j = T_APDU.RLE;

	if(0 == revFlag)   //联机余额查询
	{
		pTag72 = g_APDU.pInData+7;
	    Tag72_Len = Rest_Len = g_APDU.PP3-7;
	}
	else			   //消费，圈存
	{
		pTag72 = g_APDU.pInData+1;
	    Tag72_Len = Rest_Len = g_APDU.PP3-1;
	}	

	command_num = 0;

	while(Rest_Len>0)
	{
		pTag72 = g_APDU.pInData+g_APDU.PP3-Rest_Len;
		pIS =  SearchTagforScript(pTag72, Rest_Len, EMV_TAG_0072_ISSUER_SCRIPT_TEMPLATE_2);	 //查找发卡行脚本
		if(NULL == pIS)
		{

			//TODO: 圈存如果没找到72脚本，出错处理
			if(revFlag == 2)
			{
				*g_APDU.pOutData = 0x01;   //脱机拒绝	
//				*g_APDU.pOutData = 0x21;	   	
				g_APDU.RLE =1;
//				MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);
//				return SW_OPERATION_SUCCESS;
				return SW_UNKOWN_ERROR;	
			}

			break;	
		}
		else
		{
			pTag72 = pIS;
			Tag72_Len = *(pTag72+1);
			Rest_Len = g_APDU.PP3-(pTag72-g_APDU.pInData)-Tag72_Len-2;	    
			script_len += Tag72_Len+2;
			
			*puchTsi |= 0x04;	//设置TSI，脚本已处理
	
			scriptflag += 5;	//如果有脚本，设置脚本执行结果数据对象长度为5 
			pTag9F18 =  SearchTagforScript(pTag72, Tag72_Len+2, EMV_TAG_9F18_ISSUER_SCRIPT_IDENTIFIER);	 //查找Issuer Script Identifier
			if(NULL != pTag9F18)
			{
				if(*(pTag9F18+2) != 4)
				{	
					memset(ScriptRes+scriptflag-4, 0x00, 4);
					i = Tag72_Len-3-*(pTag9F18+2);	//获取脚本实际长度 excluded：'9F18' Tag(2Bytes) Len(1Byte) ScriptID(4Bytes) 
					pTag72 += 3+*(pTag9F18+2); 
				}
				else
				{
					memcpy(ScriptRes+scriptflag-4, pTag9F18+3, 4);
					i = Tag72_Len-7;	//获取脚本实际长度 excluded：'9F18' Tag(2Bytes) Len(1Byte) ScriptID(4Bytes) 
					pTag72 += 9; 	
				}	
			}
			else 
			{
				pTag9F20 =  SearchTagforScript(pTag72, Tag72_Len+2, 0x9F20);	 
				if(NULL != pTag9F20)
				{
					memset(ScriptRes+scriptflag-4, 0, 4);	//脚本标识固定设置为0
					ScriptRes[scriptflag-5] = 0x00; 	
					*(puchTvr+4) |= 0x10; //设置72脚本处理失败标识
 				  continue;
				}
				else
				{ 					
					memset(ScriptRes+scriptflag-4, 0, 4);	//脚本标识固定设置为0
					ScriptRes[scriptflag-5] = 0x00; 			
					i = Tag72_Len;
				}
			}
			
			if(script_len > 128)
			{
				ScriptRes[scriptflag-5] = 0x00; 				
				*(puchTvr+4) |= 0x10; //设置72脚本处理失败标?
				break;
			}			

			pIS = pTag72;
			command_num = 0;
			while(i > 0)
			{		
				pIS =  SearchTagforScript(pIS, i, EMV_TAG_0086_ISSUER_SCRIPT_COMMAND);	 //查找Issuer Script Command Tag '86'
				
				if(NULL != pIS)
				{
					command_num++;	
#if 0					
					if((*(pIS+1)<5) || (*(pIS+1)>i-2)|| (*(pIS+1) != *(pIS+6)+5))
					{
						*(puchTvr+4) |= 0x10; //设置72脚本处理失败标识
						ScriptRes[scriptflag-5] = 0x00; 
						break;
					}
#endif
					i -= (*(pIS+1)+2);
					if((i>0) && (i<7))
					{
						*(puchTvr+4) |= 0x10; //设置72脚本处理失败标识
						ScriptRes[scriptflag-5] = 0x00; 
						break;
					}

					ScriptRes[scriptflag-5] = ScriptRes[scriptflag-5]&0x0F + 0x20;
				
				    T_APDU.CLA = *(pIS+2);//*(T_APDU.pCommBuff+0) = *(pIS+2);
					T_APDU.INS = *(pIS+3);//*(T_APDU.pCommBuff+1) = *(pIS+3);
					T_APDU.PP1 = *(pIS+4);//*(T_APDU.pCommBuff+2) = *(pIS+4);
					T_APDU.PP2 = *(pIS+5);//*(T_APDU.pCommBuff+3) = *(pIS+5);
					T_APDU.PP3 = *(pIS+6);//*(T_APDU.pCommBuff+4) = *(pIS+6);
		
					if(*(pIS+1) > 5)
					{
						T_APDU.LC  = T_APDU.PP3;
						T_APDU.LE  = 0x00;
						T_APDU.RLE = 0x00;	
						memcpy(T_APDU.pInData, pIS+7, *(pIS+1)-5);
					}
					else
					{
						T_APDU.LC  = 0;
						T_APDU.LE  = T_APDU.PP3;
						T_APDU.RLE = 0x00;	
					}
//					AU9541_ICCRAD_TransCeive(&T_APDU);
					HAL_Card_APDU(&T_APDU,5+T_APDU.LC);
					
					if((0x90 == T_APDU.SW1) || (0x62 == T_APDU.SW1) || (0x63 == T_APDU.SW1))
					{
						pIS += *(pIS+1)+2;
						ScriptRes[scriptflag-5] = 0x20;
					}
					else
					{
						pIS += *(pIS+1)+2;
						*(puchTvr+4) |= 0x10; //设置72脚本处理失败标识
						
						if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
						{
							//冲正标志位置位
							temp = 0x31;
							MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
						}

						if ( 0x0f != (ScriptRes[scriptflag-5] & 0x0f))	  
						{
							ScriptRes[scriptflag-5] = command_num; 
						}
						ScriptRes[scriptflag-5] += 0x10;  
						break;
					}
				}
				else
				{
					*(puchTvr+4) |= 0x10; //设置72脚本处理失败标识
					ScriptRes[scriptflag-5] = 0x00; 
					break;
				}
			}	
		}
 	}

// Response_Packing:
		T_APDU.RLE = j;
		memcpy(T_APDU.pOutData, res, T_APDU.RLE);
		if((G_B_GACResult & 0xf0)  == 0x00)	
		{ 	
			if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
			{		
				//冲正标志位置位
				temp = 0x31;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);
				
					
				*g_APDU.pOutData = 0x01;
				g_APDU.RLE = 1;


//				MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);
				return 	SW_FUNCTION_NOTSUPPORT;
//				return 	SW_OPERATION_SUCCESS;
			}
//			*g_APDU.pOutData = 0x01;   //脱机拒绝	
			*g_APDU.pOutData = 0x01;	  		
//			MID_DISP_SmallPic(SMALLPIC_JIAOYIJUJUE);
		}
		else
		{
			*g_APDU.pOutData = 0x00;	//交易批准
			//MID_DISP_SmallPic(SMALLPIC_JIAOYIPIZHUN);
			
			if (0 == revFlag)
			{
//				ShowPriBalance(0);			//显示主账户余额
			}
			if( 1 == G_flag_BalanceOrLoad)	 //圈存功能
			{
				//冲正标志位清零
				temp = 0x30;
				MID_NVMEM_Write(REVERSAL_FLAG,1,&temp);	
			}

		}	

		ptr = g_APDU.pOutData+1;

		*ptr = 0x9f;	  //AC
		ptr++;
		*ptr = 0x26;
		ptr++;
		*ptr = 0x08;
		ptr++;
		memcpy(ptr, g_APDU.pOutData+5, 8);
//		memcpy(ptr, uchAc+1, 8);
		ptr += 8;

		*ptr = 0x9f;	  //ATC
		ptr++;
		*ptr = 0x36;
		ptr++;
		*ptr = 0x02;
		ptr++;
		memcpy(ptr, T_APDU.pOutData+3, 2);	
//		memcpy(ptr, uchAtc+1, 2);									
		ptr += 2;

		*ptr = 0x9b;	  //TSI
		ptr++;
		*ptr = 0x02;
		ptr++;
	   	*ptr = *puchTsi;
		ptr++;
		*ptr = 0x00;
		ptr++;

		if(uchAppLabel[0])
		{
			*ptr = 0x50;	  //App Prefered Name
			ptr++;
			*ptr = uchAppLabel[0];
			ptr++;
			memcpy(ptr, uchAppLabel + 1, uchAppLabel[0]);
			ptr += uchAppLabel[0];
		}
		else{}

		if(uchAppPrefName[0])
		{
			*ptr = 0x9f;	  //App Prefered Name
			ptr++;
			*ptr = 0x12;
			ptr++;
			*ptr = uchAppPrefName[0];
			ptr++;
			memcpy(ptr, uchAppPrefName + 1, uchAppPrefName[0]);
			ptr += uchAppPrefName[0];
		}
		else{}

		*ptr = 0x9f;	  //不可预知数
		ptr++;
		*ptr = 0x37;
		ptr++;
	   	*ptr = 0x04;
		ptr++;
		memcpy(ptr, puchUnpreData, 4);
		ptr += 4;

		*ptr = 0x95;  //TVR
		ptr++;
		*ptr = 0x05;
		ptr++;
		memcpy(ptr, puchTvr, 5);
		ptr += 5;
		
		*ptr = 0x9a;	  //交易时间
		ptr++;
		*ptr = 0x03;
		ptr++;
		memcpy(ptr, g_sCardReaderInfo.ucTimestamp + 1, 3);
		ptr += 3; 

		*ptr = 0x9c;	  //交易类型
		ptr++;
		*ptr = 0x01;
		ptr++;
	   	*ptr = *puchTranType;
		ptr++;

		if(scriptflag)
		{
			*ptr = 0xdf;
			ptr++;			
			*ptr = 0x31; 			
			ptr++;
			*ptr = scriptflag;
			ptr++;
			if(scriptflag)
			{
				memcpy(ptr, ScriptRes, scriptflag);
				ptr += scriptflag;
			}
		}
		else{}

		if(*(T_APDU.pOutData + 1) - 11)
		{
			*ptr = 0x9f;	  //发卡行应用数据
			ptr++;
			*ptr = 0x10;
			ptr++;
			*ptr = *(T_APDU.pOutData+1)-11;
	//		*ptr = uchIad[0];
			ptr++;
			memcpy(ptr, T_APDU.pOutData+13, *(T_APDU.pOutData+1)-11);
	//		memcpy(ptr, uchIad+1, uchIad[0]);
			ptr += *(T_APDU.pOutData+1)-11;
	//		ptr += uchIad[0];
		}
		else{}
		 						
		*ptr = 0x9f;	  //CID
		ptr++;
		*ptr = 0x27;
		ptr++;
		*ptr = 0x01;
		ptr++;
		*ptr = *(T_APDU.pOutData+2);
//		*ptr = uchCid[1];
		ptr += 1;

		
		*ptr = 0xdf;	//自定义判断成功的标签
		ptr++;
		*ptr = 0x75;
		ptr++;
	   	*ptr = 0x01;
		ptr++;
		*ptr = 0x01;
		ptr++;
		//DF760400000000
#if 0
		*ptr = 0xdf;	 
		ptr++;
		*ptr = 0x76;
		ptr++;
	   	*ptr = 0x04;
		ptr++;
		memset(ptr, 0x00, 4);
		ptr += 4;
#endif
		g_APDU.RLE = ptr-g_APDU.pOutData;
		
		if(1 == revFlag)	   //消费
		{
//			MID_DISP_SmallPic(SMALLPIC_JIAOYICHENGGONG);
		}
		if(2 == revFlag)	   //圈存
		{
			if ((scriptflag)&&(ScriptRes[0]&0x20))
			{
//				MID_DISP_SmallPic(SMALLPIC_JIAOYICHENGGONG);
				return SW_OPERATION_SUCCESS;
			}
			else if ((scriptflag)&&(ScriptRes[0]&0x10))
			{	
//				MID_DISP_SmallPic(SMALLPIC_JIAOYISHIBAI);
				g_APDU.RLE  = 1;
				*g_APDU.pOutData = 0x02;
//				return SW_OPERATION_SUCCESS;
				return SW_UNKOWN_ERROR;	
			}
			else
			{
//				MID_DISP_SmallPic(SMALLPIC_JIAOYISHIBAI);
				g_APDU.RLE  = 1;
				*g_APDU.pOutData = 0x02;
//				return SW_OPERATION_SUCCESS;
				return SW_UNKOWN_ERROR;				
			}
		}
		return SW_OPERATION_SUCCESS;
}

//选择卡片ADF
unsigned short	EMV_SelectADFforLogEntry(unsigned char* AID,unsigned char AID_len)
{
	unsigned char *ptr = NULL;
	unsigned char *pTag = NULL;
	unsigned char *pIn = NULL;
	unsigned char totalLen = 0;
	unsigned int  nLen  = 0;

	if((NULL == AID) || (AID_len>16) || (AID_len<5))
	{
		Nok_Reason = NONE;
		return NOK;
	}

    g_APDU.CLA = 0x00;//*(g_APDU.pCommBuff+0) = 0x00;
	g_APDU.INS = 0xA4;//*(g_APDU.pCommBuff+1) = 0xA4;
	g_APDU.PP1 = 0x04;//*(g_APDU.pCommBuff+2) = 0x04;
	g_APDU.PP2 = 0x00;	//*(g_APDU.pCommBuff+3) = 0x00;	
	g_APDU.PP3 = AID_len;//*(g_APDU.pCommBuff+4) = AID_len;
	g_APDU.LC = AID_len;
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;
	memcpy(g_APDU.pInData, AID, AID_len);
//	AU9541_ICCRAD_TransCeive(&g_APDU);
//	EMV_ExchangeAPDU(ICC_Protocol,5+AID_len);
	HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);

	if((0x6A == g_APDU.SW1) && (0x81 == g_APDU.SW2))	  //Card is blocked or SELECT command is not supported, terminate the session
	{
		Nok_Reason = CARD_BLOCKED;
		return NOK;
	} 
	else if((0x62 == g_APDU.SW1) && (0x83 == g_APDU.SW2))  //Application is blocked (0x6283)
	{
		Nok_Reason = APPLICATION_BLOCKED;
		return NOK;
	}
	else if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))  
	{
		Nok_Reason = NONE;
		return NOK;
	}
	else   		//9000
	{	
		//保存当前选中的终端AID名称
		*(puchTerminalAID-1) = AID_len;
		memcpy(puchTerminalAID,AID,AID_len);   
		
		//查找FCI模板 (M)
		if(EMV_TAG_006F_FCI_TEMPLATE != g_APDU.pOutData[0])		  
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}

		if(g_APDU.pOutData[1] <0x80)
		{
			pIn = g_APDU.pOutData+2;
			totalLen = g_APDU.pOutData[1];
		}
		else if (0x81 == g_APDU.pOutData[1])
		{
			pIn = g_APDU.pOutData+3;
			totalLen = g_APDU.pOutData[2];
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		//查找DF名 (M)
		ptr =  SearchTag(pIn, totalLen, EMV_TAG_0084_DF_NAME);	 
		if(NULL == ptr)
		{
				Nok_Reason = MISSING_DATA;
// 				*puchTvr |= 0x20;		//设置IC卡数据缺失
				return NOK;
		}
		else
		{
			if((*(ptr+1) != AID_len)||(0 != memcmp(ptr+2,AID,AID_len)))
			{
				Nok_Reason = DATA_ERROR;
				return NOK;
			}			
			uchAid[0] = *(ptr+1);
			memcpy(uchAid+1, ptr+2, uchAid[0]); 	//保存DF名
		}

		//查找FCI数据专用模板 (M)
		ptr =  SearchTag(pIn, totalLen,EMV_TAG_00A5_FCI_PROPRIETARY_TEMPLATE);	
		if(NULL == ptr)
		{
			Nok_Reason = MISSING_DATA;
// 			*puchTvr |= 0x20;		//设置IC卡数据缺失
			return NOK;
		}

		if(*(ptr+1) <0x80)
		{
			nLen = *(ptr+1);
			pTag = ptr+2;
		}
		else if (0x81 == *(ptr+1))
		{
			nLen = *(ptr+2);
			pTag = ptr+3;
		}
		else
		{
			Nok_Reason = DATA_ERROR;
			return NOK;
		}

		//查找发卡行自定义数据Tag (O)
		ptr =  SearchTag(pTag, nLen, EMV_TAG_BF0C_FCI_ISSUER_DISCRETIONARY_DATA);	
		if(NULL != ptr)   //Length = 0~222 Byte
		{
				if(*(ptr+2) < 0x80)
				{
					uchFCI[0] = *(ptr+2);
				  memcpy(uchFCI+1,ptr+3,*(ptr+2)); 
				}
				else if ((*(ptr+2) == 0x81)&&(*(ptr+3)<= 222))
				{
					uchFCI[0] = *(ptr+2);
					uchFCI[1] = *(ptr+3);
				  memcpy(uchFCI+2,ptr+4,*(ptr+3)); 	
				}
				else
				{			
				}					
			
						//查找日志入口数据元
				ptr = SearchTag(ptr+3, *(ptr+2), EMV_TAG_9F4D_ICC_LOG_ENTRY);	
				if((NULL!=ptr) && (2==*(ptr+2)) && (*(ptr+3)>10) && (*(ptr+3)<31))
				{
						uchLogEntry[0] = 2;
						memcpy(&uchLogEntry[1],ptr+3,2);
				}

		}	
		else
		{
				uchFCI[0] = 0;
				memset(uchFCI+1,0x00,222);
		}
	}
		
	return SW_OPERATION_SUCCESS;
}

unsigned int GetCash(APDU *psAPDU)
{	
	unsigned char *ptr = NULL;
//	unsigned char  disp,i,j,nozero,disp_a;

//	读取Tag 0x9f79，电子现金余额		
	g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
	g_APDU.INS = 0xca;//*(g_APDU.pCommBuff+1) = 0xca;
	g_APDU.PP1 = 0x9f;//*(g_APDU.pCommBuff+2) = 0x9f;
	g_APDU.PP2 = 0x79;//*(g_APDU.pCommBuff+3) = 0x79;
	g_APDU.PP3 = 0;//*(g_APDU.pCommBuff+4) =0;
	g_APDU.LC = 0;
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;
	
//	AU9541_ICCRAD_TransCeive(&g_APDU);
	HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
	if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))
	{	
		return g_APDU.SW1*256+g_APDU.SW2;
	}
	else
	{
		ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, 0x9f79);
		if(NULL == ptr)
		{
			return SW_FILE_NOTFOUND;
		}
		else
		{
			memcpy(psAPDU->pOutData, ptr + 3, 6);
//			memcpy(g_APDU.pOutData, ptr + 3, 6);
			psAPDU->RLE = 6;
//			g_APDU.RLE = 6;									  
		}
	}
	return SW_OPERATION_SUCCESS;
}

//读圈存限额
unsigned int ReadUpLimitPrice(APDU *psAPDU)
{
	unsigned char *ptr = NULL;
	g_APDU.CLA = 0x80;//*(g_APDU.pCommBuff+0) = 0x80;
	g_APDU.INS = 0xca;//*(g_APDU.pCommBuff+1) = 0xca;
	g_APDU.PP1 = 0x9f;//*(g_APDU.pCommBuff+2) = 0x9f;
	g_APDU.PP2 = 0x77;//*(g_APDU.pCommBuff+3) = 0x77;
	g_APDU.PP3 = 0;//*(g_APDU.pCommBuff+4) =0;
	g_APDU.LC = 0;
	g_APDU.LE = 0x00;
	g_APDU.RLE = 0x00;   	
//	memcpy(g_APDU.pInData, uchAfl+1, g_APDU.LC); 
//	AU9541_ICCRAD_TransCeive(&g_APDU);
	HAL_Card_APDU(&g_APDU, 5+g_APDU.LC);
	if((0x90 != g_APDU.SW1) || (0x00 != g_APDU.SW2))
	{
		psAPDU->RLE = 0;	
		return g_APDU.SW1*256+g_APDU.SW2;
	}
	else
	{
		ptr = SearchTag(g_APDU.pOutData, g_APDU.RLE, 0x9f77);
		if(NULL == ptr)
		{
			psAPDU->RLE = 0;	
			return SW_FILE_NOTFOUND;
		}
		else
		{
			memcpy(psAPDU->pOutData, ptr + 3, 6);
			psAPDU->RLE = 6;								  
		}
	}
	return SW_OPERATION_SUCCESS;

}


