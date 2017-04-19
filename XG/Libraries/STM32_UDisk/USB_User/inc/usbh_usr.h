//========================================
//<--预定义处理，防止重复定义-->
#ifndef _usbh_usr_H
#define _usbh_usr_H

//========================================
//头文件
#include "ff.h"
#include "usbh_core.h"
#include <stdio.h>
#include "usbh_msc_core.h"


//========================================
//宏定义




//#define HOST_OVRCURR_PORT                  GPIOE
//#define HOST_OVRCURR_LINE                  GPIO_Pin_1
//#define HOST_OVRCURR_PORT_SOURCE           GPIO_PortSourceGPIOE
//#define HOST_OVRCURR_PIN_SOURCE            GPIO_PinSource1
//#define HOST_OVRCURR_PORT_RCC              RCC_APB2Periph_GPIOE
//#define HOST_OVRCURR_EXTI_LINE             EXTI_Line1
//#define HOST_OVRCURR_IRQn                  EXTI1_IRQn 

#define 				HOST_POWERSW_PORT_RCC              RCC_APB2Periph_GPIOC					//控制USB_OTG 是否需要供电
#define 				HOST_POWERSW_PORT                  GPIOC
#define 				HOST_POWERSW_VBUS                  GPIO_Pin_4

// #define 				HOST_SOF_OUTPUT_RCC                RCC_APB2Periph_GPIOA
// #define 				HOST_SOF_PORT                      GPIOA
// #define 				HOST_SOF_SIGNAL                    GPIO_Pin_8






/* State Machine for the USBH_USR_ApplicationState */
#define USH_USR_FS_INIT       0
#define USH_USR_FS_READLIST   1
#define USH_USR_FS_WRITEFILE  2
#define USH_USR_FS_READFILE   3
#define USH_USR_FS_NULL       4

//========================================
//外调变量声明	
extern  uint8_t USBH_USR_ApplicationState ;
extern  USBH_Usr_cb_TypeDef USR_Callbacks;



extern  UINT br;
extern  FATFS fs;         /* Work area (file system object) for logical drive */
extern  FIL fsrc;         /* file objects */
extern  FRESULT res;
//========================================
//外调函数声明
void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_HexToASCII(uint32_t);
int USBH_USR_MSC_Application(void);
void DisplayBMPImage(void);
void USBH_USR_HexToASCII(uint32_t);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);

//========================================
//<--预处理结束-->
#endif

