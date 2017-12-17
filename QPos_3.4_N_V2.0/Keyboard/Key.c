
#include "stm32f10x.h"
#include "key.h"
#include "systick_delay.h"	
#include "VectorTable.h"

const unsigned char KeyValue_tab[]={'9','*','0','#',
								   'O','1','2','E',
								   '6','3','4','C',
								   '5','7','8'
									};

KEY_STATE vKey_State;
void key_delay()//消抖
{
	Delay_ms(3);
}
void vKey_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
    RCC_APB2PeriphClockCmd(KEY_CLK,ENABLE);      	
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN|KEY5_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);   
}
unsigned char vKey_Scan(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	u16 key;
	u8 i;
	int counter=0x150000;

	//扫描接地的最后一行，所有的设置为输入
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN|KEY5_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);  

	key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS;
	if(key!=KEY_BITS)//有按键按下
	{
		key_delay();
		key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS;
		if(key!=KEY_BITS)//有按键按下
		{	
			for(i=0;i<5;i++)
			{
				if(!((key>>(5+i))&0x01))
				{
					while(key!=KEY_BITS)//检查按键送开
					{
						key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS;
						counter--;
						if(!counter)
							return i;
					}
					return i;
				}
			}
		}
		
	}
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;//改为下拉输入
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 

	GPIO_SetBits(KEY_PORT,KEY1_PIN);
	key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS1;
	if(key)//有按键按下
	{
		key_delay();
		key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS1;
		if(key)
		{
			for(i=0;i<4;i++)
			{
				if((key>>(6+i))&0x01)
				{		
					while(key)//检查按键送开
					{
						key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS1;
						counter--;
						if(!counter)
							return i+5;
					}
					return i+5;
				}
			}
		}
		
	}
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_PORT,KEY2_PIN);
	key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS2;
	if(key)//有按键按下
	{
		key_delay();
		key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS2;
		if(key)//有按键按下
		{
			for(i=0;i<3;i++)
			{
				if((key>>(7+i))&0x01)
				{
					while(key)//检查按键送开
					{
						key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS2;
						counter--;
						if(!counter)
							return 9+i;
					}
					return 9+i;
				}
			}
		}
		
	}

	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_PORT,KEY3_PIN);
	key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS3;
	if(key)//有按键按下
	{
		key_delay();
		key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS3;
		if(key)//有按键按下
		{
			for(i=0;i<2;i++)
			{
				if((key>>(8+i))&0x01)
				{
					while(key)//检查按键送开
					{
						key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS3;
						counter--;
						if(!counter)
							return 12+i;
					}
					return 12+i;
				}
			}
		}
		
	}

	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure); 
	GPIO_SetBits(KEY_PORT,KEY4_PIN);
	key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS4;
	if(key)//有按键按下
	{	
		key_delay();
		key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS4;
		if(key)//有按键按下
		{
			if((key>>9)&0x01)
			{
				while(key)//检查按键送开
				{
					key= GPIO_ReadInputData(KEY_PORT)&KEY_BITS4;
					counter--;
					if(!counter)
						return 14;
				}
				return 14;
			}
		}
				
	}
	return 0xff;
	
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : flag ,蜂鸣器标志
//* Output         : None
//* Return         : None
//***************************************************************************************
unsigned char ucKey_Scan(unsigned char flag)
{
    unsigned char key,ucRet = 0xff;
	IWDG_ReloadCounter();
    	key=vKey_Scan();
	if(key!=0xff)
	{	
		if(flag)
			iBuzzer_On(0);
		 ucRet=KeyValue_tab[key];
		 BackLightCmd(ENABLE);
	}  
    return ucRet;
}
//***************************************************************************************
//* Function Name  : 
//* Description    : /*add by Roc.*/ 
//*       
//* Input          : 
//* Output         : None
//* Return         : None
//***************************************************************************************
void vClearKeyBuf(void)
{
}

unsigned char vKeyWaitOK(void)
{
	unsigned char ucRet;
	while(1)
	{
		ucRet = ucKey_Scan(1); 
		if(ucRet == 'O')		//ok key
		{
			return (0);
		}
	}
}

unsigned char vKeyTest(void)
{
	unsigned char buf[15];
	unsigned char buf1[15]="123456789*0#ECO";
	unsigned char key,i=0;
	while(i<15)
	{
		vLCD_GUI_Show12X12_Col("请按下",3,0);
		vLCD_GUI_Show12X12_Col("键",3,88);
		if(0<=i&&i<=11)
			vLCD_GUI_Show12X12_Ex(&buf1[i],1,3,60);
		else if(i==12)
			vLCD_GUI_Show12X12_Col("取消",3,52);
		else if(i==13)
			vLCD_GUI_Show12X12_Col("删除",3,52);
		else if(i==14)
			vLCD_GUI_Show12X12_Col("确认",3,52);
		while(1)
		{
			key = ucKey_Scan(1);
			if(key!=0xff)
			{
				buf[i]=key;
				//vLCD_ShowNumber6X16(&key,1,5,6*i);	
				if(buf[i]!=buf1[i])
					return 1;
				else
				{					
					i++;
					break;
				}
				
			}
		}
		
	}
	return 0;//键值相等	

}
void vKey_Processing(void)
{

}

/*******************************end of  file**********************************************/
