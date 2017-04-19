/*
程序说明:  	
            IO操作程序 --- STM32F107VCT 
						
注意事项：	1、注意管脚的复用问题						
						
						
端口使用:		见Doc文件夹内说明
创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/

//========================================
//头文件
#include "main.h"

//========================================
//变量定义
struct ControllerData	ControllerInfo;
volatile union FlagController flagctrl;

//========================================
//函数声明
void StartUpGearDeal(void);								//启动时刻挡位处理程序
void GearScanVSDeal(void);								//挡位扫描和处理程序

void IOControl_Init(void);   							//IO初始化
uint8_t ReadGearLine_Status(void);					//读取换挡线束的电平
void GearToSolenoidValve(uint8_t gear,uint8_t status_brake);			//档位与电磁阀动作的转换

//========================================
//子模块函数


/***********************************************************************
函数名称：void IOControl_Init(void)
功    能：IOControl初始化
输入参数：无
输出参数：无
编写时间：2013.11.19
编 写 人：masterjay
注    意：
***********************************************************************/
void IOControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      //定义GPIO结构体	

//************************************
//采集信号IO
// 	
//	RCC_APB2PeriphClockCmd(DigIO_SCLK_RCC_APB2Periph_GPIOX, ENABLE);			//sclk				
//	GPIO_InitStructure.GPIO_Pin = DigIO_SCLK_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(DigIO_SCLK_GPIOX, &GPIO_InitStructure);		
//	
//	RCC_APB2PeriphClockCmd(DigIO_RCLK_RCC_APB2Periph_GPIOX, ENABLE);			//rclk				
//	GPIO_InitStructure.GPIO_Pin = DigIO_RCLK_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(DigIO_RCLK_GPIOX, &GPIO_InitStructure);		

//	RCC_APB2PeriphClockCmd(DigIO_DATA_RCC_APB2Periph_GPIOX, ENABLE);			//data				
//	GPIO_InitStructure.GPIO_Pin = DigIO_DATA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//GPIO_Mode_Out_PP GPIO_Mode_AF_PP --推拉输出 GPIO_Mode_AF_OD -- 开漏输出  
//	GPIO_Init(DigIO_DATA_GPIOX, &GPIO_InitStructure);		
//	
//	DigIO_DATA_OUT(0);		
//	DigIO_SCLK_OUT(0); 
//	DigIO_RCLK_OUT(0);	

	
	
//// 	RCC_APB2PeriphClockCmd(DigIO_IO1_RCC_APB2Periph_GPIOX, ENABLE);								
//// 	GPIO_InitStructure.GPIO_Pin = DigIO_IO1_PIN;
//// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//// 	GPIO_Init(DigIO_IO1_GPIOX, &GPIO_InitStructure);

 	RCC_APB2PeriphClockCmd(Btn_Start_RCC_APB2Periph_GPIOX, ENABLE);							
 	GPIO_InitStructure.GPIO_Pin = Btn_Start_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(Btn_Start_GPIOX, &GPIO_InitStructure); 	
	
	
//////	RCC_APB2PeriphClockCmd(Btn_Start_RCC_APB2Periph_GPIOX, ENABLE);					
//////	GPIO_InitStructure.GPIO_Pin = Btn_Start_PIN;
//////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//////	GPIO_Init(Btn_Start_GPIOX, &GPIO_InitStructure);	
////	
////	
////	
 	RCC_APB2PeriphClockCmd(Btn_Stop_RCC_APB2Periph_GPIOX, ENABLE);								
 	GPIO_InitStructure.GPIO_Pin = Btn_Stop_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(Btn_Stop_GPIOX, &GPIO_InitStructure);
	
//// 	
//// 	RCC_APB2PeriphClockCmd(DigIO_IO4_RCC_APB2Periph_GPIOX, ENABLE);								
//// 	GPIO_InitStructure.GPIO_Pin = DigIO_IO4_PIN;
//// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//// 	GPIO_Init(DigIO_IO4_GPIOX, &GPIO_InitStructure);
//// 	
//////************************************
//////电磁阀控制信号IO

////	RCC_APB2PeriphClockCmd(SolenoidValve_M1_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M1
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M1_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M1_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M2_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M2
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M2_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M2_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M3_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M3
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M3_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M3_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M4_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M4
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M4_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M4_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M5_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M5
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M5_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M5_GPIOX, &GPIO_InitStructure);	


////	RCC_APB2PeriphClockCmd(SolenoidValve_M6_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M6
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M6_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M6_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M7_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M7
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M7_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M7_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M8_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M8
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M8_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M8_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M9_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M9
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M9_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M9_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M10_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M10
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M10_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M10_GPIOX, &GPIO_InitStructure);	
////	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M11_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M11
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M11_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M11_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M12_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M12
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M12_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M12_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M13_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M13
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M13_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M13_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M14_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M14
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M14_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M14_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M15_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M15
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M15_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M15_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M16_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M16
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M16_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M16_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M17_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M17
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M17_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M17_GPIOX, &GPIO_InitStructure);
////	
////	RCC_APB2PeriphClockCmd(SolenoidValve_M18_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M18
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M18_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M18_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M19_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M19
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M19_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_Init(SolenoidValve_M19_GPIOX, &GPIO_InitStructure);	

////	RCC_APB2PeriphClockCmd(SolenoidValve_M20_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M20
////	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M20_PIN;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
////	GPIO_Init(SolenoidValve_M20_GPIOX, &GPIO_InitStructure);	
////	
////	SolenoidValve_M1(0);
////	SolenoidValve_M2(0);
////	SolenoidValve_M3(0);
////	SolenoidValve_M4(0);
////	SolenoidValve_M5(0);
////	SolenoidValve_M6(0);
////	SolenoidValve_M7(0);
////	SolenoidValve_M8(0);
////	SolenoidValve_M9(0);
////	SolenoidValve_M10(0);	
////	SolenoidValve_M11(0);
////	SolenoidValve_M12(0);
////	SolenoidValve_M13(0);
////	SolenoidValve_M14(0);
////	SolenoidValve_M15(0);
////	SolenoidValve_M16(0);
////	SolenoidValve_M17(0);
////	SolenoidValve_M18(0);
////	SolenoidValve_M19(0);
////	SolenoidValve_M20(0);		
////	
	
}

