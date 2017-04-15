//========================================
//程序说明：
//端口使用：
//创建者：
//创建时间：
//最后修改时间
//========================================
//========================================
//头文件
#include "main.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

//========================================
//变量定义

volatile union FlagAF flagA;
struct XGData XG;									//装载机参数信息

uint8_t LedValue=0x00;						//LED指示灯值
uint8_t ShiftModeStatus;					//换挡模式
uint8_t ErrorMessage;							//错误信息

//========================================
//函数声明
extern void SystemInit(void);
void Init_System(void);    
  
void delay(uint16_t k);     //延时程序
//========================================
//主函数
int main(void) 
{
//	uint8_t mi;																																//临时变量
	//**********************************************************************
	//系统初始化
	Init_System();
	
	//**********************************************************************
	//
	ControllerInfo.ScanTimeMode=GearScanT10ms;						//挡位时间扫描模式置初始值	
	LedValue=(LedValue & 0xFE) | 0x01;										//LED指示灯值
	I2C_WriteData_PCF8574(0x70, LedValue);								//LED显示控制--电源指示灯亮	
	XG.ChannelNo[0]=1;																		//压力显示值为1号通道	1--6
	XG.ChannelNo[1]=1;																		//转速显示值为2号通道	2--4
	XG.Function=XG.Function%100+XG.ChannelNo[0]*1000+XG.ChannelNo[1]*100;			//通道显示
	
	XG.GearRatio[0]=4.166;																//1挡转速比
	XG.GearRatio[1]=2.594;																//2挡转速比
	XG.GearRatio[2]=1.178;																//3挡转速比
	XG.GearRatio[3]=0.672;																//4挡转速比
	XG.DriveAxleRatio=22.75;															//驱动桥速比
	
//	//**********************************************************************
//	//延时启动程序
//  XG.Startup_DelayTime=2;
//	while(XG.Startup_DelayTime)
//	{
//		;
//	}
	//**********************************************************************
	//ADC通道值校准	
	ADCRes.ADCMaxValue[0]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[0]=0.6;						//通道最大测得电压值0.6V	
	ADCRes.ADCMaxValue[1]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[1]=0.6;						//通道最大测得电压值0.6V
	ADCRes.ADCMaxValue[2]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[2]=0.6;						//通道最大测得电压值0.6V	
	ADCRes.ADCMaxValue[3]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[3]=0.6;						//通道最大测得电压值0.6V
	ADCRes.ADCMaxValue[4]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[4]=0.6;						//通道最大测得电压值0.6V	
	ADCRes.ADCMaxValue[5]=3;							//通道最大测得电压值3V
	ADCRes.ADCMinValue[5]=0.6;						//通道最大测得电压值0.6V

	//**********************************************************************
	//初始化启动，挡位需处理N才能进入系统	
	while(1)
	{
		StartUpGearDeal();																			//启动时刻挡位处理程序	
		if(flagctrl.Bits.StartUpFromNx)													//初始化时启动信号从N开始
		{			
			ControllerInfo.ProcessGear=ControllerInfo.NowGear;
			ControllerInfo.GoalGear=ControllerInfo.ProcessGear;		//目标挡位
			ControllerInfo.gearinfo=ControllerInfo.NowGear;					
			break;																								//装载机正式启动
		}		
	}
	
	//**********************************************************************
	//主要参数初始化
	ShiftModeStatus=HandShiftMode;												//默认：手动换挡模式
	ControllerInfo.ProcessGear=Init_GearN1;								//设置初始挡位值 N1	
	ControllerInfo.GoalGear=Init_GearN1;									//设置初始挡位值 N1	
//	GearToSolenoidValve(Init_GearN1,Init_Brakeinfo);			//电磁阀状态初始化	
	flagctrl.Bits.gearchange=TRUE;												//更新电磁阀值
	//**********************************************************************
	//装载机启动处理程序		
  while (1)
  {
		//**********************************************************************
		//共同区域处理区
		//**********************************************************************		
		//**********************
		//U盘存储处理
//		UserProcess_UDisk();					//U盘读写用户处理区
		
		//**********************
		//控制器信号处理		
	  if(flagctrl.Bits.ScanTimeFlag)		//10ms扫描一次											
		{
			flagctrl.Bits.ScanTimeFlag=FALSE;	
			GearScanVSDeal();								//挡位扫描和处理程序	
			
		}
		//**********************
		//键盘扫描程序
		KeyScan_PCF8574();
		if(!PCF8574.Bits.KeyScan_Flag)	//判断键值是否已经读取
		{
			//*********************
			if(KeyValue_PCF8574==7)											//自动/手动按键
			{
				PCF8574.Bits.KeyScan_Flag=TRUE;						//当前键盘值已读取，等待复位
				if(ShiftModeStatus==HandShiftMode)
				{
					ShiftModeStatus=AutoShiftMode;					//自动模式
					LedValue=(LedValue & 0x7F) | 0x80;			//LED指示灯值;
					
				}
				else
				{
					ShiftModeStatus=HandShiftMode;					//手动模式
					LedValue=LedValue & 0x7F;								//LED指示灯值;
					if(flagA.Bits.Gear1Keep_Flag)						//1挡保持生效时
					{
						flagA.Bits.Gear1Keep_Flag=FALSE;			//清1挡保持值
						LedValue=LedValue & 0xDF;							//LED指示灯值;
						
					}				
					if(flagA.Bits.Gear2Keep_Flag)						//2挡保持生效时
					{
						flagA.Bits.Gear2Keep_Flag=FALSE;			//清2挡保持值
						LedValue=LedValue & 0xBF;							//LED指示灯值;
					}
					
				}	
				I2C_WriteData_PCF8574(0x70, LedValue);		//LED显示控制--电源指示灯亮				
			}
			//*********************
			if(KeyValue_PCF8574==6)												//2挡保持
			{
				PCF8574.Bits.KeyScan_Flag=TRUE;							//当前键盘值已读取，等待复位				
				if(ShiftModeStatus==AutoShiftMode)					//自动模式下有效
				{
					if(!flagA.Bits.Gear2Keep_Flag)						//2挡保持未生效时
					{
						flagA.Bits.Gear2Keep_Flag=TRUE;
						LedValue=(LedValue & 0xBF) | 0x40;			//LED指示灯值;
						if(flagA.Bits.Gear1Keep_Flag)						//1挡保持生效时
						{
							flagA.Bits.Gear1Keep_Flag=FALSE;			//清1挡保持值
							LedValue=LedValue & 0xDF;							//LED指示灯值;
							
						}								
						I2C_WriteData_PCF8574(0x70, LedValue);	//LED显示控制--电源指示灯亮						
					}		
					else
					{
						flagA.Bits.Gear2Keep_Flag=FALSE;
						LedValue=LedValue & 0xBF;								//LED指示灯值;
						I2C_WriteData_PCF8574(0x70, LedValue);	//LED显示控制--电源指示灯亮				
					}
				}			
			}
			//*********************
			if(KeyValue_PCF8574==5)											//1挡保持
			{
				PCF8574.Bits.KeyScan_Flag=TRUE;						//当前键盘值已读取，等待复位
				if(ShiftModeStatus==AutoShiftMode)				//自动模式下有效
				{	
					if(!flagA.Bits.Gear1Keep_Flag)						//2挡保持未生效时
					{
						flagA.Bits.Gear1Keep_Flag=TRUE;
						LedValue=(LedValue & 0xDF) | 0x20;			//LED指示灯值;
						if(flagA.Bits.Gear2Keep_Flag)						//2挡保持生效时
						{
							flagA.Bits.Gear2Keep_Flag=FALSE;			//清2挡保持值
							LedValue=LedValue & 0xBF;							//LED指示灯值;
						}							
						I2C_WriteData_PCF8574(0x70, LedValue);	//LED显示控制--电源指示灯亮							
					}		
					else
					{
						flagA.Bits.Gear1Keep_Flag=FALSE;
						LedValue=LedValue & 0xDF;								//LED指示灯值;
						I2C_WriteData_PCF8574(0x70, LedValue);	//LED显示控制--电源指示灯亮							
					}
				}					
			}			
			
			//*********************
			if(KeyValue_PCF8574==2)											//转速显示通道选择
			{
				PCF8574.Bits.KeyScan_Flag=TRUE;						//当前键盘值已读取，等待复位
				if(XG.ChannelNo[1]>=3)
				{
					XG.ChannelNo[1]=1;
				}
				else
				{
					XG.ChannelNo[1]++;
				}		
				XG.Function=XG.Function%100+XG.ChannelNo[0]*1000+XG.ChannelNo[1]*100;			//通道显示	
			}				
			//*********************
			if(KeyValue_PCF8574==1)											//压力显示通道选择
			{
				PCF8574.Bits.KeyScan_Flag=TRUE;						//当前键盘值已读取，等待复位
				
				if(XG.ChannelNo[0]>=6)
				{
					XG.ChannelNo[0]=1;
				}
				else
				{
					XG.ChannelNo[0]++;
				}		
				XG.Function=XG.Function%100+XG.ChannelNo[0]*1000+XG.ChannelNo[1]*100;			//通道显示					
			}			
		}
		//**********************
		//ADC处理程序
		if(flagADC.Bits.ADCOK)
		{			
			flagADC.Bits.ADCOK=FALSE;								
			ADC_ValueFilter();																												//ADC滤波--耗时约3ms	
			ADCRes.ADCSumValue[0]+=ADCRes.ADCResValue[0];
			ADCRes.ADCSumCount[0]++;
			ADCRes.ADCSumValue[1]+=ADCRes.ADCResValue[1];
			ADCRes.ADCSumCount[1]++;
			ADCRes.ADCSumValue[2]+=ADCRes.ADCResValue[2];
			ADCRes.ADCSumCount[2]++;
			ADCRes.ADCSumValue[3]+=ADCRes.ADCResValue[3];
			ADCRes.ADCSumCount[3]++;
			ADCRes.ADCSumValue[4]+=ADCRes.ADCResValue[4];
			ADCRes.ADCSumCount[4]++;			
			ADCRes.ADCSumValue[5]+=ADCRes.ADCResValue[5];
			ADCRes.ADCSumCount[5]++;
		}					
		//**********************
		//

		
		//**********************************************************************
		//换挡模式处理区
		//**********************************************************************		
		if(ShiftModeStatus==AutoShiftMode)	//自动换挡模式
		{//自动换挡
			if(flagctrl.Bits.KDchange)				//KD键使能--1挡保持		
			{
				ControllerInfo.GoalGear=(ControllerInfo.gearinfo/0x10)*0x10+1;
			}		
			else
			{
				ControllerInfo.GoalGear=ControllerInfo.gearinfo;							//目标挡位				
			}
		
			//******************************************************************************
			//过程挡位处理		
		
			if(ControllerInfo.ProcessGear!=ControllerInfo.GoalGear)														//过程挡位与目标挡位不一致的时候执行
			{
				if(!flagTIM5.Bits.T1_5sec_Flag)																									//换挡延时判断
				{					
					if((ControllerInfo.ProcessGear/0x10)!=(ControllerInfo.GoalGear/0x10))					//方向不同,挡位直接切换至目标方向1挡
					{
//						if(ControllerInfo.ProcessGear%0x10>1)																				//挡位大于1挡，减1挡
//						{
//							ControllerInfo.ProcessGear=ControllerInfo.ProcessGear-1;
//						}
//						else																																				//挡位等于1挡，开始换向	
//						{
//							ControllerInfo.ProcessGear=(ControllerInfo.GoalGear/0x10)*0x10+1;				
//						}		
						ControllerInfo.ProcessGear=(ControllerInfo.GoalGear/0x10)*0x10+1;				
					}
					else																																					//方向相同后，通过进1挡方式切换到指定挡位
					{
						if((ControllerInfo.ProcessGear%0x10)<(ControllerInfo.GoalGear%0x10))
						{
							ControllerInfo.ProcessGear=ControllerInfo.ProcessGear+1;					
						}	
						else if((ControllerInfo.ProcessGear%0x10)>(ControllerInfo.GoalGear%0x10))
						{
							ControllerInfo.ProcessGear=ControllerInfo.ProcessGear-1;
						}
					}					
					flagctrl.Bits.gearchange=TRUE;
					flagTIM5.Bits.T1_5sec_Flag=TRUE;																							//启用换挡延时控制--1.5s			
				}
			}					
			
			
			//******************************************************************************
			//档位等信息处理过程			
			if(flagctrl.Bits.gearchange || flagctrl.Bits.brakechange)							//电磁阀更新
			{
				flagctrl.Bits.gearchange=FALSE;
				flagctrl.Bits.brakechange=FALSE;
				GearToSolenoidValve(ControllerInfo.ProcessGear,ControllerInfo.brakeinfo);			//电磁阀状态更新			
			}	

			
		}//自动换挡
		else																//手动换挡模式
		{//手动换挡
			ControllerInfo.GoalGear=ControllerInfo.gearinfo;							//目标挡位	
			ControllerInfo.ProcessGear=ControllerInfo.GoalGear;						//过程挡位=目标挡位			
			//******************************************************************************
			//档位等信息处理过程			
			if(flagctrl.Bits.gearchange || flagctrl.Bits.brakechange)							//电磁阀更新
			{
				flagctrl.Bits.gearchange=FALSE;
				flagctrl.Bits.brakechange=FALSE;
				GearToSolenoidValve(ControllerInfo.ProcessGear,ControllerInfo.brakeinfo);			//电磁阀状态更新			
			}		
			ControllerInfo.SuggestGear=ControllerInfo.ProcessGear;

			
		}//手动换挡	
		
		//**********************************************************************
		//100ms 更新程序		
		if(flagTIM5.Bits.T100msec_Flag)
		{
			flagTIM5.Bits.T100msec_Flag=FALSE;						
			//**************************************************************
			//	 
			ADCRes.ADCSumValue[0]=ADCRes.ADCSumValue[0]/ADCRes.ADCSumCount[0];			
			ADCRes.ADCSumValue[0]=(ADCRes.ADCSumValue[0]-ADCRes.ADCMinValue[0])*40/(ADCRes.ADCMaxValue[0]-ADCRes.ADCMinValue[0]);
			XG.BoomLargeCavityPressure=ADCRes.ADCSumValue[0]*1000;										//动臂大腔压力 ：0 - 40MPa // 4 - 20mA // 150欧姆电阻 //ADC1

			ADCRes.ADCSumValue[1]=ADCRes.ADCSumValue[1]/ADCRes.ADCSumCount[1];	
			ADCRes.ADCSumValue[1]=(ADCRes.ADCSumValue[1]-ADCRes.ADCMinValue[1])*40/(ADCRes.ADCMaxValue[1]-ADCRes.ADCMinValue[1]);
			XG.WorkingPumpPressure=ADCRes.ADCSumValue[1]*1000;										    //工作泵压力 ：0 - 40MPa // 4 - 20mA // 150欧姆电阻 //ADC1

			ADCRes.ADCSumValue[2]=ADCRes.ADCSumValue[2]/ADCRes.ADCSumCount[2];	
			ADCRes.ADCSumValue[2]=(ADCRes.ADCSumValue[2]-ADCRes.ADCMinValue[2])*40/(ADCRes.ADCMaxValue[2]-ADCRes.ADCMinValue[2]);
			XG.SteeringPumpPressure=ADCRes.ADCSumValue[2]*1000;									    	//转向泵压力 ：0 - 40MPa // 4 - 20mA // 150欧姆电阻 //ADC1

			ADCRes.ADCSumValue[3]=ADCRes.ADCSumValue[3]/ADCRes.ADCSumCount[3];				
			ADCRes.ADCSumValue[3]=(ADCRes.ADCSumValue[3]-ADCRes.ADCMinValue[3])*25/(ADCRes.ADCMaxValue[3]-ADCRes.ADCMinValue[3]);
			XG.RotatingBucketLargeCavityPressure=ADCRes.ADCSumValue[3]*1000;					//转斗大腔压力	 ：0 - 25MPa // 4 - 20mA // 150欧姆电阻 //ADC1

			ADCRes.ADCSumValue[4]=ADCRes.ADCSumValue[4]/ADCRes.ADCSumCount[4];				
			ADCRes.ADCSumValue[4]=(ADCRes.ADCSumValue[4]-ADCRes.ADCMinValue[4])*40/(ADCRes.ADCMaxValue[4]-ADCRes.ADCMinValue[4]);
			XG.BoomSmallCavityPressure=ADCRes.ADCSumValue[4]*1000;										//动臂小腔压力 ：0 - 40MPa // 4 - 20mA // 150欧姆电阻 //ADC1		

			ADCRes.ADCSumValue[5]=ADCRes.ADCSumValue[5]/ADCRes.ADCSumCount[5];	
			ADCRes.ADCSumValue[5]=(ADCRes.ADCSumValue[5]-ADCRes.ADCMinValue[5])*25/(ADCRes.ADCMaxValue[5]-ADCRes.ADCMinValue[5]);
			XG.RotatingBucketSmallCavityPressure=ADCRes.ADCSumValue[5]*1000;					//转斗小腔压力	 ：0 - 25MPa // 4 - 20mA // 150欧姆电阻 //ADC1

			ADCRes.ADCSumValue[0]=0;
			ADCRes.ADCSumCount[0]=0;
			ADCRes.ADCSumValue[1]=0;
			ADCRes.ADCSumCount[1]=0;			
			ADCRes.ADCSumValue[2]=0;
			ADCRes.ADCSumCount[2]=0;			
			ADCRes.ADCSumValue[3]=0;
			ADCRes.ADCSumCount[3]=0;			
			ADCRes.ADCSumValue[4]=0;
			ADCRes.ADCSumCount[4]=0;
			ADCRes.ADCSumValue[5]=0;
			ADCRes.ADCSumCount[5]=0;
			
			
			XG.CANPressure[0]=XG.BoomLargeCavityPressure;															//动臂大腔压力 ：0 - 40MPa
			XG.CANPressure[1]=XG.WorkingPumpPressure; 																//工作泵压力   ：0 - 40MPa
			XG.CANPressure[2]=XG.SteeringPumpPressure;																//转向泵压力   ：0 - 40MPa
			XG.CANPressure[3]=XG.RotatingBucketLargeCavityPressure;										//转斗大腔压力 ：0 - 25MPa
			XG.CANPressure[4]=XG.BoomSmallCavityPressure;															//动臂小腔压力 ：0 - 40MPa
			XG.CANPressure[5]=XG.RotatingBucketSmallCavityPressure;										//转斗小腔压力 ：0 - 25MPa	


			XG.PressureShow=XG.CANPressure[(XG.ChannelNo[0]-1)];

		}			
		//**********************************************************************
		//1 S更新程序		
		if(flagTIM5.Bits.T1sec_Flag)
		{
			flagTIM5.Bits.T1sec_Flag=FALSE;	
		
			//**************************************************************
			//转速计算
			//**********************************************************************
			//1s计数程序
			if(flagTIM5.Bits.Freq_1sec_Flag)
			{
				flagTIM5.Bits.Freq_1sec_Flag=FALSE;
				TIM4FreqCap[0].Freq=TIM4FreqCap[0].pulse;
				TIM4FreqCap[0].pulse=0;
				TIM4FreqCap[1].Freq=TIM4FreqCap[1].pulse;
				TIM4FreqCap[1].pulse=0;
				TIM4FreqCap[2].Freq=TIM4FreqCap[2].pulse;
				TIM4FreqCap[2].pulse=0;
				TIM4FreqCap[3].Freq=TIM4FreqCap[3].pulse;
				TIM4FreqCap[3].pulse=0;	
				
				XG.CANSpeed[0]=TIM4FreqCap[0].Freq;				
				XG.CANSpeed[1]=TIM4FreqCap[1].Freq*60/46;					//涡轮转速
				XG.CANSpeed[2]=TIM4FreqCap[2].Freq*60/3;					//输出轴转速 
				XG.CANSpeed[3]=TIM4FreqCap[3].Freq;
				
				XG.TurbineSpeed=XG.CANSpeed[1];										//涡轮转速 -- PLUSE2
				XG.RotationalSpeed=XG.CANSpeed[2];								//输出轴转速 -- PLUSE3			
						
				XG.SpeedShow=XG.CANSpeed[XG.ChannelNo[1]];					
				
			}	
			
			
			//**************************************************************
			//装载机系统运行显示	
			LoaderSystemDisplay();
			//**************************************************************
			//


		}	
		//**********************************************************************		
  }
}

//========================================
//子模块函数



//========================================
//函数名称:void Init_System(void)
//函数功能: 系统初始化
//入口参数:    
//出口参数:      
//========================================
void Init_System(void)
{

		//************************
		//函数初始化
		SystemInit();
		flagA.flagBits=0;
		IOControl_Init();   			//IO初始化
		MAX7219_Configuration();	//完成MAX7219的配置
	  Init_PCF8574();				    //PCF8574的初始化程序
		Uart_Init();              //Uart串口初始化
		TIM5_Init();              //Timer5定时器初始化
//		USB_UDisk_Init();					//U盘初始化程序
		ADC_Configuration();
		FreqCap_Configuration(0xffff,(72-1));					//完成PULSE的配置,以1Mhz的频率计数	
		CAN_Configuration();			//完成CAN的配置	
//		RTCInit_Configuration();	//完成RTC的初始化设置
	
}


//========================================
//函数名称:void delay(uint16_t k)
//函数功能: 延时程序
//入口参数:    
//出口参数:      
//========================================
void delay(uint16_t k)
{ 
      while(k--)
      { 
            ;   
      }
}
