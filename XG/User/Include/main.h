//========================================
//<--预定义处理，防止重复定义-->
#ifndef _main_H
#define _main_H

//========================================
//头文件

//库文件

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include "stm32f10x.h"


//用户定义文件

#include "TimerX.h"
#include "USARTx.h"
#include "USB_UDisk.h"
#include "IOControl.h"
#include "CANx.h"
#include "MAX7219.h"
#include "RTC.h"
#include "PCF8574.h"
#include "ADC.h"
#include "FrequencyCapture.h"

//========================================
//宏定义
#define 			TRUE 							1
#define 			FALSE							0

#define 			HandShiftMode			1
#define				AutoShiftMode			2

#define				GearScanT10ms			1
#define				GearScanT100ms		2

#define				Init_GearN1				0x21
#define				Init_Brakeinfo		0x00

//========================================
//端口定义




//========================================
//外调变量声明
union FlagAF  
{ 
      uint8_t flagBits; 
      struct       
      { 
            uint8_t		Gear1Keep_Flag							:	1;			
						uint8_t		Gear2Keep_Flag							:	1;
						uint8_t		Bit2							:	1;				
						uint8_t		Bit3							:	1;	
						uint8_t		Bit4							:	1;		 			
						uint8_t		Bit5							:	1;	
						uint8_t		Bit6							:	1;				
						uint8_t		Bit7							:	1;
				
      } Bits; 
}; 
extern volatile union FlagAF flagA;



//========================================
//外调变量声明
extern uint8_t ShiftModeStatus;						//换挡模式
extern uint8_t ErrorMessage;							//错误信息

struct XGData 
{
	uint8_t  Startup_DelayTime;							//启动延时计时--s
	uint16_t BoomLargeCavityPressure;									//动臂大腔压力
	uint16_t WorkingPumpPressure;											//工作泵压力
	uint16_t SteeringPumpPressure;										//转向泵压力
	uint16_t RotatingBucketLargeCavityPressure;				//转斗大腔压力
	uint16_t BoomSmallCavityPressure;									//动臂小腔压力	
	uint16_t RotatingBucketSmallCavityPressure;				//转斗小腔压力	
	uint16_t CANPressure[6];
	uint16_t CANSpeed[4];
	uint16_t PressureShow;
	uint16_t SpeedShow;

	
  uint8_t Throttle;												//油门开度
	float SpeedRatio;												//转速比
	uint16_t PumpSpeed;											//泵轮转速
	uint16_t TurbineSpeed;									//涡轮转速
	uint16_t RotationalSpeed;								//输出轴转速
	
	uint8_t ChannelNo[2];										//通道选项: 0-- 压力显示通道  1-- 转速显示通道    ...
	uint16_t Function;											//功能显示
	float GearRatio[4];											//挡位速比
  float DriveAxleRatio;										//驱动桥传动比
};

extern struct XGData XG;


//========================================
//外调函数声明





extern void delay(uint16_t k);     //延时程序

//========================================
//<--预处理结束-->
#endif
