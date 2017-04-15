//========================================
//<--预定义处理，防止重复定义-->
#ifndef _RTC_H
#define _RTC_H

//========================================
//头文件



//========================================
//宏定义







//========================================
//端口定义



//========================================
//外调变量声明

//时间结果
struct DATE
{
	uint16_t msec;          //毫秒
 	uint8_t hour;           //时
	uint8_t min;            //分
	uint8_t sec;            //秒
	uint16_t 	year;         //年
	uint8_t month;          //月
	uint8_t day;            //日
	uint8_t week;           //周
	
	char cord[5];
	char string[24];				//更改数组长度时，注意修改对应的清空长度	
	
};
extern struct DATE NowTime;

//时间结果-标志位
union FlagRTCA  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			T1sec_Flag				:	1;					//1秒标志位
						uint8_t			Bit1							:	1;	
						uint8_t 		Bit2							:	1;				
						uint8_t 		Bit3							:	1;
						uint8_t 		Bit4							:	1;				
						uint8_t 		Bit5							:	1;
						uint8_t 		Bit6							:	1;				
						uint8_t 		Bit7							:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagRTCA flagRTC;

//========================================
//外调函数声明
extern void RTCInit_Configuration(void);		//完成RTC的初始化设置
extern uint8_t RTC_SetTime(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);		//设置时钟程序
extern uint8_t RTC_GetTime(void);																														//获取时钟程序


//========================================
//<--预处理结束-->
#endif
