/**
  ******************************************************************************
  * @file    usbh_msc_fs_interface.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   Header file for usbh_msc_fs_interface.c
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
#ifndef __USBH_MSC_FS_INTERFACE_H__
#define __USBH_MSC_FS_INTERFACE_H__



/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */
  
/** @defgroup USBH_MSC_FS_INTERFACE
  * @brief This file is the Header file for usbh_msc_fs_interface.c
  * @{
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Exported_Types
  * @{
  */ 
struct  hwInterface{
  /*FILE *imageFile;*/
  int32_t  	sectorCount;
};
typedef struct hwInterface hwInterface;
/**
  * @}
  */ 



/** @defgroup USBH_MSC_FS_INTERFACE_Exported_Defines
  * @{
  */ 
#define EFS_ERROR   -1
#define EFS_PASS     0

/**
  * @}
  */ 

/** @defgroup USBH_MSC_FS_INTERFACE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_MSC_FS_INTERFACE_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_MSC_FS_INTERFACE_Exported_FunctionsPrototype
  * @{
  */ 
int8_t if_initInterface(hwInterface* file,char* opts);
int8_t if_readBuf(hwInterface* file,uint32_t address,uint8_t* buf);
int8_t if_writeBuf(hwInterface* file,uint32_t address,uint8_t* buf);
/**
  * @}
  */ 


#endif  /*__USBH_MSC_FS_INTERFACE_H__*/


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



