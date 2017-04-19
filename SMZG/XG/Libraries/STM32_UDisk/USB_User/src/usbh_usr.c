/*
程序说明:  	
            USB_OTG 用户程序 --- STM32F107VCT 
						
注意事项：	1、注意管脚的复用问题						
						
						
端口使用:		见Doc文件夹内说明
创建者:     FangYikaii   
创建时间:   2014-04-21 
修改时间:   2014-04-21
*/

//========================================
//头文件
#include "main.h"

#include "ff.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

//========================================
//变量定义


uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t temp[]                        = "          \n";
uint8_t buffer[1024]; 

UINT br;
FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc;         /* file objects */
FRESULT res;


/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,															//U盘启动初始
  USBH_USR_DeviceAttached,										//U盘连接提示
  USBH_USR_ResetDevice,												//U盘复位处理程序
  USBH_USR_DeviceDisconnected,								//U盘拔出提示
  USBH_USR_OverCurrentDetected,								//
  USBH_USR_DeviceSpeedDetected,								//U盘速度检测提示
  USBH_USR_Device_DescAvailable,							//
  USBH_USR_DeviceAddressAssigned,							//
  USBH_USR_Configuration_DescAvailable,				//U盘设备ID提示
  USBH_USR_Manufacturer_String,								//
  USBH_USR_Product_String,										//
  USBH_USR_SerialNum_String,									//
  USBH_USR_EnumerationDone,										//
  USBH_USR_UserInput,													//
  USBH_USR_DeviceNotSupported,								//
  USBH_USR_UnrecoveredError										//U盘出错提示

};

//========================================
//子模块函数



/***********************************************************************
函数名称：void USBH_USR_Init(void)
功    能：U盘启动初始
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_Init(void)
{
 	printf(">启动u盘初始化\r\n");
}

/***********************************************************************
函数名称：void USBH_USR_DeviceAttached(void)
功    能：U盘连接提示
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_DeviceAttached(void)
{
	USBInit_STATUS=0;				//U盘初始化标志	
  printf("检测到u盘！");
  printf("\r\n");
}


/***********************************************************************
函数名称：void USBH_USR_UnrecoveredError (void)
功    能：U盘出错提示
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_UnrecoveredError (void)
{
 
  printf("不可恢复状态错误");
  printf("\r\n");
  
}


/***********************************************************************
函数名称：void USBH_USR_DeviceDisconnected (void)
功    能：U盘拔出提示
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_DeviceDisconnected (void)
{
 
 	printf("u盘已拔出！\r\n");

}

/***********************************************************************
函数名称：void USBH_USR_ResetDevice(void)
功    能：U盘复位处理程序
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/***********************************************************************
函数名称：void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
功    能：U盘速度检测提示
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/

void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
		printf("usb设备全速。");
		printf("\r\n");
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
		printf("usb设备慢速。");
		printf("\r\n");
  }
  else
  {
		printf("usb设备错误。");
		printf("\r\n");
  }
}

/***********************************************************************
函数名称：void USBH_USR_Device_DescAvailable(void *DeviceDesc)
功    能：U盘设备ID提示
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  uint8_t temp[50];    
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  
  
  
  sprintf((char *)temp , "VID          = %04Xh" , (uint32_t)(*hs).idVendor); 
  printf("VID          = %04Xh \r\n" , (uint32_t)(*hs).idVendor);  
  
  sprintf((char *)temp , "PID          = %04Xh" , (uint32_t)(*hs).idProduct); 
  printf("PID          = %04Xh \r\n" , (uint32_t)(*hs).idProduct);  

}

/***********************************************************************
函数名称：void USBH_USR_DeviceAddressAssigned(void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}


/***********************************************************************
函数名称：void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  uint8_t temp[50];
  
  id = itfDesc;
  
  sprintf ((char *)temp, "端点号 = %d" , (uint32_t)(*id).bNumEndpoints);
  printf("端点号 = %d \r\n" , (uint32_t)(*id).bNumEndpoints);
  
  
  if((*id).bInterfaceClass  == 0x08)
  {
    
    printf("海量存储类设备");
		printf("\r\n");

  }
  else if((*id).bInterfaceClass  == 0x03)
  {
    
    printf("HID类设备");
		printf("\r\n");
  }    
}

/***********************************************************************
函数名称：void USBH_USR_Manufacturer_String(void *ManufacturerString)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  char temp[100];
  sprintf(temp, "厂商 : %s", (char *)ManufacturerString);
  printf("厂商 : %s \r\n", (char *)ManufacturerString);  
}

/***********************************************************************
函数名称：void USBH_USR_Product_String(void *ProductString)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_Product_String(void *ProductString)
{
  char temp[100];
  sprintf((char *)temp, "Product : %s", (char *)ProductString);  
  printf("产品 : %s \r\n", (char *)ProductString); 
}

/***********************************************************************
函数名称：USBH_USR_SerialNum_String(void *SerialNumString)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  uint8_t temp[100];
  sprintf((char *)temp, "串口号 : %s", (char *)SerialNumString);    
  printf("串口号 : %s \r\n", (char *)SerialNumString); 

} 



/***********************************************************************
函数名称：void USBH_USR_EnumerationDone(void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_EnumerationDone(void)
{
  
  /* Enumeration complete */
  printf("枚举完成。"); 
  printf("----------------------------------------"); 
  printf("\r\n");
  printf("\r\n");
  printf("\r\n");
} 


/***********************************************************************
函数名称：void USBH_USR_DeviceNotSupported(void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_DeviceNotSupported(void)
{
  
  if(MSC_Machine.maxLun > 0)
  {
		printf("> Contains more than one LUN! \r\n ");  

    /*Display the LUN in the connected device*/
    USBH_USR_HexToASCII((uint32_t)(MSC_Machine.maxLun));

		printf("LUN Available in the device:");
		printf("\r\n");  


    printf("-----------------------------------------");
		printf("\r\n");  


		printf( "%s" , temp );
		printf("\r\n");

    printf("-----------------------------------------");
		printf("\r\n");    
  }
  /* 2 seconds delay */
  USB_OTG_BSP_mDelay(2000);
  
}  


/***********************************************************************
函数名称：USBH_USR_Status USBH_USR_UserInput(void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
USBH_USR_Status USBH_USR_UserInput(void)
{
//   USBH_USR_Status usbh_usr_status;
  return USBH_USR_RESP_OK;
}  

/***********************************************************************
函数名称：void USBH_USR_OverCurrentDetected (void)
功    能：
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_OverCurrentDetected (void)
{
  

  printf("u盘读写\r\n");
  
  printf("ERR : OVERCURRENT DETECTED \r\n ");

  
    /* 2 seconds delay */
    USB_OTG_BSP_mDelay(2000);
}


/***********************************************************************
函数名称：int USBH_USR_MSC_Application(void)
功    能：U盘工作状态检测程序
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
int USBH_USR_MSC_Application(void)
{
// 	uint i;
// 	ulong addr123=0;
	
  uint8_t read_txt_file_buff[512]; 
  char textFileBuffer[] = "感谢您使用本采集设备！！\r\n";   
  FILINFO fno;
  DIR dir;
 /**********状态机******************/
  switch(USBH_USR_ApplicationState)
  {
		case USH_USR_FS_INIT:  //文件初始化，读取u盘大小、
    
				/* Initialises the file system */
				f_mount(0,&fs);																//挂载U盘
				// f_mount(0, NULL);													//取消挂载
		
				/*Display the size of the disk*/
				USBH_USR_HexToASCII((((USBH_MSC_Param.MSCapacity) * \
														 (USBH_MSC_Param.MSPageLength))/(1048576)));

				printf("该u盘的大小为（MBytes）：");	//"* Size of the disk in MBytes: "
				//printf("\r\n");

				printf("%s",temp);			//U盘大小
				printf("\r\n");

				printf("-----------------------------------------");
				printf("\r\n");   
			 
				if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED) //如果U盘写保护
				{
			
					printf("该u盘已写保护！");
					printf("\r\n");  
				
					printf("-----------------------------------------");
					printf("\r\n");  
				}
				USBH_USR_ApplicationState = USH_USR_FS_READLIST;
				break;
    
		case USH_USR_FS_READLIST:	//读取u盘的根目录文件名称
    
					/*Reads the dir list*/
					res = f_opendir(&dir, "0:");
					if( res != FR_OK )
					{
						/*Could not open the selected directory*/
						return(-2);
					}
						

					printf("该u盘根目录文件列表如下：");
					printf("\r\n");  
					printf("\r\n");
					printf("\r\n");     
					
					/**************读u盘根目录并将名称发到串口，注意不支持长文件名***************/
					while ((HCD_IsDeviceConnected(&USB_OTG_FS_dev)) && ( f_readdir(&dir, &fno) == FR_OK ) && fno.fname[0] != 0 )
					{       

						printf("%s",fno.fname);
						printf("\r\n");    
					}
					USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;
					break;
    
		case USH_USR_FS_WRITEFILE:		//向U盘创建文件并写入数据
    
					/* Writes a text file, U盘例程.TXT in the disk*/
				
					if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)//如果u盘写保护
					{		
							printf("该u盘已写保护！");
							printf("\r\n");

							printf("无法创建“说明.TXT”文件");
							printf("\r\n");
						
							USBH_USR_ApplicationState = USH_USR_FS_READFILE;
							break;
					}				
					res = f_open( &fsrc , "0:/说明.TXT" , FA_CREATE_NEW | FA_WRITE);	//创建“U盘例程.TXT”文件	
//					res = f_open( &fsrc , "0:/说明.TXT" , FA_OPEN_EXISTING | FA_WRITE);	//创建“U盘例程.TXT”文件						
					
					if ( res == FR_OK )
					{ 
						/* Write buffer to file */
						res = f_write(&fsrc, textFileBuffer, strlen(textFileBuffer), &br); 	
						
//下面程序为测试程序，偏移地址应该使数据不能跨区写入，应该以1024作为一个存储区
// 						addr123=0;
// 						for(i=0;i<10000;i++)
// 						{
// 							addr123=i*64;
// 							res=f_lseek(&fsrc, addr123);
// 							/* Write buffer to file */
// 							res = f_write(&fsrc, textFileBuffer, strlen(textFileBuffer), &br); 	
// 							printf("Now the I = %d; the addr=%d \r\n",i,addr123);
// 							
// 							res=f_sync(&fsrc);
// 						}						
						
						printf("***********************************************************");
						printf("\r\n");

						printf("说明.TXT 创建成功！");
						printf("\r\n");
					
						/*close file */
						f_close(&fsrc);      
					}
					else if ( res == FR_EXIST )	//如果“U盘例程.TXT”已经存在
					{
						printf("-----------------------------------------");
						printf("\r\n");	  
						printf("说明.TXT 文件已存在。");
						printf("\r\n");
						
					}
					
					USBH_USR_ApplicationState = USH_USR_FS_READFILE;					
					break;
    
		case USH_USR_FS_READFILE:	  //读取文件
		 
					printf("-----------------------------------------");
					printf("\r\n");

					if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
					{
						f_mount(0, &fs);      
						res = f_opendir(&dir, "0:");
						if( res != FR_OK )
						{
							/*Could not open the selected directory*/
							return(-2);
						}
						
						if((HCD_IsDeviceConnected(&USB_OTG_FS_dev)) && ( f_readdir(&dir, &fno) == FR_OK ) && fno.fname[0] != 0 )
						{ 
							res = f_open(&fsrc, "0:/说明.TXT", FA_OPEN_EXISTING | FA_READ);
							if( res == FR_OK )
							{
								printf("说明.TXT  读取成功！");
								printf("\r\n");
								res = f_read( &fsrc, read_txt_file_buff, sizeof(read_txt_file_buff) , &br );
								printf("说明.TXT  的内容是：");
								printf("\r\n");
								printf("%s",read_txt_file_buff); //将读取到的内容发到串口  								
								/*close file */
								f_close(&fsrc);
							}
							else
							{
								printf("-----------------------------------------");
								printf("\r\n");	
								printf("没有找到 说明.TXT");
								printf("\r\n");	
							}						
						}
					}
					USBH_USR_ApplicationState = USH_USR_FS_NULL;
					break;
					
		case USH_USR_FS_NULL:	  //文件打开成功，允许进行读写操作				
					
					
		default: 
					break;
  }
  return(0);
}



/***********************************************************************
函数名称：void USBH_USR_HexToASCII(uint32_t receivedValue)
功    能：16进制转换成ASCII程序
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_HexToASCII(uint32_t receivedValue)
{
  uint8_t count;
  uint8_t localTemp[10] = "         "; 
  uint8_t *ptr;
  uint8_t counter = 0;
  
  for(count = 0; count < 8; count++ )
	{
    temp[count] = 0x20;		
	} 
  temp[8] = 0;
  for(count = 0; count < 8; count++ )
  {
    localTemp[count] = receivedValue % 10;    
    if(localTemp[count] < 0x0A)
		{
      localTemp[count] += 0x30;
		}
    else
		{
      localTemp[count] += 0x37; 
		}			
    
    if(receivedValue < 10)
    {     
      break;
    }    
    receivedValue = receivedValue / 10;   
  } 
  count = 0;
  
  /*check for end of data*/
  ptr = localTemp;
  while(*ptr != 0x20)
  {
    count++;
    ptr++;
  }
  
  counter = 0;
  ptr--;
  
  while(count)
  {
    temp[counter] = *ptr;
    ptr--;
    count--;
    counter++;
  }  
  
}

/***********************************************************************
函数名称：void USBH_USR_DeInit(void)
功    能：U盘初始化
输入参数：无
输出参数：无
编写时间：2014.04.27
编 写 人：FangYikaii
注    意：
***********************************************************************/
void USBH_USR_DeInit(void)
{
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
}


























