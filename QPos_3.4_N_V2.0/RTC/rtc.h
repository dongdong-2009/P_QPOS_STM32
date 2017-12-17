#ifndef __RTC_H
#define __RTC_H	    
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6

//时间结构体
typedef struct 
{
	unsigned short w_year;
	unsigned char  w_month;
	unsigned char  w_date;
 	unsigned char hour;
	unsigned char min;
	unsigned char sec;			
	//公历日月年周
	unsigned char  week;		 
}CALENDAR_OBJ;
					 
extern CALENDAR_OBJ g_sCalendar;	//日历结构体

extern unsigned char const mon_table[12];	//月份日期数据表

unsigned char RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;

unsigned char Is_Leap_Year(unsigned short year);//平年,闰年判断
unsigned char RTC_Get(void);         //更新时间   
unsigned char RTC_Get_Week(unsigned short year,unsigned char month,unsigned char day);
unsigned char RTC_Set(unsigned short syear,unsigned char smon,unsigned char sday,unsigned char hour,unsigned char min,unsigned char sec);//设置时间			 
#endif


