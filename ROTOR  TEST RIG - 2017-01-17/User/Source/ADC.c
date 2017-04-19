/*
³ÌĞòËµÃ÷:  	
            ADC²É¼¯´¦Àí³ÌĞò --- STM32F107VCT 
						
×¢ÒâÊÂÏî£º	ÔÚ¶¨ÒåADC¼Ä´æÆ÷µØÖ·Ê±£¬Ò»¶¨ÒªÊ¹ÓÃ__IO uint16_tÊı¾İ¸ñÊ½£¬·ñÔò»á³ö´í¡£
						ÁíÍâĞè×¢ÒâADCColValue[ADCCollectTime][ADCChannelNum]´ËÊı×éµÄ½á¹¹¡£					
						

´´½¨Õß:     FangYikaii   
´´½¨Ê±¼ä:   2017-01-18 
ĞŞ¸ÄÊ±¼ä:   2017-01-18
*/

/*
ADC£ºAnalog-To-Digital Converter Ä£ÄâĞÅºÅ£ºĞÅÏ¢²ÎÊıÔÚ¸ø¶¨·¶Î§ÄÚ±íÏÖÎªÁ¬ĞøĞÅºÅ(µçÑ¹/µçÁ÷/ÉùÒô) Êı×ÖĞÅºÅ£º·ù¶ÈµÄÈ¡ÖµÊÇÀëÉ¢µÄ£¬·ùÖµ±íÊ¾Îª±»ÏŞÖÆÔÚÓĞÏŞ¸öÊıÖµÄÚ
ADC: ×ª»»¾«¶ÈÓĞ¹Ø,Ò»°ã12Î»¾«¶È---¶ÔÄ£ÄâĞÅºÅÖğ´Î±Æ½ü×ª»¯ÎªÊı×ÖĞÅºÅÀ´×öµÄ£¨Í¨¹ıÄ£ÄâĞÅºÅÓëĞ¾Æ¬ÄÚ²¿Ä£ÄâĞÅºÅ²¿¼ş£¬Ò²¾ÍÊÇ²Î¿¼µçÑ¹µÄ¶Ô±È£¬×ª±ä³ÉÊı×ÖĞÅºÅ¸øĞ¾Æ¬ÄÚ²¿µÄÊı×ÖĞÅºÅ¼Ä´æÆ÷£¬ÓÃ0/1·ÅÔÚ¼Ä´æÆ÷£©
²½Öè£º1¡¢²ÉÑù£º°ÑÒ»¶ÎÄ£ÄâĞÅºÅ·Ö½âÎªĞí¶à½Úµã£
			2¡¢±£³Ö£º°ÑĞÅºÅ±£³ÖÒ»¶ÎÊ±¼ä£¬·ñÔò¿ÉÄÜÊÇÒ»¸ö¸ÉÈÅ»òÕßĞÅºÅÃ«´Ì£¬ºóĞøµÄÁ¿»¯¹ı³ÌĞèÒªÒ»¶ÎµÄÊ±¼ä
			3¡¢Á¿»¯£º·ùÖµÁ¿»¯£¬°Ñ²ÉÑùĞÅºÅÍ¨¹ıÉáÈë»ò½ØÎ²µÄ·½·¨±äÎªÖ»ÓĞÓĞÏŞ¸öÓĞĞ§Êı×ÖµÄÊı
			4¡¢±àÂë£º½«ÀëÉ¢·ùÖµ±äÎª¶ş½øÖÆÊı

ADC·Ö±æÂÊ£»Êä³öÊı×ÖÁ¿±ä»¯Ò»¸ö×îĞ¡Á¿Ê±£¬Ä£ÄâĞÅºÅµÄ±ä»¯Á¿£¬8Î»AD,¿ÉÒÔÃèÊö255¸ö¿Ì¶ÈµÄ¾«¶È,±ÈÈç²âÁ¿5vµçÑ¹£¬¾«¶È5/255=0.02vµÄ·Ö±æÂÊ
Á¿»¯Îó²î£ºADCÓĞÏŞÎ»Êı¶ÔÄ£ÄâÁ¿½øĞĞÁ¿»¯¶øÒıÆğµÄÎó²î
×ª»»ËÙÂÊ£º×ª»»ËÙÂÊÊÇÖ¸Íê³ÉÒ»´Î´ÓÄ£Äâ×ª»»µ½Êı×ÖËùĞèÒªµÄÊ±¼äµÄµ¹Êı
ÏßĞÔ¶È£ºÊµ¼Ê×ª»»ÌØĞÔÓëÀíÏëÖ±ÏßµÄ×î´óÆ«²î
¾ø¶Ô¾«¶È£ºÊµ¼ÊÄ£ÄâÁ¿ÓëÀíÂÛÄ£ÄâÊäÈëµÄÎó²îµÄ×î´óÖµ

ADC¹©µçÒªÇó£º2.4V-3.6V

Ê¹ÓÃ·½·¨£º1.ÅäÖÃ×î´óÖµºÍ×îĞ¡Öµ ADCRes.ADCMaxValue[ADCChannelNum] ADCRes.ADCMinValue[ADCChannelNum] 
					2.²É¼¯Ã¿Ãë£¬Ã¿¸öÍ¨µÀµÄADCÖµ£¬¼ÆËã²É¼¯¸öÊı²¢ÇóºÍ---¼ÆËãÃ¿ÃëÃ¿¸öÍ¨µÀµÄADCÆ½¾ùÖµ£¬´ËÊ±Æ½¾ùÖµÎªµçÑ¹
					3.Àı£ºÈç¹û²âÑ¹Á¦£¬Ñ¹Á¦Öµ0-25MPA£¬ÓÃ¹éÒ»µÄÖµ*Ñ¹Á¦ÏµÊı  (ADCRes.ADCSumValue[5]-ADCRes.ADCMinValue[5])*25/(ADCRes.ADCMaxValue[5]-ADCRes.ADCMinValue[5])  ¡¾Ñ¹Á¦¡¿
					4.ADCµÄºÍ£¬ADCµÄCount¹éÁã
*/
//========================================
//Í·ÎÄ¼ş
#include "main.h"

//========================================
//±äÁ¿¶¨Òå
volatile union FlagADCA flagADC;
struct ADCResult ADCRes;

//========================================
//º¯ÊıÉùÃ÷
void ADC_Configuration(void);										//Íê³ÉADCµÄÅäÖÃ
void ADC_ValueFilter(void);											//ADC²É¼¯ÖµÂË²¨


//========================================
//×ÓÄ£¿éº¯Êı


/***********************************************************************
º¯ÊıÃû³Æ£ºvoid ADC_Configuration(void)
¹¦    ÄÜ£ºÍê³ÉADCµÄÅäÖÃ
ÊäÈë²ÎÊı£º
Êä³ö²ÎÊı£º
±àĞ´Ê±¼ä£º 2017-01-18 
±à Ğ´ ÈË£º FangYikaii
×¢    Òâ£º
***********************************************************************/
void ADC_Configuration(void)
{

	ADC_InitTypeDef ADC_InitStructure;      //ADC³õÊ¼»¯½á¹¹ÌåÉùÃ÷
	DMA_InitTypeDef DMA_InitStructure;      //DMA³õÊ¼»¯½á¹¹ÌåÉùÃ÷
  GPIO_InitTypeDef GPIO_InitStructure;    //GPIO³õÊ¼»¯½á¹¹ÌåÉùÃ÷
	NVIC_InitTypeDef NVIC_InitStructure;    //NVIC³õÊ¼»¯½á¹¹ÌåÉùÃ÷
	
	//GPIOµÄÅäÖÃ
	
  /* Configure PC0-PC6 (ADC Channel10-15) as analog input -------------------------*/
  //PC0 ×÷ÎªÄ£ÄâÍ¨µÀ10ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN11_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      //Ä£ÄâÊäÈë
  GPIO_Init(ADCNN11_GPIOX, &GPIO_InitStructure);
  RCC_APB2PeriphClockCmd(ADCNN11_RCC_APB2Periph_GPIOX, ENABLE);	
	
  //PC1 ×÷ÎªÄ£ÄâÍ¨µÀ11ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN12_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN12_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN12_RCC_APB2Periph_GPIOX, ENABLE);	
	
  //PC2 ×÷ÎªÄ£ÄâÍ¨µÀ12ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN13_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN13_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN13_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC3 ×÷ÎªÄ£ÄâÍ¨µÀ13ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN14_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN14_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN14_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC4 ×÷ÎªÄ£ÄâÍ¨µÀ14ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN15_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN15_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN15_RCC_APB2Periph_GPIOX, ENABLE);
	
  //PC5 ×÷ÎªÄ£ÄâÍ¨µÀ15ÊäÈëÒı½Å                         
  GPIO_InitStructure.GPIO_Pin = ADCNN16_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADCNN16_GPIOX, &GPIO_InitStructure);	
  RCC_APB2PeriphClockCmd(ADCNN1_RCC_APB2Periph_GPIOX, ENABLE);
	
	//DMAµÄÅäÖÃ DMA1
	
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;    								//DMA¶ÔÓ¦µÄÍâÉè»ùµØÖ·
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCRes.ADCColValue;   			//ÄÚ´æ´æ´¢»ùµØÖ·
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;														//DMAµÄ×ª»»Ä£Ê½ÎªSRCÄ£Ê½£¬ÓÉÍâÉè°áÒÆµ½ÄÚ´æ
  DMA_InitStructure.DMA_BufferSize = ADCCollectTime*ADCChannelNum;		   				//DMA»º´æ´óĞ¡£¬ADCCollectTime*ADCChannelNum¸ö,µ¥Î»ÎªDMA_MemoryDataSize
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//½ÓÊÕÒ»´ÎÊı¾İºó£¬Éè±¸µØÖ·½ûÖ¹ºóÒÆ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//¹Ø±Õ½ÓÊÕÒ»´ÎÊı¾İºó£¬Ä¿±êÄÚ´æµØÖ·ºóÒÆ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  	//¶¨ÒåÍâÉèÊı¾İ¿í¶ÈÎª16Î»
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  					//DMA°áÊı¾İ³ß´ç£¬HalfWord¾ÍÊÇÎª16Î»
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   														//×ª»»Ä£Ê½£¬Ñ­»·»º´æÄ£Ê½¡£
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;														//DMAÓÅÏÈ¼¶¸ß
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  														//M2MÄ£Ê½½ûÓÃ
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);           												//DMA_Channel1 Í¨µÀ³õÊ¼»¯
	
 	DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1, ENABLE);																//DMA_Channel1 Í¨µÀÖĞ¶ÏÅäÖÃ--6Í¨µÀ¶à´Î²ÉÑù£¬ºÄÊ±Ô¼Îª3ms	 
 	DMA_ClearITPendingBit(DMA1_IT_TC1);

	/* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

	//NVICµÄÅäÖÃ£¬2ÇÀ¶Ï£¬0ÏìÓ¦
	
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//ÇÀ¶ÏÓÅÏÈ¼¶ 0-15   ÏìÓ¦ÓÅÏÈ¼¶ 0
	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	//ADCµÄÅäÖÃ£¬ADC1

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  										//¶ÀÁ¢µÄ×ª»»Ä£Ê½
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  											//¿ªÆôÉ¨ÃèÄ£Ê½
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   										//¿ªÆôÁ¬Ğø×ª»»Ä£Ê½
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//ADCÍâ²¿¿ª¹Ø£¬¹Ø±Õ×´Ì¬
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   								//¶ÔÆë·½Ê½,ADCÎª12Î»ÖĞ£¬ÓÒ¶ÔÆë·½Ê½
  ADC_InitStructure.ADC_NbrOfChannel = ADCChannelNum;	 										//¿ªÆôÍ¨µÀÊı£¬ADCChannelNum¸ö
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration ADCÍ¨µÀ×é£¬ µÚ10¸öÍ¨µÀ ²ÉÑùË³Ğò1£¬×ª»»Ê±¼ä */ 
  ADC_RegularChannelConfig(ADC1, ADCNN11_ChannelNO	, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN12_ChannelNO	, 2, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN13_ChannelNO	, 3, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN14_ChannelNO	, 4, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN15_ChannelNO	, 5, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADCNN16_ChannelNO	, 6, ADC_SampleTime_55Cycles5);	
	
	//ADC1×ª»»³öÀ´µÄÊı¾İÍ¨¹ıDMA1´«Êä	
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	  															//ADCÃüÁî£¬Ê¹ÄÜDMA´«Êä
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);  																	//¿ªÆôADC1
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);	  														//ÖØĞÂĞ£×¼
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));  							//µÈ´ıÖØĞÂĞ£×¼Íê³É
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);																//¿ªÊ¼Ğ£×¼
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	   								//µÈ´ıĞ£×¼Íê³É
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);										//Á¬Ğø×ª»»¿ªÊ¼£¬ADCÍ¨¹ıDMA·½Ê½²»¶ÏµÄ¸üĞÂRAMÇø¡£

}

/***********************************************************************
º¯ÊıÃû³Æ£ºvoid ADC_ValueFilter(void)
¹¦    ÄÜ£ºADC²É¼¯ÖµÂË²¨--ºÄÊ±Ô¼3ms
ÊäÈë²ÎÊı£º
Êä³ö²ÎÊı£º
±àĞ´Ê±¼ä£º2017.01.18
±à Ğ´ ÈË£º
×¢    Òâ£º
***********************************************************************/
void ADC_ValueFilter(void)
{
	uint32_t sum;
	uint16_t t;
	uint16_t i,j,k;
	float fvalue;
	
	//ADCRes.ADCColValue[][k]°´´óµ½Ğ¡Ë³ĞòÅÅÁĞ
	//²É¼¯Í¨µÀ¸öÊıµÄÑ­»·
	for(k=0;k<ADCChannelNum;k++)			
	{
		//²É¼¯Ê±¼äÄÚµÄÑ­»·
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
	//²É¼¯Í¨µÀ¸öÊıµÄÑ­»·
	for(k=0;k<ADCChannelNum;k++)									//¾ùÖµÂË²¨
	{
		sum=0;
		for(i=5;i<(ADCCollectTime-5);i++)
		{
			sum+=ADCRes.ADCColValue[i][k];	
		}
		//
		fvalue=(3.3*sum)/4096;			 								//2^12=4096,½«ADÖµ×ª»»ÎªµçÑ¹Öµ3.3V		
		//²ÉÑùÊ±¼äÄÚµÄ¾ùÖµ
		fvalue=fvalue/(ADCCollectTime-10);	
		ADCRes.ADCResValue[k]=fvalue;
//		fvalue=fvalue*1000;
//		ADCRes.ADCResValue[k]=(uint16_t)fvalue;			
	}
}

//========================================
//º¯ÊıÃû³Æ:void TIM5_IRQHandler(void)
//º¯Êı¹¦ÄÜ: TIM5ÖĞ¶Ï´¦Àí³ÌĞò
//Èë¿Ú²ÎÊı:    
//³ö¿Ú²ÎÊı:      
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
