/*
程序说明:  	
            TimerX 定时程序 --- STM32F107VCT 
						Timer5 用于一般的定时，同时作为USARTx的通信超时溢出寄存器
						Timer2 用于USB通信的精确定时
注意事项：						
						

创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/
//========================================
//头文件
#include "main.h"

//========================================
//变量定义
struct TIM5Result Timer5;
volatile union FlagTimer flagTIM5;

//========================================
//函数声明
void TIM5_Init(void);   		//Timer5定时器初始化
void TIM5_TimeCount(void);	//TIM5时间处理程序




//========================================
//子模块函数



/***********************************************************************
函数名称：void TIM5_Init(void)
功    能：Timer5定时器初始化
输入参数：
输出参数：
编写时间：2013.11.19
编 写 人：
注    意：
***********************************************************************/
void TIM5_Init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //定义定时器结构体

		NVIC_InitTypeDef NVIC_InitStructure;



		/* TIM5 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	 /* ---------------------------------------------------------------
		TIMxCLK = 72 MHz, Prescaler = 720, T=100kHz // 0.01ms  * 100 = 1ms
		--------------------------------------------------------------- */      
		/* Time base configuration */
		//这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
		TIM_TimeBaseStructure.TIM_Period = (100 - 1);
		// 这个就是预分频系数，当由于为0时表示不分频所以要减1
//      TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
		TIM_TimeBaseStructure.TIM_Prescaler = (7200-6480- 1);	
		// 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
		// 使用的采样频率之间的分频比例
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //与滤波器采样有关，与分频无关
		//向上计数
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		//初始化定时器5
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		/* Clear TIM5 update pending flag[清除TIM5溢出中断标志] */
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		/* TIM IT enable */ //打开溢出中断
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		/* TIM5 enable counter */
		TIM_Cmd(TIM5, ENABLE);  //计数器使能，开始工作
		
		
		

		/* Set the Vector Table base address at 0x08000000 */
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0

		/* Enable the TIM5 gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;							//设定通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	//抢断优先级设置  0或1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//响应优先级设置
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 			
}

/***********************************************************************
函数名称：void TIM5_TimeCount(void)
功    能：TIM5时间处理程序
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/
void TIM5_TimeCount(void)
{
	Timer5.msecond++;
	Timer5.ms_Record++;
	
	if(flagTIM5.Bits.T1_5sec_Flag)
	{
		Timer5.T1_5secmsCount++;
		if(Timer5.T1_5secmsCount>=1500)
		{
			Timer5.T1_5secmsCount=0;
			flagTIM5.Bits.T1_5sec_Flag=FALSE;		
		}	
	}
	
//	RdData.collecttime_s=RdData.collecttime_s+0.001;
	
// 	//IO口产生脉冲
// 	PULSEGeneration(Timer5.msecond%2);
	
	//串口接收溢出计时
	if(flagCOM1.Bits.OverTime_Flag)//串口溢出计时	
	{
		
		if(Rec_COM1.TimeOut<OVERFLOW_TIME)
		{
			Rec_COM1.TimeOut++;										//串口溢出计时
		}
		else
		{
			Rec_COM1.Count=0;											//串口接收清零				
			Rec_COM1.TimeOut=0;										//串口溢出计时
			flagCOM1.Bits.OverTime_Flag=FALSE;				//串口溢出计时					
			
		}			
	}	
	if(flagCOM2.Bits.OverTime_Flag)//串口溢出计时	
	{
		
		if(Rec_COM2.TimeOut<OVERFLOW_TIME)
		{
			Rec_COM2.TimeOut++;										//串口溢出计时
		}
		else
		{
			Rec_COM2.Count=0;											//串口接收清零				
			Rec_COM2.TimeOut=0;										//串口溢出计时
			flagCOM2.Bits.OverTime_Flag=FALSE;				//串口溢出计时					
			
		}			
	}
	
	if(Timer5.msecond%10==0)									//10ms中断一次
	{
		flagUDisk.Bits.SaveTime_10ms=TRUE;	
		if(ControllerInfo.ScanTimeMode%2==1)		//GearScanT10ms = 1
		{
			flagctrl.Bits.ScanTimeFlag=TRUE;			
		}
		flagTIM5.Bits.T10msec_Flag=TRUE;
	}	
	if(Timer5.msecond%100==0)									//100ms中断一次
	{
		if(ControllerInfo.ScanTimeMode%2==0)		//GearScanT100ms = 2
		{
			flagctrl.Bits.ScanTimeFlag=TRUE;			
		}		
		flagTIM5.Bits.T100msec_Flag=TRUE;

	}
	if(Timer5.msecond%100==0)									//100ms中断一次
	{
		flagUDisk.Bits.SaveTime_100ms=TRUE;		
	}
	
	//常规定时计时
	if(Timer5.msecond>=1000)
	{
		//********************************************************************************
		//脉冲捕捉计时程序
		if(TIM1FreqCap[0].IsUsedFlag==1)
		{
			if(TIM1FreqCap[0].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM1FreqCap[0].NOINT_s++;
			}
			else
			{
				TIM1FreqCap[0].Freq=0;
			}			
		}
		if(TIM1FreqCap[1].IsUsedFlag==1)
		{
			if(TIM1FreqCap[1].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM1FreqCap[1].NOINT_s++;
			}
			else
			{
				TIM1FreqCap[1].Freq=0;				
			}			
		}
		if(TIM1FreqCap[2].IsUsedFlag==1)
		{
			if(TIM1FreqCap[2].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM1FreqCap[2].NOINT_s++;
			}
			else
			{
				TIM1FreqCap[2].Freq=0;				
			}			
		}		
		if(TIM1FreqCap[3].IsUsedFlag==1)
		{
			if(TIM1FreqCap[3].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM1FreqCap[3].NOINT_s++;
			}
			else
			{
				TIM1FreqCap[3].Freq=0;				
			}			
		}		
		
		if(TIM4FreqCap[0].IsUsedFlag==1)
		{
			if(TIM4FreqCap[0].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM4FreqCap[0].NOINT_s++;
			}
			else
			{
				TIM4FreqCap[0].Freq=0;
			}			
		}
		if(TIM4FreqCap[1].IsUsedFlag==1)
		{
			if(TIM4FreqCap[1].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM4FreqCap[1].NOINT_s++;
			}
			else
			{
				TIM4FreqCap[1].Freq=0;				
			}			
		}
		if(TIM4FreqCap[2].IsUsedFlag==1)
		{
			if(TIM4FreqCap[2].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM4FreqCap[2].NOINT_s++;
			}
			else
			{
				TIM4FreqCap[2].Freq=0;				
			}			
		}		
		if(TIM4FreqCap[3].IsUsedFlag==1)
		{
			if(TIM4FreqCap[3].NOINT_s<Fre_NOINTMAXTIME)
			{
				TIM4FreqCap[3].NOINT_s++;
			}
			else
			{
				TIM4FreqCap[3].Freq=0;				
			}			
		}			
		
		//脉冲捕捉计时程序			
		//********************************************************************************
	
			Timer5.msecond=0;			
			Timer5.second++;
		  if(XG.Startup_DelayTime>0)
			{
				XG.Startup_DelayTime=XG.Startup_DelayTime-1;
			}		
			flagTIM5.Bits.T1sec_Flag=TRUE;		
			flagTIM5.Bits.Freq_1sec_Flag=TRUE;
			flagUDisk.Bits.SaveTime_1s=TRUE;
			if(Timer5.second>=60)
			{
						Timer5.second=0;
						flagTIM5.Bits.T1min_Flag=TRUE;
						Timer5.minute++;
						if(Timer5.minute>=60)
						{
									Timer5.minute=0;
						}
			}
	}	
}


 //========================================
//函数名称:void TIM5_IRQHandler(void)
//函数功能: TIM5中断处理程序
//入口参数:    
//出口参数:      
//========================================
/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
      if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
      {
            TIM_ClearITPendingBit(TIM5, TIM_IT_Update);     //清中断标志
            TIM5_TimeCount();																//TIM5时间处理程序      
      }
}
