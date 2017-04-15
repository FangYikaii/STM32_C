/**
  ******************************************************************************
  * @file    usbh_msc_fs_interface.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   This file is the interface between File Systems and Host MSC class
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

/* Includes ----------------------------------------------------------------*/
#include "efs.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbh_msc_fs_interface.h"


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
  * @brief This file is the interface between EFSL ans Host mass-storage class
  * @{
  */ 

/** @defgroup USBH_MSC_FS_INTERFACE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Private_Variables
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_MSC_FS_INTERFACE_Private_Functions
  * @{
  */ 


/**
  * @brief  if_initInterface
  *         Initialises the EFSL interface parameters
  * @param  file : File pointer
  * @param  opts : Optional Parameter. Not used here.
  * @retval Status : 0 -> Pass, -1 -> Fail
  */
int8_t if_initInterface(hwInterface* file, char* opts)
{
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
        file->sectorCount = USBH_MSC_Param.MSCapacity; 
  }
  return(EFS_PASS);
}


/**
  * @brief  if_readBuf
  *         This function is responsible to read a sector from the disc and 
  *         store it in a user supplied buffer.
  * @param  file : File pointer
  * @param  address : An LBA address, relative to the beginning of the disc
  * @param  buf : Buffer where the data will be stored after reading
  * @retval Status : 0 -> Pass, -1 -> Fail
  */

int8_t if_readBuf(hwInterface* file,uint32_t address,uint8_t* buf)
{
  int8_t status = EFS_ERROR;
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    
    do
    {
      status = USBH_MSC_Read10(buf,address,512);
      USBH_MSC_HandleBOTXfer();
    }
    while((status == USBH_MSC_BUSY ) && (HCD_IsDeviceConnected(&USB_OTG_FS_dev)));
    
  }
  
  return(status);
  
}


/**
  * @brief  if_writeBuf
  *         This function is responsible to write a sector of data on the disc  
  *         from a user supplied buffer.
  * @param  file : File pointer
  * @param  address : An LBA address, relative to the beginning of the disc
  * @param  buf : Buffer from where the data will be taken to write
  * @retval Status : 0 -> Pass, -1 -> Fail
  */
         
int8_t if_writeBuf(hwInterface* file,uint32_t address,uint8_t* buf)
{
  int8_t status = EFS_ERROR;
  
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    do
    {
      status = USBH_MSC_Write10(buf,address,USBH_MSC_PAGE_LENGTH);
      USBH_MSC_HandleBOTXfer();
    }
    while((status == USBH_MSC_BUSY ) && \
      (HCD_IsDeviceConnected(&USB_OTG_FS_dev)));
  }
  return(status);
}


/**
  * @brief  if_TestUnitReady
  *         This function is responsible to issue a Test Unit Ready command.User
  *         can issue a Test Unit Ready command by calling this function.
  * @param  None
  * @retval Status : 0 -> Pass, -1 -> Fail
  */

int8_t if_TestUnitReady(void)
{
  int8_t status = EFS_ERROR;
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    do
    {
      status = USBH_MSC_TestUnitReady();
      USBH_MSC_HandleBOTXfer();
    }
    while((status == USBH_MSC_BUSY ) && \
      (HCD_IsDeviceConnected(&USB_OTG_FS_dev)));
  }
  
  return(status);
  
}


/**
  * @brief  if_RequestSense
  *         This function is responsible to issue a RequestSense command. User
  *         can issue a RequestSense command by calling this function.
  * @param  None
  * @retval Status : 0 -> Pass, -1 -> Fail
  */

int8_t if_RequestSense(void)
{
  int8_t status = EFS_ERROR;
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    
    do
    {
      status = USBH_MSC_RequestSense();
      USBH_MSC_HandleBOTXfer();
    }
    while((status == USBH_MSC_BUSY ) && \
      (HCD_IsDeviceConnected(&USB_OTG_FS_dev)));
  }
  
  return(status);
  
}


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

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/



