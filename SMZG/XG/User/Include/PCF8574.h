//========================================
//<--预定义处理，防止重复定义-->
#ifndef _PCF8574_H
#define _PCF8574_H

//========================================
//头文件



//========================================
//宏定义
#define 			DelayTime_PCF8574			10



//========================================
//端口定义
#define 			SCL_PCF8574_GPIOX													GPIOE
#define 			SDA_PCF8574_GPIOX												  GPIOE

#define 			SCL_PCF8574_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE
#define 			SDA_PCF8574_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE

#define 			SCL_PCF8574_GPIO_PIN											GPIO_Pin_5
#define 			SDA_PCF8574_GPIO_PIN											GPIO_Pin_3


#define 			SCL_PCF8574(x)   													((x) ? (GPIO_SetBits(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)) : (GPIO_ResetBits(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)))
#define 			SDA_PCF8574(x)    												((x) ? (GPIO_SetBits(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)) : (GPIO_ResetBits(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)))

#define 			SCL_Read_PCF8574												GPIO_ReadInputDataBit(SCL_PCF8574_GPIOX, SCL_PCF8574_GPIO_PIN)
#define 			SDA_Read_PCF8574												GPIO_ReadInputDataBit(SDA_PCF8574_GPIOX, SDA_PCF8574_GPIO_PIN)


//========================================
//外调变量声明
extern uint8_t KeyValue_PCF8574;						//PCF8574键盘扫描值

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



