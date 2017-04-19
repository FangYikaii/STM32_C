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
创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
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
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void Init_PCF8574(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(SCL_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SCL_PCF8574 configure 					 
			
	GPIO_InitStructure.GPIO_Pin = SCL_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SCL_PCF8574_GPIOX, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 
			
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	
	SCL_PCF8574(0);
	SDA_PCF8574(0);
	
	I2C_WriteData_PCF8574(0x70, 0x00);		//初始化LED显示控制
		
	
}

/***********************************************************************
函数名称：void I2C_Start_PCF8574(void)
功    能：PCF8574的I2C开始信号
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_Start_PCF8574(void)
{

	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);	
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	SDA_PCF8574(0);	
	delay(DelayTime_PCF8574);	

	
}
/***********************************************************************
函数名称：void I2C_Stop_PCF8574(void)
功    能：PCF8574的I2C停止信号
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_Stop_PCF8574(void)
{
	
	SDA_PCF8574(0);
	delay(DelayTime_PCF8574);	
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	SDA_PCF8574(1);	
	delay(DelayTime_PCF8574);	

}

/***********************************************************************
函数名称：void I2C_NOAck_PCF8574(void)
功    能：PCF8574的I2C发送应答信息
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_NOAck_PCF8574(void)
{	
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);			
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);				
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		


}

/***********************************************************************
函数名称：void I2C_Ack_PCF8574(void)
功    能：PCF8574的I2C接收应答信息
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_Ack_PCF8574(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE);	 	//SDA_PCF8574 configure 					 	
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	delay(DelayTime_PCF8574);	

	SCL_PCF8574(1);	
	delay(DelayTime_PCF8574);	
	while((SDA_Read_PCF8574==1) && (i<200))
	{
		i++;
	}
	delay(DelayTime_PCF8574);		
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	


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
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_Write1Byte_PCF8574(uint8_t data)
{
	uint8_t i;	
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
	for(i=0x80;i>0;i>>=1)
	{
		if(i&data)
		{
			SDA_PCF8574(1);
		}
		else
		{
			SDA_PCF8574(0);	
		}
		delay(DelayTime_PCF8574);	
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
	}
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);
	
 	I2C_Ack_PCF8574();
}
/***********************************************************************
函数名称：uint8_t I2C_Rec1Byte_PCF8574(void)
功    能：PCF8574的I2C接收一个字节程序
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
uint8_t I2C_Rec1Byte_PCF8574(void)
{
	uint8_t i,data;
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
		data=data<<1;
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		if(SDA_Read_PCF8574)
		{
			data=data | 0x01;
		}
		
	}
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	
	
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 
			
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);		

	I2C_NOAck_PCF8574();
	return data;
}

/***********************************************************************
函数名称：void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
功    能：PCF8574的I2C写数据程序
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
{
	I2C_Start_PCF8574();
	I2C_Write1Byte_PCF8574(addr);
	I2C_Write1Byte_PCF8574(data);	
	I2C_Stop_PCF8574();	
}

/***********************************************************************
函数名称：uint8_t I2C_RecData_PCF8574(uint8_t addr)
功    能：PCF8574的I2C读数据程序
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
uint8_t I2C_RecData_PCF8574(uint8_t addr)
{
	uint8_t temp;
	I2C_Start_PCF8574();
	I2C_Write1Byte_PCF8574(addr);
	temp=I2C_Rec1Byte_PCF8574();
	I2C_Stop_PCF8574();		
	return temp;
}

/***********************************************************************
函数名称：void KeyScan_PCF8574(uint8_t addr)
功    能：PCF8574的键盘扫描程序
输入参数：
输出参数：
编写时间：2014.4.14
编 写 人：
注    意：
***********************************************************************/
void KeyScan_PCF8574(void)
{
		uint8_t kvalue=0;
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
