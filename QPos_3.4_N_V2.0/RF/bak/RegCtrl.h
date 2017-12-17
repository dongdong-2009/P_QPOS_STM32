#ifdef __REGCTRL_H__ 
#define __REGCTRL_H__ 


/************************************************* 
Function:       RcSetReg 
Description: 
Write data to register of RC522 
Parameter: 

RegAddr       The address of the regitster 
RegVal        The value to be writen 
Return: 
None 
**************************************************/ 
void RcSetReg(unsigned char RegAddr, unsigned char RegVal); 


/************************************************* 
Function:       RcGetReg 
Description: 
Write data to register of RC522 
Parameter: 

RegAddr       The address of the regitster to be readed 
Return: 
The value of the specify register 
**************************************************/ 
unsigned char RcGetReg(unsigned char RegAddr); 


/************************************************* 
Function:       RcModifyReg 
Description: 
Change some bits of the register 
Parameter: 
RegAddr       The address of the regitster 

ModifyVal        The value to change to 
MaskByte      Only the corresponding bit '1' is valid 
Return: 
None 
**************************************************/ 
void RcModifyReg(unsigned char RegAddr, unsigned char ModifyVal, unsigned char MaskByte); 



#endif