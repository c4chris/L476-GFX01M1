/**
  ******************************************************************************
  * File Name          : Target/lcd_conf.h
  * Description        : This file provides code for the configuration
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
#ifndef __LCD_CONF_H__
#define __LCD_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#include "lcd.h"

/* BUS IO Instance handlers */
extern  SPI_HandleTypeDef               hspi1;
extern  DMA_HandleTypeDef               hdma_spi1_rx;
extern  DMA_HandleTypeDef               hdma_spi1_tx;
#define hLCDSPI                         hspi1
#define hLCDDMA_rx                      hdma_spi1_rx
#define hLCDDMA_tx                      hdma_spi1_tx

/* BUS IO functions */
#define LCD_SPI_Recv                    BSP_SPI1_Recv
#define LCD_SPI_Send                    BSP_SPI1_Send

/* NCS Pin mapping */
#define LCD_NCS_GPIO_PORT               GPIOB
#define LCD_NCS_GPIO_PIN                GPIO_PIN_5

/* DCX Pin mapping */
#define LCD_DCX_GPIO_PORT               GPIOB
#define LCD_DCX_GPIO_PIN                GPIO_PIN_3

/* NRESET Pin mapping */
#define LCD_NRESET_GPIO_PORT            GPIOA
#define LCD_NRESET_GPIO_PIN             GPIO_PIN_1

/* TE Pin mapping */
#define LCD_TE_GPIO_PIN                 GPIO_PIN_0
#define LCD_TE_GPIO_LINE                0
#define LCD_TE_GPIO_IRQn                EXTI0_1_IRQn

/* Chip Select macro definition */
#define LCD_CS_LOW()                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_CS_HIGH()                   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)

/* Data/Command macro definition */
#define LCD_DC_LOW()                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define LCD_DC_HIGH()                   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)

#ifdef __cplusplus
}
#endif
#endif /* __LCD_CONF_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
