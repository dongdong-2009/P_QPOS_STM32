
#ifndef __KEY_H__
#define __KEY_H__


#define KEY_PORT 	GPIOC
#define KEY_CLK		RCC_APB2Periph_GPIOC
#define KEY1_PIN		GPIO_Pin_5
#define KEY2_PIN		GPIO_Pin_6
#define KEY3_PIN		GPIO_Pin_7
#define KEY4_PIN		GPIO_Pin_8
#define KEY5_PIN		GPIO_Pin_9

#define KEY_BITS			0x03e0
#define KEY_BITS1		0x03c0
#define KEY_BITS2		0x0380
#define KEY_BITS3		0x0300
#define KEY_BITS4		0x0200

typedef enum
{
	KeyWaitForPress=0,
	KeyWaitForESCorOK,
	KeyWaitForPassword
	
}KEY_STATE;
extern KEY_STATE vKey_State;
unsigned char vKey_Scan(void);

unsigned char ucKey_Scan(unsigned char flag);

void vClearKeyBuf(void);
void vKey_Processing(void);

unsigned char vKeyWaitOK(void);
unsigned char vKeyTest(void);

#endif //_KEY_H__









