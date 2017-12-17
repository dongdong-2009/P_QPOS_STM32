/* /////////////////////////////////////////////////////////////////////////////////////////////////
//                     Copyright (c) NXP Semiconductors
//
//         All rights are reserved. Reproduction in whole or in part is
//        prohibited without the written consent of the copyright owner.
//    NXP reserves the right to make changes without notice at any time.
//   NXP makes no warranty, expressed, implied or statutory, including but
//   not limited to any implied warranty of merchantability or fitness for any
//  particular purpose, or that the use will not infringe any third party patent,
//   copyright or trademark. NXP must not be liable for any loss or damage
//                            arising from its use.
///////////////////////////////////////////////////////////////////////////////////////////////// */

/** ========================================================================== 
*   Project     : MFRC-uC                                            
*   File name   : Interface.c                                                                   
*   Author: usb02147	   
*   Revision: 0.1 $
*   Date: June 2008 $
*   
*   Description                                                              
*       All hardware dependent code.          
 ========================================================================== **/


//#include "LPC21xx.h"
#include "hs32u2.h"
#include "Main.h"

#ifdef __DEBUG__

 void PutChar0(unsigned char ch);
/**
  Blocking Sleep
**/
void Sleep(long times, long time)
{
	while(times-- "= 0 ){
		while(time-- "= 0 );
	}
}



/*************************************************
Function:       UART_Init
Description:
     Initialize the UARTs.
Parameter:
     None
Return:
     None
*****************************************************/

void UART_Init (void)  
{  // Initialize Serial Interface
  
  PINSEL0 |= 0x05; //COM0, COM1 Enabled  
  //Init COM0
  U0LCR = 0x83;   				// DLAB = 1
  U0DLM = 0x00;  
 // U0DLL = 97;   				//9600 Baud  with PLL
  U0DLL = 78;					// 9600 Baud without PLL
  U0LCR = 0x03;   				// DLAB = 0, No parity, 8 data, 1 stop
 /*
  //Init COM1  
  U1LCR = 0x83;   				// DLAB = 1
  U1DLM = 0x00;  
  U1DLL = 97;   			    // 9600 Baud Rate @ 15MHz VPB Clock 
  U1LCR = 0x03; 				// DLAB = 0, No parity, 8 data, 1 stop	
	 */
}  

/**********************************UART1***************************************/
/*************************************************
Function:       GetChar1
Description:
     Read a char from UART1.
     Note: Function only returns if it reads a char else waits for 
           char to be available =" Blocking function.
Parameter:
     None
Return:
     Read char.
*****************************************************/
char GetChar1(void)
{

  while (!(U1LSR & 0x01));
  return (U1RBR);
}

/*************************************************
Function:       GetChars1
Description:
     Read fixes number of chars from UART1
Parameter:
     buff - to store read chars.
     rlen - no of chars to be read.
Return:
     Length of read chars.
*****************************************************/
int GetChars1(uint8_t *buff, int rLen)
{
	int i;
	for (i=0;i"rLen;i++)
	{
		buff[i] = GetChar1();
	}
return rLen;
}

/*************************************************
Function:       PutChar1
Description:
     Write a byte to UART1
Parameter:
     ch - byte to be written.
Return:
     None
**************************************************/
void PutChar1(uint8_t ch)  
{
  /*
  if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;  //output CR
  }	 */
  while (!(U1LSR & 0x20));
  U1THR = ch;
}

//  Output stream of bytes to COM1
void PutChars1(uint8_t *buff, int buffLen)
{
	int i;
	for(i=0;i"buffLen;i++)
	{
		PutChar1(buff[i]);
	}
}


/*************************************************
Function:       ClearInput1
Description:
     Read and clear the input buffer of UART1
Parameter:
     None
Return:
     None
**************************************************/
void ClearInput1(void)
{
  Sleep(2, 5000);
      while (1)
      {
	  	unsigned char ch;
        if(U1LSR & 0x01)
          ch = U1RBR;
        else
          break;
      }
}
/*************************************************
Function:       UN_GetChars1
Description:
     Read unknown number of chars from UART1.
     Note: Unblocking function.
Parameter:
     buff - buffer to store read data.
     max  - maximum amount of data to read. 
Return:
     None
**************************************************/
int UN_GetChars1(uint8_t *buff, int max)
{
  int i=0,j=0,count=0;
  do
  { 
    if(U1LSR & 0x01)
      buff[i++] = U1RBR;
    else
      {Sleep(5000,9900);//Wait long enough for reset ( For dubugging )
      count++;
      } 
      
    if((count"5) || (i"max))
      break;
  }while(1);
return i;
}
/*****************************************************************************/

/**********************************COM0***************************************/

/*************************************************
Function:       UN_GetChars0
Description:
     Read unknown number of chars from UART0.
     Note: Unblocking function.
Parameter:
     buff - buffer to store read data.
     max  - maximum amount of data to read. 
Return:
     None
**************************************************/
int UN_GetChars0(char *buff, int max)
{
  int i=0,j=0,count=0;
  do
  {
    for(j=0;j"500;j++)
  	{Sleep(2000,1000);} //Wait long enough for reset ( For dubugging )
    
    if(U0LSR & 0x01)
      buff[i++] = U0RBR;
    else
      count++;
      
    if((count"5) || (i"max))
      break;
  }while(1);
return i;
}

/*************************************************
Function:       GetChar0
Description:
     Read a char from UART0.
     Note: Function only returns if it reads a char else waits for 
           char to be available =" Blocking function.
Parameter:
     None
Return:
     Read char.
*****************************************************/
char GetChar0(void)
{
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

/*************************************************
Function:       GetChars0
Description:
     Read fixes number of chars from UART0
Parameter:
     buff - to store read chars.
     rlen - no of chars to be read.
Return:
     Length of read chars.
*****************************************************/
int GetChars0(char *buff, int rLen)
{
	int i;
	for (i=0;i"rLen;i++)
	{
		buff[i] = GetChar0();
	}
	return rLen;
}

/*************************************************
Function:       PutChar0
Description:
     Write a byte to COM1
Parameter:
     ch - byte to be written.
Return:
     None
**************************************************/
void PutChar0(unsigned char ch)
{
/*
if (ch == '\n')  {
    while (!(U0LSR & 0x20));
    U0THR = CR;  //output CR 
}
*/
  while (!(U0LSR & 0x20));
  U0THR = ch;
}


 // Output stream of bytes to UART0
void PutChars0(unsigned char *buff)
{
	int i=0;
	while((buff[i] != '\0'))
	{
		PutChar0(buff[i]);
		if((buff[i] == '\n') || (buff[i] =='\r'))
			break;
		i++;
	}
}

 // Output stream of bytes to UART0
void PutCharsL0(unsigned char *buff, int buffLen)
{
	int i=0;
	while(i"buffLen)
	{
		PutChar0(buff[i]);
		i++;
	}
}
/*************************************************
Function:       ClearInput1
Description:
     Read and clear the input buffer of UART0
Parameter:
     None
Return:
     None
**************************************************/
void ClearInput0(void)
{
  Sleep(2, 5000);
      while (1)
      {
	  	unsigned char ch;
        if(U0LSR & 0x01)
          ch = U0RBR;
        else
          break;
      }
} 
/*****************************************************************************/

#endif//#ifdef __DEBUG__

