//========================================
//<--预定义处理，防止重复定义-->
#ifndef _ADC_H
#define _ADC_H

//========================================
//头文件





//========================================
//宏定义
#define 			ADCCollectTime	    30								//ADC采集次数-- 大于10个												
#define 			ADCChannelNum	    	6						  		//ADC采集通道数

#define 			ADC1_DR_Address    ((uint32_t)0x4001244C)  		//ADC1 DR寄存器基地址




//========================================
//端口定义

//GPIO_MODE_AIN 模拟输入
//GPIO_MODE_FLOATING 浮空输入
//GPIO_MODE_IPD 下拉输入
//GPIO_MODE_IPU 上拉输入
//GPIO_MODE_Out_OD 开漏输出
//GPIO_MODE_Out_PP 推挽输出
//GPIO_MODE_AF_OD 复用开漏输出
//GPIO_MODE_AF_PP 复用推挽输出

/*
ADC1各通道对应的IO端口
ADCCN1 -- PA0  ADCCN2 -- PA1  ADCCN3 -- PA2  ADCCN4 -- PA3    
ADCCN5 -- PA4  ADCCN6 -- PA5  ADCCN7 -- PA6  ADCCN8 -- PA7

ADCCN9 -- PB0  ADCCN10-- PB1  ADCCN11-- PC0  ADCCN12-- PC1
ADCCN13-- PC2  ADCCN14-- PC3  ADCCN15-- PC4  ADCCN16-- PC5
*/

#define 			ADCNN1_GPIOX												GPIOA
#define 			ADCNN2_GPIOX												GPIOA
#define 			ADCNN3_GPIOX												GPIOA
#define 			ADCNN4_GPIOX												GPIOA
#define 			ADCNN5_GPIOX												GPIOA
#define 			ADCNN6_GPIOX												GPIOA
#define 			ADCNN7_GPIOX												GPIOA
#define 			ADCNN8_GPIOX												GPIOA

#define 			ADCNN9_GPIOX												GPIOB
#define 			ADCNN10_GPIOX												GPIOB

#define 			ADCNN11_GPIOX												GPIOC
#define 			ADCNN12_GPIOX												GPIOC
#define 			ADCNN13_GPIOX												GPIOC
#define 			ADCNN14_GPIOX												GPIOC
#define 			ADCNN15_GPIOX												GPIOC
#define 			ADCNN16_GPIOX												GPIOC

#define 			ADCNN1_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN2_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN3_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN4_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN5_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN6_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN7_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA
#define 			ADCNN8_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOA

#define 			ADCNN9_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOB
#define 			ADCNN10_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOB

#define 			ADCNN11_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC
#define 			ADCNN12_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC
#define 			ADCNN13_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC
#define 			ADCNN14_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC
#define 			ADCNN15_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC
#define 			ADCNN16_RCC_APB2Periph_GPIOX				RCC_APB2Periph_GPIOC


#define 			ADCNN1_GPIO_PIN											GPIO_Pin_0
#define 			ADCNN2_GPIO_PIN											GPIO_Pin_1
#define 			ADCNN3_GPIO_PIN											GPIO_Pin_2
#define 			ADCNN4_GPIO_PIN											GPIO_Pin_3
#define 			ADCNN5_GPIO_PIN											GPIO_Pin_4
#define 			ADCNN6_GPIO_PIN											GPIO_Pin_5
#define 			ADCNN7_GPIO_PIN											GPIO_Pin_6
#define 			ADCNN8_GPIO_PIN											GPIO_Pin_7

#define 			ADCNN9_GPIO_PIN											GPIO_Pin_0
#define 			ADCNN10_GPIO_PIN										GPIO_Pin_1

#define 			ADCNN11_GPIO_PIN										GPIO_Pin_0
#define 			ADCNN12_GPIO_PIN										GPIO_Pin_1
#define 			ADCNN13_GPIO_PIN										GPIO_Pin_2
#define 			ADCNN14_GPIO_PIN										GPIO_Pin_3
#define 			ADCNN15_GPIO_PIN										GPIO_Pin_4
#define 			ADCNN16_GPIO_PIN										GPIO_Pin_5


#define 			ADCNN1_ChannelNO										0
#define 			ADCNN2_ChannelNO										1
#define 			ADCNN3_ChannelNO										2
#define 			ADCNN4_ChannelNO										3
#define 			ADCNN5_ChannelNO										4
#define 			ADCNN6_ChannelNO										5
#define 			ADCNN7_ChannelNO										6
#define 			ADCNN8_ChannelNO										7

#define 			ADCNN9_ChannelNO										8
#define 			ADCNN10_ChannelNO										9

#define 			ADCNN11_ChannelNO										10
#define 			ADCNN12_ChannelNO										11
#define 			ADCNN13_ChannelNO										12
#define 			ADCNN14_ChannelNO										13
#define 			ADCNN15_ChannelNO										14
#define 			ADCNN16_ChannelNO										15

#define 			ADCNN17_ChannelNO										16
#define 			ADCNN18_ChannelNO										17

//========================================
//外调变量声明

//联合体 ADC标志位
union FlagADCA  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			ADCOK 						:	1;					
						uint8_t			Bit1							:	1;	
						uint8_t 		Bit2							:	1;				
						uint8_t 		Bit3							:	1;
						uint8_t 		Bit4							:	1;				
						uint8_t 		Bit5							:	1;
						uint8_t 		Bit6							:	1;				
						uint8_t 		Bit7							:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagADCA flagADC;

//volatile 不让编译器进行优化，每次读取或修改值的时候，必须从内存或者寄存器中读取或者修改 [如#define __IO volatile，每次输入输出都得重新读，即使它们一样]
//结构体  ADC 结果
struct ADCResult 
{
	__IO uint16_t ADCColValue[ADCCollectTime][ADCChannelNum];						//AD采集数据寄存器  [采集次数][采集通道数]
	uint16_t ADCConvertedValueLocal[ADCCollectTime][ADCChannelNum];			//ADC采集暂存值
	float ADCResValue[ADCChannelNum];																		//AD采集滤波后数据
	float ADCMaxValue[ADCChannelNum];																		//AD采集滤波后数据-最大值
	float ADCMinValue[ADCChannelNum];																		//AD采集滤波后数据-最小值
	float ADCSumValue[ADCChannelNum];																		//AD采样值总和
	uint16_t ADCSumCount[ADCChannelNum];									    					//AD采样值计数
};
extern struct ADCResult ADCRes;


//========================================
//外调函数声明
extern void ADC_Configuration(void);									//完成ADC的配置
extern void ADC_ValueFilter(void);										//ADC采集值滤波





//========================================
//<--预处理结束-->
#endif



