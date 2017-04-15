//========================================
//程序说明:       STM32F107VC中断处理程序
//端口使用: 
//创建者:         FangYikaii   
//创建时间:       2017-01-17 
//修改时间:       2017-01-17 
//========================================
//========================================
//头文件
#include "main.h"

//========================================
//变量定义


//========================================
//函数声明

void USART1_IRQHandler(void);    //UART_COM1串口中断处理程序
void USART2_IRQHandler(void);    //UART_COM2串口中断处理程序


//========================================
//子模块函数

//*********************************************************
//接收中断处理程序
/***********************************************************************
函数名称：void USART1_IRQHandler(void) 
功    能：UART_COM1串口中断处理程序
输入参数：
输出参数：
***********************************************************************/
void USART1_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	//标志位：USART_IT_RXNE!=RESET
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_COM1_GetInputString();
	}
	//**********************************************************	
	//发送中断处理程序
	//标志位：USART_IT_TXE!=RESET
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
功    能：UART_COM2串口中断处理程序
输入参数：
输出参数：
***********************************************************************/
void USART2_IRQHandler(void)  
{
	//**********************************************************
	//接收中断处理程序
	//标志位：USART_IT_RXNE!=RESET
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_COM2_GetInputString();											//串口接收数据处理
	}
	
	//**********************************************************	
	//发送中断处理程序
	//标志位：USART_IT_TXE!=RESET
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
