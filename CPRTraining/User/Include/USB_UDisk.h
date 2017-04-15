//========================================
//<--预定义处理，防止重复定义-->
#ifndef _USB_UDisk_H
#define _USB_UDisk_H

//========================================
//头文件

#include "usbh_usr.h"
#include "usb_bsp.h" 



//========================================
//宏定义
#define 			ERROR						0									//错误返回信息值
#define				NORMAL					1									//正常返回信息

#define				CONTENTSIZE			64								//TXT文档说明内容长度
#define				RecordTMSIZE		21								//创建TXT文档时间长度
#define				DATAINFOSIZE		15								//TXT数据信息长度
#define				DATASTARADDR		100								//数据记录的起始地址
#define				ROWSIZE					128								//数据每行长度           1024/N

#define				MS_Eable				1
#define				MS_UnEable			0
//========================================
//端口定义




//========================================
//外调变量声明
// struct DATE
// {
// 	uint8_t hour;
// 	uint8_t min;
// 	uint8_t sec;
// 	uint16_t 	year;
// 	uint8_t month;
// 	uint8_t day;
// 	uint8_t week;
// 	uint16_t  msec;
// 	
// 	char cord[5];
// 	char string[24];				//更改数组长度时，注意修改对应的清空长度	
// 	
// };
// extern struct DATE NowTime;

struct TXT
{
	char Name[50];					//TXT文档名称
	char Content[64];				//TXT文档记录内容说明
	uint16_t RowLength;					//每行长度
	uint32_t RecordCount;			//记录数据个数
	uint32_t RecordAddr;				//记录数据地址
	
};
extern struct TXT TXTInfo[];

struct RecordDATA				
{
	char 	TimeString[23];									//记录时间信息-- 14-05-09 17:15:00 1000(年-月-日 时:分:秒 毫秒)	
	uint8_t	GearInfo;											//档位信息--2个字符
	float AnalogChannelData[8];						//模拟通道采集值--电压值(油门开度、转向油缸压力、抬举压力等)
	uint16_t	DigitalChannelData[4];			//模拟通道采集值--脉冲值(泵轮转速、涡轮转速、车速等)
	float collecttime_s;									//采样时间--s.
	uint16_t recordcount;
	
};
extern struct RecordDATA RdData;
extern FIL fsrc_FIL[];       					  /* file objects */
extern uint8_t USBInit_STATUS;						//U盘初始化标志

union FlagUSB  
{ 
      uint8_t flagBits; 
      struct       
      { 
						uint8_t			SaveTime_1s				:	1;					
						uint8_t			SaveTime_100ms		:	1;	
						uint8_t			SaveTime_10ms			:	1;			
						uint8_t 		Bit3							:	1;
						uint8_t 		Bit4							:	1;				
						uint8_t 		Bit5							:	1;
						uint8_t 		Bit6							:	1;				
						uint8_t 		Bit7							:	1;		
 						
      } Bits; 
}; 
extern volatile union FlagUSB flagUDisk;




//========================================
//外调函数声明
extern void USB_UDisk_Init(void);																//U盘初始化程序
extern void UserProcess_UDisk(void);														//U盘读写用户处理区
extern uint8_t OpenTXT_UDisk(char *str,FIL *fsrcN);								//U盘打开TXT文件程序
extern uint8_t CloseTXT_UDisk(FIL *fsrcN);												//U盘关闭TXT文件程序
extern uint8_t CreatTXT_UDisk(char *str,FIL *fsrcN);							//U盘创建TXT文件程序
extern uint8_t DelTXT_UDisk(char *str,FIL *fsrcN);								//U盘删除文件程序
extern uint8_t WriteRdDataToTxt_UDisk(char *str, FIL *fsrcN);			//U盘写入采集数据到TXT文档内



//========================================
//<--预处理结束-->
#endif



