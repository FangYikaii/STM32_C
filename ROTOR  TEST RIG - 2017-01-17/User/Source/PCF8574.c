/*
³ÌÐòËµÃ÷:  	
            PCF8574 IOÍØÕ¹µçÂ·Éè¼Æ³ÌÐò --- STM32F107VCT 
						
						µØÖ·(0x70) -- LEDÏÔÊ¾¿ØÖÆµØÖ·
						µØÖ·(0x72) -- °´¼ü¶ÁÈ¡¿ØÖÆµØÖ·
						µØÖ·(0x7A) -- µç´Å·§¿ØÖÆÐ¾Æ¬Êä³öÐÅºÅ¶ÁÈ¡µØÖ·(Î´ÓÃ)
						
						
×¢ÒâÊÂÏî£º						
						1¡¢PCF8574 µÄSCL¡¢SDA¡¢INT µÈ¶Ë¿ÚÐèÒª¼Ó10Kµç×èÉÏÀ­£¬·ñÔò¶ÁÐ´»á³ö´í¡£						
						2¡¢PCF8574 ÖÐ¶ÏÉèÖÃ·ÅÓÚEXTI.CÄÚ£¬×¢ÒâIO¿ÚÒ²ÐèÒª³õÊ¼»¯¡£
						3¡¢PCF8574 ÖÐ¶ÏÉ¨Ãè¾­³£³öÏÖÎÊÌâ£¬½¨ÒéÊ¹ÓÃÑ­»·É¨Ãè£¬ÊÓÇé¿ö¶ø¶¨¡£

¶Ë¿ÚÊ¹ÓÃ:		¼ûDocÎÄ¼þ¼ÐÄÚËµÃ÷
´´½¨Õß:     FangYIkaii 
´´½¨Ê±¼ä:   2017-01-20 
ÐÞ¸ÄÊ±¼ä:   2017-01-20
*/
/*
##I2C-EEPROMµÄ¶ÁÐ´²âÊÔÊµÑé£ºÓë³£¼ûµÄEEPROMÐ¾Æ¬AT2402ÎªÀý µç²Á³ýµÄROM##
I2C(Inter-Integrated Circuit)¼¯³ÉµçÂ·×ÜÏß£ºÓÉÊý¾ÝÏßSDAºÍÊ±ÖÓSCL¹¹³ÉµÄ´®ÐÐ×ÜÏß£¬¿ÉÒÔ·¢ËÍºÍ½ÓÊÕÊý¾Ý£¬ËÙ¶È¿ÉÒÔ´ïµ½400Kbs
1.Ã¿¸öÁ¬½Óµ½×ÜÏßµÄÉè±¸¿ÉÒÔÍ¨¹ýÎ¨Ò»µÄÉè±¸µØÖ·µ¥¶À·ÃÎÊ
2.´®ÐÐ8Î»Ë«ÏòÊý¾Ý´«Êä£¬Î»ËÙÂÊÏÂ±ê×¼Ä£Ê½¿ÉÒÔ´ïµ½100kbs/s,¿ìËÙÄ£Ê½400kbs/s,¸ßËÙÄ£Ê½3.4Mb/s
3.×ÜÏß³¤¶È×î³¤7.6m£¬Æ¬ÉÏÂË²¨Æ÷¿ÉÒÔÔö¼Ó¿¹¸ÉÈÅÄÜÁ¦£
4.×ÜÏßÊÇ7Î»µØÖ·£¬×î¶à128¸öÉè±¸£»¶àÖ÷»úÏµÍ³£¬Ò»Ìõ×ÜÏßÉÏ¿ÉÒÔÍ¬Ê±ÓÐ¶à¸öÖ÷»ú´æÔÚ£¬Í¨¹ý³åÍ»¼ì²âºÍÑÓÊ±·ÀÖ¹Êý¾Ý²»±»ÆÆ»µ

¹¤×÷×´Ì¬£º
¿ªÊ¼ÐÅºÅ£ºSCL¸ßµçÆ½ÆÚ¼ä£¬SDAÓÉ¸ßµçÆ½ÏòµÍµçÆ½Ìø±ä£¬¿ªÊ¼´«ËÍÊý¾Ý
½áÊøÐÅºÅ£ºSCL¸ßµçÆ½ÆÚ¼ä£¬SDAÓÉµÍµçÆ½Ïò¸ßµçÆ½Ìø±ä£¬½áÊø´«ËÍÊý¾Ý
Ó¦´ðÐÅºÅ£º½ÓÊÕÊý¾ÝµÄIC½ÓÊÕ8bitÊý¾Ýºó£¬Ïò·¢ËÍÊý¾ÝµÄIC·¢³öÌØ¶¨µÄµÍµçÆ½Âö³å£¬±íÊ¾ÊÕµ½Êý¾Ý£¬ÈôÎ´ÊÕµ½Ó¦´ðÐÅºÅ£¬ÅÐ¶ÏÊÜ¿Øµ¥Ôª¹ÊÕÏ

STM32F107µÄI2C½Ó¿ÚÌØÐÔÈçÏÂ£º
1.Ð¾Æ¬¾ùÓÐ2¸öI2C×ÜÏß½Ó¿Ú£¬I2C1£¬I2C2
2.ÄÜ¹»¹¤×÷Óë¶àÖ÷Ä£Ê½»ò´ÓÄ£Ê½£¬Ö§³Ö±ê×¼Ä£Ê½100kbs/sºÍ¿ìËÙÄ£Ê½400kbs/s,²»Ö§³Ö7Î»Ñ°Ö·
3.ÄÚÖÃÓ²¼þcrc·¢ÉúÐ£ÑéÆ÷£¬½ÓÊÕ·¢ËÍ¿ÉÒÔÍ¨¹ýDMA

SCL:Ê±Ðò¼´ÎªI2CÐ­ÒéÖÐµÄÊ±ÖÓÐÅºÅ£¬ÓÉI2C½Ó¿Ú¸ù¾ÝÊ±ÖÓ¿ØÖÆ¼Ä´æÆ÷CCR¿ØÖÆ£¬¿ØÖÆµÄ²ÎÊýÖ÷ÒªÎªÊ±ÖÓÆµÂÊ
SDA:Í¨¹ýÒ»ÏµÁÐÊý¾Ý¿ØÖÆ¼Ü¹¹£¬ÔÚ½«Òª·¢ËÍµÄÊý¾ÝµÄ»ù´¡ÉÏ£¬¸ù¾ÝÐ­ÒéÌí¼Ó¸÷ÖÖÆðÊ¼ÐÅºÅ¡¢Ó¦´ðÐÅºÅ¡¢µØÖ·ÐÅºÅ£¬ÊµÏÖÒÔI2CÐ­Òé·¢ËÍ³öÈ¥
*/
/*
<----PCF8574 µØÖ· Êý¾Ý   ---->
            µØÖ·    Êý¾Ý
³õÊ¼»¯     0x70      0x00
¶Á¼üÅÌ     0x73
   

<----PCF8574 ¼üÅÌ¼üÖµ ÅäÖÃ  ---->
S1  -- 0xFE --  S9
S2  -- 0xFD --  S10
S3  -- 0xFB --  S11	
S4  -- 0xF7 --  S12

S5  -- 0xEF --  S13
S6  -- 0xDF --  S14
S7  -- 0xBF --  S15	
S8  -- 0x7F --  S16
*/

//******************************************
//Í·ÎÄ¼þ
#include "main.h"

//******************************************
//±äÁ¿¶¨Òå
uint8_t KeyValue_PCF8574;								//PCF8574¼üÅÌÉ¨ÃèÖµ
volatile union FlagPCF8574 PCF8574;

//******************************************
//º¯ÊýÉùÃ÷
void Init_PCF8574(void);																	//PCF8574µÄ³õÊ¼»¯³ÌÐò

void I2C_Start_PCF8574(void);															//PCF8574µÄI2C¿ªÊ¼ÐÅºÅ
void I2C_Stop_PCF8574(void);															//PCF8574µÄI2CÍ£Ö¹ÐÅºÅ
void I2C_NOAck_PCF8574(void);															//PCF8574µÄI2C·¢ËÍÓ¦´ðÐÅÏ¢
void I2C_Ack_PCF8574(void);																//PCF8574µÄI2C½ÓÊÕÓ¦´ðÐÅÏ¢
void I2C_Write1Byte_PCF8574(uint8_t data);									//PCF8574µÄI2CÐ´Ò»¸ö×Ö½Ú³ÌÐò
uint8_t I2C_Rec1Byte_PCF8574(void);													//PCF8574µÄI2C½ÓÊÕÒ»¸ö×Ö½Ú³ÌÐò

void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data);				//PCF8574µÄI2CÐ´Êý¾Ý³ÌÐò
uint8_t I2C_RecData_PCF8574(uint8_t addr);										//PCF8574µÄI2C¶ÁÊý¾Ý³ÌÐò
void KeyScan_PCF8574(void);																//PCF8574µÄ¼üÅÌÉ¨Ãè³ÌÐò

//******************************************
//×ÓÄ£¿éº¯Êý


/***********************************************************************
º¯ÊýÃû³Æ£ºvoid Init_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄ³õÊ¼»¯³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017.01.20
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£º
***********************************************************************/
void Init_PCF8574(void)
{
	//GPIO½á¹¹Ìå³õÊ¼»¯
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//GPIOÅäÖÃ-SCL
	//Ê¹ÄÜÊ±ÖÓ£¬¹Ü½Å£¬¶Ë¿Ú£¬ËÙ¶È[50MHz]£¬Ä£Ê½[PPÍÆÍìÊä³ö]
	RCC_APB2PeriphClockCmd(SCL_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SCL_PCF8574 configure 					 			
	GPIO_InitStructure.GPIO_Pin = SCL_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SCL_PCF8574_GPIOX, &GPIO_InitStructure);	
	
	//GPIOÅäÖÃ-SDA
	//Ê¹ÄÜÊ±ÖÓ£¬¹Ü½Å£¬¶Ë¿Ú£¬ËÙ¶È[50MHz]£¬Ä£Ê½[PPÍÆÍìÊä³ö]
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 			
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	
	//SCLºÍSDAÐ´µÍµçÆ½
	SCL_PCF8574(0);
	SDA_PCF8574(0);
	
	I2C_WriteData_PCF8574(0x70, 0x00);		//³õÊ¼»¯LEDÏÔÊ¾¿ØÖÆ
	
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_Start_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄI2C¿ªÊ¼ÐÅºÅ
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£ºSCL¸ßµçÆ½ÆÚ¼ä£¬SDAÓÉ¸ßµçÆ½ÏòµÍµçÆ½Ìø±ä£¬¿ªÊ¼´«ËÍÊý¾Ý
***********************************************************************/
void I2C_Start_PCF8574(void)
{
	//SDAÐ´¸ßµçÆ½
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);	
	//SCLÐ´¸ßµçÆ½
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SDAÐ´µÍµçÆ½(¸ßµçÆ½-µÍµçÆ½-Ìø±ä)
	SDA_PCF8574(0);	
	delay(DelayTime_PCF8574);	
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_Stop_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄI2CÍ£Ö¹ÐÅºÅ
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£ºSCL¸ßµçÆ½ÆÚ¼ä£¬SDAÓÉµÍµçÆ½Ïò¸ßµçÆ½Ìø±ä£¬½áÊø´«ËÍÊý¾Ý
***********************************************************************/
void I2C_Stop_PCF8574(void)
{
	//SDAÐ´µÍµçÆ½
	SDA_PCF8574(0);
	delay(DelayTime_PCF8574);	
	//SCLÐ´¸ßµçÆ½
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SDAÐ´¸ßµçÆ½(µÍµçÆ½-¸ßµçÆ½-Ìø±ä)
	SDA_PCF8574(1);	
	delay(DelayTime_PCF8574);	
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_NOAck_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄI2C·¢ËÍÓ¦´ðÐÅÏ¢
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£º½ÓÊÕÊý¾ÝµÄIC½ÓÊÕ8bitÊý¾Ýºó£¬Ïò·¢ËÍÊý¾ÝµÄIC·¢³öÌØ¶¨µÄµÍµçÆ½Âö³å£¬±íÊ¾ÊÕµ½Êý¾Ý£¬ÈôÎ´ÊÕµ½Ó¦´ðÐÅºÅ£¬ÅÐ¶ÏÊÜ¿Øµ¥Ôª¹ÊÕÏ
          ·¢ËÍ£ºSCL:µÍ--[¶¯×÷]--¸ß-µÍ  SDA:¸ß
***********************************************************************/
void I2C_NOAck_PCF8574(void)
{	
	//SCLÐ´µÍµçÆ½
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);			
	//SDAÐ´¸ßµçÆ½
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);				
	//SCLÐ´¸ßµçÆ½
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	//SCLÐ´µÍµçÆ½
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_Ack_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄI2C½ÓÊÕÓ¦´ðÐÅÏ¢
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£º½ÓÊÕ£ºSCL:¸ß--µÍ  ¶ÁSDA¸ßµçÆ½
***********************************************************************/
void I2C_Ack_PCF8574(void)
{
	uint8_t i;
	
	//SDA GPIOÅäÖÃ--¸¡¿ÕÊäÈë--FLOATING
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE);	 	//SDA_PCF8574 configure 					 	
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);
	delay(DelayTime_PCF8574);	

	//SCLÐ´¸ßµçÆ½
	SCL_PCF8574(1);	
	delay(DelayTime_PCF8574);	
	//¶ÁSDAÊäÈëµÄ¸ßµçÆ½
	while((SDA_Read_PCF8574==1) && (i<200))
	{
		i++;
	}
	delay(DelayTime_PCF8574);		
	//SCLÐ´µÍµçÆ½
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	

	//SDA GPIOÅäÖÃ--ÍÆÍìÊä³ö--PP
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 		
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);				
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_Write1Byte_PCF8574(uint8_t data)
¹¦    ÄÜ£ºPCF8574µÄI2CÐ´Ò»¸ö×Ö½Ú³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£º
×¢    Òâ£º·¢ËÍ£ºSCL£ºµÍ--[¶¯×÷]-¸ß-µÍ
***********************************************************************/
void I2C_Write1Byte_PCF8574(uint8_t data)
{
	uint8_t i;	
	//SCLÐ´µÍµçÆ½
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
	for(i=0x80;i>0;i>>=1)  //i=10000000>>01000000>>0010000.....>>00000001
	{
		//Èç¹ûµÚi¸öÊý¾ÝÎª1
		if(i&data)
		{
			//SDAÐ´¸ßµçÆ½
			SDA_PCF8574(1);
		}
		else
		{
			//·ñÔòSDAÐ´µÍµçÆ½
			SDA_PCF8574(0);	
		}
		delay(DelayTime_PCF8574);	
		//SCLÐ´¸ßµçÆ½
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		//SCLÐ´µÍµçÆ½
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
	}
	//SDAÐ´¸ßµçÆ½
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);
	
	//PCF8574µÄI2C·¢ËÍÓ¦´ðÐÅÏ¢
 	I2C_Ack_PCF8574();
}

/***********************************************************************
º¯ÊýÃû³Æ£ºuint8_t I2C_Rec1Byte_PCF8574(void)
¹¦    ÄÜ£ºPCF8574µÄI2C½ÓÊÕÒ»¸ö×Ö½Ú³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-21
±à Ð´ ÈË£º
×¢    Òâ£º
***********************************************************************/
uint8_t I2C_Rec1Byte_PCF8574(void)
{
	uint8_t i,data;
	
	//SDA GPIOÅäÖÃ--¸¡¿ÕÊäÈë--FLOATING
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE);	 	//SDA_PCF8574 configure 					 	
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);	
	
	data=0x00;	
	for(i=0;i<8;i++)
	{
		//×óÒÆÒ»Î»
		data=data<<1;
		//SCLÐ´µÍµçÆ½
		SCL_PCF8574(0);	  
		delay(DelayTime_PCF8574);		
		//SCLÐ´¸ßµçÆ½
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		//Èç¹ûSDA¶ÁÈëµçÆ½Îª¸ß
		if(SDA_Read_PCF8574)
		{
			//¸ÃÎ»ÖÃ1
			data=data | 0x01;
		}		
	}
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	
	
	//SDA GPIOÅäÖÃ--ÍÆÍìÊä³ö--PP
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 		
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);		

	//PCF8574µÄI2C½ÓÊÕÓ¦´ðÐÅÏ¢
	I2C_NOAck_PCF8574();
	return data;
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
¹¦    ÄÜ£ºPCF8574µÄI2CÐ´Êý¾Ý³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-20
±à Ð´ ÈË£ºFangYikaii
×¢    Òâ£º1.I2C¿ªÊ¼
          2.Ð´ÒªÐ´Êý¾ÝµÄµØÖ·
          3.Ð´ÒªÐ´Êý¾ÝµÄÊý¾Ý
					4.I2CÍ£Ö¹
***********************************************************************/
void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
{
	//PCF8574µÄI2C¿ªÊ¼ÐÅºÅ
	I2C_Start_PCF8574();
	//PCF8574µÄI2CÐ´Ò»¸ö×Ö½Ú:µØÖ·
	I2C_Write1Byte_PCF8574(addr);
	//PCF8574µÄI2CÐ´Ò»¸ö×Ö½Ú:Êý¾Ý
	I2C_Write1Byte_PCF8574(data);	
	//PCF8574µÄI2CÍ£Ö¹ÐÅºÅ
	I2C_Stop_PCF8574();	
}

/***********************************************************************
º¯ÊýÃû³Æ£ºuint8_t I2C_RecData_PCF8574(uint8_t addr)
¹¦    ÄÜ£ºPCF8574µÄI2C¶ÁÊý¾Ý³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-20
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£º1.I2C¿ªÊ¼
          2.Ð´Òª¶ÁÊý¾ÝµÄµØÖ·
          3.½ÓÊÕÊý¾Ý+¸³Öµ
					4.I2CÍ£Ö¹
***********************************************************************/
uint8_t I2C_RecData_PCF8574(uint8_t addr)
{
	uint8_t temp;
	//PCF8574µÄI2C¿ªÊ¼ÐÅºÅ
	I2C_Start_PCF8574();
	//PCF8574µÄI2CÐ´Ò»¸ö×Ö½Ú:µØÖ·
	I2C_Write1Byte_PCF8574(addr);
	//PCF8574µÄI2C½ÓÊÕÒ»¸ö×Ö½Ú³ÌÐò
	temp=I2C_Rec1Byte_PCF8574();
	//PCF8574µÄI2CÍ£Ö¹ÐÅºÅ
	I2C_Stop_PCF8574();		
	return temp;
}

/***********************************************************************
º¯ÊýÃû³Æ£ºvoid KeyScan_PCF8574(uint8_t addr)
¹¦    ÄÜ£ºPCF8574µÄ¼üÅÌÉ¨Ãè³ÌÐò
ÊäÈë²ÎÊý£º
Êä³ö²ÎÊý£º
±àÐ´Ê±¼ä£º2017-01-20
±à Ð´ ÈË£ºFangYIkaii
×¢    Òâ£º
					<----PCF8574 ¼üÅÌ¼üÖµ ÅäÖÃ  ---->
					S1  -- 0xFE --  S9     11111110
					S2  -- 0xFD --  S10    11111101
					S3  -- 0xFB --  S11	   11111011
					S4  -- 0xF7 --  S12    11110111

					S5  -- 0xEF --  S13    11101111
					S6  -- 0xDF --  S14    11011111
					S7  -- 0xBF --  S15	   10111111
					S8  -- 0x7F --  S16    01111111
***********************************************************************/
void KeyScan_PCF8574(void)
{
		uint8_t kvalue=0;
	  //¶ÁÖµ
		kvalue=I2C_RecData_PCF8574(0x73);	
		if(kvalue==0xFE)
		{
			KeyValue_PCF8574=1;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}		
		else if(kvalue==0xFD)
		{
			KeyValue_PCF8574=2;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}		
		else if(kvalue==0xFB)
		{
			KeyValue_PCF8574=3;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}			
		else if(kvalue==0xF7)
		{
			KeyValue_PCF8574=4;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}
		else if(kvalue==0xEF)
		{
			KeyValue_PCF8574=5;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}		
		else if(kvalue==0xDF)
		{
			KeyValue_PCF8574=6;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}		
		else if(kvalue==0xBF)
		{
			KeyValue_PCF8574=7;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}			
		else if(kvalue==0x7F)
		{
			KeyValue_PCF8574=8;						//PCF8574¼üÅÌÉ¨ÃèÖµ
		}	
		else
		{
			PCF8574.Bits.KeyScan_Flag=FALSE;	//¼üÅÌ°´¼üÒÑ¸´Î»
			KeyValue_PCF8574=0;								//PCF8574¼üÅÌÉ¨ÃèÖµ
		}
}
