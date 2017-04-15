/**
  ******************************************************************************
  * @file    usbh_core.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   Header file for usbh_core.c
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_CORE_H
#define __USBH_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_hcd.h"
#include "usbh_def.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/
  
/** @defgroup USBH_CORE
  * @brief This file is the Header file for usbh_core.c
  * @{
  */ 


/** @defgroup USBH_CORE_Exported_Defines
  * @{
  */ 

#define MSC_CLASS                         0x08
#define HID_CLASS                         0x03
#define MSC_PROTOCOL                      0x50
#define CBI_PROTOCOL                      0x01

#define USBH_MAX_NUM_ENDPOINTS                          2
#define USBH_MAX_ERROR_COUNT                            2
#define USBH_DEVICE_ADDRESS_DEFAULT                     0
#define USBH_DEVICE_ADDRESS                             1


/**
  * @}
  */ 


/** @defgroup USBH_CORE_Exported_Types
  * @{
  */

typedef enum {
  USBH_OK   = 0,
  USBH_BUSY,
  USBH_FAIL,
  USBH_NOT_SUPPORTED,
  USBH_UNRECOVERED_ERROR,
  USBH_ERROR_SPEED_UNKNOWN,
  USBH_APPLY_DEINIT
}USBH_Status;

/* Following states are used for gState */
typedef enum {
  HOST_IDLE =0,
  HOST_ISSUE_CORE_RESET,
  HOST_DEV_ATTACHED,
  HOST_DEV_DISCONNECTED,  
  HOST_ISSUE_RESET,
  HOST_DETECT_DEVICE_SPEED,
  HOST_ENUMERATION,
  HOST_CLASS_REQUEST,  
  HOST_CLASS,
  HOST_CTRL_XFER,
  HOST_USR_INPUT,
  HOST_SUSPENDED,
  HOST_ERROR_STATE  
}HOST_State;  

/* Following states are used for EnumerationState */
typedef enum {
  ENUM_IDLE = 0,
  ENUM_GET_FULL_DEV_DESC,
  ENUM_SET_ADDR,
  ENUM_GET_CFG_DESC,
  ENUM_GET_FULL_CFG_DESC,
  ENUM_GET_MFC_STRING_DESC,
  ENUM_GET_PRODUCT_STRING_DESC,
  ENUM_GET_SERIALNUM_STRING_DESC,
  ENUM_SET_CONFIGURATION,
  ENUM_DEV_CONFIGURED
} ENUM_State;  

/* Following states are used for RequestState */
typedef enum {
  CMD_IDLE =0,
  CMD_SEND,
  CMD_WAIT
} CMD_State;  

/* Following states are used for CtrlXferStateMachine */
typedef enum {
  CTRL_IDLE =0,
  CTRL_SETUP,
  CTRL_SETUP_WAIT,
  CTRL_DATA_IN,
  CTRL_DATA_IN_WAIT,
  CTRL_DATA_OUT,
  CTRL_DATA_OUT_WAIT,
  CTRL_STATUS_IN,
  CTRL_STATUS_IN_WAIT,
  CTRL_STATUS_OUT,
  CTRL_STATUS_OUT_WAIT,
  CTRL_ERROR
}
CTRL_State;  

typedef enum {
  USBH_USR_NO_RESP   = 0,
  USBH_USR_RESP_OK = 1,
}
USBH_USR_Status;

typedef struct _Ctrl
{
  uint8_t               hc_num_in; 
  uint8_t               hc_num_out; 
  uint8_t               ep0size;  
  uint8_t               *buff;
  uint16_t              length;
  uint8_t               errorcount;
  uint16_t              timer;  
  CTRL_STATUS            status;
  USB_Setup_TypeDef    *setup;
  CTRL_State            state;  

} USBH_Ctrl_TypeDef;


typedef struct _DeviceProp
{
  
  uint8_t                           address;
  uint8_t                           speed;
  USBH_DevDesc_TypeDef              Dev_Desc;
  USBH_CfgDesc_TypeDef              Cfg_Desc;  
  USBH_InterfaceDesc_TypeDef        Itf_Desc; /* single interface */
  USBH_EpDesc_TypeDef               Ep_Desc[USBH_MAX_NUM_ENDPOINTS];
  USBH_HIDDesc_TypeDef              HID_Desc;
  
}USBH_DeviceProp_TypeDef;

typedef struct _Device_cb
{
  USBH_Status  (*Init)\
    (USB_OTG_CORE_HANDLE *pdev , USBH_DeviceProp_TypeDef *hdev);
  void         (*DeInit)\
    (USB_OTG_CORE_HANDLE *pdev , USBH_DeviceProp_TypeDef *hdev);
  USBH_Status  (*Requests)\
    (USB_OTG_CORE_HANDLE *pdev , USBH_DeviceProp_TypeDef *hdev);  
  USBH_Status  (*Machine)\
    (USB_OTG_CORE_HANDLE *pdev , USBH_DeviceProp_TypeDef *hdev);     
  
} USBH_Class_cb_TypeDef;


typedef struct _DeviceConnStatus_cb
{
  void (*Disconnect) (USB_OTG_CORE_HANDLE *pdev);
  void (*Connect) (USB_OTG_CORE_HANDLE *pdev); 
  uint8_t ConnStatus;
  uint8_t DisconnStatus;
  uint8_t ConnHandled;
  uint8_t DisconnHandled;
} USBH_DeviceConnStatus_cb_TypeDef;


typedef struct _USBH_USR_PROP
{
  void (*Init)(void);       /* HostLibInitialized */
  void (*DeviceAttached)(void);           /* DeviceAttached */
  void (*ResetDevice)(void);
  void (*DeviceDisconnected)(void); 
  void (*OverCurrentDetected)(void);  
  void (*DeviceSpeedDetected)(uint8_t DeviceSpeed);          /* DeviceSpeed */
  void (*DeviceDescAvailable)(void *);    /* DeviceDescriptor is available */
  void (*DeviceAddressAssigned)(void);  /* Address is assigned to USB Device */
  void (*ConfigurationDescAvailable)(USBH_CfgDesc_TypeDef *,
                                     USBH_InterfaceDesc_TypeDef *,
                                     USBH_EpDesc_TypeDef *); 
  /* Configuration Descriptor available */
  void (*ManufacturerString)(void *);     /* ManufacturerString*/
  void (*ProductString)(void *);          /* ProductString*/
  void (*SerialNumString)(void *);        /* SerialNubString*/
  void (*EnumerationDone)(void);           /* Enumeration finished */
  USBH_USR_Status (*UserInput)(void);
  void (*USBH_USR_DeviceNotSupported)(void); /* Device is not supported*/
  void (*UnrecoveredError)(void);

}
USBH_Usr_cb_TypeDef;

typedef struct _Process
{
  HOST_State            gState;       /*  Host State Machine Value */
  HOST_State            gStateBkp;    /* backup of previous State machine value */
  ENUM_State            EnumState;    /* Enumeration state Machine */
  CMD_State             RequestState;       
  USBH_Ctrl_TypeDef     Control;
  USBH_Class_cb_TypeDef               *class_cb;  
  USBH_Usr_cb_TypeDef  	              *usr_cb;
  USBH_DeviceConnStatus_cb_TypeDef    *conn_cb;  
  
} USBH_Machine_TypeDef;

/**
  * @}
  */ 



/** @defgroup USBH_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBH_CORE_Exported_Variables
  * @{
  */ 
extern USBH_Machine_TypeDef         USBH_Machine;
extern USB_OTG_CORE_HANDLE          USB_OTG_FS_dev;
extern USBH_DeviceConnStatus_cb_TypeDef  USBH_DeviceConnStatus_cb;

/**
  * @}
  */ 

/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */ 
void USBH_Init(USB_OTG_CORE_HANDLE *pdev,
               USBH_Class_cb_TypeDef *class_cb, 
               USBH_Usr_cb_TypeDef *usr_cb);
               

void USBH_Process(void);
void USBH_ErrorHandle(USBH_Status);

/**
  * @}
  */ 

#endif /* __USBH_CORE_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/



