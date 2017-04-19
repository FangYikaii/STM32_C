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

uint16_t FrqTimCout=0;						//输出频率计数

//========================================
//变量定义


uint8_t digitalcode[12]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0,0x40}; //字模
                    //0   1    2    3    4    5    6     7    8    9  空格 -//共阴笔画
uint8_t seg_table_p[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};//共阳笔画
uint8_t seg_table_n[]={0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};//共阴笔画

uint8_t presscount=0;				//按压次数

uint8_t uplaserstd=0x40;
uint8_t downlaserstd=0x40;	//选上下2个点作为来回按压次数的判断点，根据需要调节
uint8_t uplaserstatus=0;
uint8_t downlaserstatus=0;	//上下点状态值

uint8_t uplaserflag=0;
uint8_t downlaserflag=0;		//上下点状态变化标志


//	uint16_t mi;																																//临时变量
uint8_t data[4]; //定义四位显示字符串数组，显示按压次数
uint8_t cycles=0;	
	
uint8_t LaserModel_Input1;
uint8_t LaserModel_Input2;
	
//========================================
//函数声明
extern void SystemInit(void);
void Init_System(void);    


void LedRowOut(u32 Data);
void S74HC595Send(u8 data);
void S74HC595Load(void);


void delay(uint16_t k);     //延时程序
//========================================
//主函数
int main(void) 
{

	//**********************************************************************
	//系统初始化
	Init_System();


	//**********************************************************************
	//
	while(1)
	{		
			if(Btn_Start_Read)		//开始--按钮
			{
				presscount=0;				//按压次数
				uplaserflag=0;
				downlaserflag=0;		//上下点状态变化标志
				flagA.Bits.Start_Flag=TRUE;			
				
				
				
			}		

			if(Btn_Stop_Read)		//停止--按钮
			{
				flagA.Bits.Start_Flag=FALSE;		


				
				
			}		
			
			
			if(flagA.Bits.Start_Flag)		//CPR 训练中
			{
					LaserModel_Input1=I2C_RecData_PCF8574(0x71);
					LaserModel_Input2=I2C_RecData_PCF8574(0x73);						//注意管脚的短接冒是否连接正确	
					//此处假设初始状态时，上部指定的激光模块被激光照到，下部指定的激光模块没被照到
					if(LaserModel_Input1 && uplaserstd)											//所选管脚高电平 -- 上标准  激光照射
					{
						if(uplaserstatus==0)			//状态改变
						{
							uplaserstatus=1;							
						}						
						
					}
					else		//无激光照射
					{
						if(uplaserstatus==1)			//状态改变
						{
							uplaserstatus=0;			
							uplaserflag=1;					//激光向下移动	
						}							
						
					}
					if(LaserModel_Input2 && downlaserstd)											//所选管脚高电平 -- 下标准  -- 激光照射
					{
						if(downlaserstatus==0)		//状态改变
						{
							downlaserstatus=1;
							downlaserflag=1;				//激光向下移动，移到指定位置
						}
					}		
					else
					{
						if(downlaserstatus==1)		//状态改变
						{
							downlaserstatus=0;
						}
					}	
				
					if(uplaserflag && downlaserflag)		//完成一次按压动作
					{
						presscount++;				//按压次数
						uplaserflag=0;
						downlaserflag=0;
					}
									
			}



			
		
		if(flagTIM5.Bits.T1sec_Flag)					//1S中断
		{
			flagTIM5.Bits.T1sec_Flag=FALSE;
					
			
			
		}		
		
		if(flagTIM5.Bits.T100msec_Flag)			//100ms中断
		{			
			flagTIM5.Bits.T100msec_Flag=FALSE;
			hc595_display(presscount);							//按压次数显示 -- 显示程序根据实际调整下
			

		}

	}

	
}


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
		IOControl_Init();   			//开始与停止等2个按钮的初始化
	  Init_PCF8574();				    //PCF8574的初始化程序
		hc595_init();							//数码管显示器初始化
		TIM5_Init();              //Timer5定时器初始化
	
	
	
//		MAX7219_Configuration();	//完成MAX7219的配置	
//		Uart_Init();              //Uart串口初始化	
//		USB_UDisk_Init();					//U盘初始化程序
//		ADC_Configuration();
//		FreqCap_Configuration(0xffff,(72-1));					//完成PULSE的配置,以1Mhz的频率计数	
//		CAN_Configuration();			//完成CAN的配置	
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
