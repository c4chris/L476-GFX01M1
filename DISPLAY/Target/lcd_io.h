/**
  ******************************************************************************
  * File Name          : Target/lcd_io.h
  * Description        : This file provides code for the exported APIs
  *                      of the LCD instances.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_IO_H__
#define __LCD_IO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_nucleo_errno.h"
#include "lcd_conf.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation);
int32_t BSP_LCD_DeInit(uint32_t Instance);
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *YSize);
int32_t BSP_LCD_DisplayOn(uint32_t Instance);
int32_t BSP_LCD_DisplayOff(uint32_t Instance);
int32_t BSP_LCD_SetDisplayWindow(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
int32_t BSP_LCD_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Length);

#ifdef __cplusplus
}
#endif
#endif /* __LCD_IO_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
