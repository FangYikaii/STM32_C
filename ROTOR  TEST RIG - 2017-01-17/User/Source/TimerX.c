/*
程序说明:  	
            TimerX 定时程序 --- STM32F107VCT 
						Timer5 用于一般的定时，同时作为USARTx的通信超时溢出寄存器
						Timer2 用于USB通信的精确定时
注意事项：						
						

创建者:     FangYikaii   
创建时间:   2017-01-16 
修改时间:   2017-01-16
*/
//========================================
//定时器：7个（除了2watchdog timers：独立看门狗、窗口看门狗）
//2基本定时器：TIM6，TIM7[最基本功能：累加时钟脉冲数超过预定值，触发中断或触发DMA][向上计数】
//4通用定时器：TIM2，TIM3，TIM4，TIM5[测量输入脉冲频率、脉冲宽和输入PWM脉冲等场合][向上，向下计数】
//1高级定时器：TIM1[还有三相六部电机接口、刹车功能][向上向下计数]

//时钟配置Systick=72MHz，APB1预分频器的分频系数被配为2，则PCK1刚好最大值36MHz
//预分频:TIMx_PSC=0不分频  TIMx_PSC=1 2分频
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
编写时间：2017.01.16
编 写 人：FangYIkaii
注    意：1.TIM5使能
					2.周期、预分频次数初始化
					3.计数方式
					4.初始化定时器
					5.清除溢出中断
					6.打开溢出中断
					7.计算器使能
				  每次进入中断的时间：
					((1+TIM_Prescaler)/72M)*(1+TIM_Period)=(1+719)/72000000*(1+100-1)=0.001S
***********************************************************************/
void TIM5_Init(void)
{
		//定义定时器结构体
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       	
	
		//TIM5使能
		/* TIM5 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	 /* ---------------------------------------------------------------
		TIMxCLK = 72 MHz, Prescaler = 720, T=100kHz // 0.01ms  * 100 = 1ms
	  T：72000000/720=1/100000
		周期：T=1/100000*100=0.001  1MS
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
}

/***********************************************************************
函数名称：void TIM5_TimeCount(void)
功    能：TIM5时间处理程序
输入参数：无
输出参数：无
编写时间：2017.01.17
编 写 人：FangYikaii
注    意：
***********************************************************************/
void TIM5_TimeCount(void)
{
	//ms++
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
	
	//U盘读写程序
	RdData.collecttime_s=RdData.collecttime_s+0.001;
	
 	//IO口产生脉冲
 	PULSEGeneration(Timer5.msecond%2);
	
	//串口接收溢出计时
	if(flagCOM1.Bits.OverTime_Flag)//串口溢出计时	
	{		
		//串口溢出计数<最大接受数
		if(Rec_COM1.TimeOut<OVERFLOW_TIME)
		{
			Rec_COM1.TimeOut++;										//串口溢出计时
		}
		//串口溢出计数>最大接受数
		else
		{
			Rec_COM1.Count=0;											//串口接收清零				
			Rec_COM1.TimeOut=0;										//串口溢出计时
			flagCOM1.Bits.OverTime_Flag=FALSE;				//串口溢出计时标志位	
		}			
	}	
	
	//串口溢出计时	
	if(flagCOM2.Bits.OverTime_Flag)//串口溢出计时	
	{
		//串口溢出计数<最大接受数
		if(Rec_COM2.TimeOut<OVERFLOW_TIME)
		{
			Rec_COM2.TimeOut++;										//串口溢出计时
		}
		//串口溢出计数>最大接受数
		else
		{
			Rec_COM2.Count=0;											//串口接收清零				
			Rec_COM2.TimeOut=0;										//串口溢出计时
			flagCOM2.Bits.OverTime_Flag=FALSE;				//串口溢出计时标志位				
		}			
	}
	
	//10ms中断一次
	if(Timer5.msecond%10==0)
	{
		//USB
		//10ms存储标志位--True
		flagUDisk.Bits.SaveTime_10ms=TRUE;
		//IO Ctrl
		if(ControllerInfo.ScanTimeMode%2==1)		//GearScanT10ms = 1
		{
			//IO Ctrl扫描标志位--模式1下
			flagctrl.Bits.ScanTimeFlag=TRUE;			
		}
		//TIM5 10ms标志位
		flagTIM5.Bits.T10msec_Flag=TRUE;
	}	
	//100ms中断一次
	if(Timer5.msecond%100==0)									
	{
		//USB
		//100ms存储标志位--True
		flagUDisk.Bits.SaveTime_100ms=TRUE;		
		//IO Ctrl
		if(ControllerInfo.ScanTimeMode%2==0)		//GearScanT100ms = 2
		{
			//IO Ctrl扫描标志位--模式2下
			flagctrl.Bits.ScanTimeFlag=TRUE;			
		}
		//TIM5 100ms标志位
		flagTIM5.Bits.T100msec_Flag=TRUE;
	}
	
	//常规定时计时--1s
	if(Timer5.msecond>=1000)
	{
		//********************************************************************************
		//脉冲捕捉计时程序
		//TIM1-通道1
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
		
		//TIM1-通道2
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
		
		//TIM1-通道3
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
		
		//TIM1-通道4
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
		
		//TIM4-通道1
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
		
		//TIM4-通道2
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
		
		//TIM4-通道3
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

		//TIM4-通道4
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
			//1秒-标志位
			flagTIM5.Bits.T1sec_Flag=TRUE;		
			//脉冲捕获标志位
			flagTIM5.Bits.Freq_1sec_Flag=TRUE;
			//USB存储1秒标志位
			flagUDisk.Bits.SaveTime_1s=TRUE;
			
			if(Timer5.second>=60)
			{
						Timer5.second=0;
						//1min标志位
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
//编写时间：2017.01.17
//编 写 人：FangYikaii
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
