/**
******************************************************************************
* @file    usbh_hid_core.c
* @author  MCD Application Team
* @version V1.0.0
* @date    11/29/2010
* @brief   This file is the HID Layer Handlers for USB Host HID class.
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
#include "usbh_hid_core.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_HID_CLASS
* @{
*/

/** @defgroup USBH_HID_CORE 
* @brief    This file includes HID Layer Handlers for USB Host HID class.
* @{
*/ 

/** @defgroup USBH_HID_CORE_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_CORE_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_CORE_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_CORE_Private_Variables
* @{
*/
HID_Machine_TypeDef        HID_Machine;
HID_Report_TypeDef         HID_Report;
USB_Setup_TypeDef          HID_Setup;
USBH_HIDDesc_TypeDef       HID_Desc; 

__IO uint8_t flag = 0;
/**
* @}
*/ 


/** @defgroup USBH_HID_CORE_Private_FunctionPrototypes
* @{
*/ 

static USBH_Status USBH_HID_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev , 
                                            USBH_DeviceProp_TypeDef *hdev);

static void USBH_HID_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev , 
                                       USBH_DeviceProp_TypeDef *hdev);

static USBH_Status USBH_HID_Handle(USB_OTG_CORE_HANDLE *pdev , 
                                   USBH_DeviceProp_TypeDef *hdev);

static USBH_Status USBH_HID_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                         USBH_DeviceProp_TypeDef *hdev);

static USBH_Status USBH_Get_HID_ReportDescriptor (USB_OTG_CORE_HANDLE *pdev, 
                                                  uint16_t length);

static USBH_Status USBH_Set_Idle (USB_OTG_CORE_HANDLE *pdev, 
                                  uint8_t duration,
                                  uint8_t reportId);

static USBH_Status USBH_Set_Protocol (USB_OTG_CORE_HANDLE *pdev, 
                                      uint8_t protocol);


USBH_Class_cb_TypeDef  HID_cb = 
{
  USBH_HID_InterfaceInit,
  USBH_HID_InterfaceDeInit,
  USBH_HID_ClassRequest,
  USBH_HID_Handle
};
/**
* @}
*/ 


/** @defgroup USBH_HID_CORE_Private_Functions
* @{
*/ 

/**
* @brief  USBH_HID_InterfaceInit 
*         The function init the HID class.
* @param  pdev: Selected device
* @param  hdev: Selected device property
* @retval  USBH_Status :Response for USB HID driver intialization
*/
static USBH_Status USBH_HID_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, 
                                           USBH_DeviceProp_TypeDef *hdev)
{	
  uint8_t maxEP;
  uint8_t num =0;
  USBH_Status status = USBH_BUSY ;
  HID_Machine.state = HID_ERROR;
  
  
  if(hdev->Itf_Desc.bInterfaceSubClass  == HID_BOOT_CODE)
  {
    /*Decode Bootclass Protocl: Mouse or Keyboard*/
    if(hdev->Itf_Desc.bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
    {
      HID_Machine.cb = &HID_KEYBRD_cb;
    }
    else if(hdev->Itf_Desc.bInterfaceProtocol  == HID_MOUSE_BOOT_CODE)		  
    {
      HID_Machine.cb = &HID_MOUSE_cb;
    }
    
    HID_Machine.state     = HID_IDLE;
    HID_Machine.ctl_state = HID_REQ_IDLE; 
    HID_Machine.ep_addr   = hdev->Ep_Desc[0].bEndpointAddress;
    HID_Machine.length    = hdev->Ep_Desc[0].wMaxPacketSize;
    HID_Machine.poll      = hdev->Ep_Desc[0].bInterval ;
    
    /* Check fo available number of endpoints */
    /* Find the number of EPs in the Interface Descriptor */      
    /* Choose the lower number in order not to overrun the buffer allocated */
    maxEP = ( (hdev->Itf_Desc.bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ? 
             hdev->Itf_Desc.bNumEndpoints :
                 USBH_MAX_NUM_ENDPOINTS); 
    
    
    /* Decode endpoint IN and OUT address from interface descriptor */
    for (num=0; num < maxEP; num++)
    {
      if(hdev->Ep_Desc[num].bEndpointAddress & 0x80)
      {
        HID_Machine.HIDIntInEp = (hdev->Ep_Desc[num].bEndpointAddress);
        HID_Machine.hc_num_in  =\
               USBH_Alloc_Channel(&USB_OTG_FS_dev, 
                                  hdev->Ep_Desc[num].bEndpointAddress);
        
        /* Open channel for IN endpoint */
        USBH_Open_Channel  (pdev,
                            HID_Machine.hc_num_in,
                            hdev->address,
                            hdev->speed,
                            EP_TYPE_INTR,
                            HID_Machine.length); 
      }
      else
      {
        HID_Machine.HIDIntOutEp = (hdev->Ep_Desc[num].bEndpointAddress);
        HID_Machine.hc_num_out  =\
                USBH_Alloc_Channel(&USB_OTG_FS_dev, 
                                   hdev->Ep_Desc[num].bEndpointAddress);
        
        /* Open channel for OUT endpoint */
        USBH_Open_Channel  (pdev,
                            HID_Machine.hc_num_out,
                            hdev->address,
                            hdev->speed,
                            EP_TYPE_INTR,
                            HID_Machine.length); 
      }
      
    }   
    
     flag =0;
     status = USBH_OK; 
  }
  else
  {
    USBH_Machine.usr_cb->USBH_USR_DeviceNotSupported();    
  }
  
  return status;
  
}



/**
* @brief  USBH_HID_InterfaceDeInit 
*         The function DeInit the Host Channels used for the HID class.
* @param  pdev: Selected device
* @param  hdev: Selected device property
* @retval None
*/
void USBH_HID_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev,
                               USBH_DeviceProp_TypeDef *hdev)
{	
  if(HID_Machine.hc_num_in != 0x00)
  {   
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), HID_Machine.hc_num_in);
    USBH_Free_Channel  (pdev, HID_Machine.hc_num_in);
    HID_Machine.hc_num_in = 0;     /* Reset the Channel as Free */  
  }
  
  if(HID_Machine.hc_num_out != 0x00)
  {   
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), HID_Machine.hc_num_out);
    USBH_Free_Channel  (pdev, HID_Machine.hc_num_out);
    HID_Machine.hc_num_out = 0;     /* Reset the Channel as Free */  
  }
 
  flag = 0;
}

/**
* @brief  USBH_HID_ClassRequest 
*         The function is responsible for handling HID Class requests
*         for HID class.
* @param  pdev: Selected device
* @param  hdev: Selected device property
* @retval  USBH_Status :Response for USB Set Protocol request
*/
static USBH_Status USBH_HID_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                         USBH_DeviceProp_TypeDef *hdev)
{   
  
  USBH_Status status         = USBH_BUSY;
  USBH_Status classReqStatus = USBH_BUSY;
  
  
  /* Switch HID state machine */
  switch (HID_Machine.ctl_state)
  {
  case HID_IDLE:    
  case HID_REQ_GET_REPORT_DESC:
    
    
    /* Get Report Desc */ 
    if (USBH_Get_HID_ReportDescriptor(pdev ,HID_Desc.wItemLength) == USBH_OK)
    {
      HID_Machine.ctl_state = HID_REQ_SET_IDLE;
    }
    
    break;
    
  case HID_REQ_SET_IDLE:
    
    classReqStatus = USBH_Set_Idle (pdev, 0, 0);
    
    /* set Idle */
    if (classReqStatus == USBH_OK)
    {
      HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;  
    }
    else if(classReqStatus == USBH_NOT_SUPPORTED) 
    {
      HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;        
    } 
    break; 
    
  case HID_REQ_SET_PROTOCOL:
    /* set protocol */
    if (USBH_Set_Protocol (pdev , 0) == USBH_OK)
    {
      HID_Machine.ctl_state = HID_REQ_IDLE;
      
      /* all requests performed*/
      status = USBH_OK; 
    } 
    break;
    
  default:
    break;
  }
  
  return status; 
}


/**
* @brief  USBH_HID_Handle 
*         The function is for managing state machine for HID data transfers 
* @param  pdev: Selected device
* @param  hdev: Selected device property
* @retval USBH_Status
*/
static USBH_Status USBH_HID_Handle(USB_OTG_CORE_HANDLE *pdev , 
                                   USBH_DeviceProp_TypeDef   *hdev)
{
  
  
  USBH_Status status = USBH_OK;
  
  switch (HID_Machine.state)
  {
    
  case HID_IDLE:
    HID_Machine.cb->Init();
    HID_Machine.state = HID_GET_DATA;
    break;  
    
  case HID_GET_DATA:
    
    /* Sync with start of Even Frame */
    while(USB_OTG_IsEvenFrame(pdev) == FALSE);
    
    USBH_InterruptReceiveData(pdev, 
                              HID_Machine.buff,
                              HID_Machine.length,
                              HID_Machine.hc_num_in);
    flag = 1;
    
    HID_Machine.state = HID_POLL;
    HID_Machine.timer = HCD_GetCurrentFrame(pdev);
    break;
    
  case HID_POLL:
    if(( HCD_GetCurrentFrame(pdev) - HID_Machine.timer) >= HID_Machine.poll)
    {
      HID_Machine.state = HID_GET_DATA;
    }
    else if(HCD_GetURB_State(pdev) == URB_DONE)
    {
      if(flag == 1) /* handle data once */
      {
        flag = 0;
        HID_Machine.cb->Decode(HID_Machine.buff);
      }
    }
    else if(HCD_GetURB_State(pdev) == URB_STALL) /* IN Endpoint Stalled */
    {
      
      /* Issue Clear Feature on interrupt IN endpoint */ 
      if( (USBH_ClrFeature(pdev, 
                           HID_Machine.ep_addr,
                           HID_Machine.hc_num_in)) == USBH_OK)
      {
        /* Change state to issue next IN token */
        HID_Machine.state = HID_GET_DATA;
        
      }
      
    }      
    break;
    
  default:
    break;
  }
  return status;
}


/**
* @brief  USBH_Get_HID_ReportDescriptor
*         Issue report Descriptor command to the device. Once the response 
*         received, parse the report descriptor and update the status.
* @param  pdev   : Selected device
* @param  Length : HID Report Descriptor Length
* @retval USBH_Status : Response for USB HID Get Report Descriptor Request
*/
static USBH_Status USBH_Get_HID_ReportDescriptor (USB_OTG_CORE_HANDLE *pdev,\
  uint16_t length)
{
  
  USBH_Status status;
  
  status = USBH_GetDescriptor(pdev, 
                              USB_REQ_RECIPIENT_INTERFACE
                                | USB_REQ_TYPE_STANDARD,                                  
                                USB_DESC_HID_REPORT, 
                                Rx_Buffer,
                                length);
  
  /* HID report descriptor is available in Rx_Buffer.
  In case of USB Boot Mode devices for In report handling ,
  HID report descriptor parsing is not required.
  In case, for supporting Non-Boot Protocol devices and output reports,
  user may parse the report descriptor*/
  
  
  return status;
}

/**
* @brief  USBH_Set_Idle
*         Set Idle State. 
* @param  pdev: Selected device
* @param  duration: Duration for HID Idle request
* @param  reportID : Targetted report ID for Set Idle request
* @retval USBH_Status : Response for USB Set Idle request
*/
static USBH_Status USBH_Set_Idle (USB_OTG_CORE_HANDLE *pdev, 
                                  uint8_t duration,
                                  uint8_t reportId)
{
  
  HID_Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE |\
    USB_REQ_TYPE_CLASS;
  
  
  HID_Setup.b.bRequest = USB_HID_SET_IDLE;
  HID_Setup.b.wValue.w = (duration << 8 ) | reportId;
  
  HID_Setup.b.wIndex.w = 0;
  HID_Setup.b.wLength.w = 0;
  
  return USBH_CtlReq(pdev, &HID_Setup, 0 , 0 );
}


/**
* @brief  USBH_Set_Report
*         Issues Set Report 
* @param  pdev: Selected device
* @param  reportType  : Report type to be sent
* @param  reportID    : Targetted report ID for Set Report request
* @param  reportLen   : Length of data report to be send
* @param  reportBuff  : Report Buffer
* @retval USBH_Status : Response for USB Set Idle request
*/
USBH_Status USBH_Set_Report (USB_OTG_CORE_HANDLE *pdev, 
                                    uint8_t reportType,
                                    uint8_t reportId,
                                    uint8_t reportLen,
                                    uint8_t* reportBuff)
{
  
  HID_Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE |\
    USB_REQ_TYPE_CLASS;
  
  
  HID_Setup.b.bRequest = USB_HID_SET_REPORT;
  HID_Setup.b.wValue.w = (reportType << 8 ) | reportId;
  
  HID_Setup.b.wIndex.w = 0;
  HID_Setup.b.wLength.w = reportLen;
  
  return USBH_CtlReq(pdev, &HID_Setup, reportBuff , reportLen );
}


/**
* @brief  USBH_Set_Protocol
*         Set protocol State.
* @param  pdev: Selected device
* @param  protocol : Set Protocol for HID : boot/report protocol
* @retval USBH_Status : Response for USB Set Protocol request
*/
static USBH_Status USBH_Set_Protocol(USB_OTG_CORE_HANDLE *pdev,uint8_t protocol)
{
  
  
  HID_Setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE |\
    USB_REQ_TYPE_CLASS;
  
  
  HID_Setup.b.bRequest = USB_HID_SET_PROTOCOL;
  
  if(protocol != 0)
  {
    /* Boot Protocol */
    HID_Setup.b.wValue.w = 0;
  }
  else
  {
    /*Report Protocol*/
    HID_Setup.b.wValue.w = 1;
  }
  
  HID_Setup.b.wIndex.w = 0;
  HID_Setup.b.wLength.w = 0;
  
  return USBH_CtlReq(pdev, &HID_Setup, 0 , 0 );
  
}

/**
* @brief  USBH_ParseHIDDesc 
*         This function Parse the HID descriptor
* @param  buf: Buffer where the source descriptor is available
* @retval None
*/
void  USBH_ParseHIDDesc (uint8_t *buf)
{
  
  HID_Desc.bLength                  = *(uint8_t  *) (buf + 0);
  HID_Desc.bDescriptorType          = *(uint8_t  *) (buf + 1);
  HID_Desc.bcdHID                   =  LE16  (buf + 2);
  HID_Desc.bCountryCode             = *(uint8_t  *) (buf + 4);
  HID_Desc.bNumDescriptors          = *(uint8_t  *) (buf + 5);
  HID_Desc.bReportDescriptorType    = *(uint8_t  *) (buf + 6);
  HID_Desc.wItemLength              =  LE16  (buf + 7);
  
} 

/**
* @brief  USBH_ParseClassDesc 
*         This function Parse the HID class descriptor
* @param  itf_desc: Interface Descriptor address
* @param  buf: Buffer where the HID descriptor is available
* @retval Class Desc len
*/
uint8_t  USBH_ParseClassDesc (USBH_InterfaceDesc_TypeDef* itf_desc,uint8_t *buf)
{
  /*Decode HID Descriptor*/
  if(itf_desc->bInterfaceClass  == USBH_HID_CLASS)
  {
    USBH_ParseHIDDesc (buf);   
    return HID_Desc.bLength;
  }
  else
  {  
    return 0;
  }
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
