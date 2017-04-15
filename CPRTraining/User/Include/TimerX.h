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
struct TIM5Result 
{
		uint32_t ms_Record; 
    uint16_t msecond;
		uint16_t T1_5secmsCount;
	
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
};
extern struct TIM5Result Timer5;


union FlagTimer  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			T100msec_Flag 		:	1;				
						uint8_t			T1sec_Flag  			:	1;
						uint8_t			T1min_Flag				:	1;
						uint8_t 		T10msec_Flag			:	1;
						uint8_t 		T1_5sec_Flag			:	1;				
						uint8_t 		Freq_1sec_Flag		:	1;
						uint8_t 		T1msec_Flag	    	:	1;				
						uint8_t 		Bit7		:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagTimer flagTIM5;

//========================================
//外调函数声明
extern void TIM5_Init(void);   			//Timer5定时器初始化
extern void TIM5_TimeCount(void);		//TIM5时间处理程序




//========================================
//<--预处理结束-->
#endif



