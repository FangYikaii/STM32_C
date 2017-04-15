/*
程序说明:  	
            USARTx 串口通信程序 --- STM32F107VCT 
						
注意事项：	1、注意管脚的复用问题						
						
						
端口使用:		见Doc文件夹内说明
创建者:     FangYIkaii   
创建时间:   2017-01-19
修改时间:   2017-01-19
*/
/*
RS232：三条接口线，发送数据、接收数据、信号地-----传输电平信号（信号1为-3V到-15V,信号0为3V到15V,容易损坏接口电路芯片，还需要转换电平电路与TTL电路相连，抗干扰差）
2:RxD,Input 3:TxD,Output 5:GND

RS485：传输差分信号（信号1以电压差+2-+6V表示，信号0以两线电压差-2V--6V表示，与TTL电平兼容，方便与TTL电路的连接）
485+ 485-

传输距离：RS232：15到25m，最高波特率19200bps   RS485：120到3000米，最高传输速率10Mbps
*/
/*
USARTx_TX 全双工模式          推挽复用输出
          半双工同步模式      推挽复用输出
USARTx_RX 全双工模式          浮空输入或带上拉输入
          半双工同步模式      未用，可作为通用I/O
GPIO_MODE_AIN 模拟输入  GPIO_MODE_FLOATING 浮空输入   GPIO_MODE_IPD 下拉输入  GPIO_MODE_IPU 上拉输入
GPIO_MODE_Out_OD 开漏输出  GPIO_MODE_Out_PP 推挽输出  GPIO_MODE_AF_OD 复用开漏输出  GPIO_MODE_AF_PP 复用推挽输出
*/
//========================================
//头文件
#include "main.h"

//========================================
//变量定义
//定义结构体 UART结果
struct UartResult Rec_COM1;
struct UartResult Rec_COM2;
//定义联合体 UART标志位
volatile union FlagUart flagCOM1;
volatile union FlagUart flagCOM2;
//定义联合体 UART接收标志位
volatile union FlagUartRec flagUartRecCOM1;
volatile union FlagUartRec flagUartRecCOM2;

USART_InitTypeDef USART_InitStructure;
//COM_USART[COMn] 串口名字
USART_TypeDef* COM_USART[COMn] = {UART_COM1, UART_COM2, UART_COM3, UART_COM4, UART_COM5}; 
//COM_TX_PORT[COMn] 串口发送端口
GPIO_TypeDef* COM_TX_PORT[COMn] = {UART_COM1_TX_GPIO_PORT, UART_COM2_TX_GPIO_PORT, UART_COM3_TX_GPIO_PORT, UART_COM4_TX_GPIO_PORT, UART_COM5_TX_GPIO_PORT}; 
//COM_RX_PORT[COMn] 串口接收端口
GPIO_TypeDef* COM_RX_PORT[COMn] = {UART_COM1_RX_GPIO_PORT, UART_COM2_RX_GPIO_PORT, UART_COM3_RX_GPIO_PORT, UART_COM4_RX_GPIO_PORT, UART_COM5_RX_GPIO_PORT}; 
//COM_USART_CLK[COMn] 串口时钟
const uint32_t COM_USART_CLK[COMn] = {UART_COM1_CLK, UART_COM2_CLK, UART_COM3_CLK, UART_COM4_CLK, UART_COM5_CLK};
//COM_TX_PORT_CLK[COMn] 串口发送时钟
const uint32_t COM_TX_PORT_CLK[COMn] = {UART_COM1_TX_GPIO_CLK, UART_COM2_TX_GPIO_CLK, UART_COM3_TX_GPIO_CLK, UART_COM4_TX_GPIO_CLK, UART_COM5_TX_GPIO_CLK};
//COM_RX_PORT_CLK[COMn] 串口接收时钟
const uint32_t COM_RX_PORT_CLK[COMn] = {UART_COM1_RX_GPIO_CLK, UART_COM2_RX_GPIO_CLK, UART_COM3_RX_GPIO_CLK, UART_COM4_RX_GPIO_CLK, UART_COM5_RX_GPIO_CLK};
//COM_TX_PIN[COMn] 串口发送管脚
const uint16_t COM_TX_PIN[COMn] = {UART_COM1_TX_PIN, UART_COM2_TX_PIN, UART_COM3_TX_PIN, UART_COM4_TX_PIN, UART_COM5_TX_PIN};
//COM_RX_PIN[COMn] 串口接收管脚
const uint16_t COM_RX_PIN[COMn] = {UART_COM1_RX_PIN, UART_COM2_RX_PIN, UART_COM3_RX_PIN, UART_COM4_RX_PIN, UART_COM5_RX_PIN};

//========================================
//函数声明
void Uart_Init(void);   //串口初始化
void STM_UART_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);	//串口初始配置程序
void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data);	//串口初始配置程序

void USART_COM1_GetInputString(void);			//串口接收数据处理
void USART_COM2_GetInputString(void);			//串口接收数据处理
void RS485_COM1_PutChar(USART_TypeDef* USARTx, uint8_t Data);	//RS485_COM2发送单字节数据

uchar LRC_Check(uchar *data,uchar length);	//LRC检验程序

//========================================
//子模块函数



/***********************************************************************
函数名称：void Uart_Init(void)
功    能：Uart串口初始化
输入参数：无
输出参数：无
编写时间：2017-01-19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void Uart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      //定义GPIO结构体	

	//RS485 COM1
	RCC_APB2PeriphClockCmd(RS485_COM1_RCC_APB2Periph_EnGPIOX, ENABLE); 					 
	GPIO_InitStructure.GPIO_Pin = RS485_COM1_EnPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
	GPIO_Init(RS485_COM1_EnGPIOX, &GPIO_InitStructure);
	RS485_COM1_RxEnable(1);																			//RS485_COM1处于接收状态

//串口计数清零
	Rec_COM1.Count=0;	
	Rec_COM2.Count=0;	
	
//串口参数设置	
	USART_InitStructure.USART_BaudRate = 9600;                          //波特率 9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //数据位 8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;              //停止位 1位
	USART_InitStructure.USART_Parity = USART_Parity_No;                 //奇偶校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //模式：Rx||Tx

	STM_UART_COMInit(COM1, &USART_InitStructure);				//串口初始配置程序--IO设置等
	USART_ITConfig(UART_COM1, USART_IT_RXNE, ENABLE);		//串口接收中断使能
//USART_ITConfig(UART_COM1, USART_IT_TXE, ENABLE);		//串口发送中断使能
	USART_ITConfig(UART_COM1, USART_IT_TXE, DISABLE);		//串口发送中断使能不能开，否则没办法接收
	USART_ClearITPendingBit(UART_COM1, USART_IT_TC);		//清除中断TC位
		
	STM_UART_COMInit(COM2, &USART_InitStructure);				//串口初始配置程序--IO设置等
	USART_ITConfig(UART_COM2, USART_IT_RXNE, ENABLE);		//串口接收中断使能
//USART_ITConfig(UART_COM2, USART_IT_TXE, ENABLE);		//串口发送中断使能
	USART_ITConfig(UART_COM2, USART_IT_TXE, DISABLE);		//串口发送中断不能开，否则没办法接收
	USART_ClearITPendingBit(UART_COM2, USART_IT_TC);		//清除中断TC位
	
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
		
			//USART Tx 发送端配置
      /* Configure USART Tx as alternate function push-pull */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     //USART Tx:发送端的输出--推挽输出PUSH PULL
      GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];                      //USART Tx:发送端的管脚定义--
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   //USART Tx:发送端的速度--50MHz
      GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);                   //USART Tx:发送端的端口配置

			//USART Tx 发送端配置			
      /* Configure USART Rx as input floating */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //USART Rx:接收端的输入--浮空输入FLOATING
      GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];                      //USART Rx:接收端的管脚定义--
      GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);                   //USART Rx:接收端的端口配置--
			
			//USART 配置
      /* USART configuration */
      USART_Init(COM_USART[COM], USART_InitStruct);
    
			//USART 使能
      /* Enable USART */
      USART_Cmd(COM_USART[COM], ENABLE);
}



/***********************************************************************
函数名称：void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data)
功    能：串口发送单字节数据 char  0x00
输入参数：串口号，数值
输出参数：无
编写时间：2017-01-19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void SerialPutChar(USART_TypeDef* USARTx, uint8_t Data)
{
    USART_SendData(USARTx, Data);
		//直到复位跳出
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//  while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == SET)) 
    {
        ;
    }
}

/***********************************************************************
函数名称：void RS485_COM1_PutChar(USART_TypeDef* USARTx, uint8_t Data)
功    能：RS485_COM1发送单字节数据  char 0x00
输入参数：串口号，数值
输出参数：无
编写时间：2017-01-19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void RS485_COM1_PutChar(USART_TypeDef* USARTx, uint8_t Data)
{
		RS485_COM1_RxEnable(0);																	//RS485_COM1发送状态	
		delay(10);
    USART_SendData(USARTx, Data);
	  //直到复位跳出
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//  while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == SET)) 
    {
        ;
    }
		RS485_COM1_RxEnable(1);																	//RS485_COM1处于接收状态		
}


/***********************************************************************
函数名称：void USART_COM1_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2017-01-19
编 写 人：FangYikaii
注    意：发送：0x3A 0x01 0x长度 ...内容... 0x0D 0x0A   [3A起始位；01选择发送模式；第三位长度；0D 0A 结束位][内容长度为第三位定义的长度]
					读取：0x3A 0x02 0x长度 ...内容... 0x0D 0x0A   [3A起始位；02选择读取模式；第三位长度；0D 0A 结束位][内容长度为第三位定义的长度] 
***********************************************************************/
void USART_COM1_GetInputString(void)
{
	uchar data;	
	data=USART_ReceiveData(USART1);	
	
	//第一位为0
	if(Rec_COM1.Count==0)
	{		
		//起始位0x3A
		if(data==0x3A)	
		{
			Rec_COM1.Data[Rec_COM1.Count]=data;	
			Rec_COM1.Count++;	
		}				
	}	 
	//第二位
	else if(Rec_COM1.Count==1)
	{
		Rec_COM1.Data[Rec_COM1.Count]=data;
		//第二位：0x01 发送模式
		if(Rec_COM1.Data[Rec_COM1.Count]==0x01)
		{
			flagUartRecCOM1.Bits.SetFlag=TRUE;
			Rec_COM1.Count++;
		}
		//第二位：0x02 读取模式
		else if(Rec_COM1.Data[Rec_COM1.Count]==0x02)
		{
			flagUartRecCOM1.Bits.ReadFlag=TRUE;
			Rec_COM1.Count++;
		}
		else
		{
			Rec_COM1.Count=0;
		}
	}
	//长度--读取的内容长度
	else if(Rec_COM1.Count==2)
	{
		Rec_COM1.Data[Rec_COM1.Count]=data;
		Rec_COM1.length=Rec_COM1.Data[Rec_COM1.Count];
		Rec_COM1.Count++;
	}
	else  
	{
		//发送模式
		if(flagUartRecCOM1.Bits.SetFlag==TRUE)
		{			
			Rec_COM1.Data[Rec_COM1.Count]=data;
			Rec_COM1.Count++;					
			//数据格式  Count>内容长度+其他5个格式定义字符 && 最后是0x0A && 最后第二个是0x0D
			if((Rec_COM1.Count>=Rec_COM1.length+5)&&Rec_COM1.Data[Rec_COM1.length+4]==0x0A && Rec_COM1.Data[Rec_COM1.length+3]==0x0D)
			{	
					flagUartRecCOM1.Bits.SetFlag=FALSE;
					Rec_COM1.Count=0;
					flagUartRecCOM1.Bits.UART_Rec_Flag = TRUE;
			}
			else			
			{
				if(Rec_COM1.Count>=RX_MaxSize)
				{
					//发送标志位置位
					flagUartRecCOM1.Bits.SetFlag=FALSE;
					//Count置0
					Rec_COM1.Count=0;
				}					
			}
		}
		//读取模式
		if(flagUartRecCOM1.Bits.ReadFlag==TRUE)
		{
				Rec_COM1.Data[Rec_COM1.Count]=data;
				Rec_COM1.Count++;											
				if((Rec_COM1.Count>=Rec_COM1.length+5)&&Rec_COM1.Data[Rec_COM1.length+4]==0x0A && Rec_COM1.Data[Rec_COM1.length+3]==0x0D)
				{	
						//读取标志位置位
						flagUartRecCOM1.Bits.ReadFlag=FALSE;
						//Count置0
						Rec_COM1.Count=0;
						//写串口1接收标志位
						flagUartRecCOM1.Bits.UART_Rec_Flag = TRUE;					
				}
				else
				{
					if(Rec_COM1.Count>=RX_MaxSize)
					{
						//读取标志位置位
						flagUartRecCOM1.Bits.ReadFlag=FALSE;
						//Count置0
						Rec_COM1.Count=0;
					}					
				}					
		}
	}				
}

/***********************************************************************
函数名称：void USART_COM2_GetInputString(void)
功    能：串口接收数据处理
输入参数：无
输出参数：无
编写时间：2017-01-19
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USART_COM2_GetInputString(void)
{
	uchar check_data;
	uchar check_orgi;
	uchar data;	
	data=USART_ReceiveData(USART2);	
	if(Rec_COM2.Count==0)
	{		
		if(data==0x3A)	
		{
			Rec_COM2.TimeOut=0;										//串口溢出计时
//			flagA.Bits.COM2_TimeFlag=TRUE;				//串口溢出计时
			Rec_COM2.Data[Rec_COM2.Count]=data;	
			Rec_COM2.Count++;	
		}				
	}	 
	else if(Rec_COM2.Count<60)
	{
		Rec_COM2.Data[Rec_COM2.Count]=data;
		Rec_COM2.Count++;											//数据格式  3A   ...  0D 0A
		if(Rec_COM2.Data[0]==0x3a)
		{
			if(Rec_COM2.Count>=4)
			{
				if(Rec_COM2.Data[Rec_COM2.Count-1]==0x0A && Rec_COM2.Data[Rec_COM2.Count-2]==0x0D)
				{
					check_data=LRC_Check(Rec_COM2.Data,(Rec_COM2.Count-5));					//LRC检验
					if(Rec_COM2.Data[Rec_COM2.Count-4]<0x3A)
					{
						check_orgi=Rec_COM2.Data[Rec_COM2.Count-4]%0x30;				
					}
					else
					{
						check_orgi=Rec_COM2.Data[Rec_COM2.Count-4]%0x41;
						check_orgi=check_orgi+10;	
					}
					check_orgi=check_orgi*0x10;						
					if(Rec_COM2.Data[Rec_COM2.Count-3]<0x3A)
					{
						check_orgi+=Rec_COM2.Data[Rec_COM2.Count-3]%0x30;				
					}
					else
					{
						check_orgi+=Rec_COM2.Data[Rec_COM2.Count-3]%0x41;
						check_orgi=check_orgi+10;	
					}
					if(check_data==check_orgi)
					{
						flagUartRecCOM2.Bits.UART_Rec_Flag=TRUE;
					}				
					Rec_COM2.Count=0;					
					Rec_COM2.TimeOut=0;										     	//串口溢出计时
					flagCOM2.Bits.OverTime_Flag=FALSE;					//串口溢出计时							
										
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
函数名称：uchar LRC_Check(uchar *data,uchar length)
功    能：LRC检验程序
输入参数：无
输出参数：无
编写时间：2014.04.11
编 写 人：masterjay
注    意：
***********************************************************************/
uchar LRC_Check(uchar *data,uchar length)
{
	uchar i;
	uint k;
	uchar result;
	uchar lrcdata[RX_MaxSize];
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

