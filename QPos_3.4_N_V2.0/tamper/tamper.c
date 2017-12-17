#include "stm32f10x.h"
#include"tamper.h"

unsigned char g_Tamper_Flag;
void vTamper_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable TAMPER IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


unsigned char  vTamper_Init(void)
{
	uint16_t tmp;
	
	vTamper_NVIC_Configuration();

	 /* Enable PWR and BKP clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();
  
  tmp=BKP_ReadBackupRegister(BKP_DR1);
  if(tmp!=0x5050)
  {
	return 1;
  }
  
  /* Disable Tamper pin */
  BKP_TamperPinCmd(DISABLE);
  
  /* Disable Tamper interrupt */
  BKP_ITConfig(DISABLE);

  /* Tamper pin active on low level */
  BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);

  /* Clear Tamper pin Event(TE) pending flag */
  BKP_ClearFlag();

  /* Enable Tamper interrupt */
  BKP_ITConfig(ENABLE);

  /* Enable Tamper pin */
  BKP_TamperPinCmd(ENABLE);


  return 0;
}
