/*
    this file is the headerfile for bluetooch.
*/
#ifndef __BUZZER_H__
#define __BUZZER_H__

typedef enum
{
    BUZZER_NO_DATA = 0,
	BUZZER_DATA,
    BUZZER_HEADER_ERROR,
    BUZZER_LENGTH_ERROR,
    BUZZER_XOR_ERROR,
    BUZZER_SUCEESCE
}BUZZER_RECV_STATUS;

#define BUZZER_Pin GPIO_Pin_15
#define BUZZER_Port	GPIOA
#define BUZZER_CLK	RCC_APB2Periph_GPIOA

extern BUZZER_RECV_STATUS g_eBuzzerRecvStatus;

void iBUZZER_GPIO_Config(void);
void iBUZZER_Init(void);
int iBuzzer_On(unsigned char ucTime);

#endif
/*******************************end of  file**********************************************/




