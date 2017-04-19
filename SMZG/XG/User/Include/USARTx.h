//========================================
//<--预定义处理，防止重复定义-->
#ifndef _USARTx_H
#define _USARTx_H

//========================================
//头文件


//========================================
//宏定义

#define 			RX_MaxSzie						60				//接收最大数据个数
#define 			OVERFLOW_TIME					200				//溢出时间 200 X 1ms=200ms
#define      	COMn             		  5

typedef enum 
{
      COM1 = 0,
      COM2 = 1,
      COM3 = 2,
      COM4 = 3,	
      COM5 = 4	
} COM_TypeDef;

//#define USE_USART2												//定义一个宏	
//#undef USE_USART2													//取消一个宏	
// #ifdef USE_USART2												//该宏用法
//   #define USARTx   USART2
// #else
//   #define USARTx   USART1
// #endif
//========================================
//端口定义

//Definition for COM port1, connected to USART1
//TX--PA9 RX--PA10(Default) ;TX--PB6 RX--PB7(Remap);
#define UART_COM1_STR                           "USART1"
#define UART_COM1                             	USART1
#define UART_COM1_CLK                           RCC_APB2Periph_USART1
#define UART_COM1_TX_PIN                      	GPIO_Pin_9
#define UART_COM1_TX_GPIO_PORT        					GPIOA
#define UART_COM1_TX_GPIO_CLK           				RCC_APB2Periph_GPIOA
#define UART_COM1_RX_PIN                      	GPIO_Pin_10
#define UART_COM1_RX_GPIO_PORT        					GPIOA
#define UART_COM1_RX_GPIO_CLK           				RCC_APB2Periph_GPIOA
#define UART_COM1_IRQn                          USART1_IRQn

//Definition for COM port2, connected to USART2
//TX--PA2 RX--PA3 ;TX--PD5 RX--PD6(Remap);														-- 此块电路板使用PD5/PD6管脚	MAX232			

#define UART_COM2_STR                     		"USART2"
#define UART_COM2                        			USART2
#define UART_COM2_CLK                     		RCC_APB1Periph_USART2
#define UART_COM2_TX_PIN                   		GPIO_Pin_5
#define UART_COM2_TX_GPIO_PORT         				GPIOD
#define UART_COM2_TX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM2_RX_PIN                  		GPIO_Pin_6
#define UART_COM2_RX_GPIO_PORT          			GPIOD
#define UART_COM2_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM2_IRQn                     		USART2_IRQn
 
//Definition for COM port3, connected to USART3
//TX--PB10 RX--PB11(Default) ;TX--PD8 RX--PD9(Remap) ; TX--PC10 RX--PC11(Remap2) ;

#define UART_COM3_STR                     		"USART3"
#define UART_COM3                        			USART3
#define UART_COM3_CLK                     		RCC_APB1Periph_USART3
#define UART_COM3_TX_PIN                   		GPIO_Pin_8
#define UART_COM3_TX_GPIO_PORT         				GPIOD
#define UART_COM3_TX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM3_RX_PIN                  		GPIO_Pin_9
#define UART_COM3_RX_GPIO_PORT          			GPIOD
#define UART_COM3_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM3_IRQn                     		USART3_IRQn
 
//Definition for COM port4, connected to USART4
//TX--PC10 RX--PC11(Default) ;TX-- RX--(Remap) ;												-- 此块电路板使用PC10/PC11管脚   MAX485

#define UART_COM4_STR                     		"UART4"
#define UART_COM4                        			UART4
#define UART_COM4_CLK                     		RCC_APB1Periph_UART4
#define UART_COM4_TX_PIN                   		GPIO_Pin_10
#define UART_COM4_TX_GPIO_PORT         				GPIOC
#define UART_COM4_TX_GPIO_CLK            			RCC_APB2Periph_GPIOC
#define UART_COM4_RX_PIN                  		GPIO_Pin_11
#define UART_COM4_RX_GPIO_PORT          			GPIOC
#define UART_COM4_RX_GPIO_CLK            			RCC_APB2Periph_GPIOC
#define UART_COM4_IRQn                     		USART4_IRQn

//Definition for COM port5, connected to USART5													--  此块电路板使用PC12/PD2管脚   MAX232
//TX--PC12 RX--PD2(Default) ;TX-- RX--(Remap) ;

#define UART_COM5_STR                     		"UART5"
#define UART_COM5                        			UART5
#define UART_COM5_CLK                     		RCC_APB1Periph_UART5
#define UART_COM5_TX_PIN                   		GPIO_Pin_12
#define UART_COM5_TX_GPIO_PORT         				GPIOC
#define UART_COM5_TX_GPIO_CLK            			RCC_APB2Periph_GPIOC
#define UART_COM5_RX_PIN                  		GPIO_Pin_2
#define UART_COM5_RX_GPIO_PORT          			GPIOD
#define UART_COM5_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM5_IRQn                     		USART5_IRQn



#define     	RS485_COM4_EnPIN											GPIO_Pin_3
#define     	RS485_COM4_EnGPIOX										GPIOD	
#define     	RS485_COM4_RCC_APB2Periph_EnGPIOX			RCC_APB2Periph_GPIOD

//RS485_COM1接收使能
#define 			RS485_COM4_RxEnable(x)    						((x) ? (GPIO_ResetBits(RS485_COM4_EnGPIOX , RS485_COM4_EnPIN)) : (GPIO_SetBits(RS485_COM4_EnGPIOX , RS485_COM4_EnPIN)))

	
//========================================
//外调变量声明
   
union FlagUart  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			Rec_Flag					:	1;					
						uint8_t			OverTime_Flag			:	1;	
						uint8_t 		Bit2							:	1;				
						uint8_t 		Bit3							:	1;
						uint8_t 		Bit4		:	1;				
						uint8_t 		Bit5		:	1;
						uint8_t 		Bit6		:	1;				
						uint8_t 		Bit7		:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagUart flagCOM5;
extern volatile union FlagUart flagCOM4;
extern volatile union FlagUart flagCOM3;
extern volatile union FlagUart flagCOM2;
extern volatile union FlagUart flagCOM1;
struct UartResult 
{
	uint8_t  TimeOut;		
	uint8_t WaitT10ms;	
	uint8_t Count;			
	uint8_t Data[RX_MaxSzie];		
	uint8_t NoAck;		
};

extern struct UartResult Rec_COM1;
extern struct UartResult Rec_COM2;
extern struct UartResult Rec_COM3;
extern struct UartResult Rec_COM4;
extern struct UartResult Rec_COM5;

//========================================
//外调函数声明
extern void Uart_Init(void);        															//Uart串口初始化
extern void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data);		//串口发送数据处理
extern void USART_COM1_GetInputString(void);											//串口接收数据处理
extern void USART_COM2_GetInputString(void);											//串口接收数据处理
extern void USART_COM3_GetInputString(void);											//串口接收数据处理
extern void USART_COM4_GetInputString(void);											//串口接收数据处理
extern void USART_COM5_GetInputString(void);											//串口接收数据处理

void RS485_COM4_PutChar(USART_TypeDef* USARTx, uint8_t Data);			//RS485_COM2发送单字节数据

extern uint8_t LRC_Check(uint8_t *data,uint8_t length);									//LRC检验程序
//========================================
//<--预处理结束-->
#endif



