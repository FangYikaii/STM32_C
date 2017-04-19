//========================================
//≥Ã–ÚÀµ√˜:       STM32F107VC÷–∂œµ»º∂≈‰÷√
//∂Àø⁄ π”√: 
//¥¥Ω®’ﬂ:          FangYikaii   
//¥¥Ω® ±º‰:       2017-01-17 
//–ﬁ∏ƒ ±º‰:       2013-01-17 
//========================================
//========================================
//”≈œ»≥Ã∂»£∫œ»«¿∂œNVIC_IRQChannelPreemptionPriority--‘ŸœÏ”¶NVIC_IRQChannelSubPriority
//÷–∂œœÚ¡ø  «¿’º”≈œ»º∂  œÏ”¶”≈œ»º∂
//A          0          0
//B          1          0
//C          1          1
//’˝‘⁄÷¥––C,Cø…±ª«¿’º”≈œ»º∂∏¸∏ﬂµƒA¥Ú∂œ£¨B∫ÕCµƒ«¿’º”≈œ»º∂“ª—˘£¨C≤ªƒ‹±ªB¥Ú∂œ£
//»ÙB°¢CÕ¨ ±µΩ¥Ô£¨Bœ»œÏ”¶

//Õ∑Œƒº˛
#include "main.h"

//========================================
//±‰¡ø∂®“Â



//========================================
//∫Ø ˝…˘√˜

void NVIC_Configuration(void);      //÷–∂œµ»º∂≈‰÷√  


//========================================
//◊”ƒ£øÈ∫Ø ˝

/*******************************************************************************
* Function Name    : void NVIC_Configuration(void)
* Description          : Configures the used IRQ Channels and sets their priority.
* Input                   : None
* Output                : None
* Return                : None
*******************************************************************************/
/***********************************************************************
∫Ø ˝√˚≥∆£∫ void NVIC_Configuration(void)
π¶    ƒ‹£∫÷–∂œµ»º∂≈‰÷√
 ‰»Î≤Œ ˝£∫
 ‰≥ˆ≤Œ ˝£∫
±‡–¥ ±º‰£∫2017.01.16
±‡ –¥ »À£∫
◊¢    “‚£∫
IO÷–∂œ∂‘”¶µƒ÷–∂œæ‰±˙πÿœµŒ™£∫	GPIOX0 -- EXTI0_IRQHandler
															GPIOX1 -- EXTI1_IRQHandler					
															GPIOX2 -- EXTI1_IRQHandler	
															GPIOX3 -- EXTI1_IRQHandler					
															GPIOX4 -- EXTI1_IRQHandler	

															GPIOX5~GPIOX9 -- EXTI9_5_IRQHandler						
															GPIOX10~GPIOX15 -- EXTI15_10_IRQHandler	
***********************************************************************/
void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//«¿∂œ”≈œ»º∂ 0   		œÏ”¶”≈œ»º∂ 0-15	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//«¿∂œ”≈œ»º∂ 0-1    œÏ”¶”≈œ»º∂ 0-7	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//«¿∂œ”≈œ»º∂ 0-3    œÏ”¶”≈œ»º∂ 0-3	
// 			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//«¿∂œ”≈œ»º∂ 0-7    œÏ”¶”≈œ»º∂ 0-1	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//«¿∂œ”≈œ»º∂ 0-15   œÏ”¶”≈œ»º∂ 0
  
	//USART1_IRQn
	/* Enable the USART1_IRQn gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	              //…Ë∂®Õ®µ¿
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //«¿∂œ”≈œ»º∂…Ë÷√ 0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	            //œÏ”¶”≈œ»º∂0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	//USART2_IRQn
	/* Enable the USART2_IRQn gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	              //…Ë∂®Õ®µ¿
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       //«¿∂œ”≈œ»º∂…Ë÷√ 0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	            //«¿∂œ”≈œ»º∂…Ë÷√ 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//TIM5-∆’Õ®º∆ ±
	//”≈œ»≥Ã∂»µÕ°æ«¿∂œ=15°ø
	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;							//…Ë∂®Õ®µ¿
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	//«¿∂œ”≈œ»º∂…Ë÷√  0ªÚ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//œÏ”¶”≈œ»º∂…Ë÷√
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	//TIM4-∆µ¬ ≤‚¡ø
	/* Enable the TIM5 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


// 	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =6;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);   
}
