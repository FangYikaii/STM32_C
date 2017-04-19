/**
  ******************************************************************************
  * @file    usbh_stdreq.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/29/2010
  * @brief   This file implements the standard requests for device enumeration
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
#include "usbh_stdreq.h"
#include "usbh_hid_core.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_STDREQ 
* @brief This file implements the standard requests for device enumeration
* @{
*/


/** @defgroup USBH_STDREQ_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_STDREQ_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 



/** @defgroup USBH_STDREQ_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_STDREQ_Private_Variables
* @{
*/
USB_Setup_TypeDef    setup;
uint8_t              Rx_Buffer [MAX_DATA_LENGTH];
/**
* @}
*/ 


/** @defgroup USBH_STDREQ_Private_FunctionPrototypes
* @{
*/
static void USBH_ParseDevDesc (USBH_DevDesc_TypeDef* , uint8_t *buf, uint16_t length);

static void USBH_ParseCfgDesc (USBH_CfgDesc_TypeDef* cfg_desc,
                               USBH_InterfaceDesc_TypeDef* itf_desc,
                               USBH_EpDesc_TypeDef*  ep_desc,                                                           
                               uint8_t *buf, 
                               uint16_t length);

static void USBH_ParseInterfaceDesc (USBH_InterfaceDesc_TypeDef  *if_descriptor, uint8_t *buf);
static void USBH_ParseEPDesc (USBH_EpDesc_TypeDef  *ep_descriptor, uint8_t *buf);

static void USBH_ParseStringDesc (uint8_t* psrc, uint8_t* pdest, uint16_t length);
/**
* @}
*/ 


/** @defgroup USBH_STDREQ_Private_Functions
* @{
*/ 


/**
* @brief  USBH_Get_DevDesc
*         Issue Get Device Descriptor command to the device. Once the response 
*         received, it parses the device descriptor and updates the status.
* @param  pdev: Selected device
* @param  dev_desc: Device Descriptor buffer address
* @param  Rx_Buffer: Receive Buffer address
* @param  length: Length of the descriptor
* @retval Status
*/
USBH_Status USBH_Get_DevDesc(USB_OTG_CORE_HANDLE *pdev,
                             USBH_DevDesc_TypeDef *dev_desc,
                             uint8_t length)
{
  
  USBH_Status status;
  
  if((status = USBH_GetDescriptor(pdev, 
                                  USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD,                          
                                  USB_DESC_DEVICE, 
                                  Rx_Buffer,
                                  length)) == USBH_OK)
  {
    /* Commands successfully sent and Response Received */       
    USBH_ParseDevDesc(dev_desc, Rx_Buffer, length);
  }
  return status;      
}

/**
* @brief  USBH_Get_CfgDesc
*         Issues Configuration Descriptor to the device. Once the response 
*         received, it parses the configuartion descriptor and updates the 
*         status.
* @param  pdev: Selected device
* @param  cfg_desc: Configuration Descriptor address
* @param  itf_desc: Interface Descriptor address
* @param  ep_desc: Endpoint Descriptor address
* @param  length: Length of the descriptor
* @retval Status
*/
USBH_Status USBH_Get_CfgDesc(USB_OTG_CORE_HANDLE *pdev, 
                             USBH_CfgDesc_TypeDef* cfg_desc,
                             USBH_InterfaceDesc_TypeDef* itf_desc,
                             USBH_EpDesc_TypeDef*  ep_desc,                       
                             uint16_t length)

{
  USBH_Status status;
  
  if((status = USBH_GetDescriptor(pdev,
                                  USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD,                          
                                  USB_DESC_CONFIGURATION, 
                                  Rx_Buffer,
                                  length)) == USBH_OK)
  {
    /* Commands successfully sent and Response Received  */       
    USBH_ParseCfgDesc (cfg_desc,
                       itf_desc,
                       ep_desc, 
                       Rx_Buffer,
                       length); 
    
  }
  return status;
}


/**
* @brief  USBH_Get_StringDesc
*         Issues string Descriptor command to the device. Once the response 
*         received, it parses the string descriptor and updates the status.
* @param  pdev: Selected device
* @param  string_index: String index for the descriptor
* @param  buff: Buffer address for the descriptor
* @param  length: Length of the descriptor
* @retval Status
*/
USBH_Status USBH_Get_StringDesc(USB_OTG_CORE_HANDLE *pdev, 
                                uint8_t string_index, 
                                uint8_t *buff, 
                                uint16_t length)
{
  USBH_Status status;
  
  if((status = USBH_GetDescriptor(pdev,
                                  USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD,                                    
                                  USB_DESC_STRING | string_index, 
                                  Rx_Buffer,
                                  length)) == USBH_OK)
  {
    /* Commands successfully sent and Response Received  */       
    USBH_ParseStringDesc(Rx_Buffer,buff, length);    
  }
  return status;
}

/**
* @brief  USBH_GetDescriptor
*         Issues Descriptor command to the device. Once the response received,
*         it parses the descriptor and updates the status.
* @param  pdev: Selected device
* @param  req_type: Descriptor type
* @param  value_idx: wValue for the GetDescriptr request
* @param  buff: Buffer to store the descriptor
* @param  length: Length of the descriptor
* @retval Status
*/
USBH_Status USBH_GetDescriptor(USB_OTG_CORE_HANDLE *pdev, 
                               uint8_t  req_type,
                               uint16_t value_idx, 
                               uint8_t* buff, 
                               uint16_t length )
{ 
  setup.b.bmRequestType = USB_D2H | req_type;
  setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
  setup.b.wValue.w = value_idx;
  
  if ((value_idx & 0xff00) == USB_DESC_STRING)
  {
    setup.b.wIndex.w = 0x0409;
  }
  else
  {
    setup.b.wIndex.w = 0;
  }
  setup.b.wLength.w = length;           
  return USBH_CtlReq(pdev, &setup, buff , length );     
}

/**
* @brief  USBH_SetAddress
*         This command sets the address to the connected device
* @param  pdev: Selected device
* @param  DeviceAddress: Device address to assign
* @retval Status
*/
USBH_Status USBH_SetAddress(USB_OTG_CORE_HANDLE *pdev, uint8_t DeviceAddress)
{
  setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_DEVICE | \
    USB_REQ_TYPE_STANDARD;
  
  setup.b.bRequest = USB_REQ_SET_ADDRESS;
  
  setup.b.wValue.w = (uint16_t)DeviceAddress;
  setup.b.wIndex.w = 0;
  setup.b.wLength.w = 0;
  
  return USBH_CtlReq(pdev, &setup, 0 , 0 );
}

/**
* @brief  USBH_SetCfg
*         The command sets the configuration value to the connected device
* @param  pdev: Selected device
* @param  cfg_idx: Configuration value
* @retval Status
*/
USBH_Status USBH_SetCfg(USB_OTG_CORE_HANDLE *pdev, 
                        uint16_t cfg_idx)
{
  
  setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_DEVICE |\
    USB_REQ_TYPE_STANDARD;
  setup.b.bRequest = USB_REQ_SET_CONFIGURATION;
  setup.b.wValue.w = cfg_idx;
  setup.b.wIndex.w = 0;
  setup.b.wLength.w = 0;           
  
  return USBH_CtlReq(pdev, &setup, 0 , 0 );      
}

/**
* @brief  USBH_ClrFeature
*         This request is used to clear or disable a specific feature.

* @param  pdev: Selected device
* @param  ep_num: endpoint number 
* @param  hc_num: Host channel number 
* @retval Status
*/
USBH_Status USBH_ClrFeature(USB_OTG_CORE_HANDLE *pdev, 
                            uint8_t ep_num, 
                            uint8_t hc_num) 
{
  
  setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_ENDPOINT | \
    USB_REQ_TYPE_STANDARD;
  
  setup.b.bRequest = USB_REQ_CLEAR_FEATURE;
  setup.b.wValue.w = FEATURE_SELECTOR_ENDPOINT;
  setup.b.wIndex.w = ep_num;
  setup.b.wLength.w = 0;           
  
  if ((ep_num & USB_REQ_DIR_MASK ) == USB_D2H)
  { /* EP Type is IN */
    pdev->host.hc[hc_num].toggle_in = 0; 
  }
  else
  {/* EP Type is OUT */
    pdev->host.hc[hc_num].toggle_out = 0; 
  }
  
  return USBH_CtlReq(pdev, &setup, 0 , 0 );   
}

/**
* @brief  USBH_ParseDevDesc 
*         This function Parses the device descriptor
* @param  dev_desc: device_descriptor destinaton address 
* @param  buf: Buffer where the source descriptor is available
* @param  length: Length of the descriptor
* @retval None
*/
static void  USBH_ParseDevDesc (USBH_DevDesc_TypeDef* dev_desc, 
                                uint8_t *buf, 
                                uint16_t length)
{
  dev_desc->bLength            = *(uint8_t  *) (buf +  0);
  dev_desc->bDescriptorType    = *(uint8_t  *) (buf +  1);
  dev_desc->bcdUSB             = LE16 (buf +  2);
  dev_desc->bDeviceClass       = *(uint8_t  *) (buf +  4);
  dev_desc->bDeviceSubClass    = *(uint8_t  *) (buf +  5);
  dev_desc->bDeviceProtocol    = *(uint8_t  *) (buf +  6);
  dev_desc->bMaxPacketSize     = *(uint8_t  *) (buf +  7);
  
  if (length > 8)
  { /* For 1st time after device connection, Host may issue only 8 bytes for 
    Device Descriptor Length  */
    dev_desc->idVendor           = LE16 (buf +  8);
    dev_desc->idProduct          = LE16 (buf + 10);
    dev_desc->bcdDevice          = LE16 (buf + 12);
    dev_desc->iManufacturer      = *(uint8_t  *) (buf + 14);
    dev_desc->iProduct           = *(uint8_t  *) (buf + 15);
    dev_desc->iSerialNumber      = *(uint8_t  *) (buf + 16);
    dev_desc->bNumConfigurations = *(uint8_t  *) (buf + 17);
  }
}

/**
* @brief  USBH_ParseCfgDesc 
*         This function Parses the configuration descriptor
* @param  cfg_desc: Configuration Descriptor address
* @param  itf_desc: Interface Descriptor address
* @param  ep_desc: Endpoint Descriptor address
* @param  buf: Buffer where the source descriptor is available
* @param  length: Length of the descriptor
* @retval None
*/
static void  USBH_ParseCfgDesc (USBH_CfgDesc_TypeDef* cfg_desc,
                                USBH_InterfaceDesc_TypeDef* itf_desc,
                                USBH_EpDesc_TypeDef*  ep_desc, 
                                uint8_t *buf, 
                                uint16_t length)
{
  uint8_t num;
  uint8_t maxEP;
  uint8_t classDesclen;
  
  cfg_desc->bLength             = *(uint8_t  *) (buf + 0);
  cfg_desc->bDescriptorType     = *(uint8_t  *) (buf + 1);
  cfg_desc->wTotalLength        = LE16 (buf + 2);
  cfg_desc->bNumInterfaces      = *(uint8_t  *) (buf + 4);
  cfg_desc->bConfigurationValue = *(uint8_t  *) (buf + 5);
  cfg_desc->iConfiguration      = *(uint8_t  *) (buf + 6);
  cfg_desc->bmAttributes        = *(uint8_t  *) (buf + 7);
  cfg_desc->bMaxPower           = *(uint8_t  *) (buf + 8);
  
  if (length > USB_CONFIGURATION_DESC_SIZE)
  {/* This means, interface and Endpoint descriptrs also available */
    
    /* Move Pointer to Interface Desc Rx_Buffer */
    buf += USB_CONFIGURATION_DESC_SIZE; 
     
    if((*(uint8_t  *) (buf + 0) == INTERFACE_DESC_SIZE) && 
      (*(uint8_t  *) (buf + 1) == INTERFACE_DESC_TYPE))
    {   
      USBH_ParseInterfaceDesc (itf_desc, buf);
      
      buf += itf_desc->bLength; /* Move Pointer to EP Desc Rx_Buffer */
      
      /* Parse class descriptor */
      classDesclen =  USBH_ParseClassDesc(itf_desc,buf);       
      
      buf += classDesclen;
      
      /* Find the number of EPs in the Interface Descriptor */      
      /* Choose the lower number in order not to overrun the buffer allocated */
      maxEP = ( (itf_desc->bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ? 
               itf_desc->bNumEndpoints :
                   USBH_MAX_NUM_ENDPOINTS); 
      
           
      for (num=0; num < maxEP; num++)
      {
        /* buf was pointing to start of IF desc in 1st loop and next time 
        start of previous EP Desc */
        if(*(uint8_t  *) (buf + 1) == ENDPOINT_DESC_TYPE)
        {
          USBH_ParseEPDesc(&ep_desc[num], buf);
          buf += ep_desc[num].bLength;  /* Move Pointer to Next EP Rx_Buffer */
        }
      }
    }
  } 
}


/**
* @brief  USBH_ParseInterfaceDesc 
*         This function Parses the interface descriptor
* @param  if_descriptor : Interface descriptor destination
* @param  buf: Buffer where the descriptor data is available
* @retval None
*/
static void  USBH_ParseInterfaceDesc (USBH_InterfaceDesc_TypeDef *if_descriptor, 
                                      uint8_t *buf)
{
  if_descriptor->bLength            = *(uint8_t  *) (buf + 0);
  if_descriptor->bDescriptorType    = *(uint8_t  *) (buf + 1);
  if_descriptor->bInterfaceNumber   = *(uint8_t  *) (buf + 2);
  if_descriptor->bAlternateSetting  = *(uint8_t  *) (buf + 3);
  if_descriptor->bNumEndpoints      = *(uint8_t  *) (buf + 4);
  if_descriptor->bInterfaceClass    = *(uint8_t  *) (buf + 5);
  if_descriptor->bInterfaceSubClass = *(uint8_t  *) (buf + 6);
  if_descriptor->bInterfaceProtocol = *(uint8_t  *) (buf + 7);
  if_descriptor->iInterface         = *(uint8_t  *) (buf + 8);
}

/**
* @brief  USBH_ParseEPDesc 
*         This function Parses the endpoint descriptor
* @param  ep_descriptor: Endpoint descriptor destination address
* @param  buf: Buffer where the parsed descriptor stored
* @retval None
*/
static void  USBH_ParseEPDesc (USBH_EpDesc_TypeDef  *ep_descriptor, 
                               uint8_t *buf)
{
  
  ep_descriptor->bLength          = *(uint8_t  *) (buf + 0);
  ep_descriptor->bDescriptorType  = *(uint8_t  *) (buf + 1);
  ep_descriptor->bEndpointAddress = *(uint8_t  *) (buf + 2);
  ep_descriptor->bmAttributes     = *(uint8_t  *) (buf + 3);
  ep_descriptor->wMaxPacketSize   = LE16 (buf + 4);
  ep_descriptor->bInterval        = *(uint8_t  *) (buf + 6);
}

/**
* @brief  USBH_ParseStringDesc 
*         This function Parses the string descriptor
* @param  psrc: Source pointer containing the descriptor data
* @param  pdest: Destination address pointer
* @param  length: Length of the descriptor
* @retval None
*/
static void USBH_ParseStringDesc (uint8_t* psrc, 
                                  uint8_t* pdest, 
                                  uint16_t length)
{
  uint16_t strlength;
  uint16_t idx;
  
  /* The UNICODE string descriptor is not NULL-terminated. The string length is
  computed by substracting two from the value of the first byte of the descriptor.
  */
  
  /* Check which is lower size, the Size of string or the length of bytes read 
  from the device */
  
  if ( psrc[1] == USB_DESC_TYPE_STRING)
  { /* Make sure the Descriptor is String Type */
    
    /* psrc[0] contains Size of Descriptor, subtract 2 to get the length of string */      
    strlength = ( ( (psrc[0]-2) <= length) ? (psrc[0]-2) :length); 
    psrc += 2; /* Adjust the offset ignoring the String Len and Descriptor type */
    
    for (idx = 0; idx < strlength; idx+=2 )
    {/* Copy Only the string and ignore the UNICODE ID, hence add the src */
      *pdest =  psrc[idx];
      pdest++;
    }  
    *pdest = 0; /* mark end of string */  
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

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/




