一、总体流程
1.	系统初始化
（1）	初始化系统时钟
（2）	初始化IO（电磁阀M1/M2/M3/M4;BR/RF/AS/XX）
（3）	初始化MAX7219
（4）	初始化TIM5定时器
（5）	初始化脉冲捕捉（转速）
（6）	初始化CAN
2.	基本配置
（1）	IO配置：挡位时间扫描模式（10ms）
ControllerInfo.ScanTimeMode=GearScanT10ms;		//挡位时间扫描模式置初始值	
（2）	LED指示灯显示：电源指示灯亮
LedValue=(LedValue & 0xFE) | 0x01;						//LED指示灯值
I2C_WriteData_PCF8574(0x70, LedValue);				//LED显示控制--电源指示灯亮	
（3）	显示配置：压力显示，转速显示
XG.ChannelNo[0]=1;							//压力显示值为1号通道	1--6
XG.ChannelNo[1]=1;						//转速显示值为2号通道	2--4
XG.Function=XG.Function%100+XG.ChannelNo[0]*1000+XG.ChannelNo[1]*100;			//通道显示
（4）	转速比配置：1-4挡，驱动桥速比
（5）	换挡模式配置：手动
ShiftModeStatus=HandShiftMode;					//默认：手动换挡模式
3.	循环
（1）	IO扫描(扫描间隔：flagctrl.Bits.ScanTimeFlag)
1.	IO扫描标志位-false
2.	换挡模式：手动-手动挡位扫描和处理
自动-自动挡位扫描和处理
（2）	显示更新扫描（扫描间隔：flagTIM5.Bits.Tisec_Flag—1s）
1.	显示更新扫描标志位-false
2.	装载机系统运行显示
（3）	脉冲捕获扫描（扫描间隔：flagTIM5.Bits.Freq_1sec_Flag—1s）
1.	脉冲捕获扫描标志位-false
2.	通道1、2、3脉冲捕获，保存，清零
3.	涡轮转速、输出轴转速计算
4.	转速显示
（4）	键盘捕获（无扫描间隔）
如果键值已经读取：PCF8574.Bits.KeyScan_Flag==FALSE:
如果为【自动/手动】切换按键:KeyValue_PCF8574==7
1.	键值读取标志位复位：PCF8574.Bits.KeyScan_Flag==TRUE;
2.	如果当前模式--手动挡：ShiftMdeStatus==AutoShiftMode
           Led指示灯值= (LedValue & 0x7F) | 0x80;
自动挡：ShiftModeStatus==HandShiftMode
 	   Led指示灯值=LedValue & 0x7F;		
3.	LED显示控制
二、IO数字开关量采集
1、读取信号
（1）DIIN1:从换挡选择器读取24V信号M1（1挡）—PE8
（2）DIIN2:从换挡选择器读取24V信号M2（2挡）—PE7
（3）DIIN3:从换挡选择器读取24V信号M3（3挡）—PE10
（4）DIIN4:从换挡选择器读取24V信号M4（4挡）—PE9
（5）DIIN5:从换挡选择器读取24V信号AS（空挡）—PE12
（6）DIIN6:从换挡选择器读取24V信号RF（倒挡）—PE11
（7）DIIN7:从制动器读取24V信号BR（制动）—PE14
（8）DIIN8：预留—PE13
2、输出信号
（1）DOOUT1：控制电磁阀M1（1挡）—PA0
（2）DOOUT2：控制电磁阀M2（2挡）—PA1
（3）DOOUT3：控制电磁阀M3（3挡）—PA2
（4）DOOUT4：控制电磁阀M4（4挡）—PA3
（5）DOOUT5：模拟制动信号BR（制动）--PA6
（6）DOOUT1：控制电磁阀BF（倒挡）—PA7
（6）DOOUT1：控制电磁阀AS（空挡）—PB0
（6）DOOUT1：预留—PB1
3、读取变量
struct ControllerData				
{
	uint8_t ScanTimeMode;	//档位扫描时间模式				
	uint8_t NowGear;			//当前档位信息	-- 读取获得
	uint8_t NowBrake;			//当前刹车状态	-- 读取获得
	uint8_t GoalGear;			//目标挡位			-- 控制值
	uint8_t ProcessGear;		//过程挡位      -- 控制值
	uint8_t SuggestGear;		//推荐挡位      -- 预测值
	uint8_t	gearinfo;			//档位					-- 处理获得
	uint8_t brakeinfo;			//制动信号			-- 处理获得
	uint8_t gearCGcount;		//档位变化次数计数
	uint8_t brakeCGcount;		//档位变化次数计数	
};
4、标志位
union FlagController  
{ 
      uint8_t flagBits; 
      struct       
      { 
            uint8_t 		gearchange		:	1;
			uint8_t 		brakechange		:	1;				
			uint8_t 		KDchange    	:	1;
			uint8_t 		StartUpFromNx	:	1;
			uint8_t 		ScanTimeFlag	:	1;				
			uint8_t 		KDScan_Flag  	:	1;
			uint8_t 		Bit6		:	1;				
			uint8_t 		Bit7		:	1;					
      } Bits; 
};
5、IO初始化：void IOControl_Init(void)；
（1）GPIO初始化
l	输入:M1/M2/M3/M4/AS/RF/BR/XX：
端口翻转速度：GPIO_Speed_50MHz; 
浮空输入：GPIO_Mode_IN_FLOATING
l	输出:M1/M2/M3/M4/BR/AS/RF/XX：
端口翻转速度：GPIO_Speed_50MHz; 
普通推挽输出：GPIO_Mode_Out_PP
（2）GPIO置0
6、手动挡位扫描和处理
void GearScanVSDeal_HandShiftMode(void)
{		
	ReadGearLine_Status();					//档位信息读取
	GearToSolenoidValve(ControllerInfo.NowGear,ControllerInfo.NowBrake);  //发送（当前挡位信息和当前刹车状态）到电磁阀和制动信号
}
7、自动挡位扫描和处理
void GearScanVSDeal_AutoShiftMode(void)
{		
	ReadGearLine_Status();					//档位信息读取
	if(ControllerInfo.NowBrake==0)	        //是否刹车 == 否
	{
		if(ControllerInfo.NowGear/0x10==0)		//前进档
		{	
			
		}
		else if(ControllerInfo.NowGear/0x10==1)			//后退档
		{
			
		}			
	}
	else //是否刹车 == 是
	{
		
	}	
	GearToSolenoidValve(ControllerInfo.NowGear,ControllerInfo.NowBrake);
	
}
8、挡位信息读取
各档位对应采集值列表
		前进档 V     空档N 			 倒退档R        KD
P   0x14				 0x34				 0x24		 0xXX & 0x01=0x00
1   0x10				 0x30				 0x20		 0xXX & 0x01=0x00	
2   0x12				 0x32				 0x22		 0xXX & 0x01=0x00
3   0x16				 0x36				 0x26 	     0xXX & 0x01=0x00
4   0x1E				 0x3E				 0x2E 		  0xXX & 0x01=0x00
三、MAX7219配置(SPI)
1、端口
l	管脚1—DIN—PE1
串行数据输入端口，时钟上升沿时数据被载入内部16位寄存器
l	管脚12—LOAD/CS—PE0
LOAD:载入数据
CS:载入数据
l	管脚13—CLK—PE2
时钟序列输入端
l	管脚14-17,20-23：7段和小数点驱动
#define 			REG_NO_OP 				    0x00    //定义空操作寄存器 
#define 			DIG_1 						0x01    //定义数码管1寄存器   
#define 			DIG_2 						0x02    //定义数码管2寄存器         
#define 			DIG_3 						0x03    //定义数码管3寄存器        
#define 			DIG_4 						0x04    //定义数码管4寄存器        
#define 			DIG_5 						0x05    //定义数码管5寄存器      
#define 			DIG_6 						0x06    //定义数码管6寄存器       
#define 			DIG_7 						0x07    //定义数码管7寄存器        
#define 			DIG_8 						0x08  //定义数码管8寄存器         #define 			REG_DECODE 				0x09   	//译码控制寄存器   
#define 			REG_INTENSITY 		        0x0a   	//亮度控制寄存器 
#define 			REG_SCAN_LIMIT 		        0x0b   	//扫描界限寄存器 
#define 			REG_SHUTDOWN 			0x0c   	//关断模式寄存器
#define 			REG_DISPLAY_TEST 	        0x0f  	//测试控制寄存器
#define 			INTENSITY_MIN 		        0x00    //定义最低显示亮度      
#define 			INTENSITY_MAX 		        0x0f    //定义最高显示亮度
2、MAX7219初始化
（1）  GPIO初始化
l	端口：CLK,LOAD/CS,DIN
端口翻转速度：GPIO_Speed_50MHz; 
普通推挽输出：GPIO_Mode_Out_PP
（2）CLK、DIN的GPIO置0；LOAD/CS的GPIO置1
（3）1/2/3片MAX7219初始化
l	工作模式
Write1stChip_MAX7219(REG_DISPLAY_TEST,0x00);      //选择工作模式（0xX0）			0x01 --test模式
l	初始亮度
Write1stChip_MAX7219(REG_INTENSITY,0x02);        	//设置初始亮度
l	全显示
Write1stChip_MAX7219(REG_SCAN_LIMIT,0x07);        //设置为全显示
l	解码模式
Write1stChip_MAX7219(REG_DECODE,0x00);       			//选用B解码模式--0xFF,非解码模式--0x00
l	开启正常工作模式
Write2ndChip_MAX7219(REG_SHUTDOWN,0x01);         	//开启正常工作模式（0xX1）	0x00 --关闭显示	
（5）	清空1/2/3片MAX721
3、装载机运行显示
void LoaderSystemDisplay(void)
{
		DisplayDigitalTube3_4Num_MAX7219 (XG.PressureShow/10,1,3);						//动臂大腔压力__第1、2号4位数码管显示程序，显示低4位
		DisplayDigitalTube3_4Num_MAX7219 (XG.Function,2,0);								//通道信息显示：按键值__第1、2号4位数码管显示程序，显示低4位		
		DisplayDigitalTube1_4Num_MAX7219 (XG.PumpSpeed,1,0);							//泵轮转速__第1、2号4位数码管显示程序，显示低4位
		DisplayDigitalTube1_4Num_MAX7219 (XG.SpeedShow,2,0);								//涡轮转速__第1、2号4位数码管显示程序，显示低4位
		GearDisplay(ControllerInfo.SuggestGear,2);		
		GearDisplay(ControllerInfo.ProcessGear,1);			//当前挡位信息显示
		if(XG.Throttle>=100)
		{
			DisplayDigitalTube2_2Num_MAX7219 (99,3,0);									//油门开度__第1、2、3、4号2位数码管显示程序，显示低4位
		}
		else
		{
			DisplayDigitalTube2_2Num_MAX7219 (XG.Throttle,3,0);							//油门开度__第1、2、3、4号2位数码管显示程序，显示低4位			
		}
		//错误信息显示__第1、2、3、4号2位数码管显示程序，显示低4位
		ErrorDisplay(ErrorMessage);
}
（1）	动臂大腔压力显示（第三组数码管，第一片，4位）
（2）	通道信息显示（第三组数码管，第2片，4位）
（3）	泵轮转速显示（第一组数码管，第一片，4位）---CAN读取
（4）	涡轮转速显示（第一组数码管，第2片，4位）---脉冲计算
（5）	油门开度显示
（6）	错误信息显示
四、PCF8574配置（I2C）
用于：写LED显示（PCF8574写数据到0x70地址）；读键盘扫描（PCF8574从0x73）读数据
1、端口
SCL—PE5
SDA—PE3
【PCF8574的地址】
地址(0x70) -- LED显示控制地址
地址(0x73) -- 按键读取控制地址
地址(0x7A) -- 电磁阀控制芯片输出信号读取地址(未用)
2、PCF8574初始化
（1）GPIO初始化
l	端口：SCL，SDA
端口翻转速度：GPIO_Speed_50MHz; 
普通推挽输出：GPIO_Mode_Out_PP
（2）SCL、SDA的GPIO置0
（3）初始化LED显示控制：往PCF8574的地址0x70写数据0x00
I2C_WriteData_PCF8574(0x70, 0x00);
{
	//PCF8574的I2C开始信号
I2C_Start_PCF8574();
// PCF8574的I2C写一个字节程序：地址0x70
I2C_Write1Byte_PCF8574(addr);
// PCF8574的I2C写一个字节程序：地址0x00
I2C_Write1Byte_PCF8574(data);	
// PCF8574的I2C停止信号
I2C_Stop_PCF8574();	
}
3、PCF8574写数据
void I2C_WriteData_PCF8574(uint8_t addr, uint8_t data)
{
	I2C_Start_PCF8574();
	I2C_Write1Byte_PCF8574(addr);
	I2C_Write1Byte_PCF8574(data);	
	I2C_Stop_PCF8574();	
}
//功    能：PCF8574的I2C开始信号
void I2C_Start_PCF8574(void)
{

	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);	
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	SDA_PCF8574(0);	
	delay(DelayTime_PCF8574);	
}
//功    能：PCF8574的I2C停止信号
void I2C_Stop_PCF8574(void)
{
	SDA_PCF8574(0);
	delay(DelayTime_PCF8574);	
	SCL_PCF8574(1);
	delay(DelayTime_PCF8574);
	SDA_PCF8574(1);	
	delay(DelayTime_PCF8574);	
}
//功    能：PCF8574的I2C写一个字节程序
void I2C_Write1Byte_PCF8574(uint8_t data)
{
	uint8_t i;	
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);		
	for(i=0x80;i>0;i>>=1)
	{
		if(i&data)
		{
			SDA_PCF8574(1);
		}
		else
		{
			SDA_PCF8574(0);	
		}
		delay(DelayTime_PCF8574);	
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
	}
	SDA_PCF8574(1);
	delay(DelayTime_PCF8574);
	
 	I2C_Ack_PCF8574();
}
4、PCF8574读数据
uint8_t I2C_RecData_PCF8574(uint8_t addr)
{
	uint8_t temp;
	I2C_Start_PCF8574();
	I2C_Write1Byte_PCF8574(addr);
	temp=I2C_Rec1Byte_PCF8574();
	I2C_Stop_PCF8574();		
	return temp;
}
//接收一个字节的数据
uint8_t I2C_Rec1Byte_PCF8574(void)
{
	uint8_t i,data;
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
		data=data<<1;
		SCL_PCF8574(0);	
		delay(DelayTime_PCF8574);		
		SCL_PCF8574(1);
		delay(DelayTime_PCF8574);
		if(SDA_Read_PCF8574)
		{
			data=data | 0x01;
		}
		
	}
	SCL_PCF8574(0);	
	delay(DelayTime_PCF8574);	
	
	RCC_APB2PeriphClockCmd(SDA_PCF8574_RCC_APB2Periph_GPIOX, ENABLE); 	//SDA_PCF8574 configure 					 
			
	GPIO_InitStructure.GPIO_Pin = SDA_PCF8574_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SDA_PCF8574_GPIOX, &GPIO_InitStructure);	
	delay(DelayTime_PCF8574);		

	I2C_NOAck_PCF8574();
	return data;
}
5、键盘扫描程序：通过写数据到0x73，读返回值
void KeyScan_PCF8574(void)
{
		uint8_t kvalue=0;
		kvalue=I2C_RecData_PCF8574(0x73);	
		if(kvalue==0xFE)
		{
			KeyValue_PCF8574=1;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xFD)
		{
			KeyValue_PCF8574=2;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xFB)
		{
			KeyValue_PCF8574=3;						//PCF8574键盘扫描值
		}			
		else if(kvalue==0xF7)
		{
			KeyValue_PCF8574=4;						//PCF8574键盘扫描值
		}
		else if(kvalue==0xEF)
		{
			KeyValue_PCF8574=5;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xDF)
		{
			KeyValue_PCF8574=6;						//PCF8574键盘扫描值
		}		
		else if(kvalue==0xBF)
		{
			KeyValue_PCF8574=7;						//PCF8574键盘扫描值
		}			
		else if(kvalue==0x7F)
		{
			KeyValue_PCF8574=8;						//PCF8574键盘扫描值
		}	
		else
		{
			PCF8574.Bits.KeyScan_Flag=FALSE;	//键盘按键已复位
			KeyValue_PCF8574=0;								//PCF8574键盘扫描值
		}
}
五、定时器
定时器：7个（除了2watchdog timers：独立看门狗、窗口看门狗）
基本定时器：TIM6，TIM7[最基本功能：累加时钟脉冲数超过预定值，触发中断或触发DMA][向上计数】-- Timer5 用于一般的定时，同时作为USARTx的通信超时溢出寄存器
通用定时器：TIM2，TIM3，TIM4，TIM5[测量输入脉冲频率、脉冲宽和输入PWM脉冲等场合][向上，向下计数】--- Timer2 用于USB通信的精确定时
高级定时器：TIM1[还有三相六部电机接口、刹车功能][向上向下计数]

时钟配置Systick=72MHz，APB1预分频器的分频系数被配为2，则PCK1刚好最大值36MHz
预分频:TIMx_PSC=0不分频  TIMx_PSC=1 2分频
1、标志位
union FlagTimer  
{ 
      uint8_t flagBits; 
      struct       
      { 
			uint8_t		T100msec_Flag 		:	1;				
			uint8_t		T1sec_Flag  			:	1;
			uint8_t		T1min_Flag			:	1;
			uint8_t 		T10msec_Flag			:	1;
			uint8_t 		T1_5sec_Flag			:	1;				
			uint8_t 		Freq_1sec_Flag		:	1;
			uint8_t 		Bit6					:	1;				
			uint8_t 		Bit7					:	1;						
      } Bits; 
};
struct TIM5Result 
{
	uint32_t ms_Record; 
    uint16_t msecond;
	uint16_t T1_5secmsCount;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
};
2、TIME5初始化
（1）定义定时器结构体
//这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
TIM_TimeBaseStructure.TIM_Period = (100 - 1);
// 这个就是预分频系数，当由于为0时表示不分频所以要减1
TIM_TimeBaseStructure.TIM_Prescaler = (7200-6480- 1);	
（2）定义中断
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//抢断优先级 0-15   响应优先级 0
NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;		//设定通道
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	//抢断优先级设置  0或1
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//响应优先级设置
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); 	
3、TIM5时间处理程序
Timer5.msecond++;
if(Timer5.msecond%10==0)									//10ms中断一次
{
		flagUDisk.Bits.SaveTime_10ms=TRUE;	
		if(ControllerInfo.ScanTimeMode%2==1)		//GearScanT10ms = 1
		{
			flagctrl.Bits.ScanTimeFlag=TRUE;			
		}
		flagTIM5.Bits.T10msec_Flag=TRUE;
}
if(Timer5.msecond%100==0)									//100ms中断一次
{
	if(ControllerInfo.ScanTimeMode%2==0)		//GearScanT100ms = 2
	{
		flagctrl.Bits.ScanTimeFlag=TRUE;			
	}		
		flagTIM5.Bits.T100msec_Flag=TRUE;
}
六、脉冲捕捉
1、端口
脉冲产生：PE2
只用了：TIM4.channel2  TIM4.channel3   TIM4.channel4
20路脉冲：
TIM1.channel1 -- PA8         TIM2.channel1 -- PA0
TIM1.channel2 -- PA9        	TIM2.channel2 -- PA1
TIM1.channel3 -- PA10        TIM2.channel3 -- PA2
TIM1.channel4 -- PA11        TIM2.channel4 -- PA3

TIM3.channel1 -- PA6         TIM4.channel1 -- PB6
TIM3.channel2 -- PA7        	TIM4.channel2 -- PB7
TIM3.channel3 -- PB0        	TIM4.channel3 -- PB8
TIM3.channel4 -- PB1         	TIM4.channel4 -- PB9

TIM5.channel1 -- PA0         	
TIM5.channel2 -- PA1        	
TIM5.channel3 -- PA2        	
TIM5.channel4 -- PA3    
2、变量声明
struct FreqCapResult 
{
	uint8_t IsUsedFlag;				//定时器是否启用标志
	uint8_t NOINT_s;				//没检测到脉冲等待时间默认3S -- Fre_NOINTMAXTIME 设置
	uint8_t count;
	uint16_t pulse1;				//第一次脉冲值
	uint16_t pulse2;				//第二次脉冲值
	uint32_t pulse;				//脉冲差值
	float Freq;					//频率值
};
3、FreqCap初始化
（1）GPIO初始化
l	脉冲产生：PE2
端口翻转速度：GPIO_Speed_50MHz; 
普通推挽输出：GPIO_Mode_Out_PP
l	TIM4.channel2  TIM4.channel3   TIM4.channel4
端口翻转速度：GPIO_Speed_50MHz; 
浮空输入：GPIO_Mode_IN_FLOATING
（2）TIM定时器：TIM1，TIM2，TIM3，TIM4
TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
TIM_ICInitStructure.TIM_ICFilter = 0x00;
TIM_ICInit(TIM4, &TIM_ICInitStructure);
TIM_ITConfig(TIM4, TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update, ENABLE);  		//打开定时器中断
TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); 		
//清除中断标志位
TIM_Cmd(TIM4, ENABLE);							//启动定时器
设置定时器是否启用
（3）NVIC中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
4、中断void TIM4_IRQHandler(void)
通道TIM4.channel2  TIM4.channel3   TIM4.channel4脉冲上升沿捕获程序
if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); //清标志位
		TIM4FreqCap[0].NOINT_s=0;	        
		TIM4FreqCap[0].pulse++;	              //脉冲数
}
七、CAN
1.端口
default：CAN1TX—PA12,CAN1RX—RA11
remap：CAN1TX—PD1,CAN1RX—PD0
default：CAN2TX—PB13,CAN2RX—RB12
remap：CAN2TX—PB6,CAN2RX—PB5
2.CAN初始化
（1）GPIO初始化
l	CAN1TX，CAN2TX
外部时钟：RCC_APB2PeriphClockCmd(CAN12TX_RCC_APB2Periph_GPIOX, ENABLE);
复用推挽输出：GPIO_Mode_AF_PP
时钟频率：GPIO_Speed_50MHz;
l	CAN2RX, ,CAN2RX
外部时钟：RCC_APB2PeriphClockCmd(CAN12RX_RCC_APB2Periph_GPIOX, ENABLE);
上拉输入：GPIO_Mode_IPU;
时钟频率：GPIO_Speed_50MHz;
（2）CAN复用
 /* Remap CAN1 and CAN2 GPIOs */
GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);				//注意IO端口选择，此处选择映射管脚
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE); 		//注意IO端口选择，此处选择映射管脚
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
（3）CAN初始化
CAN cell init
CAN filter init
（4）中断优先级
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);			//抢断优先级 0-15   响应优先级 0
NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
NVIC_Init(&NVIC_InitStructure);	
3、CAN中断

