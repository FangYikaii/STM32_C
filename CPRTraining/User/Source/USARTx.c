/*
程序说明:  	
            USARTx 串口通信程序 --- STM32F107VCT 
						
注意事项：	1、注意管脚的复用问题						
						
						
端口使用:		见Doc文件夹内说明
创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/

//========================================
//头文件
#include "main.h"

//========================================
//变量定义
volatile union FlagUart flagCOM5;
volatile union FlagUart flagCOM4;
volatile union FlagUart flagCOM3;
volatile union FlagUart flagCOM2;
volatile union FlagUart flagCOM1;

struct UartResult Rec_COM1;
struct UartResult Rec_COM2;
struct UartResult Rec_COM3;
struct UartResult Rec_COM4;
struct UartResult Rec_COM5;


USART_InitTypeDef USART_InitStructure;

USART_TypeDef* COM_USART[COMn] = {UART_COM1, UART_COM2, UART_COM3, UART_COM4, UART_COM5}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {UART_COM1_TX_GPIO_PORT, UART_COM2_TX_GPIO_PORT, UART_COM3_TX_GPIO_PORT, UART_COM4_TX_GPIO_PORT, UART_COM5_TX_GPIO_PORT}; 
GPIO_TypeDef* COM_RX_PORT[COMn] = {UART_COM1_RX_GPIO_PORT, UART_COM2_RX_GPIO_PORT, UART_COM3_RX_GPIO_PORT, UART_COM4_RX_GPIO_PORT, UART_COM5_RX_GPIO_PORT}; 
const uint32_t COM_USART_CLK[COMn] = {UART_COM1_CLK, UART_COM2_CLK, UART_COM3_CLK, UART_COM4_CLK, UART_COM5_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {UART_COM1_TX_GPIO_CLK, UART_COM2_TX_GPIO_CLK, UART_COM3_TX_GPIO_CLK, UART_COM4_TX_GPIO_CLK, UART_COM5_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {UART_COM1_RX_GPIO_CLK, UART_COM2_RX_GPIO_CLK, UART_COM3_RX_GPIO_CLK, UART_COM4_RX_GPIO_CLK, UART_COM5_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {UART_COM1_TX_PIN, UART_COM2_TX_PIN, UART_COM3_TX_PIN, UART_COM4_TX_PIN, UART_COM5_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {UART_COM1_RX_PIN, UART_COM2_RX_PIN, UART_COM3_RX_PIN, UART_COM4_RX_PIN, UART_COM5_RX_PIN};

//========================================
//函数声明
void Uart_Init(void);   //串口初始化
void STM_UART_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);	//串口初始配置程序
void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data);	//串口初始配置程序

void USART_COM1_GetInputString(void);			//串口接收数据处理
void USART_COM2_GetInputString(void);			//串口接收数据处理
void USART_COM3_GetInputString(void);			//串口接收数据处理
void USART_COM4_GetInputString(void);			//串口接收数据处理
void USART_COM5_GetInputString(void);			//串口接收数据处理

void RS485_COM4_PutChar(USART_TypeDef* USARTx, uint8_t Data);	//RS485_COM2发送单字节数据

uint8_t LRC_Check(uint8_t *data,uint8_t length);	//LRC检验程序

//========================================
//子模块函数



/***********************************************************************
函数名称：void Uart_Init(void)
功    能：Uart串口初始化
输入参数：无
输出参数：无
编写时间：2013.11.19
编 写 人：masterjay
注    意：
***********************************************************************/
void Uart_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;      //定义GPIO结构体	
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	RCC_APB2PeriphClockCmd(RS485_COM2_RCC_APB2Periph_EnGPIOX, ENABLE); 					 
//			
//	GPIO_InitStructure.GPIO_Pin = RS485_COM2_EnPIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(RS485_COM2_EnGPIOX, &GPIO_InitStructure);
//	RS485_COM2_RxEnable(1);																			//RS485_COM2处于接收状态
//	
//	RCC_APB2PeriphClockCmd(RS485_COM5_RCC_APB2Periph_EnGPIOX, ENABLE); 					 
//			
//	GPIO_InitStructure.GPIO_Pin = RS485_COM5_EnPIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(RS485_COM5_EnGPIOX, &GPIO_InitStructure);
//	RS485_COM5_RxEnable(1);																			//RS485_COM5处于接收状态	
//	

//串口计数清零
	Rec_COM1.Count=0;	
	Rec_COM2.Count=0;
	Rec_COM3.Count=0;	
	Rec_COM4.Count=0;
	Rec_COM5.Count=0;
	
	
//串口参数设置	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	STM_UART_COMInit(COM1, &USART_InitStructure);				//串口初始配置程序--IO设置等
	USART_ITConfig(UART_COM1, USART_IT_RXNE, ENABLE);		//串口接收使能
//USART_ITConfig(UART_COM1, USART_IT_TXE, ENABLE);		//串口接收使能
	USART_ITConfig(UART_COM1, USART_IT_TXE, DISABLE);		//串口发送中断不能开，否则没办法接收
	USART_ClearITPendingBit(UART_COM1, USART_IT_TC);		//清除中断TC位

//	STM_UART_COMInit(COM2, &USART_InitStructure);				//串口初始配置程序--IO设置等
//	USART_ITConfig(UART_COM2, USART_IT_RXNE, ENABLE);		//串口接收使能
////USART_ITConfig(UART_COM2, USART_IT_TXE, ENABLE);		//串口接收使能
//	USART_ITConfig(UART_COM2, USART_IT_TXE, DISABLE);		//串口发送中断不能开，否则没办法接收
//	USART_ClearITPendingBit(UART_COM2, USART_IT_TC);		//清除中断TC位

//	STM_UART_COMInit(COM4, &USART_InitStructure);				//串口初始配置程序--IO设置等
//	USART_ITConfig(UART_COM4, USART_IT_RXNE, ENABLE);		//串口接收使能
////USART_ITConfig(UART_COM4, USART_IT_TXE, ENABLE);		//串口接收使能
//	USART_ITConfig(UART_COM4, USART_IT_TXE, DISABLE);		//串口发送中断不能开，否则没办法接收
//	USART_ClearITPendingBit(UART_COM4, USART_IT_TC);		//清除中断TC位

  

//	STM_UART_COMInit(COM5, &USART_InitStructure);				//串口初始配置程序--IO设置等
//	USART_ITConfig(UART_COM5, USART_IT_RXNE, ENABLE);		//串口接收使能
////USART_ITConfig(UART_COM5, USART_IT_TXE, ENABLE);		//串口接收使能
//	USART_ITConfig(UART_COM5, USART_IT_TXE, DISABLE);		//串口发送中断不能开，否则没办法接收
//	USART_ClearITPendingBit(UART_COM5, USART_IT_TC);		//清除中断TC位
//		


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//抢断优先级 0   		响应优先级 0-15	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//抢断优先级 0-1    响应优先级 0-7	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//抢断优先级 0-3    响应优先级 0-3	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//抢断优先级 0-7    响应优先级 0-1	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0

	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/***********************************************************************
函数名称：void STM_UART_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
功    能：串口初始配置程序--IO设置等
输入参数：串口号，串口参数结构体
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/
void STM_UART_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
      GPIO_InitTypeDef GPIO_InitStructure;
      /* Enable GPIO clock */
      RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);
      if (COM == COM1)
      {
// 						/* Enable the USART1 Pins Software Remapping */
//             GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);									//特别注意，是否使能映射管脚
						
				/* Enable the USART1 Pins */
				RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
				
      }
      else if(COM==COM2)
      {
				/* Enable the USART2 Pins Software Remapping */
				GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);									//特别注意，是否使能映射管脚
				
				/* Enable the USART2 Pins */
				RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);				
      }
      else if(COM==COM3)
      {
				/* Enable the USART2 Pins Software Remapping */
// 				GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);					//特别注意，是否使能映射管脚
// 				GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);							//特别注意，是否使能映射管脚

				/* Enable the USART3 Pins */
				RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);				
      }			
      else
      {
				/* Enable the USART4/5 Pins */
				RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);				
      }			
		
      /* Configure USART Tx as alternate function push-pull */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

      /* Configure USART Rx as input floating */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
      GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

      /* USART configuration */
      USART_Init(COM_USART[COM], USART_InitStruct);
    
      /* Enable USART */
      USART_Cmd(COM_USART[COM], ENABLE);
}



/***********************************************************************
函数名称：void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data)
功    能：串口发送单字节数据
输入参数：串口号，数值
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/
void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data)
{
	
    USART_SendData(USARTx, Data);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//  while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == SET)) 
    {
        ;
    }
}

///***********************************************************************
//函数名称：void RS485_COM4_PutChar(USART_TypeDef* USARTx, uint8_t Data)
//功    能：RS485_COM1发送单字节数据
//输入参数：串口号，数值
//输出参数：无
//编写时间：2014.04.11
//编 写 人：masterjay
//注    意：
//***********************************************************************/
//void RS485_COM4_PutChar(USART_TypeDef* USARTx, uint8_t Data)
//{
//		RS485_COM4_RxEnable(0);																	//RS485_COM1发送状态	
//		delay(10);
//    USART_SendData(USARTx, Data);
//		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
////  while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == SET)) 
//    {
//        ;
//    }
//		RS485_COM4_RxEnable(1);																	//RS485_COM1处于接收状态		
//		
//}


/***********************************************************************
函数名称：void USART_COM1_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

void USART_COM1_GetInputString(void)
{
	
	uint8_t check_data;
	uint8_t check_orgi;
	uint8_t data;	
	data=USART_ReceiveData(USART1);	
	if(Rec_COM1.Count==0)
	{		
		if(data==0x3A)	
		{
			Rec_COM1.TimeOut=0;											//串口溢出计时
			flagCOM1.Bits.OverTime_Flag=TRUE;				//串口溢出计时
			Rec_COM1.Data[Rec_COM1.Count]=data;	
			Rec_COM1.Count++;	
		}				
	}	 
	else if(Rec_COM1.Count<60)
	{
		Rec_COM1.Data[Rec_COM1.Count]=data;
		Rec_COM1.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM1.Data[0]==0x3a)
		{
			if(Rec_COM1.Count>=4)
			{
				if(Rec_COM1.Data[Rec_COM1.Count-1]==0x0A && Rec_COM1.Data[Rec_COM1.Count-2]==0x0D)
				{
					check_data=LRC_Check(Rec_COM1.Data,(Rec_COM1.Count-5));					//LRC检验
					if(Rec_COM1.Data[Rec_COM1.Count-4]<0x3A)
					{
						check_orgi=Rec_COM1.Data[Rec_COM1.Count-4]%0x30;				
					}
					else
					{
						check_orgi=Rec_COM1.Data[Rec_COM1.Count-4]%0x41;
						check_orgi=check_orgi+10;	
					}
					check_orgi=check_orgi*0x10;						
					if(Rec_COM1.Data[Rec_COM1.Count-3]<0x3A)
					{
						check_orgi+=Rec_COM1.Data[Rec_COM1.Count-3]%0x30;				
					}
					else
					{
						check_orgi+=Rec_COM1.Data[Rec_COM1.Count-3]%0x41;
						check_orgi=check_orgi+10;	
					}
					if(check_data==check_orgi)
					{
						flagCOM1.Bits.Rec_Flag=TRUE;
					}				
					Rec_COM1.Count=0;	
					
					Rec_COM1.TimeOut=0;											//串口溢出计时
					flagCOM1.Bits.OverTime_Flag=FALSE;					//串口溢出计时				
										
				}
			}
		}				
	}	
	else
	{
		Rec_COM1.Count=0;					
	}	  							

}

/***********************************************************************
函数名称：void USART_COM2_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

void USART_COM2_GetInputString(void)
{
	uint8_t data;	
	data=USART_ReceiveData(USART2);	
	if(Rec_COM2.Count==0)
	{		
		if(data==0xFA)	
		{
			Rec_COM2.TimeOut=0;										//串口溢出计时
			flagCOM2.Bits.OverTime_Flag=TRUE;				//串口溢出计时
			Rec_COM2.Data[Rec_COM2.Count]=data;	
			Rec_COM2.Count++;	
		}				
	}	 
	else if(Rec_COM2.Count<5)
	{
		Rec_COM2.Data[Rec_COM2.Count]=data;
		Rec_COM2.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM2.Data[0]==0xFA)
		{
			if(Rec_COM2.Count>=4)
			{
				if(Rec_COM2.Data[Rec_COM2.Count-1]==0x0A )
				{			
					if(Rec_COM2.Data[1]==0xA2)
					{
						flagCOM2.Bits.Rec_Flag=TRUE;
						Rec_COM2.Cmd[0]=Rec_COM2.Data[2];
						
					}				
					Rec_COM2.Count=0;	
					
					Rec_COM2.TimeOut=0;										//串口溢出计时
					flagCOM2.Bits.OverTime_Flag=FALSE;		//串口溢出计时						
										
				}
			}
		}				
	}	
	else
	{
		Rec_COM2.Count=0;					
	}	  							

}


/***********************************************************************
函数名称：void USART_COM1_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

void USART_COM3_GetInputString(void)
{
	
	uint8_t check_data;
	uint8_t check_orgi;
	uint8_t data;	
	data=USART_ReceiveData(USART3);	
	if(Rec_COM3.Count==0)
	{		
		if(data==0x3A)	
		{
			Rec_COM3.TimeOut=0;										//串口溢出计时
			flagCOM3.Bits.OverTime_Flag=TRUE;				//串口溢出计时
			Rec_COM3.Data[Rec_COM3.Count]=data;	
			Rec_COM3.Count++;	
		}				
	}	 
	else if(Rec_COM3.Count<60)
	{
		Rec_COM3.Data[Rec_COM3.Count]=data;
		Rec_COM3.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM3.Data[0]==0x3a)
		{
			if(Rec_COM3.Count>=4)
			{
				if(Rec_COM3.Data[Rec_COM3.Count-1]==0x0A && Rec_COM3.Data[Rec_COM3.Count-2]==0x0D)
				{
					check_data=LRC_Check(Rec_COM3.Data,(Rec_COM3.Count-5));					//LRC检验
					if(Rec_COM3.Data[Rec_COM3.Count-4]<0x3A)
					{
						check_orgi=Rec_COM3.Data[Rec_COM3.Count-4]%0x30;				
					}
					else
					{
						check_orgi=Rec_COM3.Data[Rec_COM3.Count-4]%0x41;
						check_orgi=check_orgi+10;	
					}
					check_orgi=check_orgi*0x10;						
					if(Rec_COM3.Data[Rec_COM3.Count-3]<0x3A)
					{
						check_orgi+=Rec_COM3.Data[Rec_COM3.Count-3]%0x30;				
					}
					else
					{
						check_orgi+=Rec_COM3.Data[Rec_COM3.Count-3]%0x41;
						check_orgi=check_orgi+10;	
					}
					if(check_data==check_orgi)
					{
						flagCOM3.Bits.Rec_Flag=TRUE;
					}				
					Rec_COM3.Count=0;	
					
					Rec_COM3.TimeOut=0;											//串口溢出计时
					flagCOM3.Bits.OverTime_Flag=FALSE;					//串口溢出计时				
										
				}
			}
		}				
	}	
	else
	{
		Rec_COM3.Count=0;					
	}	  							

}

/***********************************************************************
函数名称：void USART_COM4_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

void USART_COM4_GetInputString(void)
{
	
	uint8_t check_data;
	uint8_t check_orgi;
	uint8_t data;	
	data=USART_ReceiveData(UART4);	
	if(Rec_COM4.Count==0)
	{		
		if(data==0x3A)	
		{
			Rec_COM4.TimeOut=0;										//串口溢出计时
			flagCOM4.Bits.OverTime_Flag=TRUE;				//串口溢出计时
			Rec_COM4.Data[Rec_COM4.Count]=data;	
			Rec_COM4.Count++;	
		}				
	}	 
	else if(Rec_COM4.Count<60)
	{
		Rec_COM4.Data[Rec_COM4.Count]=data;
		Rec_COM4.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM4.Data[0]==0x3a)
		{
			if(Rec_COM4.Count>=4)
			{
				if(Rec_COM4.Data[Rec_COM4.Count-1]==0x0A && Rec_COM4.Data[Rec_COM4.Count-2]==0x0D)
				{
					check_data=LRC_Check(Rec_COM4.Data,(Rec_COM4.Count-5));					//LRC检验
					if(Rec_COM4.Data[Rec_COM4.Count-4]<0x3A)
					{
						check_orgi=Rec_COM4.Data[Rec_COM4.Count-4]%0x30;				
					}
					else
					{
						check_orgi=Rec_COM4.Data[Rec_COM4.Count-4]%0x41;
						check_orgi=check_orgi+10;	
					}
					check_orgi=check_orgi*0x10;						
					if(Rec_COM4.Data[Rec_COM4.Count-3]<0x3A)
					{
						check_orgi+=Rec_COM4.Data[Rec_COM4.Count-3]%0x30;				
					}
					else
					{
						check_orgi+=Rec_COM4.Data[Rec_COM4.Count-3]%0x41;
						check_orgi=check_orgi+10;	
					}
					if(check_data==check_orgi)
					{
						flagCOM4.Bits.Rec_Flag=TRUE;
					}				
					Rec_COM4.Count=0;	
					
					Rec_COM4.TimeOut=0;											//串口溢出计时
					flagCOM4.Bits.OverTime_Flag=FALSE;					//串口溢出计时				
										
				}
			}
		}				
	}	
	else
	{
		Rec_COM4.Count=0;					
	}	  							

}

/***********************************************************************
函数名称：void USART_COM5_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

void USART_COM5_GetInputString(void)
{
	
	uint8_t check_data;
	uint8_t check_orgi;
	uint8_t data;	
	data=USART_ReceiveData(UART5);	
	if(Rec_COM5.Count==0)
	{		
		if(data==0x3A)	
		{
			Rec_COM5.TimeOut=0;										//串口溢出计时
			flagCOM5.Bits.OverTime_Flag=TRUE;				//串口溢出计时
			Rec_COM5.Data[Rec_COM5.Count]=data;	
			Rec_COM5.Count++;	
		}				
	}	 
	else if(Rec_COM5.Count<60)
	{
		Rec_COM5.Data[Rec_COM5.Count]=data;
		Rec_COM5.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM5.Data[0]==0x3a)
		{
			if(Rec_COM5.Count>=4)
			{
				if(Rec_COM5.Data[Rec_COM5.Count-1]==0x0A && Rec_COM5.Data[Rec_COM5.Count-2]==0x0D)
				{
					check_data=LRC_Check(Rec_COM5.Data,(Rec_COM5.Count-5));					//LRC检验
					if(Rec_COM5.Data[Rec_COM5.Count-4]<0x3A)
					{
						check_orgi=Rec_COM5.Data[Rec_COM5.Count-4]%0x30;				
					}
					else
					{
						check_orgi=Rec_COM5.Data[Rec_COM5.Count-4]%0x41;
						check_orgi=check_orgi+10;	
					}
					check_orgi=check_orgi*0x10;						
					if(Rec_COM5.Data[Rec_COM5.Count-3]<0x3A)
					{
						check_orgi+=Rec_COM5.Data[Rec_COM5.Count-3]%0x30;				
					}
					else
					{
						check_orgi+=Rec_COM5.Data[Rec_COM5.Count-3]%0x41;
						check_orgi=check_orgi+10;	
					}
					if(check_data==check_orgi)
					{
						flagCOM5.Bits.Rec_Flag=TRUE;
					}				
					Rec_COM5.Count=0;	
					
					Rec_COM5.TimeOut=0;													//串口溢出计时
					flagCOM5.Bits.OverTime_Flag=FALSE;					//串口溢出计时				
										
				}
			}
		}				
	}	
	else
	{
		Rec_COM5.Count=0;					
	}	  							

}


/***********************************************************************
函数名称：uint8_t LRC_Check(uint8_t *data,uint8_t length)
功    能：LRC检验程序
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/
uint8_t LRC_Check(uint8_t *data,uint8_t length)
{
	uint8_t i;
	uint16_t k;
	uint8_t result;
	uint8_t lrcdata[RX_MaxSzie];
	for(i=1;i<length+1;i++)
	{
		if(data[i]>0x40)
			lrcdata[i-1]=data[i]-0x41+10;
		else
			lrcdata[i-1]=data[i]-0x30;	
	}		
	k=0;	
	for(i=0;i<length/2;i++)
	{		
		k+=(lrcdata[2*i]*16+lrcdata[2*i+1]); 	
	}
	k=k%256;
	k=256-k;
	result=k%256;
	return result;
}

/***********************************************************************
函数名称：void printf(uint8_t *str);
功    能：printf 重定义函数
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/***********************************************************************
函数名称：
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
      while (1)
      {
            ;
      }
}
#endif


/***********************************************************************
函数名称：void USART1_IRQHandler(void) 
功    能：完成SCI的数据的接收，并做标识
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意  RS485用的是USART1.
***********************************************************************/
void USART1_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			USART_COM1_GetInputString();											//串口接收数据处理
				
	}

	//**********************************************************	
	//发送中断处理程序
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}	
	
	//**********************************************************	
	//串口通信溢出处理程序
		/* If overrun condition occurs, clear the ORE flag and recover communication */
		if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
		{
			(void)USART_ReceiveData(USART1);
		}

}

/***********************************************************************
函数名称：void USART2_IRQHandler(void) 
功    能：UART_COM3串口中断处理程序
输入参数：
输出参数：
编写时间：2014.04.11
编 写 人：
注    意  
***********************************************************************/
void USART2_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			USART_COM2_GetInputString();											//串口接收数据处理
				
	}

	//**********************************************************	
	//发送中断处理程序
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART2, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}	
	
	//**********************************************************	
	//串口通信溢出处理程序
		/* If overrun condition occurs, clear the ORE flag and recover communication */
		if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
		{
			(void)USART_ReceiveData(USART2);
		}

}

/***********************************************************************
函数名称：void USART3_IRQHandler(void) 
功    能：UART_COM3串口中断处理程序
输入参数：
输出参数：
编写时间：2014.04.11
编 写 人：
注    意  
***********************************************************************/
void USART3_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
			USART_COM3_GetInputString();											//串口接收数据处理
				
	}

	//**********************************************************	
	//发送中断处理程序
	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART3, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}	
	
	//**********************************************************	
	//串口通信溢出处理程序
		/* If overrun condition occurs, clear the ORE flag and recover communication */
		if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
		{
			(void)USART_ReceiveData(USART3);
		}

}

/***********************************************************************
函数名称：void UART4_IRQHandler(void) 
功    能：UART_COM4串口中断处理程序
输入参数：
输出参数：
编写时间：2014.04.22
编 写 人：
注    意  
***********************************************************************/
void UART4_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{	
			USART_ClearITPendingBit(UART4, USART_IT_RXNE);
			USART_COM4_GetInputString();											//串口接收数据处理
				
	}

	//**********************************************************	
	//发送中断处理程序
	if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(UART4, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}	
	
	//**********************************************************	
	//串口通信溢出处理程序
		/* If overrun condition occurs, clear the ORE flag and recover communication */
		if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)
		{
			(void)USART_ReceiveData(UART4);
		}

}

/***********************************************************************
函数名称：void UART5_IRQHandler(void) 
功    能：UART_COM5串口中断处理程序
输入参数：
输出参数：
编写时间：2014.04.11
编 写 人：
注    意  
***********************************************************************/
void UART5_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{	
			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
			USART_COM5_GetInputString();											//串口接收数据处理
				
	}

	//**********************************************************	
	//发送中断处理程序
	if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(UART5, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
	}	
	
	//**********************************************************	
	//串口通信溢出处理程序
		/* If overrun condition occurs, clear the ORE flag and recover communication */
		if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)
		{
			(void)USART_ReceiveData(UART5);
		}

}
