/*
程序说明:  	
            PCF8574 IO拓展电路设计程序 --- STM32F107VCT 
						
						地址(0x70) -- LED显示控制地址
						地址(0x72) -- 按键读取控制地址
						地址(0x7A) -- 电磁阀控制芯片输出信号读取地址(未用)
						
						
注意事项：						
						1、PCF8574 的SCL、SDA、INT 等端口需要加10K电阻上拉，否则读写会出错。						
						2、PCF8574 中断设置放于EXTI.C内，注意IO口也需要初始化。
						3、PCF8574 中断扫描经常出现问题，建议使用循环扫描，视情况而定。

端口使用:		见Doc文件夹内说明
创建者:     FangYIkaii 
创建时间:   2017-01-20 
修改时间:   2017-01-20
*/
/*
##I2C-EEPROM的读写测试实验：与常见的EEPROM芯片AT2402为例 电擦除的ROM##
I2C(Inter-Integrated Circuit)集成电路总线：由数据线SDA和时钟SCL构成的串行总线，可以发送和接收数据，速度可以达到400Kbs
1.每个连接到总线的设备可以通过唯一的设备地址单独访问
2.串行8位双向数据传输，位速率下标准模式可以达到100kbs/s,快速模式400kbs/s,高速模式3.4Mb/s
3.总线长度最长7.6m，片上滤波器可以增加抗干扰能力�
4.总线是7位地址，最多128个设备；多主机系统，一条总线上可以同时有多个主机存在，通过冲突检测和延时防止数据不被破坏

工作状态：
开始信号：SCL高电平期间，SDA由高电平向低电平跳变，开始传送数据
结束信号：SCL高电平期间，SDA由低电平向高电平跳变，结束传送数据
应答信号：接收数据的IC接收8bit数据后，向发送数据的IC发出特定的低电平脉冲，表示收到数据，若未收到应答信号，判断受控单元故障

STM32F107的I2C接口特性如下：
1.芯片均有2个I2C总线接口，I2C1，I2C2
2.能够工作与多主模式或从模式，支持标准模式100kbs/s和快速模式400kbs/s,不支持7位寻址
3.内置硬件crc发生校验器，接收发送可以通过DMA

SCL:时序即为I2C协议中的时钟信号，由I2C接口根据时钟控制寄存器CCR控制，控制的参数主要为时钟频率
SDA:通过一系列数据控制架构，在将要发送的数据的基础上，根据协议添加各种起始信号、应答信号、地址信号，实现以I2C协议发送出去
*/
/*
<----PCF8574 地址 数据   ---->
            地址    数据
初始化     0x70      0x00
读键盘     0x73
   

<----PCF8574 键盘键值 配置  ---->
S1  -- 0xFE --  S9
S2  -- 0xFD --  S10
S3  -- 0xFB --  S11	
S4  -- 0xF7 --  S12

S5  -- 0xEF --  S13
S6  -- 0xDF --  S14
S7  -- 0xBF --  S15	
S8  -- 0x7F --  S16
*/

//******************************************
//头文件
#include "main.h"

//******************************************
//变量定义
uint8_t KeyValue_PCF8574;								//PCF8574键盘扫描值
volatile union FlagPCF8574 PCF8574;

//******************************************
//函数声明
void Init_PCF8574(void);																	//PCF8574的初始化程序

void I2C_Start_PCF8574(void);															//PCF8574的I2C开始信号
void I2C_Stop_PCF8574(void);															//PCF8574的I2C停止信号
void I2C_NOAck_PCF8574(void);															//PCF8574的I2C发送应答信息
void I2C_Ack_PCF8574(void);																//PCF8574的I2C接收应答信息
void I2C_Write1Byte_PCF8574(uint8_t data);									//PCF8574的I2C写一个字节程序
uint8_t I2C_Rec1Byte_PCF8574(void);													//PCF8574的I2C接收一个字节程序

void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data);				//PCF8574的I2C写数据程序
uint8_t I2C_RecData_PCF8574(uint8_t addr);										//PCF8574的I2C读数据程序
void KeyScan_PCF8574(void);																//PCF8574的键盘扫描程序

//******************************************
//子模块函数


/***********************************************************************
函数名称：void Init_PCF8574(void)
功    能：PCF8574的初始化程序
输入参数：
输出参数：
编写时间：2017.01.20
编 写 人：FangYIkaii
注    意：
***********************************************************************/
void Init_PCF8574(void)
{
	//GPIO结构体初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//GPIO配置-SCL
	//使能时钟，管脚，端口，速度[50MHz]，模式[PP推挽输出]
	RCC_APB2PeriphClockCmd(SCL_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SCL_PCF8574 configure 					 			
	GPIO_InitStructure.GPIO_Pin = SCL_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SCL_PCF8574_GPIOX, &GPIO_InitStructure);	
	
	//GPIO配置-SDA
	//使能时钟，管脚，端口，速度[50MHz]，模式[PP推挽输出]
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 			
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	
	//SCL和SDA写低电平
	SCL_PCF8574(0);
	SDA_PCF8574(0);
	
	I2C_WriteData_PCF8574(0x70, 0x00);		//初始化LED显示控制
	
}

/***********************************************************************
函数名称：void I2C_Start_PCF8574(void)
功    能：PCF8574的I2C开始信号
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：FangYIkaii
注    意：SCL高电平期间，SDA由高电平向低电平跳变，开始传送数据
***********************************************************************/
void I2C_Start_PCF8574(void)
{
	//SDA写高电平
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);	
	//SCL写高电平
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SDA写低电平(高电平-低电平-跳变)
	SDA_PCF8574(0);	
	delay(DelayTime_PCF8574);	
}

/***********************************************************************
函数名称：void I2C_Stop_PCF8574(void)
功    能：PCF8574的I2C停止信号
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：FangYIkaii
注    意：SCL高电平期间，SDA由低电平向高电平跳变，结束传送数据
***********************************************************************/
void I2C_Stop_PCF8574(void)
{
	//SDA写低电平
	SDA_PCF8574(0);
	delay(DelayTime_PCF8574);	
	//SCL写高电平
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SDA写高电平(低电平-高电平-跳变)
	SDA_PCF8574(1);	
	delay(DelayTime_PCF8574);	
}

/***********************************************************************
函数名称：void I2C_NOAck_PCF8574(void)
功    能：PCF8574的I2C发送应答信息
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：FangYIkaii
注    意：接收数据的IC接收8bit数据后，向发送数据的IC发出特定的低电平脉冲，表示收到数据，若未收到应答信号，判断受控单元故障
          发送：SCL:低--[动作]--高-低  SDA:高
***********************************************************************/
void I2C_NOAck_PCF8574(void)
{	
	//SCL写低电平
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);			
	//SDA写高电平
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);				
	//SCL写高电平
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SCL写低电平
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
}

/***********************************************************************
函数名称：void I2C_Ack_PCF8574(void)
功    能：PCF8574的I2C接收应答信息
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：FangYIkaii
注    意：接收：SCL:高--低  读SDA高电平
***********************************************************************/
void I2C_Ack_PCF8574(void)
{
	uint8_t i;
	
	//SDA GPIO配置--浮空输入--FLOATING
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE);	 	//SDA_PCF8574 configure 					 	
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	delay(DelayTime_PCF8574);	

	//SCL写高电平
	SCL_PCF8574(1);	
	delay(DelayTime_PCF8574);	
	//读SDA输入的高电平
	while((SDA_Read_PCF8574==1) && (i<200))
	{
		i++;
	}
	delay(DelayTime_PCF8574);		
	//SCL写低电平
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	

	//SDA GPIO配置--推挽输出--PP
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 		
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);				
}

/***********************************************************************
函数名称：void I2C_Write1Byte_PCF8574(uint8_t data)
功    能：PCF8574的I2C写一个字节程序
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：
注    意：发送：SCL：低--[动作]-高-低
***********************************************************************/
void I2C_Write1Byte_PCF8574(uint8_t data)
{
	uint8_t i;	
	//SCL写低电平
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
	for(i=0x80;i>0;i>>=1)  //i=10000000>>01000000>>0010000.....>>00000001
	{
		//如果第i个数据为1
		if(i&data)
		{
			//SDA写高电平
			SDA_PCF8574(1);
		}
		else
		{
			//否则SDA写低电平
			SDA_PCF8574(0);	
		}
		delay(DelayTime_PCF8574);	
		//SCL写高电平
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		//SCL写低电平
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
	}
	//SDA写高电平
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);
	
	//PCF8574的I2C发送应答信息
 	I2C_Ack_PCF8574();
}

/***********************************************************************
函数名称：uint8_t I2C_Rec1Byte_PCF8574(void)
功    能：PCF8574的I2C接收一个字节程序
输入参数：
输出参数：
编写时间：2017-01-21
编 写 人：
注    意：
***********************************************************************/
uint8_t I2C_Rec1Byte_PCF8574(void)
{
	uint8_t i,data;
	
	//SDA GPIO配置--浮空输入--FLOATING
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE);	 	//SDA_PCF8574 configure 					 	
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);	
	
	data=0x00;	
	for(i=0;i<8;i++)
	{
		//左移一位
		data=data<<1;
		//SCL写低电平
		SCL_PCF8574(0);	  
		delay(DelayTime_PCF8574);		
		//SCL写高电平
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		//如果SDA读入电平为高
		if(SDA_Read_PCF8574)
		{
			//该位置1
			data=data | 0x01;
		}		
	}
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	
	
	//SDA GPIO配置--推挽输出--PP
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 		
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);		

	//PCF8574的I2C接收应答信息
	I2C_NOAck_PCF8574();
	return data;
}

/***********************************************************************
函数名称：void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
功    能：PCF8574的I2C写数据程序
输入参数：
输出参数：
编写时间：2017-01-20
编 写 人：FangYikaii
注    意：1.I2C开始
          2.写要写数据的地址
          3.写要写数据的数据
					4.I2C停止
***********************************************************************/
void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
{
	//PCF8574的I2C开始信号
	I2C_Start_PCF8574();
	//PCF8574的I2C写一个字节:地址
	I2C_Write1Byte_PCF8574(addr);
	//PCF8574的I2C写一个字节:数据
	I2C_Write1Byte_PCF8574(data);	
	//PCF8574的I2C停止信号
	I2C_Stop_PCF8574();	
}

/***********************************************************************
函数名称：uint8_t I2C_RecData_PCF8574(uint8_t addr)
功    能：PCF8574的I2C读数据程序
输入参数：
输出参数：
编写时间：2017-01-20
编 写 人：FangYIkaii
注    意：1.I2C开始
          2.写要读数据的地址
          3.接收数据+赋值
					4.I2C停止
***********************************************************************/
uint8_t I2C_RecData_PCF8574(uint8_t addr)
{
	uint8_t temp;
	//PCF8574的I2C开始信号
	I2C_Start_PCF8574();
	//PCF8574的I2C写一个字节:地址
	I2C_Write1Byte_PCF8574(addr);
	//PCF8574的I2C接收一个字节程序
	temp=I2C_Rec1Byte_PCF8574();
	//PCF8574的I2C停止信号
	I2C_Stop_PCF8574();		
	return temp;
}

/***********************************************************************
函数名称：void KeyScan_PCF8574(uint8_t addr)
功    能：PCF8574的键盘扫描程序
输入参数：
输出参数：
编写时间：2017-01-20
编 写 人：FangYIkaii
注    意：
					<----PCF8574 键盘键值 配置  ---->
					S1  -- 0xFE --  S9     11111110
					S2  -- 0xFD --  S10    11111101
					S3  -- 0xFB --  S11	   11111011
					S4  -- 0xF7 --  S12    11110111

					S5  -- 0xEF --  S13    11101111
					S6  -- 0xDF --  S14    11011111
					S7  -- 0xBF --  S15	   10111111
					S8  -- 0x7F --  S16    01111111
***********************************************************************/
void KeyScan_PCF8574(void)
{
		uint8_t kvalue=0;
	  //读值
		kvalue=I2C_RecData_PCF8574(0x73);	
		if(kvalue==0xFE)
		{
			KeyValue_PCF8574=1;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xFD)
		{
			KeyValue_PCF8574=2;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xFB)
		{
			KeyValue_PCF8574=3;						//PCF8574键盘扫描值
		}			
		else if(kvalue==0xF7)
		{
			KeyValue_PCF8574=4;						//PCF8574键盘扫描值
		}
		else if(kvalue==0xEF)
		{
			KeyValue_PCF8574=5;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xDF)
		{
			KeyValue_PCF8574=6;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xBF)
		{
			KeyValue_PCF8574=7;						//PCF8574键盘扫描值
		}			
		else if(kvalue==0x7F)
		{
			KeyValue_PCF8574=8;						//PCF8574键盘扫描值
		}	
		else
		{
			PCF8574.Bits.KeyScan_Flag=FALSE;	//键盘按键已复位
			KeyValue_PCF8574=0;								//PCF8574键盘扫描值
		}
}
