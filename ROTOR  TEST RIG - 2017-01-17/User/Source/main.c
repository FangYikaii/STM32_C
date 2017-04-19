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
struct XGData XG;									//装载机参数信息
uint8_t ErrorMessage;							//错误信息

//========================================
//函数声明
extern void SystemInit(void);
void Init_System(void);    
  
void delay(uint16_t k);            //延时程序
void USARTSendSpeedData(void);     //串口发送当前速度输入
//========================================
//主函数
int main(void) 
{
	Init_System();  
	while (1)
  {	
		//**********************************************************************
		//1 S更新程序		
		if(flagTIM5.Bits.T1sec_Flag)
		{			
			flagTIM5.Bits.T1sec_Flag=FALSE;	
		
			//1s计数程序
			if(flagTIM5.Bits.Freq_1sec_Flag)
			{
				flagTIM5.Bits.Freq_1sec_Flag=FALSE;
				
				//串口2发送转速脉冲数目 3A 高八位 低八位 0D 0A
				USARTSendSpeedData();
			}		

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
		IOControl_Init();   		       				 	          //IO初始化
		MAX7219_Configuration();	      						  	  //完成MAX7219的配置
	  Init_PCF8574();				           							 	  //PCF8574的初始化程序
		Uart_Init();                                      //Uart串口初始化
		TIM5_Init();                                      //Timer5定时器初始化
//		USB_UDisk_Init();			      		                //U盘初始化程序
		ADC_Configuration();                              //模数转换初始化
		FreqCap_Configuration(0xffff,(72-1));				    	//完成PULSE的配置,以1Mhz的频率计数	
		CAN_Configuration();			                        //完成CAN的配置	
		RTCInit_Configuration();	                        //完成RTC的初始化设置
		NVIC_Configuration();                             //完成优先级配置
		
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


//========================================
//函数名称:void USARTSendSpeedData(void)
//函数功能: 串口发送当前速度输入
//入口参数: 无
//出口参数: 无     
//========================================
void USARTSendSpeedData(void)
{
	uchar i;
	uchar Data[RX_MaxSize];
	uint32_t speed;
	
	speed=TIM4FreqCap[1].pulse;
	TIM4FreqCap[1].pulse=0;							
	
	Data[0]=0x3A;
	Data[1]=speed>>8;     //高八位   
	Data[2]=0xFF&speed;   //低八位
	Data[3]=0x0D;
	Data[4]=0x0A;
	
	for(i=0;i<5;i++)
	{
		SerialPutChar(USART2,Data[i]);
	}
}
