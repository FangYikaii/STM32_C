/*
程序说明:  	
            CANx 通信处理程序 --- STM32F107VCT 
						
注意事项：						
						1、注意 CANx 的IO端口选择，默认或者映射管脚
						
						
						
						
						
端口使用:		见Doc文件夹内说明
创建者:     FangYIkaii 
创建时间:   2017-01-20 
修改时间:   2017-01-20
*/
/*
CAN(Controller Area Network)控制器局域网络by BOSCH：
以报文的形式广播给网络中的所有节点，对每个节点来说，无论是否发给自己，都会接收；报文--开头11位字符位标识符，定义优先级，面向内容的编制方案；统一个系统的标识符是唯一的
CAN数据导线：黄色&绿色--CAN_High&CAN_Low; 静止状态：静电平约2.5V(隐性电平)；激活状态：如CAN_High升高一个值，CAN_Low降低同样的值，3.5V&2.5V；电压差到差动信号放大器
报文：[11位识别符：标准帧数；29位识别符：扩展帧]
1.数据帧：将数据从发送器传输到接收器
2.远程帧：总线节点发出远程帧，请求发送具有同一标识符的数据帧
3.错误帧：任何节点监测到总线错误就发出错误帧
4.过载帧：过载帧用已经先行的后续的数据帧之间提供一个附加的延时
<1>数据帧： 1.帧起始SOF:有电压差(数据)了
				  	2.仲裁场：如果有2个或者两个以上的单元同时开始传送报文，就会有总线访问冲突，仲裁机制根据标识符优先级一个一个去掉低级别的数据
						(标准格式：11位识别符和RTR位组成；扩展格式：29位识别符，SRR位，IDE位，RTR位)
						RTR位：Remote Tranmission Request Bit，远程发送请求位，在数据帧里必须为显性0，远程帧里为隐性1
						SRR位：替代远程请求位
						IDE位：Idenfitier Extension Bit
						3.控制场：6位
						4.数据场：0-8字节
						5.CRC场：Cyclic Redundancy Check 循环冗余校验码
						6.应答场ACK:长度为2个位，包含应答间隙和应答界定符，在应答场里，发送站发两个隐性位，接收器正确接收有效报文，接收器向发送器发送显性位
<2>远程帧：总线节点发送远程帧，请求以前发送给它数据帧的节点再发送一遍，具体发送哪个数据帧，由远程帧标识符决定
<3>错误帧：第一个场是不同站提供的错误标志的叠加；第二个场是错误界定符
<4>过载帧：过载标志和过载界定符
【接收消息 结构体】
typedef struct
{
  uint32_t StdId;  < Specifies the standard identifier.  指定标准格式标识符
                        This parameter can be a value between 0 to 0x7FF. 

  uint32_t ExtId;  < Specifies the extended identifier.  指定扩展格式标识符
                        This parameter can be a value between 0 to 0x1FFFFFFF.

  uint8_t IDE;     < Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type   指定将收到的消息类型

  uint8_t RTR;     < Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request   指定收到的消息框架

  uint8_t DLC;     < Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8  指定将受到消息框架的长度

  uint8_t Data[8]; < Contains the data to be received. It ranges from 0 to 
                        0xFF.  数据

  uint8_t FMI;     < Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF  指定过滤器的索引，所过滤的消息存储在mailbox里
} CanRxMsg;
【发送消息 结构体】
typedef struct
{
  uint32_t StdId;  < Specifies the standard identifier.  指定标准格式标识符
                        This parameter can be a value between 0 to 0x7FF. 

  uint32_t ExtId;  < Specifies the extended identifier.  指定扩展格式标识符
                        This parameter can be a value between 0 to 0x1FFFFFFF.

  uint8_t IDE;     < Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type   指定将收到的消息类型

  uint8_t RTR;     < Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request   指定收到的消息框架

  uint8_t DLC;     < Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8  指定将受到消息框架的长度

  uint8_t Data[8]; < Contains the data to be received. It ranges from 0 to 
                        0xFF.  数据
} CanRxMsg;
*/
//========================================
//头文件
#include "main.h"

//========================================
//变量定义
//CAN初始化
CAN_InitTypeDef        CAN_InitStructure;
//CAN过滤器初始化
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//CAN1发送信息初始化
CanTxMsg CAN1TxMessage;
//CAN1接收信息初始化
CanRxMsg CAN1RxMessage;
//CAN2发送信息初始化
CanTxMsg CAN2TxMessage;
//CAN2接收信息初始化
CanRxMsg CAN2RxMessage;

//========================================
//函数声明
void CAN_Configuration(void);				//完成CAN的配置


//========================================
//子模块函数

/***********************************************************************
函数名称：void CAN_Configuration(void)
功    能：完成CAN的配置
输入参数：
输出参数：
编写时间：2017.01.20
编 写 人：FangYIkaii
注    意：key1->PE2,
					key2->PE3,
					key3->PE4,
					key4->PE5,
***********************************************************************/
void CAN_Configuration(void)
{
	//GPIO结构体初始化
  GPIO_InitTypeDef  GPIO_InitStructure;
	//NVIC结构体初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能CAN1&CAN2的(Tx&Rx)外部时钟
  /* GPIOB, GPIOD and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(CAN1TX_RCC_APB2Periph_GPIOX, ENABLE);
  RCC_APB2PeriphClockCmd(CAN1RX_RCC_APB2Periph_GPIOX, ENABLE);
  RCC_APB2PeriphClockCmd(CAN2TX_RCC_APB2Periph_GPIOX, ENABLE);
  RCC_APB2PeriphClockCmd(CAN2RX_RCC_APB2Periph_GPIOX, ENABLE);	
	
	//使能CAN1&CAN2的外部时钟
  /* CAN1 and CAN2 Periph clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE); 
  
	//CAN1_Rx GPIO [上拉输入IPU]
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = CAN1RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(CAN1RX_GPIOX, &GPIO_InitStructure);
   
	//CAN2_Rx GPIO [上拉输入IPU]
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = CAN2RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
  GPIO_Init(CAN2RX_GPIOX, &GPIO_InitStructure);

	//CAN1_Tx GPIO [推挽输出PP]
  /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = CAN1TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CAN1TX_GPIOX, &GPIO_InitStructure);

	//CAN2_Tx GPIO [推挽输出PP]
  /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = CAN2TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(CAN2TX_GPIOX, &GPIO_InitStructure);

	//是否映射管脚
  /* Remap CAN1 and CAN2 GPIOs */
  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);					//注意IO端口选择，此处选择映射管脚
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);						//注意IO端口选择，此处选择映射管脚

	//关闭初始化CAN1&CAN2
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;									//时间触发通讯模式关闭
  CAN_InitStructure.CAN_ABOM = DISABLE;									//需软件请求从离线状态恢复
  CAN_InitStructure.CAN_AWUM = DISABLE;									//由软件来请求离开休眠模式
  CAN_InitStructure.CAN_NART = DISABLE;									//使能重传
  CAN_InitStructure.CAN_RFLM = DISABLE;									//一旦FIFO满，下一条传入的报文将覆盖
  CAN_InitStructure.CAN_TXFP = DISABLE;									//设置发送FIFO优先级；由标识符决定优先级
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;					//设CAN为正常模式
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;							//设置波特率=(72MHZ/2)/16/(1+3+5)=0.25MHZ=250KHz
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 16;									//波特率预分频数
	//初始化 CAN1&CAN2
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN1 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;												//对过滤器0进行设置
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;			//掩摸模式
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;		//32位
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;									//设置匹配的位
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;							//对标识符进行比较
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;								//将过滤器粘附到管道0
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
 	//初始化CAN过滤器 CAN1&CAN2 
  /* CAN2 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* IT Configuration for CAN1 */  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  /* IT Configuration for CAN2 */  
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	
	//优先级配置
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0
	//CAN1
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//CAN2
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
}

/***********************************************************************
函数名称：void CAN1_RX0_IRQHandler(void)
功    能：CAN1接收中断处理程序
输入参数：
输出参数：
编写时间：2017.01.20
编 写 人：FangYikaii
注    意  
***********************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	//CAN1 Receive
  CAN_Receive(CAN1, CAN_FIFO0, &CAN1RxMessage);

//*******************************************************************
//EEC1 电子发动机控制器1	--  50HZ
	if(CAN1RxMessage.ExtId == 0x0CF00400)														
	{
		XG.PumpSpeed=(CAN1RxMessage.Data[4]<<8)+CAN1RxMessage.Data[3];	//柴油发动机转速 0-8031.88 RPM
		XG.PumpSpeed=XG.PumpSpeed*0.125;																//柴油发动机转速应为实际读取值*0.125(CAN中所谓的扫描因子)		
	}
//*******************************************************************
//EEC2 	电子发动机控制器2		--  20HZ
	if(CAN1RxMessage.ExtId == 0x0CF00300)														
	{
		XG.Throttle=CAN1RxMessage.Data[1];															//油门位置 0-100
		XG.Throttle=XG.Throttle*0.4;																		//油门位置应为实际读取值*0.4(CAN中所谓的扫描因子)					
	}	
//*******************************************************************
//EEC3 	电子发动机控制器3  --  5HZ
//	if(CAN1RxMessage.ExtId == 0x18FEDF00)														
//	{
//				
//	}		

//*******************************************************************
//FlConsum	燃油消耗量
//	if(CAN1RxMessage.ExtId == 0x18FEE900)														
//	{
//		TotalFuelConsumption=(CAN1RxMessage.Data[7]<<24)+(CAN1RxMessage.Data[6]<<16)+(CAN1RxMessage.Data[5]<<8)+CAN1RxMessage.Data[4];										//总燃油消耗量
//				
//	}	

//*******************************************************************
//FLDPRES 燃油压力  --  2HZ
//	if(CAN1RxMessage.ExtId == 0x18FEE200)														
//	{		
//			
//	}	

//*******************************************************************
//FlEco 燃油经济性 -- 10HZ
//	if(CAN1RxMessage.ExtId == 0x18FEF200)														
//	{
//		FuelRate=(CAN1RxMessage.Data[1]<<8)+CAN1RxMessage.Data[0];										//小时燃油消耗率 L/H
//		InstantaneousFuelEconomy=(CAN1RxMessage.Data[3]<<8)+CAN1RxMessage.Data[2];		//瞬态燃油经济性 km/L
//		
//	}	


		//*******************************************************************
		//CAN1口读取值马上通过CAN2口转发
		
		CAN2TxMessage.StdId = 0x0000;									//设定标准标识符11位，值0-0x7ff -- 0x00~发动机控制单元
		CAN2TxMessage.ExtId = CAN1RxMessage.ExtId;		//设定拓展标准标识符29位，值0-0x1fffffff
		CAN2TxMessage.RTR = CAN_RTR_DATA;							//数据帧  CAN_RTR_REMOTE // CAN_RTR_DATA
		CAN2TxMessage.IDE = CAN_ID_EXT;								//标准帧格式[ExtID无效]CAN_ID_EXT //CAN_ID_STD
		CAN2TxMessage.DLC = 8;												//帧长度；单位字节；即发送几个数据	
		CAN2TxMessage.Data[0]=CAN1RxMessage.Data[0];
		CAN2TxMessage.Data[1]=CAN1RxMessage.Data[1];
		CAN2TxMessage.Data[2]=CAN1RxMessage.Data[2];
		CAN2TxMessage.Data[3]=CAN1RxMessage.Data[3];
		CAN2TxMessage.Data[4]=CAN1RxMessage.Data[4];
		CAN2TxMessage.Data[5]=CAN1RxMessage.Data[5];
		CAN2TxMessage.Data[6]=CAN1RxMessage.Data[6];
		CAN2TxMessage.Data[7]=CAN1RxMessage.Data[7];
		CAN_Transmit(CAN2, &CAN2TxMessage);		

//*******************************************************************
//附加发送值

//*******************************************************************
//附加发送1 -- ETC2  0x18F00503	10Hz
		if(CAN1RxMessage.ExtId == 0x18FEF200)			//FlEco 燃油经济性 -- 10HZ													
		{
			//过程挡位
			if(flagctrl.Bits.KDchange)												//KD键使能--1挡保持			
			{
				if(ControllerInfo.ProcessGear/0x10==0)					//前进挡
				{
					CAN2TxMessage.Data[0]=125+0x01+10;				
				}
				else if(ControllerInfo.ProcessGear/0x10==1)			//后退挡
				{
					CAN2TxMessage.Data[0]=125-0x01-10;			
				}
				else if(ControllerInfo.ProcessGear/0x10==2)			//空挡
				{
					CAN2TxMessage.Data[0]=125;
				}			
			}
			else
			{	
				if(ControllerInfo.ProcessGear/0x10==0)															//前进挡
				{
					CAN2TxMessage.Data[0]=125+ControllerInfo.ProcessGear+10;				
				}
				else if(ControllerInfo.ProcessGear/0x10==1)													//后退挡
				{
					CAN2TxMessage.Data[0]=125+0x10-ControllerInfo.ProcessGear-10;			
				}
				else if(ControllerInfo.ProcessGear/0x10==2)													//空挡
				{
					CAN2TxMessage.Data[0]=125+ControllerInfo.ProcessGear-0x20;
				}			
			}
			//目标挡位
			if(ControllerInfo.GoalGear/0x10==0)																		//前进挡
			{
				CAN2TxMessage.Data[3]=125+ControllerInfo.GoalGear+10;				
			}
			else if(ControllerInfo.GoalGear/0x10==1)															//后退挡
			{
				CAN2TxMessage.Data[3]=125+0x10-ControllerInfo.GoalGear-10;			
			}
			else if(ControllerInfo.GoalGear/0x10==2)															//空挡
			{
				CAN2TxMessage.Data[3]=125+ControllerInfo.GoalGear-0x20;
			}				
		
			CAN2TxMessage.StdId = 0x0000;									//设定标准标识符11位，值0-0x7ff -- 0x00~发动机控制单元
			CAN2TxMessage.ExtId = 0x18F00503;						  //设定拓展标准标识符29位，值0-0x1fffffff  【ETC2】
			CAN2TxMessage.RTR = CAN_RTR_DATA;							//数据帧  CAN_RTR_REMOTE // CAN_RTR_DATA
			CAN2TxMessage.IDE = CAN_ID_EXT;								//标准帧格式[ExtID无效]CAN_ID_EXT //CAN_ID_STD
			CAN2TxMessage.DLC = 8;												//帧长度；单位字节；即发送几个数据	
//			CAN2TxMessage.Data[0]=0x00;									//选择挡位 -- 过程挡位								
			CAN2TxMessage.Data[1]=0x00;										//
			CAN2TxMessage.Data[2]=0x00;										//
//			CAN2TxMessage.Data[3]=0x00;									//选择挡位 -- 目标挡位				
			CAN2TxMessage.Data[4]=ErrorMessage;						//报警信息
			CAN2TxMessage.Data[5]=0x00;										//
			CAN2TxMessage.Data[6]=0x00;										//
			CAN2TxMessage.Data[7]=0x00;										//
			CAN_Transmit(CAN2, &CAN2TxMessage);							
		
		}
		
//*******************************************************************
//附加发送2 -- EEC4  0x0CF00500	50Hz
//附加发送3 -- EEC5  0x0CF00600	50Hz				
		if(CAN1RxMessage.ExtId == 0x0CF00400)		//EEC1 电子发动机控制器1	--  50HZ												
		{
			CAN2TxMessage.StdId = 0x0000;									//设定标准标识符11位，值0-0x7ff -- 0x00~发动机控制单元
			CAN2TxMessage.ExtId = 0x0CF00500;						  //设定拓展标准标识符29位，值0-0x1fffffff  【EEC4】
			CAN2TxMessage.RTR = CAN_RTR_DATA;							//数据帧  CAN_RTR_REMOTE // CAN_RTR_DATA
			CAN2TxMessage.IDE = CAN_ID_EXT;								//标准帧格式[ExtID无效]CAN_ID_EXT //CAN_ID_STD
			CAN2TxMessage.DLC = 8;												//帧长度；单位字节；即发送几个数据				
			CAN2TxMessage.Data[0]=XG.CANPressure[0]%0x100;				//							
			CAN2TxMessage.Data[1]=XG.CANPressure[0]/0x100;				//压力传感器值1
			CAN2TxMessage.Data[2]=XG.CANPressure[1]%0x100;				//
			CAN2TxMessage.Data[3]=XG.CANPressure[1]/0x100;				//压力传感器值2
			CAN2TxMessage.Data[4]=XG.CANPressure[2]%0x100;				//
			CAN2TxMessage.Data[5]=XG.CANPressure[2]/0x100;				//压力传感器值3
			CAN2TxMessage.Data[6]=XG.CANPressure[3]%0x100;				//
			CAN2TxMessage.Data[7]=XG.CANPressure[3]/0x100;				//压力传感器值4
			CAN_Transmit(CAN2, &CAN2TxMessage);				
	
			CAN2TxMessage.StdId = 0x0000;									//设定标准标识符11位，值0-0x7ff -- 0x00~发动机控制单元
			CAN2TxMessage.ExtId = 0x0CF00600;						  //设定拓展标准标识符29位，值0-0x1fffffff  【EEC5】
			CAN2TxMessage.RTR = CAN_RTR_DATA;							//数据帧  CAN_RTR_REMOTE // CAN_RTR_DATA
			CAN2TxMessage.IDE = CAN_ID_EXT;								//标准帧格式[ExtID无效]CAN_ID_EXT //CAN_ID_STD
			CAN2TxMessage.DLC = 8;												//帧长度；单位字节；即发送几个数据	
			CAN2TxMessage.Data[0]=XG.CANPressure[4]%0x100;			//							
			CAN2TxMessage.Data[1]=XG.CANPressure[4]/0x100;			//压力传感器值5
			CAN2TxMessage.Data[2]=XG.CANPressure[5]%0x100;			//
			CAN2TxMessage.Data[3]=XG.CANPressure[5]/0x100;			//压力传感器值6
			CAN2TxMessage.Data[4]=0x00;										//
			CAN2TxMessage.Data[5]=0x00;										//
			CAN2TxMessage.Data[6]=0x00;										//
			CAN2TxMessage.Data[7]=0x00;										//
			CAN_Transmit(CAN2, &CAN2TxMessage);			
		}
//*******************************************************************		
//附加发送4 -- EEC6  0x0CF00700	50Hz			
	if(CAN1RxMessage.ExtId == 0x0CF00300)		//EEC2 	电子发动机控制器2		--  20HZ		
	{
			CAN2TxMessage.StdId = 0x0000;									//设定标准标识符11位，值0-0x7ff -- 0x00~发动机控制单元
			CAN2TxMessage.ExtId = 0x0CF00700;						  //设定拓展标准标识符29位，值0-0x1fffffff  【EEC6】
			CAN2TxMessage.RTR = CAN_RTR_DATA;							//数据帧  CAN_RTR_REMOTE // CAN_RTR_DATA
			CAN2TxMessage.IDE = CAN_ID_EXT;								//标准帧格式[ExtID无效]CAN_ID_EXT //CAN_ID_STD
			CAN2TxMessage.DLC = 8;												//帧长度；单位字节；即发送几个数据	
			CAN2TxMessage.Data[0]=XG.CANSpeed[0]%0x100;					//							
			CAN2TxMessage.Data[1]=XG.CANSpeed[0]/0x100;					//转速值1
			CAN2TxMessage.Data[2]=XG.CANSpeed[1]%0x100;					//
			CAN2TxMessage.Data[3]=XG.CANSpeed[1]/0x100;					//转速值2
			CAN2TxMessage.Data[4]=XG.CANSpeed[2]%0x100;					//
			CAN2TxMessage.Data[5]=XG.CANSpeed[2]/0x100;					//转速值3
			CAN2TxMessage.Data[6]=XG.CANSpeed[3]%0x100;					//
			CAN2TxMessage.Data[7]=XG.CANSpeed[3]/0x100;					//转速值4
			CAN_Transmit(CAN2, &CAN2TxMessage);			
	}
		
}

/***********************************************************************
函数名称：void CAN2_RX0_IRQHandler(void)
功    能：CAN2接收中断处理程序
输入参数：
输出参数：
编写时间：2017-01-20
编 写 人：FangYIkaii
注    意：
***********************************************************************/
void CAN2_RX0_IRQHandler(void)
{
  CAN_Receive(CAN2, CAN_FIFO0, &CAN2RxMessage);

}
