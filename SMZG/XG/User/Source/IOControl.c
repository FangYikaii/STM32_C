/*
程序说明:  	
            IO操作程序 --- STM32F107VCT 
						
注意事项：	1、注意管脚的复用问题						
						
						
端口使用:		见Doc文件夹内说明
创建者:     FangYikaii   
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
void GearScanVSDeal_HandShiftMode(void);	//挡位扫描和处理程序
void GearScanVSDeal_AutoShiftMode(void);
void IOControl_Init(void);   							//IO初始化
void ReadGearLine_Status(void);					//读取换挡线束的电平
void GearToSolenoidValve(uint8_t gear,uint8_t status_brake);			//档位与电磁阀动作的转换

//========================================
//子模块函数


/***********************************************************************
函数名称：void StartUpGearDeal(void)
功    能：启动时刻挡位处理程序
输入参数：无
输出参数：无
编写时间：2014.05.29
编 写 人：FangYikaii
注    意：
***********************************************************************/
void StartUpGearDeal(void)
{
	  if(flagctrl.Bits.ScanTimeFlag)		//10/100ms扫描一次											
		{
			flagctrl.Bits.ScanTimeFlag=FALSE;	
			ReadGearLine_Status();						//档位信息读取		
			ControllerInfo.ProcessGear=ControllerInfo.NowGear;			//过程挡位显示，未真正启动阀门切换，阀门处于N1挡
			if(!flagctrl.Bits.StartUpFromNx)												//初始化时启动信号从N开始判断
			{
				if(ControllerInfo.NowGear/0x10==2)										//启动前见到N信号
				{
						flagctrl.Bits.StartUpFromNx=TRUE;									//允许装载机启动	
						ErrorMessage=0xEE;																//错误显示信息-- -- -- 无错误										
//					if(ControllerInfo.NowGear%0x10<2)										//N+1或N+P
//					{
//						flagctrl.Bits.StartUpFromNx=TRUE;									//允许装载机启动	
//						ErrorMessage=0xEE;																//错误显示信息-- -- -- 无错误
//					}
//					else
//					{
//						ErrorMessage=0xFF;																//错误显示信息--EE	
//					}
				}
				else
				{
					ErrorMessage=0xFF;																	//错误显示信息--EE					
				}
			}
		}
		if(flagTIM5.Bits.T1sec_Flag)															//显示错误信息 EE-- 挡位未从N+1或N+P启动
		{
			flagTIM5.Bits.T1sec_Flag=FALSE;	
			ErrorDisplay(ErrorMessage);		
		}			
}

/***********************************************************************
函数名称：void GearScanVSDeal_HandShiftMode(void)
功    能：手动换挡时档位扫描和处理程序
输入参数：无
输出参数：无
编写时间：2014.05.29
编 写 人：FangYikaii
注    意：
***********************************************************************/
void GearScanVSDeal_HandShiftMode(void)
{		
	ReadGearLine_Status();					//档位信息读取
	GearToSolenoidValve(ControllerInfo.NowGear,ControllerInfo.NowBrake);
	
}

/***********************************************************************
函数名称：void GearScanVSDeal_AutoShiftMode(void)
功    能：手动换挡时档位扫描和处理程序
输入参数：无
输出参数：无
编写时间：2014.05.29
编 写 人：FangYikaii
注    意：
***********************************************************************/
void GearScanVSDeal_AutoShiftMode(void)
{		
	ReadGearLine_Status();					//档位信息读取
	if(ControllerInfo.NowBrake==0)	//是否刹车 == 否
	{
		if(ControllerInfo.NowGear/0x10==0)										//前进档
		{	
			
		}
		else if(ControllerInfo.NowGear/0x10==1)										//后退档
		{
			
		}			
	}
	else //是否刹车 == 是
	{
		
	}	
	
	GearToSolenoidValve(ControllerInfo.NowGear,ControllerInfo.NowBrake);
	
}

/***********************************************************************
函数名称：void IOControl_Init(void)
功    能：IOControl初始化
输入参数：无
输出参数：无
编写时间：2013.11.19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void IOControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      //定义GPIO结构体	

// //************************************
// //档位采集信号IO
 	
 	RCC_APB2PeriphClockCmd(DIIN1_M1_RCC_APB2Periph_GPIOX, ENABLE);								//M1
 	GPIO_InitStructure.GPIO_Pin = DIIN1_M1_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN1_M1_GPIOX, &GPIO_InitStructure);

 	RCC_APB2PeriphClockCmd(DIIN2_M2_RCC_APB2Periph_GPIOX, ENABLE);								//M2	
 	GPIO_InitStructure.GPIO_Pin = DIIN2_M2_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN2_M2_GPIOX, &GPIO_InitStructure);	
 	
 	RCC_APB2PeriphClockCmd(DIIN3_M3_RCC_APB2Periph_GPIOX, ENABLE);								//M3
 	GPIO_InitStructure.GPIO_Pin = DIIN3_M3_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN3_M3_GPIOX, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(DIIN4_M4_RCC_APB2Periph_GPIOX, ENABLE);								//M4
 	GPIO_InitStructure.GPIO_Pin = DIIN4_M4_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN4_M4_GPIOX, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(DIIN5_AS_RCC_APB2Periph_GPIOX, ENABLE);								 	//AS
 	GPIO_InitStructure.GPIO_Pin = DIIN5_AS_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN5_AS_GPIOX, &GPIO_InitStructure);

 	RCC_APB2PeriphClockCmd(DIIN6_RF_RCC_APB2Periph_GPIOX, ENABLE);								//RF
 	GPIO_InitStructure.GPIO_Pin = DIIN6_RF_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN6_RF_GPIOX, &GPIO_InitStructure);
	
 	RCC_APB2PeriphClockCmd(DIIN7_BR_RCC_APB2Periph_GPIOX, ENABLE);								//BR
 	GPIO_InitStructure.GPIO_Pin = DIIN7_BR_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN7_BR_GPIOX, &GPIO_InitStructure);	
	
 	RCC_APB2PeriphClockCmd(DIIN8_XX_RCC_APB2Periph_GPIOX, ENABLE);								//XX
 	GPIO_InitStructure.GPIO_Pin = DIIN8_XX_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(DIIN8_XX_GPIOX, &GPIO_InitStructure);		
	
//************************************
//电磁阀控制信号IO

	RCC_APB2PeriphClockCmd(DOOUT1_M1_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M1
	GPIO_InitStructure.GPIO_Pin = DOOUT1_M1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT1_M1_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DOOUT2_M2_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M2
	GPIO_InitStructure.GPIO_Pin = DOOUT2_M2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT2_M2_GPIOX, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(DOOUT3_M3_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M3
	GPIO_InitStructure.GPIO_Pin = DOOUT3_M3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT3_M3_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DOOUT4_M4_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M4
	GPIO_InitStructure.GPIO_Pin = DOOUT4_M4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT4_M4_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DOOUT5_BR_RCC_APB2Periph_GPIOX, ENABLE);								 //BR
	GPIO_InitStructure.GPIO_Pin = DOOUT5_BR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT5_BR_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DOOUT6_RF_RCC_APB2Periph_GPIOX, ENABLE);								 //RF
	GPIO_InitStructure.GPIO_Pin = DOOUT6_RF_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT6_RF_GPIOX, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(DOOUT7_AS_RCC_APB2Periph_GPIOX, ENABLE);								 //AS
	GPIO_InitStructure.GPIO_Pin = DOOUT7_AS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT7_AS_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(DOOUT8_XX_RCC_APB2Periph_GPIOX, ENABLE);								 //XX
	GPIO_InitStructure.GPIO_Pin = DOOUT8_XX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DOOUT8_XX_GPIOX, &GPIO_InitStructure);	
	
	DOOUT1_M1(0);			
	DOOUT2_M2(0);			
	DOOUT3_M3(0);		
	DOOUT4_M4(0);	
	
	DOOUT5_BR(0);	
	DOOUT6_RF(0);  
	DOOUT7_AS(0);  	
	DOOUT8_XX(0);  	
	
}

/***********************************************************************
函数名称：uint8_t ReadGearLine_Status(void)
功    能：读取换挡线束的电平
输入参数：无
输出参数：档位值
编写时间：2013.11.19
编 写 人：FangYikaii
注    意：
***********************************************************************/
/***********************************************************************
各档位对应采集值列表
		前进档 V     空档N 			 倒退档R     KD
P   0x14				 0x34				 0x24				 0xXX & 0x01=0x00
1   0x10				 0x30				 0x20				 0xXX & 0x01=0x00		
2   0x12				 0x32				 0x22				 0xXX & 0x01=0x00
3   0x16				 0x36				 0x26 			 0xXX & 0x01=0x00
4   0x1E				 0x3E				 0x2E 			 0xXX & 0x01=0x00

***********************************************************************/

void ReadGearLine_Status(void)
{
	uint8_t gear=0;
	uint8_t result=0;
	//*******************************************	
 	if(DIIN1_M1_Read)//DIIN1 -- 电磁阀M1
 	{
 		result+=1;	
//		DOOUT1_M1(1);
 	}	
	else
	{
//		DOOUT1_M1(0);		
	}
   result<<=1;
 	if(DIIN2_M2_Read)//DIIN2 -- 电磁阀M2
 	{
 		result+=1;	
//		DOOUT2_M2(1);		
 	}	
	else
	{
//		DOOUT2_M2(0);			
	}
   result<<=1; 
	
 	if(DIIN3_M3_Read)//DIIN3 -- 电磁阀M3
 	{
 		result+=1;	
//		DOOUT3_M3(1);			
 	}	
	else
	{
//		DOOUT3_M3(0);			
	}
   result<<=1;	
	
 	if(DIIN4_M4_Read)//DIIN4 -- 电磁阀M4
 	{
 		result+=1;		
//		DOOUT4_M4(1);			
 	}	
	else
	{
//		DOOUT4_M4(0);			
	}
	//*******************************************
 	if(DIIN5_AS_Read)//DIIN5 -- AS  空挡信号
 	{	
//		DOOUT7_AS(1);			//AS  空挡信号
 	}
	else
	{
//		DOOUT7_AS(0);				
	}
	
 	if(DIIN6_RF_Read)//DIIN6 -- RF  倒挡信号
 	{	
//		DOOUT6_RF(1);		//RF  倒挡信号	
 	}
	else
	{
//		DOOUT6_RF(0);				
	}
	
 	if(DIIN7_BR_Read)//DIIN7 -- BR	制动信号
 	{
		ControllerInfo.NowBrake=1;
//		DOOUT5_BR(1);		//BR	制动信号	
 	}
	else
	{
		ControllerInfo.NowBrake=0;		
//		DOOUT5_BR(0);				
	}	
	
	//*******************************************	

	//XG.PumpSpeed=result;  辅助显示，调试使用
	switch(result)
	{
 			case 0x07 :						//1档  (0111)
 			{
 				gear=1;
 				break;				
 			}
 			case 0x0d :						//2档  (1101)
 			{
 				gear=2;
 				break;					
 			}	
 			case 0x03 :						//3档  (0011)
 			{
 				gear=3;
 				break;					
 			}	
 			case 0x09 :						//4档 (1001)
 			{
 				gear=4;
 				break;					
 			}
 			case 0x02 :						//5档  (0010)
 			{
 				gear=5;
 				break;					
 			}	
 			case 0x08 :						//6档  (1000)
 			{
 				gear=6;
 				break;					
 			}			
 			case 0x0F :						//倒1档  (1111)
 			{
 				gear=1+0X10;
 				break;					
 			}
 			case 0x0B :						//倒2档  (1011)
 			{
 				gear=2+0X10;
 				break;					
 			}	
 			case 0x0A :						//倒3档  (1010)
 			{
 				gear=3+0X10;
 				break;					
 			}	
 			case 0x00 :						//空挡   (0000)
 			{
 				gear=0+0X20;
 				break;					
 			}				
 			default :
			{
 				gear=0+0X20;				
 				break;						
			}	
	}	
	ControllerInfo.NowGear=gear;
	if(ControllerInfo.NowBrake==1)		//制动信号
	{
		ControllerInfo.ProcessGear=0x20;				
	}
	else
	{
		ControllerInfo.ProcessGear=ControllerInfo.NowGear;		
	}
}
/***********************************************************************
函数名称：void GearToSolenoidValve(uint8_t gear,uint8_t status_brake)
功    能：档位与电磁阀动作的转换
输入参数：档位值
输出参数：无
编写时间：2013.11.19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void GearToSolenoidValve(uint8_t gear,uint8_t status_brake)
{
			if(gear/0x10==0)										//前进档
			{
				DOOUT7_AS(0);			//AS  空挡信号
				DOOUT6_RF(0);			//RF  倒挡信号	
				switch(gear%0x10)
				{
					case 0x01:											//前进档--1档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(0);
							DOOUT2_M2(1);							
							DOOUT3_M3(1);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(1);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);					
							
						}								
						break;						
					}
					case 0x02:											//前进档--2档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(1);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(1);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);					
							
						}							
						break;						
					}						
					case 0x03:											//前进档--3档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(1);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);					
							
						}			
					}
					case 0x04:											//前进档--4档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);					
							
						}					
						break;						
					}
					case 0x05:											//前进档--5档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(1);
							DOOUT4_M4(0);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(0);					
							
						}					
						break;						
					}					
					case 0x06:											//前进档--6档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(0);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(0);					
							
						}					
						break;						
					}					
					
					default:						
					{						
						break;						
					}						
				}					
			}
			else if(gear/0x10==1)								//倒退档
			{
				DOOUT7_AS(0);			//AS  空挡信号
				DOOUT6_RF(1);			//RF  倒挡信号	
				switch(gear%0x10)
				{					
					case 0x01:											//倒退档--1档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(1);							
							DOOUT3_M3(1);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(1);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);					
							
						}						
						break;						
					}
					case 0x02:											//倒退档--2档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(0);							
							DOOUT3_M3(1);
							DOOUT4_M4(1);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(1);							
						}						
						break;						
					}						
					case 0x03:											//倒退档--3档
					{
						if(status_brake==0)						//无制动信号
						{
							DOOUT1_M1(1);
							DOOUT2_M2(0);							
							DOOUT3_M3(1);
							DOOUT4_M4(0);							
						}
						else
						{
							DOOUT1_M1(0);
							DOOUT2_M2(0);							
							DOOUT3_M3(0);
							DOOUT4_M4(0);							
						}							
						break;						
					}				
					default:											//
					{							
						break;						
					}						
				}					
				
			}
			else if(gear/0x10==2)						//空档
			{
				DOOUT7_AS(1);			//AS  空挡信号
				DOOUT6_RF(0);			//RF  倒挡信号	
				DOOUT1_M1(0);
				DOOUT2_M2(0);							
				DOOUT3_M3(0);
				DOOUT4_M4(0);		
			}			
	
}
