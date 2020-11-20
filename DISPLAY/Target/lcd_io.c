/**
  ******************************************************************************
  * File Name          : Target/lcd_io.c
  * Description        : This file provides code for the configuration
  *                      of the LCD IO instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_io.h"
#include "../ili9341/ili9341.h"

typedef struct
{
  uint32_t Width;
  uint32_t Height;
  uint32_t IsMspCallbacksValid;
}BSP_LCD_Ctx_t;

#define BUS_SPI1_POLL_TIMEOUT 0x1000U
#define BUS_SPI2_POLL_TIMEOUT 0x1000U

/* LINK LCD */
#define START_BYTE_READ         ILI9341_RAMRD

/* Number of LCD instances */
#define   LCD_INSTANCES_NBR     1U

#define LCD_ORIENTATION_PORTRAIT         ILI9341_ORIENTATION_PORTRAIT
#define LCD_ORIENTATION_LANDSCAPE        ILI9341_ORIENTATION_LANDSCAPE
#define LCD_ORIENTATION_PORTRAIT_ROT180  ILI9341_ORIENTATION_PORTRAIT_ROT180
#define LCD_ORIENTATION_LANDSCAPE_ROT180 ILI9341_ORIENTATION_LANDSCAPE_ROT180

#define POLY_X(Z)               ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)               ((int32_t)((Points + (Z))->Y))
#define ABS(X)                  (((X) > 0U) ? (X) : -(X))

extern SPI_HandleTypeDef hspi2;

void                 *LcdCompObj = NULL;
BSP_LCD_Ctx_t        LcdCtx[LCD_INSTANCES_NBR];

static LCD_Drv_t        *LcdDrv = NULL;
static ILI9341_IO_t     IOCtx = { 0 };
static ILI9341_Object_t ObjCtx = { 0 };

static int32_t LCD_Probe(uint32_t Instance, uint32_t Orientation);

/**
  * @brief  Initializes the LCD.
  * @param  Instance    LCD Instance
  * @param  Orientation LCD_ORIENTATION_PORTRAIT or LCD_ORIENTATION_LANDSCAPE
  *                     or LCD_ORIENTATION_PORTRAIT_ROT180
  *                     or LCD_ORIENTATION_LANDSCAPE_ROT180
  * @retval BSP status
  */
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= LCD_INSTANCES_NBR ) || (Orientation > LCD_ORIENTATION_LANDSCAPE_ROT180))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Probe the LCD Component driver */
    ret = LCD_Probe(Instance, Orientation);
  }

  return ret;
}

/**
  * @brief  De-Initializes the LCD resources.
  * @param  Instance LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(LcdDrv->DeInit(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Pointer to data buffer to send
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_SPI_Transmit(&hspi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) != HAL_OK)
  {
      ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Pointer to data buffer to receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI1_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_SPI_Receive(&hspi1, pData, Length, BUS_SPI1_POLL_TIMEOUT) != HAL_OK)
  {
      ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Pointer to data buffer to send
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_SPI_Transmit(&hspi2, pData, Length, BUS_SPI2_POLL_TIMEOUT) != HAL_OK)
  {
      ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Pointer to data buffer to receive
  * @param  Length: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI2_Recv(uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_SPI_Receive(&hspi2, pData, Length, BUS_SPI2_POLL_TIMEOUT) != HAL_OK)
  {
      ret = BSP_ERROR_UNKNOWN_FAILURE;
  }
  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @param  Instance LCD Instance
  * @param  pXSize pointer to Used LCD X size
  * @retval BSP status
  */
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *pXSize)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetXSize != NULL)
  {
    if(LcdDrv->GetXSize(LcdCompObj, pXSize) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    *pXSize = LcdCtx[Instance].Width;
  }
  return ret;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance LCD Instance
  * @param  pYSize pointer to Used LCD Y size
  * @retval BSP status
  */
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *pYSize)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->GetYSize != NULL)
  {
    if(LcdDrv->GetYSize(LcdCompObj, pYSize) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    *pYSize = LcdCtx[Instance].Height;
  }
  return ret;
}

/**
  * @brief  Switch On the display.
  * @param  Instance    LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DisplayOn != NULL)
  {
    if(LcdDrv->DisplayOn(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Switch Off the display.
  * @param  Instance    LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->DisplayOff != NULL)
  {
    if(LcdDrv->DisplayOff(LcdCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Sets a display window
  * @param  Xpos X position.
  * @param  Ypos   specifies the Y bottom left position.
  * @param  Height height of the rectangle to fill.
  * @param  Width  display window width.
  * @retval Component status
  */
int32_t BSP_LCD_SetDisplayWindow(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(LcdDrv->SetDisplayWindow != NULL)
  {
    /* Fill the Rectangle lines on LCD */
    if (LcdDrv->SetDisplayWindow(LcdCompObj, Xpos, Ypos, Width, Height) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  return ret;
}

/**
  * @brief  Send data to select the LCD GRAM.
  * @param  pData  pointer to data to write to LCD GRAM.
  * @param  Length length of data to write to LCD GRAM
  * @retval Error status
  */
int32_t BSP_LCD_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Select : Chip Select low */
    LCD_CS_LOW();

    /* Set the SPI in 16-bit mode to match endianess */
    hLCDSPI.Init.DataSize = SPI_DATASIZE_16BIT;
    HAL_SPI_Init(&hLCDSPI);

    /* Send Data */
    if(LCD_SPI_Send(pData, Length)!= BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }

    /* Go back to 8-bit mode */
    hLCDSPI.Init.DataSize = SPI_DATASIZE_8BIT;
    HAL_SPI_Init(&hLCDSPI);

    /* Deselect : Chip Select high */
    LCD_CS_HIGH();
  }

  return ret;
}

/**
  * @brief  Send data to select the LCD GRAM.
  * @param  pData  pointer to data to write to LCD GRAM.
  * @param  Length length of data to write to LCD GRAM
  * @retval Error status
  */
static int32_t LCD_IO_SendData(uint8_t *pData, uint32_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Select : Chip Select low */
  LCD_CS_LOW();

  /* Send Data */
  if(LCD_SPI_Send(pData, Length)!= BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();

  return ret;
}

/**
  * @brief  Writes register on LCD register.
  * @param  Reg    Register to be written
  * @param  Length length of data be read from the LCD GRAM
  * @retval BSP status
  */
static int32_t LCD_IO_WriteReg(uint8_t *Reg, uint32_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Select : Chip Select low */
  LCD_CS_LOW();

  LCD_DC_LOW();
  if(LCD_SPI_Send(Reg, Length) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  LCD_DC_HIGH();

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();

  return ret;
}

/**
  * @brief  Read data from LCD data register.
  * @param  pData  pointer to the read data from LCD GRAM.
  * @param  Reg    Register to be read
  * @retval BSP status
  */
static int32_t LCD_IO_ReadReg(uint8_t *pData, uint32_t Reg)
{
  int32_t ret = BSP_ERROR_NONE;
  uint16_t readvalue;
  uint8_t data[2];

  /* Send Reg value to Read */
  if(LCD_IO_WriteReg(pData, Reg) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();
  LCD_DC_LOW();

  /* Send Start Byte */
  data[0] = START_BYTE_READ;
  data[1] = 0xFF;

  if (ret == BSP_ERROR_NONE)
  {
    if(LCD_SPI_Send(data, 2) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      LCD_DC_HIGH();
      if(LCD_SPI_Recv(data, 2) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
      else
      {
        readvalue = (uint16_t)(data[0]) << 8;
        readvalue |= (uint16_t)data[1];
        HAL_Delay(10);
        *pData = (uint8_t)readvalue;
      }
    }
  }
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();

  return ret;
}

/**
  * @brief  Initializes LCD low level.
  * @retval BSP status
  */
static int32_t LCD_IO_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  HAL_GPIO_WritePin(LCD_NRESET_GPIO_PORT, LCD_NRESET_GPIO_PIN, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(LCD_NRESET_GPIO_PORT, LCD_NRESET_GPIO_PIN, GPIO_PIN_SET);
  HAL_Delay(100);

  /* turn LCD on = drive pin low (active low) */
  /* LCD_CS_PIN and LCD_DC_PIN configuration is done in the MSP Init*/
  /* Set or Reset the control line */
  LCD_CS_HIGH();
  LCD_DC_LOW();

  return ret;
}

/**
  * @brief  DeInitializes LCD low level
  * @retval BSP status
  */
static int32_t LCD_IO_DeInit(void)
{
  return 0; //BSP_SPI1_DeInit();
}

/**
  * @brief  Register Bus IOs for instance 0 if IL9341 ID is OK
  * @param  Orientation
  * @retval BSP status
  */
static int32_t LCD_Probe(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the lcd driver : map to LCD_IO function*/
  IOCtx.Init             = LCD_IO_Init;
  IOCtx.DeInit           = LCD_IO_DeInit;
  IOCtx.ReadReg          = LCD_IO_ReadReg;
  IOCtx.WriteReg         = LCD_IO_WriteReg;
  IOCtx.SendData         = LCD_IO_SendData;
  IOCtx.RecvData         = 0;
  IOCtx.GetTick          = HAL_GetTick;

  if(ILI9341_RegisterBusIO(&ObjCtx, &IOCtx) != ILI9341_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    LcdCompObj = &ObjCtx;

    /* LCD Initialization */
    LcdDrv = (LCD_Drv_t *)&ILI9341_LCD_Driver;

    ObjCtx.IsInitialized = 0;
    if(LcdDrv->Init(LcdCompObj, ILI9341_FORMAT_DEFAULT, Orientation) == ILI9341_OK)
    {
      LcdDrv->GetXSize(LcdCompObj, &LcdCtx[Instance].Width);
      LcdDrv->GetYSize(LcdCompObj, &LcdCtx[Instance].Height);
    }
    else
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
