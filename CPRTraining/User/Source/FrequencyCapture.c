//========================================
//程序说明:       STM32F107VC 频率采集程序
//端口使用: 
//创建者:          Masterjay   
//创建时间:       2013-11-19 
//修改时间:       2013-11-19 
//========================================
//========================================
//头文件
#include "main.h"

//========================================
//变量定义
struct FreqCapResult TIM1FreqCap[4];
struct FreqCapResult TIM4FreqCap[4];
uint8_t FreqCapChannelNO=1;														//脉冲通道选择
uint16_t Freq_Output;																	//输出频率值


//========================================
//函数声明

void FreqCap_Configuration(u16 arr,u16 psc);					//完成PULSE的配置




//========================================
//子模块函数







/***********************************************************************
函数名称：void FreqCap_Configuration(u16 arr,u16 psc)
功    能：完成PULSE的配置
输入参数：
输出参数：
编写时间：2014.4.16
编 写 人：
注    意：
***********************************************************************/
void FreqCap_Configuration(u16 arr,u16 psc)
{	 
  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	//脉冲产生IO配置
	
	RCC_APB2PeriphClockCmd(PULSEGeneration_RCC_APB2Periph_GPIOX, ENABLE); 	//IO端口时钟开启			 
			
	GPIO_InitStructure.GPIO_Pin = PULSEGeneration_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PULSEGeneration_GPIOX, &GPIO_InitStructure);		
		
// 	//TIM1捕捉通道
// 	RCC_APB2PeriphClockCmd(FreqCap1TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap1TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap1TIM1_GPIOX, &GPIO_InitStructure);	
// 	
// 	RCC_APB2PeriphClockCmd(FreqCap2TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap2TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap2TIM1_GPIOX, &GPIO_InitStructure);	
// 	
// 	RCC_APB2PeriphClockCmd(FreqCap3TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap3TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap3TIM1_GPIOX, &GPIO_InitStructure);	
// 	
// 	RCC_APB2PeriphClockCmd(FreqCap4TIM1_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap4TIM1_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap4TIM1_GPIOX, &GPIO_InitStructure);	
	
	
	//TIM4捕捉通道	
	
// 	RCC_APB2PeriphClockCmd(FreqCap1TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
// 	GPIO_InitStructure.GPIO_Pin = FreqCap1TIM4_GPIO_PIN;     
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(FreqCap2TIM4_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(FreqCap2TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap2TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap2TIM4_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(FreqCap3TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap3TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap3TIM4_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(FreqCap4TIM4_RCC_APB2Periph_GPIOX,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = FreqCap4TIM4_GPIO_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FreqCap4TIM4_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						//TIMx时钟开启
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);							//TIMx时钟开启

 /* ---------------------------------------------------------------
	TIMxCLK = 72 MHz, Prescaler = 72, T=1MHz // 0.001ms  脉冲信号 2ms  故2个脉冲沿可记得2000个数
	--------------------------------------------------------------- */      
	/* Time base configuration */
	
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


	TIM_Cmd(TIM4, ENABLE);							//启动定时器
	TIM4FreqCap[0].IsUsedFlag=0;				//1 -- 启用清零  0 -- 不启用清零
	TIM4FreqCap[1].IsUsedFlag=0;
	TIM4FreqCap[2].IsUsedFlag=0;
	TIM4FreqCap[3].IsUsedFlag=0;	
	
	
	TIM4FreqCap[1].pulse=((Fre_Min_Wave+Fre_Max_Wave)/2)*40;
	Freq_Output=TIM4FreqCap[1].pulse/40;														//输出频率值
	
	
//  	TIM_Cmd(TIM1, ENABLE);							//启动定时器   
//	TIM1FreqCap[0].IsUsedFlag=1;
//	TIM1FreqCap[1].IsUsedFlag=1;
//	TIM1FreqCap[2].IsUsedFlag=1;
//	TIM1FreqCap[3].IsUsedFlag=1;	  

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//抢断优先级 0   		响应优先级 0-15	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//抢断优先级 0-1    响应优先级 0-7	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//抢断优先级 0-3    响应优先级 0-3	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//抢断优先级 0-7    响应优先级 0-1	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0



	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

// 	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =6;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);      
	
}


/***********************************************************************
函数名称：void TIM1_CC_IRQHandler(void)
功    能：TIM1中断处理函数
输入参数：
输出参数：
编写时间：2014.04.14
编 写 人：
注    意  
***********************************************************************/
void TIM1_CC_IRQHandler(void)
{ 

//********************************************	
//通道1脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);		
		TIM1FreqCap[0].NOINT_s=0;	
		TIM1FreqCap[0].pulse++;	
	}		
	
//********************************************	
//通道2脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);		
		TIM1FreqCap[1].NOINT_s=0;	
		TIM1FreqCap[1].pulse++;	
	}	
	
//********************************************	
//通道3脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);		
		TIM1FreqCap[2].NOINT_s=0;	
		TIM1FreqCap[2].pulse++;	
	}		
	
//********************************************	
//通道2脉冲上升沿捕获中断程序
	if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);		
		TIM1FreqCap[3].NOINT_s=0;	
		TIM1FreqCap[3].pulse++;	
	}		
	
//********************************************		
//定时器溢出中断处理程序
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	
	}		
	
	
}

/***********************************************************************
函数名称：void TIM4_IRQHandler(void)
功    能：TIM4中断处理函数
输入参数：
输出参数：
编写时间：2014.04.14
编 写 人：
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

		if(DigIO_IO1_Read)
		{
			if(TIM4FreqCap[1].pulse<Fre_Max_Wave*40)
			{
				TIM4FreqCap[1].pulse++;	
			}	
		}
		else
		{
	
			if(TIM4FreqCap[1].pulse>Fre_Min_Wave*40)
			{
				TIM4FreqCap[1].pulse--;	
			}			
		}			
		
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
//通道2脉冲上升沿捕获中断程序
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
