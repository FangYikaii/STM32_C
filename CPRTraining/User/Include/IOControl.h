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

//***********************
//端口已更新  王少杰 2016.06.27
//***********************



 #define     	Btn_Start_PIN											GPIO_Pin_13									//开始
 #define     	Btn_Start_GPIOX										GPIOD	
 #define     	Btn_Start_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOD
 #define 			Btn_Start_Read										GPIO_ReadInputDataBit(Btn_Start_GPIOX, Btn_Start_PIN)
 
 #define 			Btn_Start_OUT(x)   							 ((x) ? (GPIO_SetBits(Btn_Start_GPIOX, Btn_Start_PIN)) : (GPIO_ResetBits(Btn_Start_GPIOX, Btn_Start_PIN)))
 
 
 

 #define     	Btn_Stop_PIN											GPIO_Pin_14									//暂停
 #define     	Btn_Stop_GPIOX										GPIOD	
 #define     	Btn_Stop_RCC_APB2Periph_GPIOX	  	RCC_APB2Periph_GPIOD
 #define 			Btn_Stop_Read											GPIO_ReadInputDataBit(Btn_Stop_GPIOX, Btn_Stop_PIN)





 #define     	DigIO_SCLK_PIN											GPIO_Pin_11									//DigIO_SCLK
 #define     	DigIO_SCLK_GPIOX										GPIOE	
 #define     	DigIO_SCLK_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOE
 #define 			DigIO_SCLK_Read										GPIO_ReadInputDataBit(DigIO_SCLK_GPIOX, DigIO_SCLK_PIN)
 
 #define 			DigIO_SCLK_OUT(x)   							 ((x) ? (GPIO_SetBits(DigIO_SCLK_GPIOX, DigIO_SCLK_PIN)) : (GPIO_ResetBits(DigIO_SCLK_GPIOX, DigIO_SCLK_PIN)))
 
 #define     	DigIO_RCLK_PIN											GPIO_Pin_13									//DigIO_RCLK
 #define     	DigIO_RCLK_GPIOX										GPIOE	
 #define     	DigIO_RCLK_RCC_APB2Periph_GPIOX	  	RCC_APB2Periph_GPIOE
 #define 			DigIO_RCLK_Read											GPIO_ReadInputDataBit(DigIO_RCLK_GPIOX, DigIO_RCLK_PIN)
 
 #define 			DigIO_RCLK_OUT(x)   							 ((x) ? (GPIO_SetBits(DigIO_RCLK_GPIOX, DigIO_RCLK_PIN)) : (GPIO_ResetBits(DigIO_RCLK_GPIOX, DigIO_RCLK_PIN)))
 
 #define     	DigIO_DATA_PIN											GPIO_Pin_15									//DigIO_DATA
 #define     	DigIO_DATA_GPIOX										GPIOE	
 #define     	DigIO_DATA_RCC_APB2Periph_GPIOX	  	RCC_APB2Periph_GPIOE
 #define 			DigIO_DATA_Read											GPIO_ReadInputDataBit(DigIO_DATA_GPIOX, DigIO_DATA_PIN)
 
 #define 			DigIO_DATA_OUT(x)   							 ((x) ? (GPIO_SetBits(DigIO_DATA_GPIOX, DigIO_DATA_PIN)) : (GPIO_ResetBits(DigIO_DATA_GPIOX, DigIO_DATA_PIN)))
 





//************************************
//采集信号IO

 #define     	DigIO_IO1_PIN											GPIO_Pin_8									//触碰
 #define     	DigIO_IO1_GPIOX										GPIOE	
 #define     	DigIO_IO1_RCC_APB2Periph_GPIOX		RCC_APB2Periph_GPIOE
 #define 			DigIO_IO1_Read										GPIO_ReadInputDataBit(DigIO_IO1_GPIOX, DigIO_IO1_PIN)




 
 
 
 #define     	DigIO_IO4_PIN											GPIO_Pin_11									//DigIO_IO4
 #define     	DigIO_IO4_GPIOX										GPIOE	
 #define     	DigIO_IO4_RCC_APB2Periph_GPIOX	  RCC_APB2Periph_GPIOE
 #define 			DigIO_IO4_Read										GPIO_ReadInputDataBit(DigIO_IO4_GPIOX, DigIO_IO4_PIN)
 
 
//******************************************************
//******************************************************

//   特别注意，由于电路引入方向器，所以电平要相反处理

//******************************************************
//******************************************************

//************************************
//电磁阀控制信号IO

#define     	SolenoidValve_M1_PIN											GPIO_Pin_10									//电磁阀--M1 
#define     	SolenoidValve_M1_GPIOX										GPIOA	
#define     	SolenoidValve_M1_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M1(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M1_GPIOX, SolenoidValve_M1_PIN)) : (GPIO_ResetBits(SolenoidValve_M1_GPIOX, SolenoidValve_M1_PIN)))

#define     	SolenoidValve_M2_PIN											GPIO_Pin_9									//电磁阀--M2 
#define     	SolenoidValve_M2_GPIOX										GPIOA	
#define     	SolenoidValve_M2_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M2(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M2_GPIOX, SolenoidValve_M2_PIN)) : (GPIO_ResetBits(SolenoidValve_M2_GPIOX, SolenoidValve_M2_PIN)))

#define     	SolenoidValve_M3_PIN											GPIO_Pin_8									//电磁阀--M3 
#define     	SolenoidValve_M3_GPIOX										GPIOA	
#define     	SolenoidValve_M3_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOA
#define 			SolenoidValve_M3(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M3_GPIOX, SolenoidValve_M3_PIN)) : (GPIO_ResetBits(SolenoidValve_M3_GPIOX, SolenoidValve_M3_PIN)))

#define     	SolenoidValve_M4_PIN											GPIO_Pin_9									//电磁阀--M4 
#define     	SolenoidValve_M4_GPIOX										GPIOC
#define     	SolenoidValve_M4_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOC
#define 			SolenoidValve_M4(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M4_GPIOX, SolenoidValve_M4_PIN)) : (GPIO_ResetBits(SolenoidValve_M4_GPIOX, SolenoidValve_M4_PIN)))

#define     	SolenoidValve_M5_PIN											GPIO_Pin_8									//电磁阀--M5 
#define     	SolenoidValve_M5_GPIOX										GPIOC	
#define     	SolenoidValve_M5_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOC
#define 			SolenoidValve_M5(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M5_GPIOX, SolenoidValve_M5_PIN)) : (GPIO_ResetBits(SolenoidValve_M5_GPIOX, SolenoidValve_M5_PIN)))

#define     	SolenoidValve_M6_PIN											GPIO_Pin_7									//电磁阀--M6 
#define     	SolenoidValve_M6_GPIOX										GPIOC	
#define     	SolenoidValve_M6_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOC
#define 			SolenoidValve_M6(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M6_GPIOX, SolenoidValve_M6_PIN)) : (GPIO_ResetBits(SolenoidValve_M6_GPIOX, SolenoidValve_M6_PIN)))

#define     	SolenoidValve_M7_PIN											GPIO_Pin_6									//电磁阀--M7 
#define     	SolenoidValve_M7_GPIOX										GPIOC	
#define     	SolenoidValve_M7_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOC
#define 			SolenoidValve_M7(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M7_GPIOX, SolenoidValve_M7_PIN)) : (GPIO_ResetBits(SolenoidValve_M7_GPIOX, SolenoidValve_M7_PIN)))

#define     	SolenoidValve_M8_PIN											GPIO_Pin_15									//电磁阀--M8 
#define     	SolenoidValve_M8_GPIOX										GPIOD	
#define     	SolenoidValve_M8_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M8(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M8_GPIOX, SolenoidValve_M8_PIN)) : (GPIO_ResetBits(SolenoidValve_M8_GPIOX, SolenoidValve_M8_PIN)))

#define     	SolenoidValve_M9_PIN											GPIO_Pin_14									//电磁阀--M9
#define     	SolenoidValve_M9_GPIOX										GPIOD	
#define     	SolenoidValve_M9_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M9(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M9_GPIOX, SolenoidValve_M9_PIN)) : (GPIO_ResetBits(SolenoidValve_M9_GPIOX, SolenoidValve_M9_PIN)))

#define     	SolenoidValve_M10_PIN											GPIO_Pin_13									//电磁阀--M10 
#define     	SolenoidValve_M10_GPIOX										GPIOD	
#define     	SolenoidValve_M10_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M10(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M10_GPIOX, SolenoidValve_M10_PIN)) : (GPIO_ResetBits(SolenoidValve_M10_GPIOX, SolenoidValve_M10_PIN)))

#define     	SolenoidValve_M11_PIN											GPIO_Pin_12									//电磁阀--M11 
#define     	SolenoidValve_M11_GPIOX										GPIOD
#define     	SolenoidValve_M11_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M11(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M11_GPIOX, SolenoidValve_M11_PIN)) : (GPIO_ResetBits(SolenoidValve_M11_GPIOX, SolenoidValve_M11_PIN)))

#define     	SolenoidValve_M12_PIN											GPIO_Pin_11									//电磁阀--M12 
#define     	SolenoidValve_M12_GPIOX										GPIOD
#define     	SolenoidValve_M12_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M12(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M12_GPIOX, SolenoidValve_M12_PIN)) : (GPIO_ResetBits(SolenoidValve_M12_GPIOX, SolenoidValve_M12_PIN)))

#define     	SolenoidValve_M13_PIN											GPIO_Pin_10									//电磁阀--M13 
#define     	SolenoidValve_M13_GPIOX										GPIOD
#define     	SolenoidValve_M13_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M13(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M13_GPIOX, SolenoidValve_M13_PIN)) : (GPIO_ResetBits(SolenoidValve_M13_GPIOX, SolenoidValve_M13_PIN)))

#define     	SolenoidValve_M14_PIN											GPIO_Pin_9									//电磁阀--M14 
#define     	SolenoidValve_M14_GPIOX										GPIOD	
#define     	SolenoidValve_M14_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M14(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M14_GPIOX, SolenoidValve_M14_PIN)) : (GPIO_ResetBits(SolenoidValve_M14_GPIOX, SolenoidValve_M14_PIN)))

#define     	SolenoidValve_M15_PIN											GPIO_Pin_8									//电磁阀--M15 
#define     	SolenoidValve_M15_GPIOX										GPIOD	
#define     	SolenoidValve_M15_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOD
#define 			SolenoidValve_M15(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M15_GPIOX, SolenoidValve_M15_PIN)) : (GPIO_ResetBits(SolenoidValve_M15_GPIOX, SolenoidValve_M15_PIN)))

#define     	SolenoidValve_M16_PIN											GPIO_Pin_15									//电磁阀--M16 
#define     	SolenoidValve_M16_GPIOX										GPIOB	
#define     	SolenoidValve_M16_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOB
#define 			SolenoidValve_M16(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M16_GPIOX, SolenoidValve_M16_PIN)) : (GPIO_ResetBits(SolenoidValve_M16_GPIOX, SolenoidValve_M16_PIN)))

#define     	SolenoidValve_M17_PIN											GPIO_Pin_14									//电磁阀--M17 
#define     	SolenoidValve_M17_GPIOX										GPIOB
#define     	SolenoidValve_M17_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOB
#define 			SolenoidValve_M17(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M17_GPIOX, SolenoidValve_M17_PIN)) : (GPIO_ResetBits(SolenoidValve_M17_GPIOX, SolenoidValve_M17_PIN)))

#define     	SolenoidValve_M18_PIN											GPIO_Pin_13									//电磁阀--M18 
#define     	SolenoidValve_M18_GPIOX										GPIOB
#define     	SolenoidValve_M18_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOB
#define 			SolenoidValve_M18(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M18_GPIOX, SolenoidValve_M18_PIN)) : (GPIO_ResetBits(SolenoidValve_M18_GPIOX, SolenoidValve_M18_PIN)))

#define     	SolenoidValve_M19_PIN											GPIO_Pin_12									//电磁阀--M19 
#define     	SolenoidValve_M19_GPIOX										GPIOB	
#define     	SolenoidValve_M19_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOB
#define 			SolenoidValve_M19(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M19_GPIOX, SolenoidValve_M19_PIN)) : (GPIO_ResetBits(SolenoidValve_M19_GPIOX, SolenoidValve_M19_PIN)))

#define     	SolenoidValve_M20_PIN											GPIO_Pin_11									//电磁阀--M20
#define     	SolenoidValve_M20_GPIOX										GPIOB
#define     	SolenoidValve_M20_RCC_APB2Periph_GPIOX	    RCC_APB2Periph_GPIOB
#define 			SolenoidValve_M20(x)   										((x) ? (GPIO_SetBits(SolenoidValve_M20_GPIOX, SolenoidValve_M20_PIN)) : (GPIO_ResetBits(SolenoidValve_M20_GPIOX, SolenoidValve_M20_PIN)))




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



