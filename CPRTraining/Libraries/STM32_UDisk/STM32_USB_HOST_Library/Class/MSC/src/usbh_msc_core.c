/**
  ******************************************************************************
  * @file    usbh_msc_core.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   This file implements the MSC class driver functions
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

/* Includes ------------------------------------------------------------------*/

#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbh_core.h"
#include "usbh_usr.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */
  
/** @defgroup USBH_MSC_CORE 
  * @brief    This file includes the mass storage related functions
  * @{
  */ 


/** @defgroup USBH_MSC_CORE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_MSC_CORE_Private_Defines
  * @{
  */ 
#define USBH_MSC_ERROR_RETRY_LIMIT 10
/**
  * @}
  */ 

/** @defgroup USBH_MSC_CORE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_MSC_CORE_Private_Variables
  * @{
  */ 

MSC_Machine_TypeDef         MSC_Machine;
USB_Setup_TypeDef           MSC_Setup;
uint8_t MSCErrorCount = 0;


/**
  * @}
  */ 


/** @defgroup USBH_MSC_CORE_Private_FunctionPrototypes
  * @{
  */ 

static USBH_Status USBH_MSC_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev , 
                                     USBH_DeviceProp_TypeDef *hdev);

static void USBH_MSC_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev , 
                                       USBH_DeviceProp_TypeDef *hdev);

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                            USBH_DeviceProp_TypeDef *hdev);

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                         USBH_DeviceProp_TypeDef *hdev);

USBH_Class_cb_TypeDef  MSC_cb = 
{
  USBH_MSC_InterfaceInit,
  USBH_MSC_InterfaceDeInit,
  USBH_MSC_ClassRequest,
  USBH_MSC_Handle,
};

void USBH_MSC_ErrorHandle(uint8_t status);

/**
  * @}
  */ 


/** @defgroup USBH_MSC_CORE_Exported_Variables
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBH_MSC_CORE_Private_Functions
  * @{
  */ 


/**
  * @brief  USBH_MSC_InterfaceInit 
  *         Interface initialization for MSC class.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */
static USBH_Status USBH_MSC_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, 
                                     USBH_DeviceProp_TypeDef *hdev)
{	 
  
  if((hdev->Itf_Desc.bInterfaceClass  == MSC_CLASS) && \
     (hdev->Itf_Desc.bInterfaceProtocol == MSC_PROTOCOL))
  {
    if(hdev->Ep_Desc[0].bEndpointAddress & 0x80)
    {
      MSC_Machine.MSBulkInEp = (hdev->Ep_Desc[0].bEndpointAddress);
      MSC_Machine.MSBulkInEpSize  = hdev->Ep_Desc[0].wMaxPacketSize;
    }
    else
    {
      MSC_Machine.MSBulkOutEp = (hdev->Ep_Desc[0].bEndpointAddress);
      MSC_Machine.MSBulkOutEpSize  = hdev->Ep_Desc[0].wMaxPacketSize;      
    }
    
    if(hdev->Ep_Desc[1].bEndpointAddress & 0x80)
    {
      MSC_Machine.MSBulkInEp = (hdev->Ep_Desc[1].bEndpointAddress);
      MSC_Machine.MSBulkInEpSize  = hdev->Ep_Desc[1].wMaxPacketSize;      
    }
    else
    {
      MSC_Machine.MSBulkOutEp = (hdev->Ep_Desc[1].bEndpointAddress);
      MSC_Machine.MSBulkOutEpSize  = hdev->Ep_Desc[1].wMaxPacketSize;      
    }
    
    MSC_Machine.hc_num_out = USBH_Alloc_Channel(&USB_OTG_FS_dev, 
                                                MSC_Machine.MSBulkOutEp);
    MSC_Machine.hc_num_in = USBH_Alloc_Channel(&USB_OTG_FS_dev,
                                                MSC_Machine.MSBulkInEp);  
    
    /* Open the new channels */
    USBH_Open_Channel  (pdev,
                        MSC_Machine.hc_num_out,
                        hdev->address,
                        hdev->speed,
                        EP_TYPE_BULK,
                        MSC_Machine.MSBulkOutEpSize);  
    
    USBH_Open_Channel  (pdev,
                        MSC_Machine.hc_num_in,
                        hdev->address,
                        hdev->speed,
                        EP_TYPE_BULK,
                        MSC_Machine.MSBulkInEpSize);    
    
  }
  
  else
  {
    USBH_Machine.usr_cb->USBH_USR_DeviceNotSupported(); 
  }
  
  return USBH_OK ;
 
}


/**
  * @brief  USBH_MSC_InterfaceDeInit 
  *         De-Initialize interface by freeing host channels allocated to interface
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval None
  */
void USBH_MSC_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev,
                                USBH_DeviceProp_TypeDef *hdev)
{	
  if ( MSC_Machine.hc_num_out)
  {
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), MSC_Machine.hc_num_out);
    USBH_Free_Channel  (pdev, MSC_Machine.hc_num_out);
    MSC_Machine.hc_num_out = 0;     /* Reset the Channel as Free */
  }
   
  if ( MSC_Machine.hc_num_in)
  {
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), MSC_Machine.hc_num_in);
    USBH_Free_Channel  (pdev, MSC_Machine.hc_num_in);
    MSC_Machine.hc_num_in = 0;     /* Reset the Channel as Free */
  } 
}

/**
  * @brief  USBH_MSC_ClassRequest 
  *         This function will only initialize the MSC state machine
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status : Status of class request handled.
  */

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                        USBH_DeviceProp_TypeDef *hdev)
{   
  
  USBH_Status status = USBH_OK ;
  USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_INIT_STATE;
  
  return status; 
}


/**
  * @brief  USBH_MSC_Handle 
  *         MSC state machine handler 
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval USBH_Status
  */

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                           USBH_DeviceProp_TypeDef   *hdev)
{
  
  USBH_Status status = USBH_BUSY;
  uint8_t mscStatus = USBH_MSC_BUSY;
  uint8_t appliStatus = 0;
  
  static uint8_t maxLunExceed = FALSE;
  
    
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {   
    switch(USBH_MSC_BOTXferParam.MSCState)
    {
    case USBH_MSC_BOT_INIT_STATE:
      USBH_MSC_Init();
      USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_RESET;  
      break;
      
    case USBH_MSC_BOT_RESET:   
      /* Issue BOT RESET request */
      status = USBH_MSC_BOTReset(pdev);
      if(status == USBH_OK )
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_GET_MAX_LUN;
      }
      
      if(status == USBH_NOT_SUPPORTED )
      {
       /* If the Command has failed, then we need to move to Next State, after
        STALL condition is cleared by Control-Transfer */
        USBH_MSC_BOTXferParam.MSCStateBkp = USBH_MSC_GET_MAX_LUN; 

        /* a Clear Feature should be issued here */
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_CTRL_ERROR_STATE;
      }  
      break;
      
    case USBH_MSC_GET_MAX_LUN:
      /* Issue GetMaxLUN request */
      status = USBH_MSC_GETMaxLUN(pdev);
      
      if(status == USBH_OK )
      {
        MSC_Machine.maxLun = *(MSC_Machine.buff) ;
        
        /* If device has more that one logical unit then it is not supported */
        if((MSC_Machine.maxLun > 0) && (maxLunExceed == FALSE))
        {
          maxLunExceed = TRUE;
          USBH_Machine.usr_cb->USBH_USR_DeviceNotSupported();
          
          break;
        }
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_TEST_UNIT_READY;
      }
      
      if(status == USBH_NOT_SUPPORTED )
      {
               /* If the Command has failed, then we need to move to Next State, after
        STALL condition is cleared by Control-Transfer */
        USBH_MSC_BOTXferParam.MSCStateBkp = USBH_MSC_TEST_UNIT_READY; 
        
        /* a Clear Feature should be issued here */
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_CTRL_ERROR_STATE;
      }    
      break;
      
    case USBH_MSC_CTRL_ERROR_STATE:
      /* Issue Clearfeature request */
      status = USBH_ClrFeature(pdev,
                               0x00,
                               USBH_Machine.Control.hc_num_out);
      if(status == USBH_OK )
      {
        /* If GetMaxLun Request not support, assume Single LUN configuration */
        MSC_Machine.maxLun = 0;  
        
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOTXferParam.MSCStateBkp;     
      }
      break;  
      
    case USBH_MSC_TEST_UNIT_READY:
      /* Issue SCSI command TestUnitReady */ 
      mscStatus = USBH_MSC_TestUnitReady();
      
      if(mscStatus == USBH_MSC_OK )
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_READ_CAPACITY10;
        MSCErrorCount = 0;
        status = USBH_OK;
      }
      else
      {
        USBH_MSC_ErrorHandle(mscStatus);
      } 
      break;
      
    case USBH_MSC_READ_CAPACITY10:
      /* Issue READ_CAPACITY10 SCSI command */
      mscStatus = USBH_MSC_ReadCapacity10();
      if(mscStatus == USBH_MSC_OK )
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_MODE_SENSE6;
        MSCErrorCount = 0;
        status = USBH_OK;
      }
      else
      {
        USBH_MSC_ErrorHandle(mscStatus);
      }
      break;

    case USBH_MSC_MODE_SENSE6:
      /* Issue ModeSense6 SCSI command for detecting if device is write-protected */
      mscStatus = USBH_MSC_ModeSense6();
      if(mscStatus == USBH_MSC_OK )
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_DEFAULT_APPLI_STATE;
        MSCErrorCount = 0;
        status = USBH_OK;
      }
      else
      {
        USBH_MSC_ErrorHandle(mscStatus);
      }
      break;
      
    case USBH_MSC_REQUEST_SENSE:
      /* Issue RequestSense SCSI command for retreiving error code */
      mscStatus = USBH_MSC_RequestSense();
      if(mscStatus == USBH_MSC_OK )
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOTXferParam.MSCStateBkp;
        status = USBH_OK;
      }
      else
      {
        USBH_MSC_ErrorHandle(mscStatus);
      }  
      break;
      
    case USBH_MSC_BOT_USB_TRANSFERS:
      /* Process the BOT state machine */
      USBH_MSC_HandleBOTXfer();
      break;
    
    case USBH_MSC_DEFAULT_APPLI_STATE:
      /* Process Application callback for MSC */
      appliStatus = USBH_USR_MSC_Application();						//调用USB设备初始化程序
      if(appliStatus == 0)
      {
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_DEFAULT_APPLI_STATE;
      }
      else if (appliStatus == 1) 
      {
        /* De-init requested from application layer */
        status =  USBH_APPLY_DEINIT;
      }
      break;
      
    case USBH_MSC_UNRECOVERED_STATE:
      
      status = USBH_UNRECOVERED_ERROR;
      
      break;
      
    default:
      break; 
      
    }
  }
   return status;
}



/**
  * @brief  USBH_MSC_BOTReset
  *         This request is used to reset the mass storage device and its 
  *         associated interface. This class-specific request shall ready the 
  *         device for the next CBW from the host.
  * @param  pdev: Selected device
  * @retval USBH_Status : Status of class request handled.
  */
USBH_Status USBH_MSC_BOTReset(USB_OTG_CORE_HANDLE *pdev)
{
  
  MSC_Setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | \
                              USB_REQ_RECIPIENT_INTERFACE;
  
  MSC_Setup.b.bRequest = USB_REQ_BOT_RESET;
  MSC_Setup.b.wValue.w = 0;
  MSC_Setup.b.wIndex.w = 0;
  MSC_Setup.b.wLength.w = 0;           
  
  return USBH_CtlReq(pdev, &MSC_Setup, 0 , 0 ); 
}


/**
  * @brief  USBH_MSC_Issue_BOTReset
  *         This function is responsible to issue a BOT Reset command. User
  *         can issue a BOT Reset command by calling this function.
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */

USBH_Status USBH_MSC_Issue_BOTReset(USB_OTG_CORE_HANDLE *pdev)
{
  USBH_Status status = USBH_BUSY;
  USBH_MSC_BOTXferParam.CmdStateMachine = CMD_SEND_STATE;
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    do
    {
      status = USBH_MSC_BOTReset(pdev);
    }
    while((status == USBH_BUSY ) && (HCD_IsDeviceConnected(&USB_OTG_FS_dev)));
  } 
  return(status); 
}


/**
  * @brief  USBH_MSC_GETMaxLUN
  *         This request is used to reset the mass storage device and its 
  *         associated interface. This class-specific request shall ready the 
  *         device for the next CBW from the host.
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */
USBH_Status USBH_MSC_GETMaxLUN(USB_OTG_CORE_HANDLE *pdev)
{
  MSC_Setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | \
                              USB_REQ_RECIPIENT_INTERFACE;
  
  MSC_Setup.b.bRequest = USB_REQ_GET_MAX_LUN;
  MSC_Setup.b.wValue.w = 0;
  MSC_Setup.b.wIndex.w = 0;
  MSC_Setup.b.wLength.w = 1;           
  
  return USBH_CtlReq(pdev, &MSC_Setup, MSC_Machine.buff , 1 ); 
}


/**
  * @brief  USBH_MSC_Issue_GETMaxLUN
  *         This function is responsible to issue a BOT Reset command. User
  *         can issue a BOT Reset command by calling this function.
  * @param  pdev: Selected device
  * @retval USBH_Status : USB ctl xfer status
  */

USBH_Status USBH_MSC_Issue_GETMaxLUN(USB_OTG_CORE_HANDLE *pdev)
{
  USBH_Status status = USBH_BUSY;
  USBH_MSC_BOTXferParam.CmdStateMachine = CMD_SEND_STATE;
  if(HCD_IsDeviceConnected(&USB_OTG_FS_dev))
  {  
    do
    {
      status = USBH_MSC_GETMaxLUN(pdev);
    }
    while((status == USBH_BUSY ) && HCD_IsDeviceConnected(&USB_OTG_FS_dev));
  } 
  return(status);
}


/**
  * @brief  USBH_MSC_ErrorHandle 
  *         The function is for handling errors occuring during the MSC
  *         state machine   
  * @param  status
  * @retval None
  */

void USBH_MSC_ErrorHandle(uint8_t status)
{  
    if(status == USBH_MSC_FAIL)
    { 
      MSCErrorCount++;
      if(MSCErrorCount < USBH_MSC_ERROR_RETRY_LIMIT)
      { /* Try MSC level error recovery, Issue the request Sense to get 
        Drive error reason  */
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_REQUEST_SENSE;
        USBH_MSC_BOTXferParam.CmdStateMachine = CMD_SEND_STATE;
      }
      else
      {
        /* Error trials exceeded the limit, go to unrecovered state */
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_UNRECOVERED_STATE;
      }
    } 
    else if(status == USBH_MSC_PHASE_ERROR)
    {
      /* Phase error, Go to Unrecoovered state */
      USBH_MSC_BOTXferParam.MSCState = USBH_MSC_UNRECOVERED_STATE;
    }
    else if(status == USBH_MSC_BUSY)
    {
      /*No change in state*/
    }
}




/**
* @brief  USBH_ParseClassDesc 
*         This function Parse the class descriptor
* @param  itf_desc: Interface Descriptor address
* @param  buf: Buffer where the class descriptor is available
* @retval Class Desc len
*/
uint8_t  USBH_ParseClassDesc (USBH_InterfaceDesc_TypeDef* itf_desc,uint8_t *buf)
{
  /*Since there is no interface class descriptor in mass-storage*/
  /* hence 0 length is returned*/
  return 0;
  
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
