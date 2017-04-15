//========================================
//<--预定义处理，防止重复定义-->
#ifndef _PCF8574_H
#define _PCF8574_H

//========================================
//头文件



//========================================
//宏定义
#define 			DelayTime_PCF8574			10                 //PCF8574处理延时

//========================================
//端口定义
//SCL:PE5     SDA:PE3
//端口；时钟；管脚；读电平；写电平(0/0)

#define 			SCL_PCF8574_GPIOX													GPIOE
#define 			SDA_PCF8574_GPIOX												  GPIOE

#define 			SCL_PCF8574_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE
#define 			SDA_PCF8574_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE

#define 			SCL_PCF8574_GPIO_PIN											GPIO_Pin_5
#define 			SDA_PCF8574_GPIO_PIN											GPIO_Pin_3

//<表达式1>?<表达式2>:<表达式3>         条件运算符，C语言唯一的三目运算符
//对表达式1检验，如果为真，返回表达式2的值，否则返回表达式3的值
//GPIO_ResetBits--GPIO置0
//GPIO_SetBits--GPIO置1

#define 			SCL_PCF8574(x)   													((x) ? (GPIO_SetBits(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)) : (GPIO_ResetBits(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)))
#define 			SDA_PCF8574(x)    												((x) ? (GPIO_SetBits(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)) : (GPIO_ResetBits(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)))

#define 			SCL_Read_PCF8574												GPIO_ReadInputDataBit(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)
#define 			SDA_Read_PCF8574												GPIO_ReadInputDataBit(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)


//========================================
//外调变量声明
extern uint8_t KeyValue_PCF8574;						//PCF8574键盘扫描值

//联合体 PCF8574标志位
union FlagPCF8574  
{ 
      uint8_t flagBits; 
      struct       
      { 
            uint8_t		KeyScan_Flag			:	1;				//扫描扫描标志
						uint8_t		Bit1							:	1;
						uint8_t		Bit2							:	1;				
						uint8_t		Bit3							:	1;	
						uint8_t		Bit4							:	1;		 			
						uint8_t		Bit5							:	1;	
						uint8_t		Bit6							:	1;				
						uint8_t		Bit7							:	1;
				
      } Bits; 
}; 
extern volatile union FlagPCF8574 PCF8574;

//========================================
//外调函数声明
extern void Init_PCF8574(void);																	//PCF8574的初始化程序
extern void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data);			//PCF8574的I2C写数据程序
extern uint8_t I2C_RecData_PCF8574(uint8_t addr);										//PCF8574的I2C读数据程序

extern void KeyScan_PCF8574(void);															//PCF8574的键盘扫描程序

//========================================
//<--预处理结束-->
#endif



