//========================================
//<--预定义处理，防止重复定义-->
#ifndef _MAX7219_H
#define _MAX7219_H

//========================================
//头文件
#define 			NOPOINT			0
#define 			ENPOINT			1

#define 			SEGLED1			1			
#define 			SEGLED2			2	
#define 			SEGLED3			3	
#define 			SEGLED4			4	

//========================================
//宏定义

#define 			REG_NO_OP 				0x00 		//定义空操作寄存器 
#define 			DIG_1 						0x01    //定义数码管1寄存器   
#define 			DIG_2 						0x02    //定义数码管2寄存器         
#define 			DIG_3 						0x03    //定义数码管3寄存器        
#define 			DIG_4 						0x04    //定义数码管4寄存器        
#define 			DIG_5 						0x05    //定义数码管5寄存器      
#define 			DIG_6 						0x06    //定义数码管6寄存器       
#define 			DIG_7 						0x07    //定义数码管7寄存器        
#define 			DIG_8 						0x08    //定义数码管8寄存器         
#define 			REG_DECODE 				0x09   	//译码控制寄存器   
#define 			REG_INTENSITY 		0x0a   	//亮度控制寄存器 
#define 			REG_SCAN_LIMIT 		0x0b   	//扫描界限寄存器 
#define 			REG_SHUTDOWN 			0x0c   	//关断模式寄存器
#define 			REG_DISPLAY_TEST 	0x0f  	//测试控制寄存器
#define 			INTENSITY_MIN 		0x00    //定义最低显示亮度      
#define 			INTENSITY_MAX 		0x0f    //定义最高显示亮度       

#define 			CHIP_1						0x01		//1号芯片
#define 			CHIP_2						0x02		//2号芯片


#define 			DelayTime_MAX7219	1

//========================================
//端口定义

#define 			DIN_MAX7219_GPIOX													GPIOE
#define 			LOAD_MAX7219_GPIOX												GPIOE
#define 			CLK_MAX7219_GPIOX													GPIOE

#define 			DIN_MAX7219_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE
#define 			LOAD_MAX7219_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE
#define 			CLK_MAX7219_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOE

#define 			DIN_MAX7219_GPIO_PIN											GPIO_Pin_1
#define 			LOAD_MAX7219_GPIO_PIN											GPIO_Pin_0
#define 			CLK_MAX7219_GPIO_PIN											GPIO_Pin_2


#define 			DIN_MAX7219(x)   				((x) ? (GPIO_SetBits(DIN_MAX7219_GPIOX, DIN_MAX7219_GPIO_PIN)) : (GPIO_ResetBits(DIN_MAX7219_GPIOX, DIN_MAX7219_GPIO_PIN)))
#define 			LOAD_MAX7219(x)    			((x) ? (GPIO_SetBits(LOAD_MAX7219_GPIOX, LOAD_MAX7219_GPIO_PIN)) : (GPIO_ResetBits(LOAD_MAX7219_GPIOX, LOAD_MAX7219_GPIO_PIN)))
#define 			CLK_MAX7219(x)   				((x) ? (GPIO_SetBits(CLK_MAX7219_GPIOX, CLK_MAX7219_GPIO_PIN)) : (GPIO_ResetBits(CLK_MAX7219_GPIOX, CLK_MAX7219_GPIO_PIN)))

				
//========================================
//外调变量声明




//========================================
//外调函数声明
extern void MAX7219_Configuration(void);																							    //完成MAX7219的配置
extern void DisplayDigitalTube1_4Num_MAX7219 (uint16_t data,uint8_t no,uint8_t point);	  //第1、2号4位数码管显示程序，显示低4位
extern void DisplayDigitalTube2_2Num_MAX7219 (uint8_t data,uint8_t no,uint8_t point);	    //第1、2、3、4号2位数码管显示程序，显示低4位
extern void DisplayDigitalTube3_4Num_MAX7219 (uint16_t data,uint8_t no,uint8_t point);	  //第1、2号4位数码管显示程序，显示低4位

extern void GearDisplay(uint8_t gear, uint8_t no);																		    //档位信息显示
extern void LoaderSystemDisplay(void);																										//装载机系统运行显示
extern void ErrorDisplay(uint8_t Error);																									//错误信息显示
//========================================
//<--预处理结束-->
#endif



