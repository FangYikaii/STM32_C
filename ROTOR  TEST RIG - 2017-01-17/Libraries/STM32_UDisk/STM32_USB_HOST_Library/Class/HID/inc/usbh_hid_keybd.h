/**
  ******************************************************************************
  * @file    usbh_hid_keybd.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   This file contains all the prototypes for the HID_kybd.c
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
#ifndef __USBH_HID_KEYBD_H
#define __USBH_HID_KEYBD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usbh_hid_core.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */
  
/** @defgroup USBH_HID_KEYBD
  * @brief This file is the Header file for USBH_HID_KEYBD.c
  * @{
  */ 


/** @defgroup USBH_HID_KEYBD_Exported_Types
  * @{
  */ 


/**
  * @}
  */ 

/** @defgroup USBH_HID_KEYBD_Exported_Defines
  * @{
  */ 
//#define QWERTY_KEYBOARD
#define AZERTY_KEYBOARD

#define  KBD_LEFT_CTRL                                  0x01
#define  KBD_LEFT_SHIFT                                 0x02
#define  KBD_LEFT_ALT                                   0x04
#define  KBD_LEFT_GUI                                   0x08
#define  KBD_RIGHT_CTRL                                 0x10
#define  KBD_RIGHT_SHIFT                                0x20
#define  KBD_RIGHT_ALT                                  0x40
#define  KBD_RIGHT_GUI                                  0x80

#define  KBR_MAX_NBR_PRESSED                            6

/**
  * @}
  */ 

/** @defgroup USBH_HID_KEYBD_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_HID_KEYBD_Exported_Variables
  * @{
  */ 

extern HID_cb_TypeDef HID_KEYBRD_cb;
/**
  * @}
  */ 

/** @defgroup USBH_HID_KEYBD_Exported_FunctionsPrototype
  * @{
  */ 
void  USR_KEYBRD_Init (void);
void  USR_KEYBRD_ProcessData (uint8_t pbuf);
/**
  * @}
  */ 

#endif /* __USBH_HID_KEYBD_H */

/**
  * @}
  */ 

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

