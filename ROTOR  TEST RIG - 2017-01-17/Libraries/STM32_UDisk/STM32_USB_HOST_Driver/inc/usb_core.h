/**
  ******************************************************************************
  * @file    usb_core.h
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
#ifndef __USB_CORE_H__
#define __USB_CORE_H__

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usb_regs.h"
#include "usb_defines.h"
#include "usb_bsp.h"

#if defined ( __CC_ARM   )
  #define __packed        __packed                     /*!< packing keyword for ARM Compiler */
#elif defined ( __ICCARM__ )
  #define __packed        __packed                     /*!< packing keyword for IAR Compiler */
#elif defined   (  __GNUC__  )
  #define __packed        __attribute__ ((__packed__)) /*!< packing keyword for GNU Compiler */
#elif defined   (  __TASKING__  )                      /*!< packing keyword for TASKING Compiler */
  #define __packed   
#endif /* __CC_ARM */

/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_CORE
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_CORE_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_CORE_Exported_Types
  * @{
  */ 
enum USB_OTG_SPEED {
  USB_SPEED_UNKNOWN = 0,
  USB_SPEED_LOW,
  USB_SPEED_FULL,
  USB_SPEED_HIGH
};

typedef enum {
  USB_OTG_OK = 0,
  USB_OTG_FAIL
}USB_OTG_STS;

typedef enum {
  HC_IDLE = 0,
  HC_XFRC,
  HC_HALTED,
  HC_NAK,
  HC_STALL,
  HC_XACTERR,  
  HC_BBLERR,   
  HC_DATATGLERR,  
}HC_STATUS;

typedef enum {
  URB_IDLE = 0,
  URB_DONE,
  URB_NOTREADY,
  URB_ERROR,
  URB_STALL
}URB_STATE;

typedef enum {
  CTRL_START = 0,
  CTRL_XFRC,
  CTRL_HALTED,
  CTRL_NAK,
  CTRL_STALL,
  CTRL_XACTERR,  
  CTRL_BBLERR,   
  CTRL_DATATGLERR,  
  CTRL_FAIL
}CTRL_STATUS;


typedef struct USB_OTG_hc
{
  uint8_t       dev_addr ;
  uint8_t       ep_num;
  uint8_t       ep_is_in;
  uint8_t       speed;
  uint8_t       do_ping;  
  uint8_t       ep_type;
  uint16_t      max_packet;
  uint8_t       data_pid;
  uint8_t       *xfer_buff;
  uint32_t      xfer_len;
  uint32_t      xfer_Count;  
  uint8_t       toggle_in;
  uint8_t       toggle_out;
  uint32_t       dma_addr;  
}
USB_OTG_HC , *PUSB_OTG_HC;


typedef struct USB_OTG_core_cfg
{
  uint32_t       host_channels;
  uint32_t       TotalFifoSize;
}
USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;


typedef struct _HCD
{
  __IO uint32_t        ConnSts;
  __IO uint32_t        ErrCnt;
  __IO uint32_t        XferCnt;
  __IO HC_STATUS       HC_Status;  
  __IO URB_STATE       URB_State;
  USB_OTG_HC hc        [USB_OTG_MAX_TX_FIFOS];
}
HCD_DEV , *USB_OTG_USBH_PDEV;


typedef struct USB_OTG_handle
{
  USB_OTG_CORE_CFGS    cfg;
  USB_OTG_CORE_REGS    regs;
  HCD_DEV              host;
}
USB_OTG_CORE_HANDLE , *PUSB_OTG_CORE_HANDLE;

/**
  * @}
  */ 


/** @defgroup USB_CORE_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_FunctionsPrototype
  * @{
  */ 

/*********************** Common APIs ********************************************/
USB_OTG_STS  USB_OTG_CoreInit        (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableGlobalInt (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev);
void*           USB_OTG_ReadPacket      (USB_OTG_CORE_HANDLE *pdev ,
    uint8_t *dest,
    uint16_t bytes);
USB_OTG_STS  USB_OTG_WritePacket     (USB_OTG_CORE_HANDLE *pdev ,
    uint8_t *src,
    uint8_t ch_ep_num,
    uint16_t bytes);
USB_OTG_STS  USB_OTG_FlushTxFifo     (USB_OTG_CORE_HANDLE *pdev , uint32_t num);
USB_OTG_STS  USB_OTG_FlushRxFifo     (USB_OTG_CORE_HANDLE *pdev);

uint32_t             USB_OTG_ReadCoreItr     (USB_OTG_CORE_HANDLE *pdev);
uint32_t             USB_OTG_ReadOtgItr      (USB_OTG_CORE_HANDLE *pdev);
uint8_t              USB_OTG_IsHostMode      (USB_OTG_CORE_HANDLE *pdev);

USB_OTG_STS  USB_OTG_SetHostMode  (USB_OTG_CORE_HANDLE *pdev);

/*********************** HOST APIs ********************************************/
USB_OTG_STS  USB_OTG_CoreInitHost    (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableHostInt   (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_HC_Init         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_Halt         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_StartXfer    (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);

uint32_t     USB_OTG_ResetPort       (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ReadHPRT0       (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_DriveVbus       (USB_OTG_CORE_HANDLE *pdev, uint8_t state);
void         USB_OTG_InitFSLSPClkSel (USB_OTG_CORE_HANDLE *pdev ,uint8_t freq);
uint8_t      USB_OTG_IsEvenFrame     (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_StopHostMode    (USB_OTG_CORE_HANDLE *pdev); 
uint32_t     USB_OTG_ReadHostAllChannels_intr (USB_OTG_CORE_HANDLE *pdev);


/**
  * @}
  */ 

#endif  /* __USB_CORE_H__ */


/**
  * @}
  */ 

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

