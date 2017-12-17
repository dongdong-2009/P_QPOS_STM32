/*
    this file is the head file for powerceck
*/

#ifndef __POWERCHECK_H__
#define __POWERCHECK_H__
//
#define PWR_Ctrl_Pin GPIO_Pin_15
#define PWR_Ctrl_Port	GPIOB
#define PWR_Ctrl_CLK		RCC_APB2Periph_GPIOB
//开关机检测键
#define PWR_SwitchCheck_Pin GPIO_Pin_3
#define PWR_SwitchCheck_Port	GPIOC
#define PWR_SwitchCheck_CLK		RCC_APB2Periph_GPIOC

//充电检测
#define PWR_ChargeCheck_Pin GPIO_Pin_11
#define PWR_ChargeCheck_Port	GPIOA
#define PWR_ChargeCheck_CLK		RCC_APB2Periph_GPIOA

#define PWR_EXIT_Port GPIO_PortSourceGPIOC
#define PWR_EXIT_Pin GPIO_PinSource3
#define PWR_EXIT_Line EXTI_Line3
#define PWR_EXIT_Num	EXTI3_IRQn
typedef enum {PowerOFF = 0, PowerON = !PowerOFF} POWERSTATE;

#define PWR_Ctrl_Check()	  	GPIO_ReadInputDataBit(PWR_SwitchCheck_Port, PWR_SwitchCheck_Pin)
#define PWR_Charge_Check	GPIO_ReadInputDataBit(PWR_ChargeCheck_Port, PWR_ChargeCheck_Pin)

extern unsigned char g_PWRKeyFlag;

void PWR_GPIO_Init(void);
void vPower_Init(void);
void PWR_Cmd(POWERSTATE NewState);
void Power_OFF(unsigned char flag);
void PWR_Check(void);
void PowerCheckInit(void);

void Audio_SendData_Power(void);

void Audio_SendData_Power_OFF(void);

void ChargeCheckInit(void);

#endif//_POWERCHECK_H


