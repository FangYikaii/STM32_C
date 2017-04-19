/**
  ******************************************************************************
  * @file    usb_defines.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   Header of the Core Layer
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
#ifndef __USB_DEF_H__
#define __USB_DEF_H__

/* Includes ------------------------------------------------------------------*/
#include  "usb_conf.h"

/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_DEFINES
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_DEFINES_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup _CORE_DEFINES_
  * @{
  */
#define USB_OTG_SPEED_PARAM_FULL 1

/**
  * @}
  */


/** @defgroup _GLOBAL_DEFINES_
  * @{
  */
#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GAHBCFG_GLBINT_ENABLE                  1

#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0
#define GRXSTS_PKTSTS_IN                       2
#define GRXSTS_PKTSTS_IN_XFER_COMP             3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5
#define GRXSTS_PKTSTS_CH_HALTED                7
/**
  * @}
  */


/** @defgroup _OnTheGo_DEFINES_
  * @{
  */
#define DEVICE_MODE                            0
#define HOST_MODE                              1
#define OTG_MODE                               2
/**
  * @}
  */


/** @defgroup __DEVICE_DEFINES_
  * @{
  */

#define DSTS_ENUMSPD_LS_PHY_6MHZ               2
#define DSTS_ENUMSPD_FS_PHY_48MHZ              3

#define DCFG_FRAME_INTERVAL_80                 0
#define DCFG_FRAME_INTERVAL_85                 1
#define DCFG_FRAME_INTERVAL_90                 2
#define DCFG_FRAME_INTERVAL_95                 3

#define EP_SPEED_LOW                           0
#define EP_SPEED_FULL                          1
#define EP_SPEED_HIGH                          2

#define EP_TYPE_CTRL                           0
#define EP_TYPE_ISOC                           1
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3
#define EP_TYPE_MSK                            3

#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6
/**
  * @}
  */


/** @defgroup __HOST_DEFINES_
  * @{
  */
#define HC_PID_DATA0                           0
#define HC_PID_DATA2                           1
#define HC_PID_DATA1                           2
#define HC_PID_SETUP                           3

#define HPRT0_PRTSPD_HIGH_SPEED                0
#define HPRT0_PRTSPD_FULL_SPEED                1
#define HPRT0_PRTSPD_LOW_SPEED                 2

#define HCFG_30_60_MHZ                         0
#define HCFG_48_MHZ                            1
#define HCFG_6_MHZ                             2

#define HCCHAR_CTRL                            0
#define HCCHAR_ISOC                            1
#define HCCHAR_BULK                            2
#define HCCHAR_INTR                            3

/**
  * @}
  */


/** @defgroup USB_DEFINES_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_DEFINES_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_DEFINES_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_DEFINES_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Internal_Macro's
  * @{
  */
#define  MIN(a, b)      (((a) < (b)) ? (a) : (b))
  
#define USB_OTG_READ_REG32(reg)  (*(__IO uint32_t *)reg)
#define USB_OTG_WRITE_REG32(reg,value) (*(__IO uint32_t *)reg = value)
#define USB_OTG_MODIFY_REG32(reg,clear_mask,set_mask) \
  USB_OTG_WRITE_REG32(reg, (((USB_OTG_READ_REG32(reg)) & ~clear_mask) | set_mask ) )
/**
  * @}
  */

#endif //__USB_DEFINES__H__


/**
  * @}
  */ 

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

