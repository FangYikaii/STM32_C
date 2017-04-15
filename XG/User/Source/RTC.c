/*
程序说明:  	
            RTC 万年历显示程序 --- STM32F107VCT 
						
注意事项：						
						1、贴片电路的晶振不起振，至今未找到问题所在。
						
端口使用:		见Doc文件夹内电路设计说明
创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/

//========================================
//头文件
#include "main.h"

//========================================
//变量定义
struct DATE NowTime;
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};		//平年月份天数表
const uint8_t table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 							//月修正系数表

volatile union FlagRTCA flagRTC;
//========================================
//函数声明
void RTC_Configuration(void);				//完成RTC的配置
void RTCInit_Configuration(void);		//完成RTC的初始化设置
uint8_t RTC_SetTime(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);		//设置时钟程序
uint8_t RTC_GetTime(void);																														//获取时钟程序
uint8_t RTC_GetWeek(uint16_t year,uint8_t month,uint8_t day);																	//获取星期几程序,只允许1901--2099


//========================================
//子模块函数


/***********************************************************************
函数名称：void RTC_Configuration(void)
功    能：完成RTC的配置
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  /* PWR时钟（电源控制）与BKP时钟（RTC后备寄存器）使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  /*使能RTC和后备寄存器访问 */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  /* 将外设BKP的全部寄存器重设为缺省值 */
  BKP_DeInit();

  /* Enable LSE */
  /* 使能LSE（外部32.768KHz低速晶振）*/
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* Wait till LSE is ready */
  /* 等待外部晶振震荡稳定输出 */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
		;
	}

  /* Select LSE as RTC Clock Source */
  /*使用外部32.768KHz晶振作为RTC时钟 */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  /* 使能 RTC 的时钟供给 */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  /*等待RTC寄存器同步 */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  /* 等待上一次对RTC寄存器的写操作完成 */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  /* 使能RTC的秒中断 */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  /* 等待上一次对RTC寄存器的写操作完成 */
  RTC_WaitForLastTask();
  
  /* Set RTC prescaler: set RTC period to 1sec */
  /* 32.768KHz晶振预分频值是32767,如果对精度要求很高可以修改此分频值来校准晶振 */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  /* 等待上一次对RTC寄存器的写操作完成 */
  RTC_WaitForLastTask();
}

/***********************************************************************
函数名称：void RTCInit_Configuration(void)
功    能：完成RTC的初始化设置
输入参数：
输出参数：
编写时间：2014.4.15
编 写 人：
注    意：
***********************************************************************/
void RTCInit_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /* 以下if...else.... if判断系统时间是否已经设置，判断RTC后备寄存器1的值
     是否为事先写入的0XA5A5，如果不是，则说明RTC是第一次上电，需要配置RTC，
     提示用户通过串口更改系统时间，把实际时间转化为RTC计数值写入RTC寄存器,
     并修改后备寄存器1的值为0XA5A5。
     else表示已经设置了系统时间，打印上次系统复位的原因，并使能RTC秒中断
  */
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		
  {
		RTC_Configuration();													//RTC Configuration		
		RTC_SetTime(2014,04,15,21,46,0);							//设置时钟程序
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);			//修改后备寄存器1的值为0XA5A5
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)				//Power On Reset occurred
    {
      ;
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)		//External Reset occurred
    {
      ;
    }
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
      
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);

  /* Clear reset flags */
  RCC_ClearFlag();	
	
	RTC_ClearITPendingBit(RTC_IT_SEC);

	/* Wait until last write operation on RTC registers has finished */
	/* 等待上一次对RTC寄存器的写操作完成 */
	RTC_WaitForLastTask();	


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//抢断优先级 0   		响应优先级 0-15	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//抢断优先级 0-1    响应优先级 0-7	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//抢断优先级 0-3    响应优先级 0-3	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//抢断优先级 0-7    响应优先级 0-1	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
}

/***********************************************************************
函数名称：uint8_t Is_Leap_Year(uint16_t year)
功    能：判断是否为闰年程序
输入参数：年份
输出参数：1--是闰年  0--非闰年
编写时间：2014.4.15
编 写 人：
注    意：
月份				1 	2 	3 	4 	5 	6 	7 	8 	9 	10 	11 	12

闰年天数 		31 	29 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

非闰年天数 	31 	28 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

***********************************************************************/
uint8_t Is_Leap_Year(uint16_t year)
{
	if(year%4==0) 				//能否被4整除
	{
		if(year%100==0)			//能否被100整除
		{
			if(year%400==0)		//能否被400整除
			{
				return 1;				//是闰年				
			}
			else 
			{
				return 0;				
			}
		}
		else 
		{
			return 1;					//是闰年			
		}
	}
	else 
	{
		return 0;		
	}

}
/***********************************************************************
函数名称：uint8_t RTC_SetTime(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
功    能：设置时钟程序
输入参数：年份
输出参数：1--是闰年  0--非闰年
编写时间：2014.4.15
编 写 人：
注    意：
月份				1 	2 	3 	4 	5 	6 	7 	8 	9 	10 	11 	12

闰年天数 		31 	29 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

非闰年天数 	31 	28 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

***********************************************************************/
uint8_t RTC_SetTime(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(year<2000||year>2099)		//year范围为1970-2099,这里设置有效范围为2000-2099
	{
		return 1;		
	}
	for(t=1970;t<year;t++)			//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))				//判断是否为闰年
		{
			seccount+=31622400;			//闰年秒钟
		}
		else 
		{
			seccount+=31536000;			//非闰年秒钟		
		}
	}
	mon-=1;
	for(t=0;t<mon;t++)					//把前面月份的秒钟相加
	{
		seccount+=(uint32_t)mon_table[t]*86400;	//月份秒钟相加
		if(Is_Leap_Year(year)&&t==1)					//闰年2月份多1天秒钟
		{
			seccount+=86400;			
		}
	}
	seccount+=(uint32_t)(day-1)*86400;			//前面天数秒钟相加
	seccount+=(uint32_t)hour*3600;					//前面小时秒钟相加
	seccount+=(uint32_t)min*60; 						//前面分钟秒钟相加
	seccount+=sec;											//前面秒钟秒钟相加

	//设置时钟
/* 
	RCC->APB1ENR|=1<<28;								//使能电源时钟
	RCC->APB1ENR|=1<<27;								//使能备份时钟
	PWR->CR|=1<<8; 											//取消备份区写保护
*/	
	PWR_BackupAccessCmd(ENABLE);
	/* Wait until last write operation on RTC registers has finished */
  /* 等待上一次对RTC寄存器的写操作完成 */
  RTC_WaitForLastTask();
  
  /* Change the current time */
  /* 把时间转化为RTC计数值写入RTC寄存器 */
  RTC_SetCounter(seccount);
  
  /* Wait until last write operation on RTC registers has finished */
  /* 等待上一次对RTC寄存器的写操作完成 */
  RTC_WaitForLastTask();	
	
	
	
	return 0;
}

/***********************************************************************
函数名称：uint8_t RTC_GetTime(void)
功    能：获取时钟程序
输入参数：无
输出参数：0--读取成功，其他值--读取失败
编写时间：2014.4.15
编 写 人：
注    意：
月份				1 	2 	3 	4 	5 	6 	7 	8 	9 	10 	11 	12

闰年天数 		31 	29 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

非闰年天数 	31 	28 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

***********************************************************************/
uint8_t RTC_GetTime(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0;
	uint32_t temp=0;
	uint16_t temp1=0;
	timecount=RTC_GetCounter();						//获取秒钟
	temp=timecount/86400; 								//得到天数
	if(daycnt!=temp)											//天数大于1
	{
		daycnt=temp;
		temp1=1970; 												//从1970年开始
		while(temp>=365)										//天数大于365
		{
			if(Is_Leap_Year(temp1))						//判断是否为闰年
			{
				if(temp>=366)										//天数大于366
				{
					temp-=366;													
				}
				else
				{
					temp1++;
					break;
				}
			}
			else
			{
				temp-=365; 											//平年，减去1年天数			
			}
			temp1++;
		}
		NowTime.year=temp1;									//得到年份
		temp1=0;
		while(temp>=28)											//超过1个月
		{
			if(Is_Leap_Year(NowTime.year)&&temp1==1)	//判断当年是不是闰年且刚好为2月份
			{
				if(temp>=29)
				{
					temp-=29;	
				}
				else 
				{
					break;
				}
			}
			else
			{
				if(temp>=mon_table[temp1])
				{
					temp-=mon_table[temp1];
				}
				else
				{
					break;
				}
			}
			temp1++;
		}
		NowTime.month=temp1+1;					//得到月份
		NowTime.day=temp+1; 						//得到日期
	}
	temp=timecount%86400; 						//得到1天的秒数
	NowTime.hour=temp/3600; 					//得到小时
	NowTime.min=(temp%3600)/60; 			//得到分钟
	NowTime.sec=(temp%3600)%60; 			//得到秒钟
	NowTime.week=RTC_GetWeek(NowTime.year,NowTime.month,NowTime.day);		//得到周
	return 0;

}
/***********************************************************************
函数名称：uint8_t RTC_GetWeek(uint16_t year,uint8_t month,uint8_t day)
功    能：获取星期几程序,只允许1901--2099
输入参数：年，月，日
输出参数：0--读取成功，其他值--读取失败
编写时间：2014.4.15
编 写 人：
注    意：
月份				1 	2 	3 	4 	5 	6 	7 	8 	9 	10 	11 	12

闰年天数 		31 	29 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

非闰年天数 	31 	28 	31 	30 	31 	30 	31 	31 	30 	31 	30 	31

***********************************************************************/
uint8_t RTC_GetWeek(uint16_t year,uint8_t month,uint8_t day)
{
	uint16_t temp2;
	uint8_t yearH,yearL;
	yearH=year/100; 
	yearL=year%100;
	if (yearH>19)						//如果为21世纪，年份加100
	{
		yearL+=100;		
	}
	temp2=yearL+yearL/4;		//所过闰年数只算1900年之后
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)
	{
		temp2--;
	}
	return(temp2%7);
}


/***********************************************************************
函数名称：void RTC_IRQHandler(void)
功    能：RTC中断处理程序
输入参数：
输出参数：
编写时间：2014.04.14
编 写 人：
注    意  
***********************************************************************/
void RTC_IRQHandler(void)
{
  /*  判断RTC是否发生了秒中断（也有可能是溢出或者闹钟中断) */
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    /* 清除秒中断标志 */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
    flagRTC.Bits.T1sec_Flag=TRUE;				//RTC秒中断置位
		
  }
}
