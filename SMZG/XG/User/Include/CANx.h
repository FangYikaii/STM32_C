//========================================
//<--预定义处理，防止重复定义-->
#ifndef _CAN_H
#define _CAN_H

//========================================
//头文件





//========================================
//宏定义
#define 			CAN1_OwnStdId				0x0701				//本机CAN1地址   【设定标准标识符11位，值0-0x7ff,拓展模式下为29位】
#define 			CAN2_OwnStdId				0x0702				//本机CAN2地址

#define 			CAN1_LinkStdId			0x0702				//CAN1对应连接机地址
#define 			CAN2_LinkStdId			0x0701				//CAN2对应连接机地址



//========================================
//端口定义

/*
							CAN1 端口情况
   CAN1TX -- PA12 , CAN1RX -- PA11 (Default);   CAN1TX -- PD1 , CAN1RX -- PD0 (Remap);					--此板选用 CAN1TX -- PD1 , CAN1RX -- PD0 
*/

#define 			CAN1TX_GPIOX												GPIOD
#define 			CAN1RX_GPIOX												GPIOD

#define 			CAN1TX_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOD
#define 			CAN1RX_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOD

#define 			CAN1TX_GPIO_PIN											GPIO_Pin_1
#define 			CAN1RX_GPIO_PIN											GPIO_Pin_0

/*
							CAN2 端口情况
   CAN2TX -- PB13 , CAN2RX -- PB12 (Default);   CAN2TX -- PB6 , CAN2RX -- PB5 (Remap);					--此板选用 CAN2TX -- PB6 , CAN2RX -- PB5
*/

#define 			CAN2TX_GPIOX												GPIOB
#define 			CAN2RX_GPIOX												GPIOB

#define 			CAN2TX_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOB
#define 			CAN2RX_RCC_APB2Periph_GPIOX					RCC_APB2Periph_GPIOB

#define 			CAN2TX_GPIO_PIN											GPIO_Pin_6
#define 			CAN2RX_GPIO_PIN											GPIO_Pin_5


//========================================
//外调变量声明
extern CAN_InitTypeDef        CAN_InitStructure;
extern CAN_FilterInitTypeDef  CAN_FilterInitStructure;
extern CanTxMsg CAN1TxMessage;
extern CanRxMsg CAN1RxMessage;
extern CanTxMsg CAN2TxMessage;
extern CanRxMsg CAN2RxMessage;

//========================================
//外调函数声明
extern void CAN_Configuration(void);				//完成CAN的配置




//========================================
//<--预处理结束-->
#endif



