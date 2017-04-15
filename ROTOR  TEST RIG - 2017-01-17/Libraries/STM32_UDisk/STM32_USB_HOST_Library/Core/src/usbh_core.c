/**
  ******************************************************************************
  * @file    usbh_core.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   This file implements the functions for the core state machine process
  *          the enumeration and the control transfer process
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

#include "usbh_ioreq.h"
#include "usb_bsp.h"
#include "usbh_usr.h"
#include "usbh_hcs.h"
#include "usbh_stdreq.h"
#include "usbh_core.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_CORE 
  * @brief TThis file handles the basic enumaration when a device is connected 
  *          to the host.
  * @{
  */ 

/** @defgroup USBH_CORE_Private_TypesDefinitions
  * @{
  */ 
void USBH_Disconnect (USB_OTG_CORE_HANDLE *pdev); 
void USBH_Connect (USB_OTG_CORE_HANDLE *pdev); 

USBH_DeviceConnStatus_cb_TypeDef  USBH_DeviceConnStatus_cb = 
{
  USBH_Disconnect,
  USBH_Connect,
  0,
  0,
  0,
  0
};
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Private_Variables
  * @{
  */ 
USB_OTG_CORE_HANDLE           USB_OTG_FS_dev;
USBH_Machine_TypeDef          USBH_Machine;
USBH_DeviceProp_TypeDef       USBH_Device;
 
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Private_FunctionPrototypes
  * @{
  */
static USBH_Status USBH_HandleEnum (USB_OTG_CORE_HANDLE *pdev); 
static USBH_Status USBH_HandleControl(USB_OTG_CORE_HANDLE *pdev); 
static USBH_Status USBH_DeInit(void);
/**
  * @}
  */ 


/** @defgroup USBH_CORE_Private_Functions
  * @{
  */ 


/**
  * @brief  USBH_Connect
  *         USB Connect callback function from the Interrupt. 
  * @param  selected device
  * @retval none
  */
void USBH_Connect (USB_OTG_CORE_HANDLE *pdev)
{
  USBH_Machine.conn_cb->ConnStatus = 1;
  USBH_Machine.conn_cb->ConnHandled = 0; 
}

/**
  * @brief  USBH_Disconnect
  *         USB Disconnect callback function from the Interrupt. 
  * @param  selected device
  * @retval none
  */

void USBH_Disconnect (USB_OTG_CORE_HANDLE *pdev)
{
  /* Make device Not connected flag true */
  USBH_Machine.conn_cb->DisconnStatus = 1; 
  USBH_Machine.conn_cb->DisconnHandled = 0;
}

/**
  * @brief  USBH_Init
  *         Host hardware and stack initializations 
  * @param  class_cb: Class callback structure address
  * @param  usr_cb: User callback structure address
  * @retval None
  */
void USBH_Init(USB_OTG_CORE_HANDLE *pdev,
               USBH_Class_cb_TypeDef *class_cb, 
               USBH_Usr_cb_TypeDef *usr_cb)
{
     
  /* Hardware Init */
  USB_OTG_BSP_Init();  
  
  /* Intialize Timer for delay function */
  USB_OTG_BSP_TimeInit();    
  
  /* configure GPIO pin used for switching VBUS power */
  USB_OTG_BSP_ConfigVBUS();  
  
  /* Host de-initializations */
  USBH_DeInit();
  
  /*Register class and user callbacks */
  USBH_Machine.class_cb = class_cb;
  USBH_Machine.usr_cb = usr_cb;  
  USBH_Machine.conn_cb = &USBH_DeviceConnStatus_cb;
  USBH_Machine.Control.hc_num_out = USBH_Alloc_Channel(&USB_OTG_FS_dev, 0x00);
  USBH_Machine.Control.hc_num_in = USBH_Alloc_Channel(&USB_OTG_FS_dev, 0x80);  
  
  USBH_Machine.conn_cb->ConnStatus = 0;   
  USBH_Machine.conn_cb->DisconnStatus = 0; 
  
    
  /* Start the USB OTG FS core */     
   HCD_Init(pdev);
   
  /* Upon Init call usr call back */
  USBH_Machine.usr_cb->Init();
  
  /* Enable Interrupts */
  USB_OTG_BSP_EnableInterrupt();
}

/**
  * @brief  USBH_DeInit 
  *         Re-Initialize Host
  * @param  None 
  * @retval status: USBH_Status
  */
static USBH_Status USBH_DeInit(void)
{
  /* Software Init */
  
  USBH_Machine.gState = HOST_IDLE;
  USBH_Machine.gStateBkp = HOST_IDLE; 
  USBH_Machine.EnumState = ENUM_IDLE;
  USBH_Machine.RequestState = CMD_SEND;  
  
  USBH_Machine.Control.state = CTRL_SETUP;
  USBH_Machine.Control.ep0size = USB_OTG_MAX_EP0_SIZE;  
  
  USBH_Device.address = USBH_DEVICE_ADDRESS_DEFAULT;
  USBH_Device.speed = HPRT0_PRTSPD_FULL_SPEED;
  
  return USBH_OK;
}

/**
* @brief  USBH_Process
*         USB Host core main state machine process
* @param  None 
* @retval None
*/
void USBH_Process(void)
{
  volatile USBH_Status status = USBH_FAIL;
    
  switch (USBH_Machine.gState)
  {
  case HOST_ISSUE_CORE_RESET :
     
    if ( HCD_ResetPort(&USB_OTG_FS_dev) == 0)
    {
      USBH_Machine.gState = HOST_IDLE;
    }
    break;
    
  case HOST_IDLE :
    
    if (HCD_IsDeviceConnected(&USB_OTG_FS_dev))  
    {
      /* Wait for USB Connect Interrupt void USBH_ISR_Connected(void) */     
      USBH_Machine.gState = HOST_DEV_ATTACHED;
    }
    break;
   
  case HOST_DEV_ATTACHED :
    
    USBH_Machine.usr_cb->DeviceAttached();
    USBH_Machine.conn_cb->DisconnStatus = 0; 
    USBH_Machine.conn_cb->ConnHandled = 1;  
    
    /* Reset USB Device */
    if ( HCD_ResetPort(&USB_OTG_FS_dev) == 0)
    {
      USBH_Machine.usr_cb->ResetDevice();
      /*  Wait for USB USBH_ISR_PrtEnDisableChange()  
      Host is Now ready to start the Enumeration 
      */
      
      USBH_Device.speed = HCD_GetCurrentSpeed(&USB_OTG_FS_dev);
      
      if ((USBH_Device.speed == HPRT0_PRTSPD_FULL_SPEED)||
          (USBH_Device.speed == HPRT0_PRTSPD_LOW_SPEED))
    {
      USBH_Machine.gState = HOST_ENUMERATION;
      USBH_Machine.usr_cb->DeviceSpeedDetected(USBH_Device.speed);
        
      /* Open Control pipes */
      USBH_Open_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_in,
                           USBH_Device.address,
                           USBH_Device.speed,
                           EP_TYPE_CTRL,
                           USBH_Machine.Control.ep0size); 
      
      /* Open Control pipes */
      USBH_Open_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_out,
                           USBH_Device.address,
                           USBH_Device.speed,
                           EP_TYPE_CTRL,
                           USBH_Machine.Control.ep0size);          
      }
      else
      {
        /* Should not be any value other than Full_Speed or Low_Speed */
        USBH_ErrorHandle(USBH_ERROR_SPEED_UNKNOWN);      
      }
   }
    break;
    
  case HOST_ENUMERATION:     
    /* Check for enumeration status */  
    if ( USBH_HandleEnum(&USB_OTG_FS_dev) == USBH_OK)
    { 
      /* The function shall return USBH_OK when full enumeration is complete */
          
      USBH_Machine.gState  = HOST_USR_INPUT;    
    }
    break;
    
  case HOST_USR_INPUT:    
    /*The function should return user response true to move to class state */
    if ( USBH_Machine.usr_cb->UserInput() == USBH_USR_RESP_OK)
    {
      if((USBH_Machine.class_cb->Init(&USB_OTG_FS_dev , &USBH_Device))\
        == USBH_OK)
      {
        USBH_Machine.gState  = HOST_CLASS_REQUEST;     
      }     
    }   
    break;
    
  case HOST_CLASS_REQUEST:  
    /* process class standard contol requests state machine */ 
    status = USBH_Machine.class_cb->Requests(&USB_OTG_FS_dev , &USBH_Device);
    
     if(status == USBH_OK)
     {
       USBH_Machine.gState  = HOST_CLASS;
     }  
     
     else
     {
       USBH_ErrorHandle(status);
     }
 
    
    break;    
  case HOST_CLASS:   
    /* process class state machine */
    status = USBH_Machine.class_cb->Machine(&USB_OTG_FS_dev , &USBH_Device);
    USBH_ErrorHandle(status);
    break;       
    
  case HOST_CTRL_XFER:
    /* process control transfer state machine */
    USBH_HandleControl(&USB_OTG_FS_dev);    
    break;
    
  case HOST_SUSPENDED:
    break;
  
  case HOST_ERROR_STATE:
    /* Re-Initilaize Host for new Enumeration */
    USBH_DeInit();
    USBH_USR_DeInit();
    USBH_Machine.class_cb->DeInit(&USB_OTG_FS_dev , &USBH_Device);
    break;
    
  default :
    break;
  }
  
  /* check device disconnection event */
   if (!(HCD_IsDeviceConnected(&USB_OTG_FS_dev)) && 
       (USBH_Machine.conn_cb->DisconnHandled == 0))
  { 
    /* Manage User disconnect operations*/
    USBH_Machine.usr_cb->DeviceDisconnected();
    
    USBH_Machine.conn_cb->DisconnHandled = 1; /* Handle to avoid the Re-entry*/
  
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), USBH_Machine.Control.hc_num_in);
    USB_OTG_HC_Halt((&USB_OTG_FS_dev), USBH_Machine.Control.hc_num_out);
    
    /* Re-Initilaize Host for new Enumeration */
    USBH_DeInit();
    USBH_USR_DeInit();
    USBH_Machine.class_cb->DeInit(&USB_OTG_FS_dev , &USBH_Device);
    USBH_Machine.conn_cb->ConnStatus = 0;
    USBH_Machine.conn_cb->ConnHandled = 0;
  }   
}


/**
  * @brief  USBH_ErrorHandle 
  *         This function handles the Error on Host side.
  * @param  errType : Type of Error or Busy/OK state
  * @retval None
  */
void USBH_ErrorHandle(USBH_Status errType)
{
  /* Error unrecovered or not supported device speed */
  if ( (errType == USBH_ERROR_SPEED_UNKNOWN) ||
       (errType == USBH_UNRECOVERED_ERROR) )
  {
    USBH_Machine.usr_cb->UnrecoveredError(); 
    USBH_Machine.gState = HOST_ERROR_STATE;   
  }  
  /* USB host restart requested from application layer */
  else if(errType == USBH_APPLY_DEINIT)
  {
    USBH_Machine.gState = HOST_ERROR_STATE;  
    /* user callback for initalization */
    USBH_Machine.usr_cb->Init();
  } 
}


/**
  * @brief  USBH_HandleEnum 
  *         This function includes the complete enumeration process
  * @param  pdev: Selected device
  * @retval USBH_Status
  */
static USBH_Status USBH_HandleEnum(USB_OTG_CORE_HANDLE *pdev)
{
  USBH_Status Status = USBH_BUSY;  
  uint8_t Local_Buffer[64];
  
  switch (USBH_Machine.EnumState)
  {
  case ENUM_IDLE:  
    /* Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize */
    if ( USBH_Get_DevDesc(pdev , &USBH_Device.Dev_Desc, 8) == USBH_OK)
    {
      USBH_Machine.Control.ep0size = USBH_Device.Dev_Desc.bMaxPacketSize;
      
      /* Issue Reset  */
      HCD_ResetPort(&USB_OTG_FS_dev);
      USBH_Machine.EnumState = ENUM_GET_FULL_DEV_DESC;
      
      /* modify control channels configuration for MaxPacket size */
      USBH_Modify_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_out,
                           0,
                           0,
                           0,
                           USBH_Machine.Control.ep0size);
      
      USBH_Modify_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_in,
                           0,
                           0,
                           0,
                           USBH_Machine.Control.ep0size);      
    }
    break;
    
  case ENUM_GET_FULL_DEV_DESC:  
    /* Get FULL Device Desc  */
    if ( USBH_Get_DevDesc(pdev, &USBH_Device.Dev_Desc, USB_DEVICE_DESC_SIZE)\
      == USBH_OK)
    {
      /* user callback for device descriptor available */
      USBH_Machine.usr_cb->DeviceDescAvailable(&USBH_Device.Dev_Desc);      
      USBH_Machine.EnumState = ENUM_SET_ADDR;
    }
    break;
   
  case ENUM_SET_ADDR: 
    /* set address */
    if ( USBH_SetAddress(pdev, USBH_DEVICE_ADDRESS) == USBH_OK)
    {
      USBH_Device.address = USBH_DEVICE_ADDRESS;
      
      /* user callback for device address assigned */
      USBH_Machine.usr_cb->DeviceAddressAssigned();
      USBH_Machine.EnumState = ENUM_GET_CFG_DESC;
      
      /* modify control channels to update device address */
      USBH_Modify_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_in,
                           USBH_Device.address,
                           0,
                           0,
                           0);
      
      USBH_Modify_Channel (&USB_OTG_FS_dev,
                           USBH_Machine.Control.hc_num_out,
                           USBH_Device.address,
                           0,
                           0,
                           0);         
    }
    break;
    
  case ENUM_GET_CFG_DESC:  
    /* get standard configuration descriptor */
    if ( USBH_Get_CfgDesc(pdev, 
                          &USBH_Device.Cfg_Desc,
                          USBH_NULL,
                          USBH_NULL,
                          USB_CONFIGURATION_DESC_SIZE) == USBH_OK)
    {
      USBH_Machine.EnumState = ENUM_GET_FULL_CFG_DESC;
    }
    break;
    
  case ENUM_GET_FULL_CFG_DESC:  
    /* get FULL config descriptor (config, interface, endpoints) */
    if (USBH_Get_CfgDesc(pdev, 
                         &USBH_Device.Cfg_Desc,
                         &USBH_Device.Itf_Desc,
                         USBH_Device.Ep_Desc,
                         USBH_Device.Cfg_Desc.wTotalLength) == USBH_OK)
    {
      /* User callback for configuration descriptors available */
      USBH_Machine.usr_cb->ConfigurationDescAvailable(&USBH_Device.Cfg_Desc,
                                                      &USBH_Device.Itf_Desc,
                                                      &USBH_Device.Ep_Desc[0]);
      
      USBH_Machine.EnumState = ENUM_GET_MFC_STRING_DESC;
    }
    break;
    
  case ENUM_GET_MFC_STRING_DESC:  
    if (USBH_Device.Dev_Desc.iManufacturer != 0)
    { /* Check that Manufacturer String is available */
      
      if ( USBH_Get_StringDesc(pdev,\
        USBH_Device.Dev_Desc.iManufacturer, Local_Buffer , 0xff) == USBH_OK)
      {
        /* User callback for Manufacturing string */
        USBH_Machine.usr_cb->ManufacturerString(Local_Buffer);
        USBH_Machine.EnumState = ENUM_GET_PRODUCT_STRING_DESC;
      }
    }
    else
    {
      USBH_Machine.usr_cb->ManufacturerString("N/A");      
      USBH_Machine.EnumState = ENUM_GET_PRODUCT_STRING_DESC;
    }
    break;
    
  case ENUM_GET_PRODUCT_STRING_DESC:   
    if (USBH_Device.Dev_Desc.iProduct != 0)
    { /* Check that Product string is available */
      if ( USBH_Get_StringDesc(pdev,\
        USBH_Device.Dev_Desc.iProduct, Local_Buffer, 0xff) == USBH_OK)
      {
        /* User callback for Product string */
        USBH_Machine.usr_cb->ProductString(Local_Buffer);
        USBH_Machine.EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
      }
    }
    else
    {
      USBH_Machine.usr_cb->ProductString("N/A");
      USBH_Machine.EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
    } 
    break;
    
  case ENUM_GET_SERIALNUM_STRING_DESC:   
    if (USBH_Device.Dev_Desc.iSerialNumber != 0)
    { /* Check that Serial number string is available */    
      if ( USBH_Get_StringDesc(pdev,\
        USBH_Device.Dev_Desc.iSerialNumber, Local_Buffer, 0xff) == USBH_OK)
      {
        /* User callback for Serial number string */
        USBH_Machine.usr_cb->SerialNumString(Local_Buffer);
        USBH_Machine.EnumState = ENUM_SET_CONFIGURATION;
      }
    }
    else
    {
      USBH_Machine.usr_cb->SerialNumString("N/A");      
      USBH_Machine.EnumState = ENUM_SET_CONFIGURATION;
    }  
    break;
      
  case ENUM_SET_CONFIGURATION:
    /* set configuration  (default config) */
    if (USBH_SetCfg(pdev, USBH_Device.Cfg_Desc.bConfigurationValue) == USBH_OK)
    {
      USBH_Machine.EnumState = ENUM_DEV_CONFIGURED;
    }
    break;

    
  case ENUM_DEV_CONFIGURED:
    /* user callback for enumeration done */
    USBH_Machine.usr_cb->EnumerationDone();
    Status = USBH_OK;
    break;
    
  default:
    break;
  }  
  return Status;
}


/**
  * @brief  USBH_HandleControl
  *         Handles the USB control transfer state machine
  * @param  pdev: Selected device
  * @retval Status
  */
USBH_Status USBH_HandleControl (USB_OTG_CORE_HANDLE *pdev)
{
  uint8_t direction;  
  static uint16_t timeout = 0;
  USBH_Status status = USBH_OK;
  URB_STATE URB_Status = URB_IDLE;
  
  USBH_Machine.Control.status = CTRL_START;
  URB_Status = HCD_GetURB_State(pdev); 
  
  switch (USBH_Machine.Control.state)
  {
  case CTRL_SETUP:
    /* send a SETUP packet */
    USBH_CtlSendSetup     (pdev, 
	                   &USBH_Machine.Control.setup->d8[0] , 
	                   USBH_Machine.Control.hc_num_out);  
    USBH_Machine.Control.state = CTRL_SETUP_WAIT;  
    break; 
    
  case CTRL_SETUP_WAIT:
    /* case SETUP packet sent successfully */
    if(URB_Status == URB_DONE)
    { 
      direction = (USBH_Machine.Control.setup->b.bmRequestType & USB_REQ_DIR_MASK);
      
      /* check if there is a data stage */
      if (USBH_Machine.Control.setup->b.wLength.w != 0 )
      {        
        timeout = DATA_STAGE_TIMEOUT;
        if (direction == USB_D2H)
        {
          /* Data Direction is IN */
          USBH_Machine.Control.state = CTRL_DATA_IN;
        }
        else
        {
          /* Data Direction is OUT */
          USBH_Machine.Control.state = CTRL_DATA_OUT;
        } 
      }
      /* No DATA stage */
      else
      {
        timeout = NODATA_STAGE_TIMEOUT;
        
        /* If there is No Data Transfer Stage */
        if (direction == USB_D2H)
        {
          /* Data Direction is IN */
          USBH_Machine.Control.state = CTRL_STATUS_OUT;
        }
        else
        {
          /* Data Direction is OUT */
          USBH_Machine.Control.state = CTRL_STATUS_IN;
        } 
      }          
      /* Set the delay timer to enable timeout for data stage completion */
      USBH_Machine.Control.timer = HCD_GetCurrentFrame(pdev);
    }
    else if(URB_Status == URB_ERROR)
    {
      USBH_Machine.Control.state = CTRL_ERROR;     
      USBH_Machine.Control.status = CTRL_XACTERR;
    }    
    break;
    
  case CTRL_DATA_IN:  
    /* Issue an IN token */ 
    USBH_CtlReceiveData(&USB_OTG_FS_dev,
                        USBH_Machine.Control.buff, 
                        USBH_Machine.Control.length,
                        USBH_Machine.Control.hc_num_in);
 
    USBH_Machine.Control.state = CTRL_DATA_IN_WAIT;
    break;    
    
  case CTRL_DATA_IN_WAIT:
    /* check is DATA packet transfered successfully */
    if  (URB_Status == URB_DONE)
    { 
      USBH_Machine.Control.state = CTRL_STATUS_OUT;
    }
   
    /* manage error cases*/
    if  (URB_Status == URB_STALL) 
    { 
      /* In stall case, return to previous machine state*/
      USBH_Machine.gState =   USBH_Machine.gStateBkp;
    }   
    else if (URB_Status == URB_ERROR)
    {
      /* Device error */
      USBH_Machine.Control.state = CTRL_ERROR;    
    }
    else if ((HCD_GetCurrentFrame(&USB_OTG_FS_dev) \
      - USBH_Machine.Control.timer) > timeout)
    {
      /* timeout for IN transfer */
      USBH_Machine.Control.state = CTRL_ERROR; 
    }   
    break;
    
  case CTRL_DATA_OUT:
    /* Start DATA out transfer (only one DATA packet)*/
    USBH_CtlSendData (&USB_OTG_FS_dev,
                      USBH_Machine.Control.buff, 
                      USBH_Machine.Control.length , 
                      USBH_Machine.Control.hc_num_out);
    
    USBH_Machine.Control.state = CTRL_DATA_OUT_WAIT;
    break;
    
  case CTRL_DATA_OUT_WAIT:
    if  (URB_Status == URB_DONE)
    { /* If the Setup Pkt is sent successful, then change the state */
      USBH_Machine.Control.state = CTRL_STATUS_IN;
    }
    
    /* handle error cases */
    else if  (URB_Status == URB_STALL) 
    { 
      /* In stall case, return to previous machine state*/
      USBH_Machine.gState =   USBH_Machine.gStateBkp;
    } 
    else if  (URB_Status == URB_NOTREADY)
    { 
      /* Nack received from device */
      USBH_Machine.Control.state = CTRL_DATA_OUT;
    }    
    else if (URB_Status == URB_ERROR)
    {
      /* device error */
      USBH_Machine.Control.state = CTRL_ERROR;      
    } 
    break;
    
    
  case CTRL_STATUS_IN:
    /* Send 0 bytes out packet */
    USBH_CtlReceiveData (&USB_OTG_FS_dev,
                         0,
                         0,
                         USBH_Machine.Control.hc_num_in);
    
    USBH_Machine.Control.state = CTRL_STATUS_IN_WAIT;
    
    break;
    
  case CTRL_STATUS_IN_WAIT:
    if  ( URB_Status == URB_DONE)
    { /* Control transfers completed, Exit the State Machine */
      USBH_Machine.gState =   USBH_Machine.gStateBkp;
    }
    
    else if (URB_Status == URB_ERROR)
    {
      USBH_Machine.Control.state = CTRL_ERROR;  
    }
    
    else if((HCD_GetCurrentFrame(&USB_OTG_FS_dev)\
      - USBH_Machine.Control.timer) > timeout)
    {
      USBH_Machine.Control.state = CTRL_ERROR; 
    }
     else if(URB_Status == URB_STALL)
    {
      /* Control transfers completed, Exit the State Machine */
      USBH_Machine.gState =   USBH_Machine.gStateBkp;
      USBH_Machine.Control.status = CTRL_STALL;
      status = USBH_NOT_SUPPORTED;
    }
    break;
    
  case CTRL_STATUS_OUT:
    USBH_CtlSendData (&USB_OTG_FS_dev,
                      0,
                      0,
                      USBH_Machine.Control.hc_num_out);
    
    USBH_Machine.Control.state = CTRL_STATUS_OUT_WAIT;
    break;
    
  case CTRL_STATUS_OUT_WAIT:  
    if  (URB_Status == URB_DONE)
    { 
      USBH_Machine.gState =   USBH_Machine.gStateBkp;    
    }
    else if  (URB_Status == URB_NOTREADY)
    { 
      USBH_Machine.Control.state = CTRL_STATUS_OUT;
    }      
    else if (URB_Status == URB_ERROR)
    {
      USBH_Machine.Control.state = CTRL_ERROR;      
    }
    break;
    
  case CTRL_ERROR:
    /* 
    After a halt condition is encountered or an error is detected by the 
    host, a control endpoint is allowed to recover by accepting the next Setup 
    PID; i.e., recovery actions via some other pipe are not required for control
    endpoints. For the Default Control Pipe, a device reset will ultimately be 
    required to clear the halt or error condition if the next Setup PID is not 
    accepted.
    */
    if (++ USBH_Machine.Control.errorcount <= USBH_MAX_ERROR_COUNT)
    {
      /* Do the transmission again, starting from SETUP Packet */
      USBH_Machine.Control.state = CTRL_SETUP; 
    }
    else
    {
      USBH_Machine.Control.status = CTRL_FAIL;
      USBH_Machine.gState =   USBH_Machine.gStateBkp;
      
      status = USBH_FAIL;
    }
    break;
    
  default:
    break;
  }
  return status;
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

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/




