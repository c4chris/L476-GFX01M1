/**
  ******************************************************************************
  * File Name          : Target/mem_conf.h
  * Description        : This file provides code for the configuration
  *                      of the MEM instances.
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
#ifndef __MEM_CONF_H__
#define __MEM_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* BUS IO Instance handlers */
extern  SPI_HandleTypeDef               hspi2;
extern  DMA_HandleTypeDef               hdma_spi2_rx;
extern  DMA_HandleTypeDef               hdma_spi2_tx;
#define hMEMSPI                         hspi2
#define hMEMDMA_rx                      hdma_spi2_rx
#define hMEMDMA_tx                      hdma_spi2_tx

/* BUS IO functions */
#define MEM_SPI_Recv                    BSP_SPI2_Recv
#define MEM_SPI_Send                    BSP_SPI2_Send

int32_t BSP_SPI2_Send(uint8_t *, uint16_t);
int32_t BSP_SPI2_Recv(uint8_t *, uint16_t);

/* NCS Pin mapping */
#define MEM_NCS_GPIO_PORT               GPIOB
#define MEM_NCS_GPIO_PIN                GPIO_PIN_9

/* Chip Select macro definition */
#define MEM_CS_LOW()                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define MEM_CS_HIGH()                   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)

#ifdef __cplusplus
}
#endif
#endif /* __MEM_CONF_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
