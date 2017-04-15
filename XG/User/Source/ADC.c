/*
程序说明:  	
            ADC采集处理程序 --- STM32F107VCT 
						
注意事项：	在定义ADC寄存器地址时，一定要使用__IO uint16_t数据格式，否则会出错。
						另外需注意ADCColValue[ADCCollectTime][ADCChannelNum]此数组的结构。					
						

创建者:     Masterjay   
创建时间:   2014-06-11 
修改时间:   2014-06-11
*/
//========================================
//头文件
#include "main.h"

//========================================
//变量定义
volatile union FlagADCA flagADC;
struct ADCResult ADCRes;

//========================================
//函数声明
void ADC_Configuration(void);										//完成ADC的配置
void ADC_ValueFilter(void);											//ADC采集值滤波


//========================================
//子模块函数







/***********************************************************************
函数名称：void ADC_Configuration(void)
功    能：完成ADC的配置
输入参数：
输出参数：
编写时间：2014.4.16
编 写 人：
注    意：
***********************************************************************/
void ADC_Configuration(void)
{

	ADC_InitTypeDef ADC_InitStructure;      //ADC初始化结构体声明
	DMA_InitTypeDef DMA_InitStructure;      //DMA初始化结构体声明
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Configure PC0-PC6 (ADC Channel10-15) as analog input -------------------------*/
  //PC0 作为模拟通道10输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN11_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN11_GPIOX, &GPIO_InitStructure);
  RCC_APB2PeriphClockCmd(ADCNN11_RCC_APB2Periph_GPIOX, ENABLE);	
	
  //PC1 作为模拟通道11输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN12_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN12_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN12_RCC_APB2Periph_GPIOX, ENABLE);	
	
  //PC2 作为模拟通道12输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN13_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN13_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN13_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC3 作为模拟通道13输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN14_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN14_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN14_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC4 作为模拟通道14输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN15_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN15_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN15_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC5 作为模拟通道15输入引脚                         
  GPIO_InitStructure.GPIO_Pin = ADCNN16_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN16_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN1_RCC_APB2Periph_GPIOX, ENABLE);
	
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;    								//DMA对应的外设基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCRes.ADCColValue;   			//内存存储基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;														//DMA的转换模式为SRC模式，由外设搬移到内存
  DMA_InitStructure.DMA_BufferSize = ADCCollectTime*ADCChannelNum;		   				//DMA缓存大小，ADCCollectTime*ADCChannelNum个,单位为DMA_MemoryDataSize
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//接收一次数据后，设备地址禁止后移
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//关闭接收一次数据后，目标内存地址后移
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  	//定义外设数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  					//DMA搬数据尺寸，HalfWord就是为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   														//转换模式，循环缓存模式。
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;														//DMA优先级高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  														//M2M模式禁用
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);           												//DMA_Channel1 通道初始化
	
 	DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1, ENABLE);																//DMA_Channel1 通道中断配置--6通道多次采样，耗时约为3ms	 
 	DMA_ClearITPendingBit(DMA1_IT_TC1);

	/* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0

	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  										//独立的转换模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  											//开启扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   										//开启连续转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//ADC外部开关，关闭状态
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   								//对齐方式,ADC为12位中，右对齐方式
  ADC_InitStructure.ADC_NbrOfChannel = ADCChannelNum;	 										//开启通道数，ADCChannelNum个
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration ADC通道组， 第10个通道 采样顺序1，转换时间 */ 
  ADC_RegularChannelConfig(ADC1, ADCNN11_ChannelNO	, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN12_ChannelNO	, 2, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN13_ChannelNO	, 3, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN14_ChannelNO	, 4, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN15_ChannelNO	, 5, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN16_ChannelNO	, 6, ADC_SampleTime_55Cycles5);	
	
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	  															//ADC命令，使能
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);  																	//开启ADC1
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);	  														//重新校准
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));  							//等待重新校准完成
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);																//开始校准
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	   								//等待校准完成
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);										//连续转换开始，ADC通过DMA方式不断的更新RAM区。

}

/***********************************************************************
函数名称：void ADC_ValueFilter(void)
功    能：ADC采集值滤波--耗时约3ms
输入参数：采集口
输出参数：
编写时间：2014.4.16
编 写 人：
注    意：
***********************************************************************/
void ADC_ValueFilter(void)
{
	uint32_t sum;
	uint16_t t;
	uint16_t i,j,k;
	float fvalue;
	for(k=0;k<ADCChannelNum;k++)									//按大到小顺序排列
	{
		for(i=0;i<(ADCCollectTime-1);i++)
		{
			for(j=i+1;j<ADCCollectTime;j++)
			{
				if(ADCRes.ADCColValue[i][k]>ADCRes.ADCColValue[j][k])
				{
					t=ADCRes.ADCColValue[i][k];
					ADCRes.ADCColValue[i][k]=ADCRes.ADCColValue[j][k];
					ADCRes.ADCColValue[j][k]=t;
				}				
			}		
			
		}		
	}
	for(k=0;k<ADCChannelNum;k++)									//均值滤波
	{
		sum=0;
		for(i=5;i<(ADCCollectTime-5);i++)
		{
			sum+=ADCRes.ADCColValue[i][k];	
		}
		fvalue=(3.3*sum)/4096;			 								//2^12=4096将AD值转换为电压值			
		fvalue=fvalue/(ADCCollectTime-10);	
		ADCRes.ADCResValue[k]=fvalue;
//		fvalue=fvalue*1000;
//		ADCRes.ADCResValue[k]=(uint16_t)fvalue;			
	}
}

 //========================================
//函数名称:void TIM5_IRQHandler(void)
//函数功能: TIM5中断处理程序
//入口参数:    
//出口参数:      
//========================================
/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA1_IT_TC1)
	{
		flagADC.Bits.ADCOK=TRUE;
		DMA_ClearITPendingBit(DMA1_IT_TC1);		
	}

}
