/*
程序说明:  	
            USB_OTG 的常规初始化程序 --- STM32F107VCT 
						USB_OTG IO的设置，定时器TIM2的初始化
						USB_OTG 与 TIM2 中断处理
注意事项：						
						
						
端口使用:		见Doc文件夹内说明
创建者:     Masterjay   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/

//========================================
//头文件
#include "main.h"

#include "usb_hcd_int.h"
#include "usbh_core.h"

//========================================
//表量定义


//========================================
//函数声明


//========================================
//宏定义

	#define USE_ACCURATE_TIME						//使能 USE_ACCURATE_TIME 下定义
// 	#define OVERCURRENT_DET							//使能 OVERCURRENT_DET 下定义
// 	#define SOF_OUTPUT_ENABLED					//使能 SOF_OUTPUT_ENABLED 下定义
	
	
	
	#define TIM_MSEC_DELAY       0x01
	#define TIM_USEC_DELAY       0x02


	ErrorStatus HSEStartUpStatus;
#ifdef USE_ACCURATE_TIME 
	__IO uint32_t BSP_delay = 0;
#endif



#ifdef USE_ACCURATE_TIME 
	static void BSP_SetTime(uint8_t Unit);
	static void BSP_Delay(uint32_t nTime,uint8_t Unit);
#endif



//========================================
//子模块函数



/***********************************************************************
函数名称：void USB_OTG_BSP_Init(void)
功    能：USB IO端口初始化
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/

void USB_OTG_BSP_Init(void)
{

//  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
	

	#ifdef SOF_OUTPUT_ENABLED
  /* To enable SOF pulse out on PAD */
  
  RCC_APB2PeriphClockCmd(HOST_SOF_OUTPUT_RCC, ENABLE);
  
  /* Configure SOF Pin */
  GPIO_InitStructure.GPIO_Pin = HOST_SOF_SIGNAL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(HOST_SOF_PORT, &GPIO_InitStructure);
	#endif  
  
  /* Configure pin for overcurrent detection */

	#ifdef   OVERCURRENT_DET
  RCC_APB2PeriphClockCmd(HOST_OVRCURR_PORT_RCC, ENABLE);
  
  /* Configure SOF Pin */
  GPIO_InitStructure.GPIO_Pin = HOST_OVRCURR_LINE;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(HOST_OVRCURR_PORT, &GPIO_InitStructure);
  
  /* Connect Button EXTI Line to Button GPIO Pin */
  GPIO_EXTILineConfig(HOST_OVRCURR_PORT_SOURCE, HOST_OVRCURR_PIN_SOURCE);  
  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = HOST_OVRCURR_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
	#endif  

}

/***********************************************************************
函数名称：void USB_OTG_BSP_EnableInterrupt(void)
功    能：USB中断使能
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/
void USB_OTG_BSP_EnableInterrupt(void)
{

  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable USB Interrupt */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	#ifdef   OVERCURRENT_DET
  
  /* Enable the Overcurrent Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = HOST_OVRCURR_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure);  

	#endif
	
	
	
	
}

/***********************************************************************
函数名称：void USB_OTG_BSP_DriveVBUS(uint8_t state)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/

void USB_OTG_BSP_DriveVBUS(uint8_t state)
{
  /*
  On-chip 5 V VBUS generation is not supported. For this reason, a charge pump 
  or, if 5 V are available on the application board, a basic power switch, must 
  be added externally to drive the 5 V VBUS line. The external charge pump can 
  be driven by any GPIO output. When the application decides to power on VBUS 
  using the chosen GPIO, it must also set the port power bit in the host port 
  control and status register (PPWR bit in OTG_FS_HPRT).
  
  Bit 12 PPWR: Port power
  The application uses this field to control power to this port, and the core 
  clears this bit on an overcurrent condition.
  */
  if (0 == state)
  { 
    /* DISABLE is needed on output of the Power Switch */
    GPIO_ResetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);
  }
  else
  {
    /*ENABLE the Power Switch by driving the Enable LOW */
    GPIO_SetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);
  }
}

/***********************************************************************
函数名称：void  USB_OTG_BSP_ConfigVBUS(void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/

void  USB_OTG_BSP_ConfigVBUS(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  
  RCC_APB2PeriphClockCmd(HOST_POWERSW_PORT_RCC, ENABLE);
  
  
  /* Configure Power Switch Vbus Pin */
  GPIO_InitStructure.GPIO_Pin = HOST_POWERSW_VBUS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(HOST_POWERSW_PORT, &GPIO_InitStructure);
  
  /* By Default, DISABLE is needed on output of the Power Switch */
  GPIO_ResetBits(HOST_POWERSW_PORT, HOST_POWERSW_VBUS);
  
  USB_OTG_BSP_mDelay(200);   /* Delay is need for stabilising the Vbus Low 
  in Reset Condition, when Vbus=1 and Reset-button is pressed by user */
  
}



/***********************************************************************
函数名称：void USB_OTG_BSP_TimeInit ( void )
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：中断等级必须高，否则检测不到U盘是否拔出
***********************************************************************/
void USB_OTG_BSP_TimeInit ( void )
{
	#ifdef USE_ACCURATE_TIME   
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
  
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStructure);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	#endif  
}

/***********************************************************************
函数名称：void USB_OTG_BSP_uDelay (const uint32_t usec)
功    能：提供精确的us延时时间
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  
	#ifdef USE_ACCURATE_TIME    
  BSP_Delay(usec,TIM_USEC_DELAY); 
	#else
  __IO uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
	#endif   
  
}


/***********************************************************************
函数名称：void USB_OTG_BSP_mDelay (const uint32_t msec)
功    能：提供精确的ms延时时间
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/
void USB_OTG_BSP_mDelay (const uint32_t msec)
{ 
#ifdef USE_ACCURATE_TIME  
    BSP_Delay(msec,TIM_MSEC_DELAY);   
#else
    USB_OTG_BSP_uDelay(msec * 1000);   
#endif    

}


/***********************************************************************
函数名称：void USB_OTG_BSP_TimerIRQ (void)
功    能：USB定时中断处理程序
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：masterjay
注    意：
***********************************************************************/
void USB_OTG_BSP_TimerIRQ (void)
{
	#ifdef USE_ACCURATE_TIME 
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if (BSP_delay > 0x00)
    { 
      BSP_delay--;
    }
    else
    {
      TIM_Cmd(TIM2,DISABLE);
    }
  }
	#endif  
} 

#ifdef USE_ACCURATE_TIME 
/**
  * @brief  BSP_Delay
  *         Delay routine based on TIM2
  * @param  nTime : Delay Time 
  * @param  unit : Delay Time unit : mili sec / micro sec
  * @retval None
  */
static void BSP_Delay(uint32_t nTime, uint8_t unit)
{
  
  BSP_delay = nTime;
  BSP_SetTime(unit);  
  while(BSP_delay != 0);
  TIM_Cmd(TIM2,DISABLE);
}

/**
  * @brief  BSP_SetTime
  *         Configures TIM2 for delay routine based on TIM2
  * @param  unit : msec /usec
  * @retval None
  */
static void BSP_SetTime(uint8_t unit)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  TIM_Cmd(TIM2,DISABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
  
  
  if(unit == TIM_USEC_DELAY)
  {  
    TIM_TimeBaseStructure.TIM_Period = 11;
  }
  else if(unit == TIM_MSEC_DELAY)
  {
    TIM_TimeBaseStructure.TIM_Period = 11999;
  }
  TIM_TimeBaseStructure.TIM_Prescaler = 5;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  /* TIM2 enable counter */ 
  TIM_Cmd(TIM2, ENABLE);  
} 

#endif
          


/***********************************************************************
函数名称：void TIM2_IRQHandler(void)
功    能：TIM2中断处理程序
输入参数：
输出参数：
编写时间：2014.04.11
编 写 人：
注    意  
***********************************************************************/
void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}


/***********************************************************************
函数名称：void OTG_FS_IRQHandler(void)
功    能：TIM2中断处理程序
输入参数：
输出参数：
编写时间：2014.04.11
编 写 人：
注    意  
***********************************************************************/
void OTG_FS_IRQHandler(void)
{
  USBH_OTG_ISR_Handler(&USB_OTG_FS_dev);
}
