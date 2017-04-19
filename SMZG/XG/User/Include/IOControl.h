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
//数字开关量采集-DIIN

 #define     	DIIN1_M1_PIN											GPIO_Pin_8									//DIIN1 -- 从换挡选择器读取输出24V信号M1
 #define     	DIIN1_M1_GPIOX										GPIOE	
 #define     	DIIN1_M1_RCC_APB2Periph_GPIOX		  RCC_APB2Periph_GPIOE
 #define 			DIIN1_M1_Read											GPIO_ReadInputDataBit(DIIN1_M1_GPIOX, DIIN1_M1_PIN)

 #define     	DIIN2_M2_PIN											GPIO_Pin_7									//DIIN2 -- 从换挡选择器读取输出24V信号M2
 #define     	DIIN2_M2_GPIOX										GPIOE	
 #define     	DIIN2_M2_RCC_APB2Periph_GPIOX	  	RCC_APB2Periph_GPIOE
 #define 			DIIN2_M2_Read											GPIO_ReadInputDataBit(DIIN2_M2_GPIOX, DIIN2_M2_PIN)

 #define     	DIIN3_M3_PIN											GPIO_Pin_10									//DIIN3 -- 从换挡选择器读取输出24V信号M3
 #define     	DIIN3_M3_GPIOX										GPIOE	
 #define     	DIIN3_M3_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			DIIN3_M3_Read											GPIO_ReadInputDataBit(DIIN3_M3_GPIOX, DIIN3_M3_PIN)

 #define     	DIIN4_M4_PIN											GPIO_Pin_9									//DIIN4 -- 从换挡选择器读取输出24V信号M4
 #define     	DIIN4_M4_GPIOX										GPIOE	
 #define     	DIIN4_M4_RCC_APB2Periph_GPIOX	  	RCC_APB2Periph_GPIOE
 #define 			DIIN4_M4_Read											GPIO_ReadInputDataBit(DIIN4_M4_GPIOX, DIIN4_M4_PIN)

 #define     	DIIN5_AS_PIN						  				GPIO_Pin_12								//DIIN5 -- 从换挡选择器读取输出24V信号AS
 #define     	DIIN5_AS_GPIOX										GPIOE	
 #define     	DIIN5_AS_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			DIIN5_AS_Read											GPIO_ReadInputDataBit(DIIN5_AS_GPIOX, DIIN5_AS_PIN)

 #define     	DIIN6_RF_PIN											GPIO_Pin_11									//DIIN6 -- 从换挡选择器读取输出24V信号RF
 #define     	DIIN6_RF_GPIOX										GPIOE	
 #define     	DIIN6_RF_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOE
 #define 			DIIN6_RF_Read											GPIO_ReadInputDataBit(DIIN6_RF_GPIOX, DIIN6_RF_PIN)

 #define     	DIIN7_BR_PIN											GPIO_Pin_14								//DIIN7 -- 从制动器上读取制动输入24V信号BR
 #define     	DIIN7_BR_GPIOX										GPIOE	
 #define     	DIIN7_BR_RCC_APB2Periph_GPIOX		  RCC_APB2Periph_GPIOE
 #define 			DIIN7_BR_Read											GPIO_ReadInputDataBit(DIIN7_BR_GPIOX, DIIN7_BR_PIN)

 #define     	DIIN8_XX_PIN											GPIO_Pin_13								//DIIN8 -- 预留
 #define     	DIIN8_XX_GPIOX										GPIOE	
 #define     	DIIN8_XX_RCC_APB2Periph_GPIOX		  RCC_APB2Periph_GPIOE
 #define 			DIIN8_XX_Read											GPIO_ReadInputDataBit(DIIN7_BR_GPIOX, DIIN7_BR_PIN)

//************************************
//电磁阀控制输出-DOOUT

#define     	DOOUT1_M1_PIN													GPIO_Pin_0									//DOOUT1 -- 电磁阀--M1
#define     	DOOUT1_M1_GPIOX												GPIOA	
#define     	DOOUT1_M1_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT1_M1(x)   												((x) ? (GPIO_ResetBits(DOOUT1_M1_GPIOX, DOOUT1_M1_PIN)) : (GPIO_SetBits(DOOUT1_M1_GPIOX, DOOUT1_M1_PIN)))

#define     	DOOUT2_M2_PIN													GPIO_Pin_1									//DOOUT2 -- 电磁阀--M2
#define     	DOOUT2_M2_GPIOX												GPIOA	
#define     	DOOUT2_M2_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT2_M2(x)   												((x) ? (GPIO_ResetBits(DOOUT2_M2_GPIOX, DOOUT2_M2_PIN)) : (GPIO_SetBits(DOOUT2_M2_GPIOX, DOOUT2_M2_PIN)))

#define     	DOOUT3_M3_PIN													GPIO_Pin_2									//DOOUT3 -- 电磁阀--M3
#define     	DOOUT3_M3_GPIOX												GPIOA	
#define     	DOOUT3_M3_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT3_M3(x)   												((x) ? (GPIO_ResetBits(DOOUT3_M3_GPIOX, DOOUT3_M3_PIN)) : (GPIO_SetBits(DOOUT3_M3_GPIOX, DOOUT3_M3_PIN)))

#define     	DOOUT4_M4_PIN													GPIO_Pin_3									//DOOUT4 -- 电磁阀--M4
#define     	DOOUT4_M4_GPIOX												GPIOA	
#define     	DOOUT4_M4_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT4_M4(x)   												((x) ? (GPIO_ResetBits(DOOUT4_M4_GPIOX, DOOUT4_M4_PIN)) : (GPIO_SetBits(DOOUT4_M4_GPIOX, DOOUT4_M4_PIN)))

#define     	DOOUT5_BR_PIN													GPIO_Pin_6									//DOOUT5 -- 模拟制动信号BR
#define     	DOOUT5_BR_GPIOX												GPIOA	
#define     	DOOUT5_BR_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT5_BR(x)   												((x) ? (GPIO_ResetBits(DOOUT5_BR_GPIOX, DOOUT5_BR_PIN)) : (GPIO_SetBits(DOOUT5_BR_GPIOX, DOOUT5_BR_PIN)))

#define     	DOOUT6_RF_PIN													GPIO_Pin_7									//DOOUT6 -- 控制倒档BF
#define     	DOOUT6_RF_GPIOX												GPIOA	
#define     	DOOUT6_RF_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOA
#define 			DOOUT6_RF(x)   												((x) ? (GPIO_ResetBits(DOOUT6_RF_GPIOX, DOOUT6_RF_PIN)) : (GPIO_SetBits(DOOUT6_RF_GPIOX, DOOUT6_RF_PIN)))

#define     	DOOUT7_AS_PIN													GPIO_Pin_0									//DOOUT7 -- 控制启动信号AS
#define     	DOOUT7_AS_GPIOX												GPIOB	
#define     	DOOUT7_AS_RCC_APB2Periph_GPIOX	  		RCC_APB2Periph_GPIOB
#define 			DOOUT7_AS(x)   												((x) ? (GPIO_ResetBits(DOOUT7_AS_GPIOX, DOOUT7_AS_PIN)) : (GPIO_SetBits(DOOUT7_AS_GPIOX, DOOUT7_AS_PIN)))

#define     	DOOUT8_XX_PIN													GPIO_Pin_1									//DOOUT8 -- 预留
#define     	DOOUT8_XX_GPIOX												GPIOB	
#define     	DOOUT8_XX_RCC_APB2Periph_GPIOX	    	RCC_APB2Periph_GPIOB
#define 			DOOUT8_XX(x)   												((x) ? (GPIO_ResetBits(DOOUT8_XX_GPIOX, DOOUT8_XX_PIN)) : (GPIO_SetBits(DOOUT8_XX_GPIOX, DOOUT8_XX_PIN)))


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
extern void GearScanVSDeal_HandShiftMode(void);								//档位扫描和处理程序
extern void GearScanVSDeal_AutoShiftMode(void);

extern void IOControl_Init(void);   																//IO初始化  
extern void ReadGearLine_Status(void);															//读取换挡线束的电平
extern void GearToSolenoidValve(uint8_t gear,uint8_t ststus_brake);			//档位与电磁阀动作的转换




//========================================
//<--预处理结束-->
#endif



