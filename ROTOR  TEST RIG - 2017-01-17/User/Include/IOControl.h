//========================================
//<--预定义处理，防止重复定义-->
#ifndef _IOControl_H
#define _IOControl_H

//========================================
//头文件


//========================================
//宏定义



//========================================
//端口定义

//************************************
//档位采集信号IO(6路)
//PE7 PE8 PE9 PE10 PE11 PE12
//管脚、端口、外设时钟、读GPIO输出电平--GPIO_ReadInputDataBit

 #define     	GearLine_Brown_PIN											GPIO_Pin_8									//线束--3 棕色
 #define     	GearLine_Brown_GPIOX										GPIOE	
 #define     	GearLine_Brown_RCC_APB2Periph_GPIOX		  RCC_APB2Periph_GPIOE
 #define 			GearLine_Brown_Read											!GPIO_ReadInputDataBit(GearLine_Brown_GPIOX, GearLine_Brown_PIN)

 #define     	GearLine_Purple_PIN											GPIO_Pin_7									//线束--4 紫色
 #define     	GearLine_Purple_GPIOX										GPIOE	
 #define     	GearLine_Purple_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOE
 #define 			GearLine_Purple_Read										!GPIO_ReadInputDataBit(GearLine_Purple_GPIOX, GearLine_Purple_PIN)

 #define     	GearLine_White_PIN											GPIO_Pin_10									//线束--5 白色
 #define     	GearLine_White_GPIOX										GPIOE	
 #define     	GearLine_White_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			GearLine_White_Read											!GPIO_ReadInputDataBit(GearLine_White_GPIOX, GearLine_White_PIN)

 #define     	GearLine_Yellow_PIN											GPIO_Pin_9									//线束--6 黄色
 #define     	GearLine_Yellow_GPIOX										GPIOE	
 #define     	GearLine_Yellow_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOE
 #define 			GearLine_Yellow_Read										!GPIO_ReadInputDataBit(GearLine_Yellow_GPIOX, GearLine_Yellow_PIN)

 #define     	GearLine_Blue_PIN						  					GPIO_Pin_12									//线束--7 蓝色
 #define     	GearLine_Blue_GPIOX										  GPIOE	
 #define     	GearLine_Blue_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			GearLine_Blue_Read											!GPIO_ReadInputDataBit(GearLine_Blue_GPIOX, GearLine_Blue_PIN)

 #define     	GearLine_Green_PIN											GPIO_Pin_11									//线束--8 绿色
 #define     	GearLine_Green_GPIOX										GPIOE	
 #define     	GearLine_Green_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			GearLine_Green_Read											!GPIO_ReadInputDataBit(GearLine_Green_GPIOX, GearLine_Green_PIN)


//******************************************************
//******************************************************

//   特别注意，由于电路引入方向器，所以电平要相反处理

//******************************************************
//******************************************************

//************************************
//电磁阀控制信号IO(5路)
//PA0 PA1 PA2 PA3 PA6
//管脚、端口、外设时钟、[控制电平--GPIO_ResetBits--GPIO_SetBits]

#define     	SolenoidValve_M1_PIN											GPIO_Pin_0									//电磁阀--M1 红色
#define     	SolenoidValve_M1_GPIOX										GPIOA	
#define     	SolenoidValve_M1_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M1(x)   											((x) ? (GPIO_ResetBits(SolenoidValve_M1_GPIOX, SolenoidValve_M1_PIN)) : (GPIO_ResetBits(SolenoidValve_M1_GPIOX, SolenoidValve_M1_PIN)))

#define     	SolenoidValve_M2_PIN											GPIO_Pin_1									//电磁阀--M2 绿色
#define     	SolenoidValve_M2_GPIOX										GPIOA	
#define     	SolenoidValve_M2_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M2(x)   											((x) ? (GPIO_ResetBits(SolenoidValve_M2_GPIOX, SolenoidValve_M2_PIN)) : (GPIO_SetBits(SolenoidValve_M2_GPIOX, SolenoidValve_M2_PIN)))

#define     	SolenoidValve_M3_PIN											GPIO_Pin_2									//电磁阀--M3 兰色
#define     	SolenoidValve_M3_GPIOX										GPIOA	
#define     	SolenoidValve_M3_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M3(x)   											((x) ? (GPIO_ResetBits(SolenoidValve_M3_GPIOX, SolenoidValve_M3_PIN)) : (GPIO_SetBits(SolenoidValve_M3_GPIOX, SolenoidValve_M3_PIN)))

#define     	SolenoidValve_M4_PIN											GPIO_Pin_3									//电磁阀--M4 黄色
#define     	SolenoidValve_M4_GPIOX										GPIOA	
#define     	SolenoidValve_M4_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M4(x)   											((x) ? (GPIO_ResetBits(SolenoidValve_M4_GPIOX, SolenoidValve_M4_PIN)) : (GPIO_SetBits(SolenoidValve_M4_GPIOX, SolenoidValve_M4_PIN)))

#define     	SolenoidValve_M5_PIN											GPIO_Pin_6									//电磁阀--M5 白色
#define     	SolenoidValve_M5_GPIOX										GPIOA	
#define     	SolenoidValve_M5_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M5(x)   											((x) ? (GPIO_ResetBits(SolenoidValve_M5_GPIOX, SolenoidValve_M5_PIN)) : (GPIO_SetBits(SolenoidValve_M5_GPIOX, SolenoidValve_M5_PIN)))


// //************************************
// //制动控制采集信息IO(1路PE14)
//管脚、端口、外设时钟、读电平--GPIO_ReadInputDataBit

 #define     	BrakeControl_PIN													GPIO_Pin_14									//制动控制--桔色
 #define     	BrakeControl_GPIOX												GPIOE	
 #define     	BrakeControl_RCC_APB2Periph_GPIOX		  		RCC_APB2Periph_GPIOE
 #define 			BrakeControl_Read													GPIO_ReadInputDataBit(BrakeControl_GPIOX, BrakeControl_PIN)



//************************************
//空档启动联锁与倒车警示灯控制信息IO(2路)
//PA7 PB0
//管脚、端口、外设时钟、[控制电平--GPIO_ResetBits--GPIO_SetBits]

#define     	AsternAlarmLED_PIN												GPIO_Pin_7									//倒车警示灯--兰色--j2
#define     	AsternAlarmLED_GPIOX											GPIOA	
#define     	AsternAlarmLED_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			AsternAlarmLED(x)   											((x) ? (GPIO_ResetBits(AsternAlarmLED_GPIOX, AsternAlarmLED_PIN)) : (GPIO_SetBits(AsternAlarmLED_GPIOX, AsternAlarmLED_PIN)))

#define     	NeutralStartInterlock_PIN											GPIO_Pin_0							//空档启动联锁--绿色--J1
#define     	NeutralStartInterlock_GPIOX										GPIOB	
#define     	NeutralStartInterlock_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOB
#define 			NeutralStartInterlock(x)   										((x) ? (GPIO_ResetBits(NeutralStartInterlock_GPIOX, NeutralStartInterlock_PIN)) : (GPIO_SetBits(NeutralStartInterlock_GPIOX, NeutralStartInterlock_PIN)))

//************************************
//预留IO
#define     	ReservedIO_PIN														GPIO_Pin_1									//
#define     	ReservedIO_GPIOX													GPIOB	
#define     	ReservedIO_RCC_APB2Periph_GPIOX	    			RCC_APB2Periph_GPIOB
#define 			ReservedIO(x)   													((x) ? (GPIO_ResetBits(ReservedIO_GPIOX, ReservedIO_PIN)) : (GPIO_SetBits(ReservedIO_GPIOX, ReservedIO_PIN)))


//========================================
//外调变量声明
struct ControllerData				
{
	uint8_t ScanTimeMode;											//档位扫描时间模式				
	uint8_t NowGear;													//当前档位信息	-- 读取获得
	uint8_t NowBrake;													//当前刹车状态	-- 读取获得
	uint8_t GoalGear;													//目标挡位			-- 控制值
	uint8_t ProcessGear;											//过程挡位      -- 控制值
	uint8_t SuggestGear;											//推荐挡位      -- 预测值
	uint8_t	gearinfo;													//档位					-- 处理获得
	uint8_t brakeinfo;												//制动信号			-- 处理获得
	uint8_t gearCGcount;											//档位变化次数计数
	uint8_t brakeCGcount;											//档位变化次数计数
	
};
extern struct ControllerData	ControllerInfo;

union FlagController  
{ 
      uint8_t flagBits; 
      struct       
      { 
            uint8_t 		gearchange		:	1;
						uint8_t 		brakechange		:	1;				
						uint8_t 		KDchange    	:	1;
						uint8_t 		StartUpFromNx	:	1;
						uint8_t 		ScanTimeFlag	:	1;     			
						uint8_t 		KDScan_Flag  	:	1;
						uint8_t 		Bit6		:	1;				
						uint8_t 		Bit7		:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagController flagctrl;


//========================================
//外调函数声明

extern void StartUpGearDeal(void);							//启动时刻挡位处理程序
extern void GearScanVSDeal(void);								//档位扫描和处理程序
extern void IOControl_Init(void);   																//IO初始化  
extern uint8_t ReadGearLine_Status(void);															//读取换挡线束的电平
extern void GearToSolenoidValve(uint8_t gear,uint8_t ststus_brake);			//档位与电磁阀动作的转换

//========================================
//<--预处理结束-->
#endif
