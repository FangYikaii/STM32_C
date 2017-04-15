/*
程序说明:  	
            U盘读写程序 --- STM32F107VCT 
						
注意事项：	1、用于U盘检测的TIM2的中断级别必须设置成最高，否则会出现拔插U盘无法显示				
						2、字符串读写的时候，结束标识符为'\o'/ NULL，故任何字符串写入和读取的时候遇到该字符
						就会自动结束，故读取字符串时注意长度。
						3、字符串中的strlen()计算的字符长度也是以'\o'结束，sizeof()只有在函数声明的局部
						范围内才能把整个数组的长度读出，否则嵌套调用遇到'\o'时也认为字符长度计算结束。
						4、注意偏移地址的位置，任何的读写最好先设置便宜地址。f_lseek(&fsrc, XX);
						5、注意文件的打开与关闭，特别是调用其他文件的读写时，不能嵌套在目前的读写状态下，否则导致文件没打开
						6、注意U盘读写时一定不要跨区读写，而是应该分区写，最长区间1024，否则读写错误。
						7、使用Printf功能，故在工程设置中需要勾选 USE MICROLIB
端口使用:		见Doc文件夹内说明
创建者:     FangYIkaii
创建时间:   2017-01-21 
修改时间:   2014-01-21
*/
//**********************************************************************************************************
/*
创建或删除文件类型说明：
FA_READ									指定读访问对象
FA_WRITE								指定写访问对象
FA_OPEN_EXISTING				打开文件，如果文件不存在，则打开失败
FA_OPEN_ALWAYS					如果文件存在，则打开，否则创建一个TXT文件
FA_CREATE_NEW						创建一个TXT文件，如果文件已存在，则创建失败
FA_CREATE_ALWAYS				创建一个TXT文件，如果文件已存在，则它将被截断并覆盖

文件读写返回值说明：
	FR_OK=0							 			(0) Succeeded 
	FR_DISK_ERR,			 				(1) A hard error occured in the low level disk I/O layer 
	FR_INT_ERR,				 				(2) Assertion failed 
	FR_NOT_READY,			 				(3) The physical drive cannot work 
	FR_NO_FILE,				 				(4) Could not find the file 
	FR_NO_PATH,				 				(5) Could not find the path 
	FR_INVALID_NAME,		 			(6) The path name format is invalid 
	FR_DENIED,				 				(7) Acces denied due to prohibited access or directory full 
	FR_EXIST,				 					(8) Acces denied due to prohibited access 
	FR_INVALID_OBJECT,		 		(9) The file/directory object is invalid 
	FR_WRITE_PROTECTED,		 		(10) The physical drive is write protected 
	FR_INVALID_DRIVE,		 			(11) The logical drive number is invalid 
	FR_NOT_ENABLED,			 			(12) The volume has no work area 
	FR_NO_FILESYSTEM,		 			(13) There is no valid FAT volume on the physical drive
	FR_MKFS_ABORTED,		 			(14) The f_mkfs() aborted due to any parameter error 
	FR_TIMEOUT,				 				(15) Could not get a grant to access the volume within defined period 
	FR_LOCKED,								(16) The operation is rejected according to the file shareing policy 
	FR_NOT_ENOUGH_CORE,		 		(17) LFN working buffer could not be allocated 
	FR_TOO_MANY_OPEN_FILES	 	(18) Number of open files > _FS_SHARE 


*/
//**********************************************************************************************************

//========================================
//头文件
#include "main.h"

//========================================
//变量定义
FIL fsrc_FIL[5];    //File Object Structure
struct TXT TXTInfo[5];  

// struct DATE NowTime;
struct RecordDATA RdData;
uint8_t USBInit_STATUS=0;						//U盘初始化标志
volatile union FlagUSB flagUDisk;

//========================================
//函数声明
void USB_UDisk_Init(void);																							//U盘初始化程序
void UserProcess_UDisk(void);																						//U盘读写用户处理区

uint8_t OpenTXT_UDisk(char *str,FIL *fsrcN);															//U盘打开TXT文件程序
uint8_t CloseTXT_UDisk(FIL *fsrcN);																				//U盘关闭TXT文件程序
uint8_t CreatTXT_UDisk(char *str,FIL *fsrcN);															//U盘创建TXT文件程序
uint8_t DelTXT_UDisk(char *str,FIL *fsrcN);																//U盘删除文件程序

uint8_t WriteRdDataToTxt_UDisk(char *str, FIL *fsrcN);										//U盘写入采集数据到TXT文档内

uint8_t ReadDataRecordNum_UDisk(char *str);																//U盘读取TXT文档内数据记录个数
uint8_t WriteDataRecordNum_UDisk(char *str);															//U盘写入TXT文档内数据记录个数															

void ClearStr(char *str,uint16_t len);							//字符串清空程序
void DateToStr(uint8_t ms_en);										//时间转换成字符串
void DateToFileName(void);											//时间转换成文件名
char *itoa(int num, char *str, int radix);			//整型转为字符串

void die(FRESULT res);													//文件操作执行结果分析

//========================================
//子模块函数



/***********************************************************************
函数名称：void USB_UDisk_Init(void)
功    能：U盘初始化程序
输入参数：无
输出参数：无
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
void USB_UDisk_Init(void)
{
		USBH_Init(&USB_OTG_FS_dev, &MSC_cb , &USR_Callbacks);
}

/***********************************************************************
函数名称：void UserProcess_UDisk(void)
功    能：U盘读写用户处理区
输入参数：无
输出参数：无
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
void UserProcess_UDisk(void)
{
	char i;
	char printstr[100];
	char nstr[20];
	USBH_Process();																		//USB_OTG内部实时检测程序
	if(USBH_USR_ApplicationState == USH_USR_FS_NULL)	//U盘正常初始化完毕，用户程序处理区
	{
		if(USBInit_STATUS==0)			//U盘初始化完成后仅执行一次操作
		{
			USBInit_STATUS=1;				//U盘初始化标志
			res = f_open(&fsrc_FIL[0], "2015.txt", FA_CREATE_ALWAYS | FA_WRITE);						//创建TXT文件	
			die(res);
			if ( res != FR_OK)
			{ 
				printf("\r\n文件打开或创建失败");
			}	
		}	
		else if(USBInit_STATUS==1)
		{
				if(flagUDisk.Bits.SaveTime_100ms)
				{
					flagUDisk.Bits.SaveTime_100ms=FALSE;						//注意清零	
					ClearStr(printstr,sizeof(printstr));	
					
					ClearStr(nstr,sizeof(nstr));
					sprintf(nstr,"%9.3f",RdData.collecttime_s);					
					strcat(printstr,nstr);
					strcat(printstr," ");	
					
					ClearStr(nstr,sizeof(nstr));
					sprintf(nstr,"%9.3f",RdData.collecttime_s);					
					strcat(printstr,nstr);
					strcat(printstr," ");	

					ClearStr(nstr,sizeof(nstr));
					sprintf(nstr,"%9.3f",RdData.collecttime_s);					
					strcat(printstr,nstr);
					strcat(printstr," ");						
					
					for(i=0;i<6;i++)
					{
						ClearStr(nstr,sizeof(nstr));
						sprintf(nstr,"%9.3f",ADCRes.ADCResValue[i]);					
						strcat(printstr,nstr);
						strcat(printstr," ");			
					}				
					strcat(printstr,"  \r\n");	
					i=strlen(printstr);
					res = f_write(&fsrc_FIL[0], printstr, strlen(printstr) , &br);  //写入数据信息
					die(res);
					if ( res != FR_OK)
					{
						printf("数据存储失败！\r\n");
					}
					else
					{
						printf("数据存储成功！\r\n");
					}
					RdData.recordcount++;
// 					f_sync(&fsrc_FIL[0]);
					printf("RdData.recordcount= %d \r\n",RdData.recordcount);
					if(RdData.recordcount>=1000)
					{
						RdData.recordcount=0;
						f_close(&fsrc_FIL[0]);
						printf("存储结束！");
						USBInit_STATUS=3;	
					}
						
				}	
			
		}
		

		
	}
	
	
	
	
	
	// 	if(USBH_USR_ApplicationState == USH_USR_FS_NULL)	//U盘正常初始化完毕，用户程序处理区
// 	{
// 		if(USBInit_STATUS==0)			//U盘初始化完成后仅执行一次操作
// 		{
// 			USBInit_STATUS=1;				//U盘初始化标志

// 			NowTime.year=2014;
// 			NowTime.month=15;
// 			NowTime.day=14;
// 			NowTime.hour=23;
// 			NowTime.min=40;
// 			NowTime.sec=15;
// 			NowTime.msec=1000;	
// 			RdData.AnalogChannelData[0]=12121.02;
// 			RdData.AnalogChannelData[1]=12122.02;		
// 			RdData.AnalogChannelData[2]=12123.02;
// 			RdData.AnalogChannelData[3]=12124.02;
// 			RdData.AnalogChannelData[4]=12125.02;
// 			RdData.AnalogChannelData[5]=12126.02;			
// 			RdData.AnalogChannelData[6]=12127.02;
// 			RdData.AnalogChannelData[7]=12128.02;
// 			RdData.DigitalChannelData[0]=100000;
// 			RdData.DigitalChannelData[1]=200000;			
// 			RdData.DigitalChannelData[2]=300000;
// 			RdData.DigitalChannelData[3]=142353;
// 			RdData.GearInfo=2;	
// 			DateToFileName();																					//时间转换为文件名
// 			ClearStr(TXTInfo[0].Name,sizeof(TXTInfo[0].Name));
// 			strcat(TXTInfo[0].Name,NowTime.string);										//文件名称：时分秒
// 			CreatTXT_UDisk(TXTInfo[0].Name,&fsrc_FIL[0]);		
// 			OpenTXT_UDisk(TXTInfo[0].Name,&fsrc_FIL[0]);	
// 		}	
// 		else
// 		{
// 			if(flagUDisk.Bits.SaveTime_100ms)
// 			{
// 				flagUDisk.Bits.SaveTime_100ms=FALSE;						//注意清零	
// 			
// 				printf("%d\r\n",Timer5.msecond);
// 				WriteRdDataToTxt_UDisk(TXTInfo[0].Name,&fsrc_FIL[0]);						
// 				printf("RecordAddr=%d\r\n",TXTInfo[0].RecordAddr);
// 				printf("RecordCount=%d\r\n",TXTInfo[0].RecordCount);			
// 				printf("%d\r\n",Timer5.msecond);
// 			}			
// 		}						
// 		

// 	}
// 	else		//U盘初始化失败，或已经断开连接
// 	{
// 		;
// 	}


}

/***********************************************************************
函数名称：uint8_t OpenTXT_UDisk(char *str,FIL *fsrcN)
功    能：U盘打开TXT文件程序
输入参数：文件名称
输出参数：1  成功    0  出错
编写时间：2014.05.15
编 写 人：masterjay
注    意：文件不要使用中文名字，否则无法创建成功。如果使用，名字务必短
***********************************************************************/
uint8_t OpenTXT_UDisk(char *str,FIL *fsrcN)
{
	uint8_t status=NORMAL;
	char printstr[100];
	char FileName[30];
	if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)						//如果u盘写保护
	{		
			ClearStr(printstr,sizeof(printstr));
			printf("该u盘已写保护！");
			printf("\r\n");
			strcat(printstr,"无法创建“");
			strcat(printstr,str);
			strcat(printstr,"”文件");
			strcat(printstr,"\r\n");		
			printf(printstr);		
			status=ERROR;																									//创建出错报警
	}	
	else
	{
		ClearStr(FileName,sizeof(FileName));
		strcat(FileName,"0:/");
		strcat(FileName,str);	
		strcat(FileName,".TXT");		
		res = f_open(fsrcN , FileName , FA_OPEN_EXISTING | FA_READ | FA_WRITE);
		die(res);
		if(res!=FR_OK)
		{
			status=ERROR;
		}
	}
	return status;
}

/***********************************************************************
函数名称：uint8_t CloseTXT_UDisk(FIL *fsrcN)
功    能：U盘关闭TXT文件程序
输入参数：文件名称
输出参数：1  成功    0  出错
编写时间：2014.05.15
编 写 人：masterjay
注    意：文件不要使用中文名字，否则无法创建成功。如果使用，名字务必短
***********************************************************************/
uint8_t CloseTXT_UDisk(FIL *fsrcN)
{
	uint8_t status=NORMAL;	
	res = f_close(fsrcN);	
	die(res);
	if(res!=FR_OK)
	{
		status=ERROR;
	}
	return status;
}

/***********************************************************************
函数名称：uint8_t CreatTXT_UDisk(char *str,FIL *fsrcN)
功    能：U盘创建TXT文件程序
输入参数：文件名称 文件内容
输出参数：1  成功    0  出错
编写时间：2014.05.15
编 写 人：masterjay
注    意：文件不要使用中文名字，否则无法创建成功。如果使用，名字务必短
***********************************************************************/
uint8_t CreatTXT_UDisk(char *str,FIL *fsrcN)
{
	uint8_t status=NORMAL;
	char printstr[100];	
	char FileName[30];
	if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)						//如果u盘写保护
	{		
			ClearStr(printstr,sizeof(printstr));
			printf("该u盘已写保护！");
			printf("\r\n");
			strcat(printstr,"无法创建“");
			strcat(printstr,str);
			strcat(printstr,"”文件");
			strcat(printstr,"\r\n");		
			printf(printstr);		
			status=ERROR;																									//创建出错报警
	}	
	else
	{
		ClearStr(FileName,sizeof(FileName));
		strcat(FileName,"0:/");
		strcat(FileName,str);	
		strcat(FileName,".TXT");			
		res = f_open(fsrcN , FileName , FA_CREATE_ALWAYS);						//创建TXT文件	
		die(res);
		if ( res != FR_OK)
		{ 
			status=ERROR;
		}	
		f_close(fsrcN);	
	}
	return status;
}

/***********************************************************************
函数名称：uint8_t DelTXT_UDisk(char *str,FIL *fsrcN)
功    能：U盘删除文件程序
输入参数：文件名称
输出参数：1  成功       0  出错
编写时间：2014.05.15
编 写 人：masterjay
注    意：文件不要使用中文名字，否则无法创建成功。如果使用，名字务必短
***********************************************************************/
uint8_t DelTXT_UDisk(char *str,FIL *fsrcN)
{
	uint8_t status=NORMAL;
	char printstr[100];
	char FileName[30];
	if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)//如果u盘写保护
	{		
			ClearStr(printstr,sizeof(printstr));
			printf("该u盘已写保护！");
			printf("\r\n");
			strcat(printstr,"无法删除“");
			strcat(printstr,str);
			strcat(printstr,"”文件");
			printf("\r\n");		
			status=ERROR;															//删除出错报警
	}	
	else
	{	
		ClearStr(FileName,sizeof(FileName));			
		strcat(FileName,"0:/");
		strcat(FileName,str);	
		strcat(FileName,".TXT");
		res = f_unlink(FileName);										//删除TXT文件	
		die(res);
		if ( res != FR_OK )
		{ 
			status=ERROR;															//删除出错报警
		}
		f_close(fsrcN); 		
	}
	return status;
}


/***********************************************************************
函数名称：uint8_t WriteRdDataToTxt_UDisk(char *str, FIL *fsrcN)
功    能：U盘写入采集数据到TXT文档内
输入参数：文件名,手柄
输出参数：1  成功    0  出错
编写时间：2014.05.15
编 写 人：masterjay
注    意：
***********************************************************************/
uint8_t WriteRdDataToTxt_UDisk(char *str, FIL *fsrcN)
{	
	uint8_t status=NORMAL;
	uint8_t i;	
	char printstr[200];
	char nstr[50];
//**********************************************************************
//格式化写入信息	-- 128个字符
	DateToStr(MS_Eable);											//将日期转换成字符串
	ClearStr(printstr,sizeof(printstr));		
	strcat(printstr,RdData.TimeString);				//记录时间信息-- 05-09 17:15:00:1000(年-月-日 时:分:秒:毫秒)	
	strcat(printstr," ");		
	
	sprintf(nstr,"%2d",RdData.GearInfo);			//档位信息--2个字符(档位值: -2 -1 1 2 3)
	strcat(printstr,nstr);
	strcat(printstr," ");		
	
	for(i=0;i<8;i++)
	{
		if(i<6)
		{		
			sprintf(nstr,"%8.2f",ADCRes.ADCResValue[i]);
		}
		else
		{
			sprintf(nstr,"%8.2f",RdData.AnalogChannelData[i]);		//模拟通道采集值--电压值(油门开度、转向油缸压力、抬举压力等)			
		}
		
	
		strcat(printstr,nstr);
		strcat(printstr," ");			
	}
	
	for(i=0;i<4;i++)
	{
		sprintf(nstr,"%6d",RdData.DigitalChannelData[i]);			//模拟通道采集值--脉冲值(泵轮转速、涡轮转速、车速等)	
		strcat(printstr,nstr);
		strcat(printstr," ");						
	}	
	strcat(printstr,"\r\n");

// 	sprintf(nstr,"%10d",Timer5.ms_Record);
// 	strcat(printstr,nstr);
// 	strcat(printstr," ");		
// 	for(i=0;i<6;i++)
// 	{
// 		sprintf(nstr,"%8.3f",ADCRes.ADCResValue[i]);		//模拟通道采集值--电压值(油门开度、转向油缸压力、抬举压力等)	
// 		strcat(printstr,nstr);
// 		strcat(printstr," ");			
// 	}	
// 	strcat(printstr,"\r\n");
	
//**********************************************************************
//开始写入数据
//	OpenTXT_UDisk(str,fsrcN);	

// 	TXTInfo[0].RecordAddr=(TXTInfo[0].RecordCount*ROWSIZE);  //写数据之时，注意以1024作为跨区段，不能跨区段写数据
// 	res = f_lseek(fsrcN,TXTInfo[0].RecordAddr);
// 	die(res);	
	
// 	res = f_lseek(fsrcN,f_size(fsrcN));
// 	die(res);	
	res = f_write(fsrcN, printstr, strlen(printstr) , &br);  //写入数据信息
	die(res);
	if ( res == FR_OK)
	{
		TXTInfo[0].RecordCount++;			
	}
	else
	{
		status=ERROR;
	}
	f_sync(fsrcN);																					//关闭读写，但不关闭文件，拔出再插入后不能继续读写
//	f_close(fsrcN);																						//关闭读写且关闭文件，拔出后再插入可继续读写
	return status;
}

/***********************************************************************
函数名称：uint8_t ReadDataRecordNum_UDisk(char *str)
功    能：U盘读取TXT文档内数据记录个数
输入参数：文件名称
输出参数：1   读取成功  0   读取失败
编写时间：2014.05.08
编 写 人：masterjay
注    意：
***********************************************************************/
uint8_t ReadDataRecordNum_UDisk(char *str)
{	
	uint32_t recordnum;
	uint8_t status=NORMAL;
	uint8_t i;	
	char FileName[20];
	char printstr[100];	

	ClearStr(FileName,sizeof(FileName));
	strcat(FileName,"0:/");
	strcat(FileName,str);	
	strcat(FileName,".TXT");		
	res = f_open( &fsrc , FileName , FA_OPEN_EXISTING | FA_READ);
	die(res);
	if ( res == FR_OK)
	{ 
		ClearStr(printstr,sizeof(printstr));
		f_lseek(&fsrc, (CONTENTSIZE+RecordTMSIZE));
		res = f_read( &fsrc, printstr, DATAINFOSIZE, &br );	
		die(res);
		recordnum=0x00;
		for(i=0;i<4;i++)				//计算数据行的长度
		{

			if(printstr[i]>=0x30)
			{
				recordnum*=16;				
				if(printstr[i]<=0x39)
				{
					recordnum+=(printstr[i]%16);
				}
				else if(printstr[i]>=0x41)
				{
					recordnum+=(printstr[i]%16+9);				
				}				
			}			
		}
		TXTInfo[0].RowLength=recordnum;			//每行总长度
		
		recordnum=0;		
		for(i=0;i<8;i++)			//计算当前存储的个数
		{

			if(printstr[i+5]>=0x30)
			{
				recordnum*=16;				
				if(printstr[i+5]<=0x39)
				{
					recordnum+=(printstr[i+5]%16);
				}
				else if(printstr[i+5]>=0x41)
				{
					recordnum+=(printstr[i+5]%16+9);				
				}				
			}			
		}
		TXTInfo[0].RecordCount=recordnum;			//数据总个数
//		printf(printstr);		
	}	
	else
	{
		status=ERROR;
	}
	f_close(&fsrc);	
	return status;
}

/***********************************************************************
函数名称：uint8_t WriteDataRecordNum_UDisk(char *str)
功    能：U盘写入TXT文档内数据记录个数
输入参数：文件名称
输出参数：1   写入成功  0   写入失败
编写时间：2014.05.08
编 写 人：masterjay
注    意：
***********************************************************************/
uint8_t WriteDataRecordNum_UDisk(char *str)
{	
	uint8_t status=NORMAL;
	char printstr[100];	
	char nstr[20];
	char FileName[20];
	
	ClearStr(FileName,sizeof(FileName));							//获取文件名字符串
	strcat(FileName,"0:/");
	strcat(FileName,str);	
	strcat(FileName,".TXT");		
	res = f_open( &fsrc , FileName , FA_OPEN_EXISTING | FA_WRITE);
	die(res);
	if ( res == FR_OK)
	{ 
			ClearStr(printstr,sizeof(printstr));
			TXTInfo[0].RowLength=ROWSIZE;											//数据记录的每行长度
			sprintf(nstr,"%4x",TXTInfo[0].RowLength); 
			strcat(printstr,nstr);
			strcat(printstr," ");														
			sprintf(nstr,"%8x",TXTInfo[0].RecordCount); 				//数据记录个数更新
			strcat(printstr,nstr);
			strcat(printstr,"\r\n");
			f_lseek(&fsrc,(CONTENTSIZE+RecordTMSIZE));		 	//偏移85个字节			
			res = f_write(&fsrc, printstr, strlen(printstr), &br); 							//******写入数据信息--15个字符
			die(res);
	}	
	else
	{
		status=ERROR;
	}
	f_close(&fsrc);	
	return status;
}



/***********************************************************************
函数名称：uint8_t ReadTxtToData_UDisk(char *str)
功    能：U盘读取TXT文档内数据
输入参数：文件名称
输出参数：1   读取出错    0   读取成功
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
uint8_t ReadTxtToData_UDisk(char *str)
{	
	uint8_t Error=FALSE;
	char printstr[100];
	char aa[100];

	strcat(printstr,"0:/");
	strcat(printstr,str);
	strcat(printstr,".TXT");	
	printf("文件存储位置：");		

	res = f_open( &fsrc , printstr , FA_OPEN_EXISTING | FA_READ);	//读取TXT文件	
	ClearStr(printstr,100);
	strcat(printstr,str);		
	strcat(printstr,".TXT");			
	if ( res == FR_OK )
	{ 
//**********************************************************************************************
//数据读取处理程序

	res = f_open(&fsrc, "0:/2014.TXT", FA_OPEN_EXISTING | FA_READ);
	if( res == FR_OK )		
	{
		
		res = f_read( &fsrc, aa, 25 , &br );							
		f_lseek(&fsrc, 25);
		res = f_read( &fsrc, aa, 25 , &br );
		
		f_close(&fsrc);
	}								
							
							
	
		

//**********************************************************************************************		
		strcat(printstr," 读取成功！");				
		printf(printstr);
		printf("\r\n");     
	}
	else		//TXT文件读取失败
	{  
		strcat(printstr," 不存在！");				
		printf(printstr);
		printf("\r\n");	
		Error=TRUE;					
	}	
	/*close file */
	f_close(&fsrc); 				

	return Error;
}


/***********************************************************************
函数名称：uint8_t CreatFile_UDisk(char *str)
功    能：U盘创建文件夹程序
输入参数：文件名称
输出参数：1   创建出错    0   创建成功
编写时间：2014.04.29
编 写 人：masterjay
注    意：文件不要使用中文名字，否则无法创建成功。如果使用，名字务必短
					程序有问题，不能使用
***********************************************************************/
uint8_t CreatFile_UDisk(char *str)
{
	uint8_t Error=FALSE;
	char printstr[100];
	ClearStr(printstr,100);
	if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)//如果u盘写保护
	{		
			printf("该u盘已写保护！");
			printf("\r\n");
			strcat(printstr,"无法创建“");
			strcat(printstr,str);
			strcat(printstr,"”文件");
			printf("\r\n");		
			Error=TRUE;																//创建出错报警
	}	
	else
	{
		strcat(printstr,"0:/");
		strcat(printstr,str);	
		printf("创建文件：");		
		printf(printstr);
		printf("\r\n");			
		res = f_mkdir(str);
		ClearStr(printstr,100);
		strcat(printstr,str);
		die(res);			
// 		if ( res == FR_OK )
// 		{ 
// 			strcat(printstr," 创建成功！");				
// 			printf(printstr);
// 			printf("\r\n");     
// 		}
// 		else if ( res == FR_EXIST )									//文件已经存在
// 		{  
// 			strcat(printstr," 文件已存在。");				
// 			printf(printstr);
// 			printf("\r\n");		
// 		}	
// 		else
// 		{
// 			printf("其他未知错误！！");
// 			printf("\r\n");				
// 		}		
	}
	return Error;	
}

/***********************************************************************
函数名称：void ClearStr(char *str,uint16_t len)
功    能：字符串清空程序
输入参数：无
输出参数：无
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
void ClearStr(char *str,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		str[i]='\0';
	}
}


/***********************************************************************
函数名称：void DateToFileName(void)
功    能：时间转换成文件名  最长8个字符
输入参数：无
输出参数：无
编写时间：2014.05.15
编 写 人：masterjay
注    意：
***********************************************************************/
void DateToFileName(void)
{
	ClearStr(NowTime.string,sizeof(NowTime.string));
	
// 	sprintf(NowTime.cord,"%4d",NowTime.year); 
// 	strcat(NowTime.string,NowTime.cord);
// 	sprintf(NowTime.cord,"%2d",NowTime.month); 
// 	strcat(NowTime.string,NowTime.cord);
// 	sprintf(NowTime.cord,"%2d",NowTime.day); 
// 	strcat(NowTime.string,NowTime.cord);	
	sprintf(NowTime.cord,"%2d",NowTime.hour); 	
	strcat(NowTime.string,NowTime.cord);	
	sprintf(NowTime.cord,"%2d",NowTime.min); 
	strcat(NowTime.string,NowTime.cord);
	sprintf(NowTime.cord,"%2d",NowTime.sec); 
	strcat(NowTime.string,NowTime.cord);	

}

/***********************************************************************
函数名称：void DateToStr(uint8_t ms_en)
功    能：时间转换成字符串
输入参数：ms是否使能
输出参数：无
编写时间：2014.05.04
编 写 人：masterjay
注    意：
***********************************************************************/
void DateToStr(uint8_t ms_en)
{
	uint8_t i;
	ClearStr(NowTime.string,sizeof(NowTime.string));
	
	sprintf(NowTime.cord,"%4d",NowTime.year); 
	strcat(NowTime.string,NowTime.cord);

	strcat(NowTime.string,"-");		

	sprintf(NowTime.cord,"%2d",NowTime.month); 
	strcat(NowTime.string,NowTime.cord);

	strcat(NowTime.string,"-");		

	sprintf(NowTime.cord,"%2d",NowTime.day); 
	strcat(NowTime.string,NowTime.cord);	

	strcat(NowTime.string," ");			

	sprintf(NowTime.cord,"%2d",NowTime.hour); 	
	strcat(NowTime.string,NowTime.cord);	

	strcat(NowTime.string,":");	

	sprintf(NowTime.cord,"%2d",NowTime.min); 
	strcat(NowTime.string,NowTime.cord);	
	
	strcat(NowTime.string,":");		
	
	sprintf(NowTime.cord,"%2d",NowTime.sec); 
	strcat(NowTime.string,NowTime.cord);	

	if(ms_en)
	{
		strcat(NowTime.string,":");				
		sprintf(NowTime.cord,"%4d",NowTime.msec); 
		strcat(NowTime.string,NowTime.cord);	
		for(i=0;i<23;i++)															//记录时间
		{
			RdData.TimeString[i]=NowTime.string[i+2];		
		}		
	}
}

/***********************************************************************
函数名称：char *itoa(int num, char *str, int radix)
功    能：整型到字符传转换程序
输入参数：待转换整数，字符存储寄存器，数值格式
输出参数：字符存储寄存器地址
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
char *itoa(int num, char *str, int radix)
{
	const char table[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *ptr = str;
	char *start;
	uint8_t negative = FALSE;
	if(num == 0)						//num=0 
	{ 
		*ptr++='0';
		*ptr='\0'; 						// don`t forget the end of the string is '\0'!!!!!!!!!
		return str;
	}
	if(num<0)								//if num is negative ,the add '-'and change num to positive 
	{ 
		 *ptr++='-';
		 num*=-1;
		 negative = TRUE;
	}
	while(num)
	{
		*ptr++ = table[num%radix];
		num/=radix;
	}
	*ptr = '\0';							
	
	// in the below, we have to converse the string
	start = (negative ? str+1 : str); //now start points the head of the string
	ptr--; //now prt points the end of the string

	while(start<ptr)
	{
		char temp = *start; 
		*start = *ptr;
		*ptr = temp;
		start++;
		ptr--;
	} 
	return str;	
}
/***********************************************************************
函数名称：void die(FRESULT res)
功    能：文件操作执行结果分析
输入参数：待转换整数，字符存储寄存器，数值格式
输出参数：字符存储寄存器地址
编写时间：2014.04.29
编 写 人：masterjay
注    意：
***********************************************************************/
void die(FRESULT res)
{
    switch(res)
		{
				case FR_OK:        //The function succeeded. 
				{
	//					printf("\r\nThe function succeeded!\r\n");
							break;
				}
				case FR_NOT_READY://The disk drive cannot work due to no medium in the drive or any other reason
				{
						printf("\r\nThe disk drive cannot work due to no medium in the drive or any other reason!\r\n");
								break;
				}
				case FR_NO_FILE://Could not find the file.
				{
						printf("\r\nCould not find the file!\r\n");
								break;
				}
				case FR_NO_PATH://Could not find the path
				{
						printf("\r\nCould not find the path!\r\n");
								break;
				}
				case FR_INVALID_NAME://The path name is invalid
				{
						printf("\r\nThe path name is invalid!\r\n");
								break;
				}
				case FR_INVALID_DRIVE://The drive number is invalid
				{
						printf("\r\nThe drive number is invalid!\r\n");
								break;
				}
				case FR_DENIED://The directory cannot be created due to directory table or disk is full. 
				{
						printf("\r\nThe directory cannot be created due to directory table or disk is full!\r\n");
								break;
				}
				case FR_EXIST://A file or directory that has same name is already existing
				{
						printf("\r\nA file or directory that has same name is already existing!\r\n");
								break;
				}
//                case FR_RW_ERROR://The function failed due to a disk error or an internal error
/*
				case FR_RW_ERROR://The function failed due to a disk error or an internal error
				{
						printp("\r\nThe function failed due to a disk error or an internal error!\r\n");
								break;
				}
*/
				case FR_WRITE_PROTECTED://The medium is write protected
				{
						printf("\r\nThe medium is write protected!\r\n");
								break;
				}
				case FR_NOT_ENABLED://The logical drive has no work area
				{
						printf("\r\nThe logical drive has no work area!\r\n");
								break;
				}
				case FR_NO_FILESYSTEM://There is no valid FAT partition on the disk
				{
						printf("\r\nThere is no valid FAT partition on the disk!\r\n");
								break;
				}
				case FR_INVALID_OBJECT://The file object is invalid
				{
						printf("\r\nThe file object is invalid!\r\n");
								break;
				}

				//The function aborted before start in format due to a reason as follows. 
				//The disk size is too small. 
				//Invalid parameter was given to any parameter. 
				//Not allowable cluster size for this drive. This can occure when number of clusters becomes around 0xFF7 and 0xFFF7. 
				case FR_MKFS_ABORTED://
				{
						printf("\r\nThe function aborted before start in format!\r\n");
								break;
				}
				
				default:
				{
						printf("\r\nerror!\r\n");
								break;
				}        
		}
		return;
}
