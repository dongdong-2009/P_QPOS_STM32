#ifndef _RC522_H_
#define _RC522_H_

#include "Mifare.h"

//void RcModifyReg(unsigned char RegAddr, unsigned char ModifyVal, unsigned char MaskByte);
void SetPowerDown(unsigned char ucFlag);
short SetTimeOut(unsigned int uiMicroSeconds);
short ChangeJoinerBaudRate(unsigned long baudrate);
short Rc522Init(void);
short Request(unsigned char req_code, unsigned char *atq);
//short Select(unsigned char sel_code, unsigned char *snr, unsigned char *sak);
short AnticollSelect(unsigned char bcnt, unsigned char *snr);
short HaltA(void);
short Authentication(unsigned char auth_mode, const unsigned char *key, const unsigned char *snr, unsigned char addr);
short Read(unsigned char addr, unsigned char *_data);
short Write( unsigned char addr, unsigned char *_data);
short ValueOper(unsigned char OperMode, unsigned char addr, unsigned char *value, unsigned char trans_addr);
short InitBlock(unsigned char addr,unsigned char *value);

#endif /* _RC522_H_ */