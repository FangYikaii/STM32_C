//========================================
//<--预定义处理，防止重复定义-->
#ifndef _USARTx_H
#define _USARTx_H

//========================================
//头文件
#define 			uchar 								unsigned char
#define 			uint 									unsigned int

#define 			RX_MaxSize						60				//接收最大数据个数
#define 			OVERFLOW_TIME					200				//溢出时间 200 X 1ms=200ms

//========================================
//宏定义
#define           COMn               5

typedef enum 
{
      COM1 = 0,
      COM2 = 1,
      COM3 = 2,
      COM4 = 3,	
      COM5 = 4	
} COM_TypeDef;



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
#define UART_COM3_TX_PIN                   		GPIO_Pin_5
#define UART_COM3_TX_GPIO_PORT         				GPIOD
#define UART_COM3_TX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM3_RX_PIN                  		GPIO_Pin_6
#define UART_COM3_RX_GPIO_PORT          			GPIOD
#define UART_COM3_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM3_IRQn                     		USART2_IRQn
 
//Definition for COM port4, connected to USART4
//TX--PC10 RX--PC11(Default) ;TX-- RX--(Remap) ;												-- 此块电路板使用PC10/PC11管脚   MAX485

#define UART_COM4_STR                     		"UART4"
#define UART_COM4                        			UART4
#define UART_COM4_CLK                     		RCC_APB1Periph_UART4
#define UART_COM4_TX_PIN                   		GPIO_Pin_5
#define UART_COM4_TX_GPIO_PORT         				GPIOD
#define UART_COM4_TX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM4_RX_PIN                  		GPIO_Pin_6
#define UART_COM4_RX_GPIO_PORT          			GPIOD
#define UART_COM4_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM4_IRQn                     		USART2_IRQn

//Definition for COM port5, connected to USART5													--  此块电路板使用PC12/PD2管脚   MAX232
//TX--PC12 RX--PD2(Default) ;TX-- RX--(Remap) ;

#define UART_COM5_STR                     		"UART5"
#define UART_COM5                        			UART5
#define UART_COM5_CLK                     		RCC_APB1Periph_UART5
#define UART_COM5_TX_PIN                   		GPIO_Pin_5
#define UART_COM5_TX_GPIO_PORT         				GPIOD
#define UART_COM5_TX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM5_RX_PIN                  		GPIO_Pin_6
#define UART_COM5_RX_GPIO_PORT          			GPIOD
#define UART_COM5_RX_GPIO_CLK            			RCC_APB2Periph_GPIOD
#define UART_COM5_IRQn                     		USART2_IRQn



#define     	RS485_COM1_EnPIN											GPIO_Pin_4
#define     	RS485_COM1_EnGPIOX										GPIOD	
#define     	RS485_COM1_RCC_APB2Periph_EnGPIOX			RCC_APB2Periph_GPIOD

//RS485_COM1接收使能
#define 			RS485_COM1_RxEnable(x)    						((x) ? (GPIO_ResetBits(RS485_COM1_EnGPIOX , RS485_COM1_EnPIN)) : (GPIO_SetBits(RS485_COM1_EnGPIOX , RS485_COM1_EnPIN)))

	
//========================================
//外调变量声明
   
//串口结果
struct UartResult 
{
	uchar TimeOut;		          //串口接收时间 
	uchar WaitT10ms;	          //
	uchar Count;			          //
	uchar	Data[RX_MaxSize];		  //接收数据
	uchar NoAck;		
	uchar length;
};
extern struct UartResult Rec_COM1;
extern struct UartResult Rec_COM2;

//串口标志位
union FlagUart  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			Rec_Flag					:	1;					//串口接收标志位
						uint8_t			OverTime_Flag			:	1;	        //是否溢出标志位
						uint8_t 		Bit2							:	1;				
						uint8_t 		Bit3							:	1;
						uint8_t 		Bit4		          :	1;				
						uint8_t 		Bit5		          :	1;
						uint8_t 		Bit6		          :	1;				
						uint8_t 		Bit7		          :	1;		 						
      } Bits; 
}; 
extern volatile union FlagUart flagCOM1;
extern volatile union FlagUart flagCOM2;

//串口接收标志位
union FlagUartRec  
{ 
      uint8_t flagBits; 
      struct       
      {
				uint8_t       SetFlag                       : 1;	
				uint8_t       ReadFlag          						:	1;	
				uint8_t   	  UART_Rec_Flag		      			:	1;
				uint8_t   		Bit3_Flag     								:	1;					
				uint8_t   		Bit4_Flag							        :	1;
				uint8_t   		Bit5_Flag							        :	1;
				uint8_t   		Bit6_Flag							        :	1;
				uint8_t   		Bit7_Flag							        :	1;		
      } Bits; 
}; 
extern volatile union FlagUartRec flagUartRecCOM1;
extern volatile union FlagUartRec flagUartRecCOM2;

//========================================
//外调函数声明
extern void Uart_Init(void);        															//Uart串口初始化
extern void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data);		//串口发送数据处理
extern void USART_COM1_GetInputString(void);											//串口1接收数据处理
extern void USART_COM2_GetInputString(void);											//串口2接收数据处理
//void RS485_COM1_PutChar(USART_TypeDef* USARTx, uint8_t Data);			//RS485_COM1发送单字节数据
extern uchar LRC_Check(uchar *data,uchar length);									//LRC检验程序
//========================================
//<--预处理结束-->
#endif
