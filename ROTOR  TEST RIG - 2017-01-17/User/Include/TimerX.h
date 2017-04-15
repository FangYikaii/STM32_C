//========================================
//<--预定义处理，防止重复定义-->
#ifndef _TimerX_H
#define _TimerX_H

//========================================
//头文件



//========================================
//宏定义




//========================================
//端口定义




//========================================
//外调变量声明

//结构体:TIMER计数结果
struct TIM5Result 
{
		uint32_t ms_Record;       //毫秒
    uint16_t msecond;         //毫秒
		uint16_t T1_5secmsCount;  //1.5秒
	
    uint8_t second;          //秒
    uint8_t minute;          //分钟
    uint8_t hour;            //时间
};
extern struct TIM5Result Timer5;

//联合体：TIMER计数标志位
union FlagTimer  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			T100msec_Flag 		:	1;			 //100ms         标志位
						uint8_t			T1sec_Flag  			:	1;       //1s            标志位
						uint8_t			T1min_Flag				:	1;			 //1min          标志位
						uint8_t 		T10msec_Flag			:	1;       //10ms          标志位
						uint8_t 		T1_5sec_Flag			:	1;			 //1.5s	         标志位
						uint8_t 		Freq_1sec_Flag		:	1;       //脉冲捕捉计数  标志位
						uint8_t 		Bit6	          	:	1;				
						uint8_t 		Bit7	           	:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagTimer flagTIM5;

//========================================
//外调函数声明
extern void TIM5_Init(void);   			//Timer5定时器初始化
extern void TIM5_TimeCount(void);		//TIM5时间处理程序
extern void TIM5_IRQHandler(void); //TIM5中断程序



//========================================
//<--预处理结束-->
#endif



