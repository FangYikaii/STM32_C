//========================================
//程序说明：三明重工平地机
//端口使用：6个24V输出，5个24V输入
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

//volatile union FlagAF flagA;
struct XGData XG;									//装载机参数信息

uint8_t LedValue=0x00;						//LED指示灯值
uint8_t ShiftModeStatus;					//换挡模式
uint8_t ErrorMessage;							//错误信息

uint8_t testi=0;
//========================================
//函数声明
extern void SystemInit(void);
void Init_System(void);    
void delay(uint16_t k);     //延时程序


//========================================
//主函数
int main(void) 
{																	
	//**********************************************************************
	//系统初始化
	Init_System();
	
	//**********************************************************************
	//基本配置	
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
	//**********************************************************************
	//主要参数初始化
	ShiftModeStatus=HandShiftMode;												//默认：手动换挡模式

	
	while(1)
	{
		//**********************
		//控制器信号处理		
	  if(flagctrl.Bits.ScanTimeFlag)					//10ms扫描一次											
		{
			flagctrl.Bits.ScanTimeFlag=FALSE;	
			
			if(ShiftModeStatus==HandShiftMode)		//手动换挡模式
			{
				GearScanVSDeal_HandShiftMode();			//挡位扫描和处理程序					
			}
			else
			{
				GearScanVSDeal_AutoShiftMode();			//挡位扫描和处理程序	
				
			}
		}		
		
		//**********************
		//1 S更新程序		
		if(flagTIM5.Bits.T1sec_Flag)
		{
			flagTIM5.Bits.T1sec_Flag=FALSE;		
			//**********************
			//装载机系统运行显示	
			LoaderSystemDisplay();
		}	
		
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
				
		//*******************************************************************************************
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
					
				}	
				I2C_WriteData_PCF8574(0x70, LedValue);		//LED显示控制--电源指示灯亮				
			}		
		}		
		
		
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
//		flagA.flagBits=0;
		IOControl_Init();   			//IO初始化
		MAX7219_Configuration();	//完成MAX7219的配置
	  Init_PCF8574();				    //PCF8574的初始化程序
//		Uart_Init();              //Uart串口初始化
		TIM5_Init();              //Timer5定时器初始化
//		USB_UDisk_Init();					//U盘初始化程序
//		ADC_Configuration();
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
