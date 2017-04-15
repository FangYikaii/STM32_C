//========================================
//程序说明:       STM32F107VC 频率采集程序
//端口使用: 
//创建者:        	FangYikaii  
//创建时间:       2017-01-16 
//修改时间:       2013-01-16 
//========================================
//========================================

//头文件
#include "main.h"

//========================================
//变量定义
struct FreqCapResult TIM1FreqCap[4];                  //频率采集结果 TIM1 FreqCap
struct FreqCapResult TIM4FreqCap[4];                  //频率采集结果 TIM4 FreqCap
uint8_t FreqCapChannelNO=1;														//脉冲通道选择



//========================================
//函数声明
void FreqCap_Configuration(u16 arr,u16 psc);					//完成PULSE的配置
void TIM1_CC_IRQHandler(void);                        //TIM1中断处理函数
void TIM4_IRQHandler(void);                           //TIM2中断处理函数



//========================================
//子模块函数
/***********************************************************************
函数名称：void FreqCap_Configuration(u16 arr,u16 psc)
功    能：完成PULSE的配置
输入参数：
输出参数：
编写时间：2017.01.17
编 写 人：FangYIkaii
注    意：
***********************************************************************/
void FreqCap_Configuration(u16 arr,u16 psc)
{	 
	//定义定时器结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//定义输入捕获结构体 <INPUT CAPTURE： TIM_Channel 通道 TIM_ICPolarity 上升或下降计数 TIM_ICSelection TIM_ICPrescaler  TIM_ICFilter>
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	//定义GPIO结构体  GPIO_Speed GPIO_Mode GPIO_Pin
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//脉冲产生IO配置
	/*PlusGeneration Enable*/
	RCC_APB2PeriphClockCmd(PULSEGeneration_RCC_APB2Periph_GPIOX, ENABLE); 	//IO端口时钟开启			 
			
	GPIO_InitStructure.GPIO_Pin = PULSEGeneration_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PULSEGeneration_GPIOX, &GPIO_InitStructure);		
		
// 	//TIM1捕捉通道
		
//	//1.开启时钟
//	//2.GPIO定义(1.GPIO_PIN，2.GPIO_Mode，3.GPIO_Speed)
//	//3.GPIO初始化
		
//	//TIM1 FC1
// 	RCC_APB2PeriphClockCmd(FreqCap1TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap1TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap1TIM1_GPIOX, &GPIO_InitStructure);	
//
//	//TIM1 FC2
// 	RCC_APB2PeriphClockCmd(FreqCap2TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap2TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap2TIM1_GPIOX, &GPIO_InitStructure);	
// 	
//	//TIM1 FC3
// 	RCC_APB2PeriphClockCmd(FreqCap3TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap3TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap3TIM1_GPIOX, &GPIO_InitStructure);	
//
//	//TIM1 FC4
// 	RCC_APB2PeriphClockCmd(FreqCap4TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap4TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap4TIM1_GPIOX, &GPIO_InitStructure);	
	
	
	//TIM4捕捉通道	
	
	//1.开启时钟
	//2.GPIO定义(1.GPIO_PIN，2.GPIO_Mode，3.GPIO_Speed)
	//3.GPIO初始化

//	//TIM4 FC1
// 	RCC_APB2PeriphClockCmd(FreqCap1TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap1TIM4_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap2TIM4_GPIOX, &GPIO_InitStructure);	
	
	//TIM4 FC2
	RCC_APB2PeriphClockCmd(FreqCap2TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap2TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap2TIM4_GPIOX, &GPIO_InitStructure);	
	
	//TIM4 FC3
	RCC_APB2PeriphClockCmd(FreqCap3TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap3TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap3TIM4_GPIOX, &GPIO_InitStructure);	
	
	//TIM4 FC4
	RCC_APB2PeriphClockCmd(FreqCap4TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap4TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap4TIM4_GPIOX, &GPIO_InitStructure);	
			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//TIMx时钟开启
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);							//TIMx时钟开启

 /* ---------------------------------------------------------------
	TIMxCLK = 72 MHz, Prescaler = 72, T=1MHz // 0.001ms  脉冲信号 2ms  故2个脉冲沿可记得2000个数
	--------------------------------------------------------------- */      
	/* Time base configuration */
	//TIM4
  //((1+TIM_Prescaler)/72M)*(1+TIM_Period)=(1+71)/72000000*(1+65535)=0.065536 s
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 										////自动装载的计数值，由于计数是从0开始的，计数0xFFFF后溢出
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; 										//设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 									//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 							//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//  	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

// 	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;      					//通道选择：1通道             
// 	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //捕捉上升沿，下降沿：TIM_ICPolarity_Falling  
// 	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //管脚与寄存器对应关系 -- 直接对应 
// 	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 					//分频器 每1次捕捉就产生中断  TIM_ICPSC_DIV2/3/4 -- 每2/3/4次捕捉产生中断
// 	TIM_ICInitStructure.TIM_ICFilter = 0x00;												//滤波器设置，经历几个周期跳变认定波形稳定  --  0x00 -- 0x0F	
// 	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//  	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                   
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//  	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;                   
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//  	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;                   
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//  	TIM_ICInit(TIM1, &TIM_ICInitStructure);	

// 	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);																								//选择触发时钟源
// 	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);																						//选择触发方式
// 	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);																//启动定时器被动触发


	TIM_ITConfig(TIM4, TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update, ENABLE);  		//打开定时器中断
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); 		//清除中断标志位

// 	TIM_ITConfig(TIM4, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update, ENABLE);  		//打开定时器中断
// 	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); 		//清除中断标志位

//  	TIM_ITConfig(TIM1,TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update, ENABLE);
//  	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update);


	TIM_Cmd(TIM4, ENABLE);							//启动定时器TIM4
	TIM4FreqCap[0].IsUsedFlag=0;
	TIM4FreqCap[1].IsUsedFlag=1;
	TIM4FreqCap[2].IsUsedFlag=1;
	TIM4FreqCap[3].IsUsedFlag=1;	
//  	TIM_Cmd(TIM1, ENABLE);							//启动定时器TIM1   
//	TIM1FreqCap[0].IsUsedFlag=1;
//	TIM1FreqCap[1].IsUsedFlag=1;
//	TIM1FreqCap[2].IsUsedFlag=1;
//	TIM1FreqCap[3].IsUsedFlag=1;	     
}


/***********************************************************************
函数名称：void TIM1_CC_IRQHandler(void)
功    能：TIM1中断处理函数
输入参数：
输出参数：
编写时间：2017.01.18
编 写 人：FangYIkaii
注    意  
***********************************************************************/
void TIM1_CC_IRQHandler(void)
{ 
	//在通道1发生捕获事件
	if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM1FreqCap[0].NOINT_s=0;																					//清未产生补捉中断计时器			
		//对两次捕获的值相减消除误差，得到正确的周期值			
		if(TIM1FreqCap[0].count==0)
		{
				 //TIM1FreqCap[0]的第一次脉冲值	
				 TIM1FreqCap[0].pulse1=TIM_GetCapture1(TIM1);									//读取TIM1 CCR1的值
				 TIM1FreqCap[0].count=1;
		}
		else
		{
			//TIM1FreqCap[0]的第二次脉冲值
			TIM1FreqCap[0].pulse2=TIM_GetCapture1(TIM1);
			TIM1FreqCap[0].count=0;
			if(TIM1FreqCap[0].pulse2>TIM1FreqCap[0].pulse1)
			{
				//pluse2>pluse1
				//TIM1FreqCap[0]的脉冲差值=pluse2-pluse1
				TIM1FreqCap[0].pulse=TIM1FreqCap[0].pulse2-TIM1FreqCap[0].pulse1;			//计算2次中断间的计时器计数差值
			}
			else
			{
				//pluse2<pluse1
				//TIM1FreqCap[0]的脉冲差值=65535+(pluse2-pluse1)
				TIM1FreqCap[0].pulse=0xffff-TIM1FreqCap[0].pulse1;
				TIM1FreqCap[0].pulse+=TIM1FreqCap[0].pulse2;											//计算2次中断间的计时器计数差值					
			}
			TIM1FreqCap[0].Freq=1000000.0/TIM1FreqCap[0].pulse;									//计算采样频率
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);					//清中断标志	
	}
	
	//在通道2发生捕获事件
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
	{
		TIM1FreqCap[1].NOINT_s=0;																					//清未产生补捉中断计时器				
		//对两次捕获的值相减消除误差，得到正确的周期值			
		if(TIM1FreqCap[1].count==0)
		{
				 //TIM1FreqCap[1]的第一次脉冲值	
				 TIM1FreqCap[1].pulse1=TIM_GetCapture2(TIM1);//读取TIM1 CCR2的值
				 TIM1FreqCap[1].count=1;
		}
		else
		{
			//TIM1FreqCap[1]的第二次脉冲值
			TIM1FreqCap[1].pulse2=TIM_GetCapture2(TIM1);
			TIM1FreqCap[1].count=0;
			if(TIM1FreqCap[1].pulse2>TIM1FreqCap[1].pulse1)
			{
				//pluse2>pluse1
				//TIM1FreqCap[1]的脉冲差值=pluse2-pluse1
				TIM1FreqCap[1].pulse=TIM1FreqCap[1].pulse2-TIM1FreqCap[1].pulse1;			//计算2次中断间的计时器计数差值
			}
			else
			{
				//pluse2<pluse1
				//TIM1FreqCap[1]的脉冲差值=65535+(pluse2-pluse1)
				TIM1FreqCap[1].pulse=0xffff-TIM1FreqCap[1].pulse1;
				TIM1FreqCap[1].pulse+=TIM1FreqCap[1].pulse2;											//计算2次中断间的计时器计数差值					
			}
			TIM1FreqCap[1].Freq=1000000.0/TIM1FreqCap[1].pulse;									//计算采样频率
		}			
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);					//清中断标志	
	}
	
	//在通道3发生捕获事件
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		TIM1FreqCap[2].NOINT_s=0;																					//清未产生补捉中断计时器				
		//对两次捕获的值相减消除误差，得到正确的周期值			
		if(TIM1FreqCap[2].count==0)
		{
				 //TIM1FreqCap[2]的第一次脉冲值	
				 TIM1FreqCap[2].pulse1=TIM_GetCapture3(TIM1);//读取TIM1 CCR3的值
				 TIM1FreqCap[2].count=1;
		}
		else
		{
			//TIM1FreqCap[2]的第二次脉冲值
			TIM1FreqCap[2].pulse2=TIM_GetCapture3(TIM1);
			TIM1FreqCap[2].count=0;
			if(TIM1FreqCap[2].pulse2>TIM1FreqCap[2].pulse1)
			{
				//pluse2>pluse1
				//TIM1FreqCap[2]的脉冲差值=pluse2-pluse1
				TIM1FreqCap[2].pulse=TIM1FreqCap[2].pulse2-TIM1FreqCap[2].pulse1;			//计算2次中断间的计时器计数差值
			}
			else
			{
				//pluse2<pluse1
				//TIM1FreqCap[2]的脉冲差值=65535+(pluse2-pluse1)
				TIM1FreqCap[2].pulse=0xffff-TIM1FreqCap[2].pulse1;
				TIM1FreqCap[2].pulse+=TIM1FreqCap[2].pulse2;											//计算2次中断间的计时器计数差值					
			}
			TIM1FreqCap[2].Freq=1000000.0/TIM1FreqCap[2].pulse;									//计算采样频率
		}			
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);					//清中断标志	
	}
	
	//在通道4发生捕获事件
	if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
	{
		TIM1FreqCap[3].NOINT_s=0;																					//清未产生补捉中断计时器			
		//对两次捕获的值相减消除误差，得到正确的周期值			
		if(TIM1FreqCap[3].count==0)
		{
				 //TIM1FreqCap[3]的第一次脉冲值
				 TIM1FreqCap[3].pulse1=TIM_GetCapture4(TIM1);//读取TIM1 CCR4的值
				 TIM1FreqCap[3].count=1;
		}
		else
		{
			//TIM1FreqCap[3]的第二次脉冲值
			TIM1FreqCap[3].pulse2=TIM_GetCapture4(TIM1);
			TIM1FreqCap[3].count=0;
			if(TIM1FreqCap[3].pulse2>TIM1FreqCap[3].pulse1)
			{
				//pluse2>pluse1
				//TIM1FreqCap[3]的脉冲差值=pluse2-pluse1
				TIM1FreqCap[3].pulse=TIM1FreqCap[3].pulse2-TIM1FreqCap[3].pulse1;			//计算2次中断间的计时器计数差值
			}
			else
			{
				//pluse2<pluse1
				//TIM1FreqCap[3]的脉冲差值=65535+(pluse2-pluse1)
				TIM1FreqCap[3].pulse=0xffff-TIM1FreqCap[3].pulse1;
				TIM1FreqCap[3].pulse+=TIM1FreqCap[3].pulse2;											//计算2次中断间的计时器计数差值					
			}
			TIM1FreqCap[3].Freq=1000000.0/TIM1FreqCap[3].pulse;									//计算采样频率
		}	
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);					//清中断标志		
	}
	
	//定时器溢出中断处理程序
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);					//清中断标志
	}
}

/***********************************************************************
函数名称：void TIM4_IRQHandler(void)
功    能：TIM4中断处理函数
输入参数：
输出参数：
编写时间：2017.01.18
编 写 人：FangYIkaii
注    意  
***********************************************************************/
void TIM4_IRQHandler(void)
{ 

//********************************************	
//通道1脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);		
		TIM4FreqCap[0].NOINT_s=0;	
		TIM4FreqCap[0].pulse++;	
	}		
	
//********************************************	
//通道2脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);		
		TIM4FreqCap[1].NOINT_s=0;	
		TIM4FreqCap[1].pulse++;	
	}	
	
//********************************************	
//通道3脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);		
		TIM4FreqCap[2].NOINT_s=0;	
		TIM4FreqCap[2].pulse++;	
	}		
	
//********************************************	
//通道4脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);		
		TIM4FreqCap[3].NOINT_s=0;	
		TIM4FreqCap[3].pulse++;	
	}		
	
//********************************************		
//定时器溢出中断处理程序
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	
	}			
}
