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
函数名称：void StartUpGearDeal(void)
功    能：启动时刻挡位处理程序
输入参数：无
输出参数：无
编写时间：2014.05.29
编 写 人：masterjay
注    意：
***********************************************************************/
void StartUpGearDeal(void)
{
	  if(flagctrl.Bits.ScanTimeFlag)		//10/100ms扫描一次											
		{
			flagctrl.Bits.ScanTimeFlag=FALSE;	
			ControllerInfo.NowGear=ReadGearLine_Status();						//档位信息读取		
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
函数名称：void GearScanVSDeal(void)
功    能：档位扫描和处理程序
输入参数：无
输出参数：无
编写时间：2014.05.29
编 写 人：masterjay
注    意：
***********************************************************************/
void GearScanVSDeal(void)
{		
	//******************************************************************************
	//档位等基本信息获取过程
	ControllerInfo.NowGear=ReadGearLine_Status();					//档位信息读取
	ControllerInfo.NowBrake=BrakeControl_Read;						//刹车制动信息读取
	if(ControllerInfo.NowGear==0x30)											//KD键
	{
		if(!flagctrl.Bits.KDScan_Flag)											//KD键之前未按下
		{
			flagctrl.Bits.KDScan_Flag=TRUE;										//KD键按下，等待复位
			if(!flagctrl.Bits.KDchange)												//KD键未生效前
			{
				flagctrl.Bits.KDchange=TRUE;										
				flagctrl.Bits.gearchange=TRUE;	
				ControllerInfo.gearCGcount=0;
				RdData.GearInfo=ControllerInfo.NowGear;					//用于挡位显示				
			}	
			else																							//当前已处于KD键状态
			{
				flagctrl.Bits.KDchange=FALSE;										
				flagctrl.Bits.gearchange=TRUE;	
				ControllerInfo.gearCGcount=0;
				RdData.GearInfo=ControllerInfo.NowGear;
				ControllerInfo.gearinfo=ControllerInfo.NowGear;	//记录挡位信息-- 注意与全面的差别
			}					
		}
		
	}	
	else
	{
		flagctrl.Bits.KDScan_Flag=FALSE;												//KD键复位处理
		if(ControllerInfo.NowGear!=ControllerInfo.gearinfo)			//档位变化处理
		{		
			ControllerInfo.gearCGcount++;													//当前挡位扫描次数，大于3时表示挡位变化稳定
			flagctrl.Bits.KDchange=FALSE;
			if(ControllerInfo.gearCGcount>=3)
			{
				flagctrl.Bits.gearchange=TRUE;	
				ControllerInfo.gearCGcount=0;
				ControllerInfo.gearinfo=ControllerInfo.NowGear;
				RdData.GearInfo=ControllerInfo.NowGear;	
			}
		}			
	}
	if(ControllerInfo.NowBrake!=ControllerInfo.brakeinfo)			//制动信号变化处理
	{
		ControllerInfo.brakeCGcount++;
		if(ControllerInfo.brakeCGcount>=3)
		{
			flagctrl.Bits.brakechange=TRUE;
			ControllerInfo.brakeCGcount=0;
			ControllerInfo.brakeinfo=ControllerInfo.NowBrake;
		}
	}	
}

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

// //************************************
// //档位采集信号IO
 	
 	RCC_APB2PeriphClockCmd(GearLine_Brown_RCC_APB2Periph_GPIOX, ENABLE);								//线束--3 棕色
 	GPIO_InitStructure.GPIO_Pin = GearLine_Brown_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_Brown_GPIOX, &GPIO_InitStructure);

 	RCC_APB2PeriphClockCmd(GearLine_Purple_RCC_APB2Periph_GPIOX, ENABLE);								//线束--4 紫色	
 	GPIO_InitStructure.GPIO_Pin = GearLine_Purple_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_Purple_GPIOX, &GPIO_InitStructure);	
 	
 	RCC_APB2PeriphClockCmd(GearLine_White_RCC_APB2Periph_GPIOX, ENABLE);								//线束--5 白色
 	GPIO_InitStructure.GPIO_Pin = GearLine_White_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_White_GPIOX, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(GearLine_Yellow_RCC_APB2Periph_GPIOX, ENABLE);								//线束--6 黄色
 	GPIO_InitStructure.GPIO_Pin = GearLine_Yellow_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_Yellow_GPIOX, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(GearLine_Blue_RCC_APB2Periph_GPIOX, ENABLE);								 	//线束--7 蓝色
 	GPIO_InitStructure.GPIO_Pin = GearLine_Blue_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_Blue_GPIOX, &GPIO_InitStructure);

 	RCC_APB2PeriphClockCmd(GearLine_Green_RCC_APB2Periph_GPIOX, ENABLE);								//线束--8 绿色
 	GPIO_InitStructure.GPIO_Pin = GearLine_Green_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(GearLine_Green_GPIOX, &GPIO_InitStructure);
	
//************************************
//电磁阀控制信号IO

	RCC_APB2PeriphClockCmd(SolenoidValve_M1_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M1 红色
	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(SolenoidValve_M1_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(SolenoidValve_M2_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M2 绿色
	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SolenoidValve_M2_GPIOX, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(SolenoidValve_M3_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M3 兰色
	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SolenoidValve_M3_GPIOX, &GPIO_InitStructure);	

	RCC_APB2PeriphClockCmd(SolenoidValve_M4_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M4 黄色
	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M4_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SolenoidValve_M4_GPIOX, &GPIO_InitStructure);	

	RCC_APB2PeriphClockCmd(SolenoidValve_M5_RCC_APB2Periph_GPIOX, ENABLE);								 //电磁阀--M5 白色
	GPIO_InitStructure.GPIO_Pin = SolenoidValve_M5_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(SolenoidValve_M5_GPIOX, &GPIO_InitStructure);	

	SolenoidValve_M1(0);
	SolenoidValve_M2(0);
	SolenoidValve_M3(0);
	SolenoidValve_M4(0);
	SolenoidValve_M5(0);
	
// //************************************
// //制动控制采集信息IO	

 	RCC_APB2PeriphClockCmd(BrakeControl_RCC_APB2Periph_GPIOX, ENABLE);								 //制动控制--桔色
 	GPIO_InitStructure.GPIO_Pin = BrakeControl_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
 	GPIO_Init(BrakeControl_GPIOX, &GPIO_InitStructure);		
	


//************************************
//空档启动联锁与倒车警示灯控制信息IO

	RCC_APB2PeriphClockCmd(AsternAlarmLED_RCC_APB2Periph_GPIOX, ENABLE);								 //倒车警示灯--兰色
	GPIO_InitStructure.GPIO_Pin = AsternAlarmLED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(AsternAlarmLED_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(NeutralStartInterlock_RCC_APB2Periph_GPIOX, ENABLE);								 //空档启动联锁--绿色
	GPIO_InitStructure.GPIO_Pin = NeutralStartInterlock_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(NeutralStartInterlock_GPIOX, &GPIO_InitStructure);	

	AsternAlarmLED(0);
	NeutralStartInterlock(0);	

//************************************
//预留IO

	RCC_APB2PeriphClockCmd(ReservedIO_RCC_APB2Periph_GPIOX, ENABLE);								 //预留IO--可控制电磁阀
	GPIO_InitStructure.GPIO_Pin = ReservedIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			
	GPIO_Init(ReservedIO_GPIOX, &GPIO_InitStructure);		
	
	ReservedIO(0);
	
	
	
}

/***********************************************************************
函数名称：uint8_t ReadGearLine_Status(void)
功    能：读取换挡线束的电平
输入参数：无
输出参数：档位值
编写时间：2013.11.19
编 写 人：masterjay
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

uint8_t ReadGearLine_Status(void)
{
	uint8_t gear=0;
	uint8_t result=0;
 	if(GearLine_Purple_Read)//线束--4 紫色
 	{
 		result+=1;		
 	}	
   result<<=1;
 	if(GearLine_Brown_Read)	//线束--3 棕色
 	{
 		result+=1;
 	}
   result<<=1;
 	if(GearLine_Green_Read)//线束--8 绿色	
 	{
 		result+=1;		
 	}	
   result<<=1;
 	if(GearLine_Blue_Read)//线束--7 蓝色	
 	{
 		result+=1;		
 	}	
   result<<=1;	
 	if(GearLine_Yellow_Read)//线束--6 黄色	
 	{
 		result+=1;		
 	}	
   result<<=1;
 	
 	if(GearLine_White_Read)//线束--5 白色
 	{
 		result+=1;		
 	}
 	if(result%2==1)			 				//KD
 	{
 		gear=0x30;
 	}
 	else
 	{
 		switch(result%0x10)
 		{
 			case 0x04 :					 //P档
 			{
 				gear=0;
 				break;				
 			}			
 			case 0x00 :						//1档
 			{
 				gear=1;
 				break;				
 			}
 			case 0x02 :						//2档
 			{
 				gear=2;
 				break;					
 			}	
 			case 0x06 :						//3档
 			{
 				gear=3;
 				break;					
 			}	
 			case 0x0E :						//4档
 			{
 				gear=4;
 				break;					
 			}
 			default :
 				break;		
 		}
 		switch(result/0x10)
 		{
 			case 0x03 :					  //空档 -- 0x2x
 			{
 				gear=gear+0x20;
 				break;				
 			}	
 			case 0x02 :					  //倒退档	-- 0x1x
 			{
 				gear=gear+0x10;
 				break;				
 			}	
 			case 0x01 :					  //前进档	-- 0x0x
 			{
 				gear=gear+0x00;
 				break;				
 			}	
 			default :
 			break;	
 		}	
 	}
  return (gear);
}
/***********************************************************************
函数名称：void GearToSolenoidValve(uint8_t gear,uint8_t status_brake)
功    能：档位与电磁阀动作的转换
输入参数：档位值
输出参数：无
编写时间：2013.11.19
编 写 人：masterjay
注    意：
***********************************************************************/
void GearToSolenoidValve(uint8_t gear,uint8_t status_brake)
{
		if(flagctrl.Bits.KDchange)				//KD键使能--1挡保持			
		{
			if(gear/0x10==0)								//前进档
			{	
				AsternAlarmLED(0); 						//倒车警示灯
				NeutralStartInterlock(0); 		//空档启动联锁					
				if(status_brake==0)						//无制动信号
				{
					SolenoidValve_M5(0);			
					SolenoidValve_M4(1);
					SolenoidValve_M3(1);				//有无制动差别	
					SolenoidValve_M2(1);					
					SolenoidValve_M1(0);
				}
				else
				{
					SolenoidValve_M5(0);			
					SolenoidValve_M4(1);
					SolenoidValve_M3(0);				//有无制动差别		
					SolenoidValve_M2(1);					
					SolenoidValve_M1(0);					
					
				}		
			}
			else if(gear/0x10==1)						//倒退档
			{
				AsternAlarmLED(1); 						//倒车警示灯
				NeutralStartInterlock(0); 		//空档启动联锁				
				if(status_brake==0)						//无制动信号
				{
					SolenoidValve_M5(0);			
					SolenoidValve_M4(1);
					SolenoidValve_M3(1);				//有无制动差别	
					SolenoidValve_M2(1);					
					SolenoidValve_M1(1);				//有无制动差别	
				}
				else
				{
					SolenoidValve_M5(0);			
					SolenoidValve_M4(1);
					SolenoidValve_M3(0);				//有无制动差别		
					SolenoidValve_M2(1);					
					SolenoidValve_M1(0);				//有无制动差别						
					
				}									
			}	
			else if(gear/0x10==2)						//空挡
			{
				AsternAlarmLED(0); 						//倒车警示灯
				NeutralStartInterlock(1); 		//空档启动联锁				

				SolenoidValve_M5(0);			
				SolenoidValve_M4(1);
				SolenoidValve_M3(0);
				SolenoidValve_M2(1);					
				SolenoidValve_M1(0);										
			}	
			
		}
		else																	//KD键未使能情况	
		{
			if(gear/0x10==0)										//前进档
			{
				AsternAlarmLED(0); 								//倒车警示灯
				NeutralStartInterlock(0); 				//空档启动联锁
				switch(gear%0x10)
				{
					case 0x00:											//前进档--P挡
					{
						
						SolenoidValve_M5(0);					//停车		
						SolenoidValve_M4(0);
						SolenoidValve_M3(0);				
						SolenoidValve_M2(0);					
						SolenoidValve_M1(0);						
						
						
//						if(status_brake==0)						//无制动信号 == 1挡
//						{
//							SolenoidValve_M5(0);			
//							SolenoidValve_M4(1);
//							SolenoidValve_M3(1);				//有无制动差别	
//							SolenoidValve_M2(1);					
//							SolenoidValve_M1(0);
//						}
//						else
//						{
//							SolenoidValve_M5(0);			
//							SolenoidValve_M4(1);
//							SolenoidValve_M3(0);				//有无制动差别		
//							SolenoidValve_M2(1);					
//							SolenoidValve_M1(0);					
//							
//						}						
						break;						
					}
					case 0x01:											//前进档--1档
					{
						if(status_brake==0)						//无制动信号
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(1);				//有无制动差别	
							SolenoidValve_M2(1);					
							SolenoidValve_M1(0);
						}
						else
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(0);				//有无制动差别		
							SolenoidValve_M2(1);					
							SolenoidValve_M1(0);					
							
						}								
						break;						
					}
					case 0x02:											//前进档--2档
					{
						if(status_brake==0)						//无制动信号
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(1);				//有无制动差别	
							SolenoidValve_M2(0);					
							SolenoidValve_M1(0);
						}
						else
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(0);				//有无制动差别		
							SolenoidValve_M2(0);					
							SolenoidValve_M1(0);					
						}							
						break;						
					}						
					case 0x03:											//前进档--3档
					{
						SolenoidValve_M5(0);							
						SolenoidValve_M4(0);						
						SolenoidValve_M3(1);
						SolenoidValve_M2(0);
						SolenoidValve_M1(0);						
						break;						
					}
					case 0x04:											//前进档--4档
					{
						SolenoidValve_M5(0);								
						SolenoidValve_M4(0);		
						SolenoidValve_M3(0);
						SolenoidValve_M2(0);						
						SolenoidValve_M1(1);				
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
				AsternAlarmLED(1); 								//倒车警示灯
				NeutralStartInterlock(0); 				//空档启动联锁
				switch(gear%0x10)
				{
					case 0x00:											//倒退档--P挡	
					{
						
						SolenoidValve_M5(0);					//  == 停车
						SolenoidValve_M4(0);
						SolenoidValve_M3(0);				
						SolenoidValve_M2(0);					
						SolenoidValve_M1(0);									
//						if(status_brake==0)						//无制动信号  == 1挡
//						{
//							SolenoidValve_M5(0);			
//							SolenoidValve_M4(1);
//							SolenoidValve_M3(1);				//有无制动差别	
//							SolenoidValve_M2(1);					
//							SolenoidValve_M1(1);				//有无制动差别	
//						}
//						else
//						{
//							SolenoidValve_M5(0);			
//							SolenoidValve_M4(1);
//							SolenoidValve_M3(0);				//有无制动差别		
//							SolenoidValve_M2(1);					
//							SolenoidValve_M1(0);				//有无制动差别						
//							
//						}						
						break;	
					}						
					case 0x01:											//倒退档--1档
					{
						if(status_brake==0)						//无制动信号
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(1);				//有无制动差别	
							SolenoidValve_M2(1);					
							SolenoidValve_M1(1);				//有无制动差别	
						}
						else
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(0);				//有无制动差别		
							SolenoidValve_M2(1);					
							SolenoidValve_M1(0);				//有无制动差别						
							
						}							
						break;						
					}
					case 0x02:											//倒退档--2档
					{
						if(status_brake==0)						//无制动信号
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(1);				//有无制动差别	
							SolenoidValve_M2(0);					
							SolenoidValve_M1(1);				//有无制动差别	
						}
						else
						{
							SolenoidValve_M5(0);			
							SolenoidValve_M4(1);
							SolenoidValve_M3(0);				//有无制动差别		
							SolenoidValve_M2(0);					
							SolenoidValve_M1(0);				//有无制动差别						
							
						}							
						break;						
					}						
					case 0x03:											//倒退档--3档
					{
						SolenoidValve_M5(0);	
						SolenoidValve_M4(0);		
						SolenoidValve_M3(1);
						SolenoidValve_M2(0);						
						SolenoidValve_M1(1);							
						break;						
					}
					case 0x04:											//倒退档--4档
					{
						SolenoidValve_M5(0);	
						SolenoidValve_M4(0);		
						SolenoidValve_M3(1);
						SolenoidValve_M2(0);						
						SolenoidValve_M1(1);							
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
				AsternAlarmLED(0); 						//倒车警示灯
				NeutralStartInterlock(1); 		//空档启动联锁				
				switch(gear%0x10)
				{
					case 0x00:									//空档--P档
					{
						
						SolenoidValve_M5(0);			// == 停车				
						SolenoidValve_M4(0);						
						SolenoidValve_M3(0);				
						SolenoidValve_M2(0);						
						SolenoidValve_M1(0);							
						
//						SolenoidValve_M5(0);			// == 1挡							
//						SolenoidValve_M4(1);						
//						SolenoidValve_M3(0);				
//						SolenoidValve_M2(1);						
//						SolenoidValve_M1(0);				
						break;						
					}					
					
					case 0x01:									//空档--1档
					{
						SolenoidValve_M5(0);
						SolenoidValve_M4(1);
						SolenoidValve_M3(0);		
						SolenoidValve_M2(1);						
						SolenoidValve_M1(0);					
						break;						
					}
					case 0x02:									//空档--2档
					{
						SolenoidValve_M5(0);							
						SolenoidValve_M4(1);						
						SolenoidValve_M3(0);						
						SolenoidValve_M2(0);						
						SolenoidValve_M1(0);					
						break;						
					}						
					case 0x03:									//空档--3档
					{
						SolenoidValve_M1(0);
						SolenoidValve_M2(0);
						SolenoidValve_M3(0);
						SolenoidValve_M4(0);
						SolenoidValve_M5(0);							
						break;						
					}
					case 0x04:									//空档--4档
					{
						SolenoidValve_M1(0);
						SolenoidValve_M2(0);
						SolenoidValve_M3(0);
						SolenoidValve_M4(0);
						SolenoidValve_M5(0);							
						break;						
					}
					default:										//
					{						
						break;						
					}						
				}			
			}			
		}	
}




