/**
  ******************************************************************************
  * File Name          : Target/key_conf.h
  * Description        : This file provides code for the configuration
  *                      of the KEY instances.
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
#ifndef __KEY_CONF_H__
#define __KEY_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* KEY_CENTER Pin mapping */
#define KEY_CENTER_GPIO_PORT     GPIOC
#define KEY_CENTER_GPIO_PIN      GPIO_PIN_8
#define KEY_CENTER_GPIO_LINE     8
#define KEY_CENTER_GPIO_IRQn     EXTI4_15_IRQn

/* KEY_DOWN Pin mapping */
#define KEY_DOWN_GPIO_PORT       GPIOC
#define KEY_DOWN_GPIO_PIN        GPIO_PIN_10
#define KEY_DOWN_GPIO_LINE       10
#define KEY_DOWN_GPIO_IRQn       EXTI4_15_IRQn

/* KEY_LEFT Pin mapping */
#define KEY_LEFT_GPIO_PORT       GPIOC
#define KEY_LEFT_GPIO_PIN        GPIO_PIN_9
#define KEY_LEFT_GPIO_LINE       9
#define KEY_LEFT_GPIO_IRQn       EXTI4_15_IRQn

/* KEY_RIGHT Pin mapping */
#define KEY_RIGHT_GPIO_PORT      GPIOC
#define KEY_RIGHT_GPIO_PIN       GPIO_PIN_11
#define KEY_RIGHT_GPIO_LINE      11
#define KEY_RIGHT_GPIO_IRQn      EXTI4_15_IRQn

/* KEY_UP Pin mapping */
#define KEY_UP_GPIO_PORT         GPIOC
#define KEY_UP_GPIO_PIN          GPIO_PIN_12
#define KEY_UP_GPIO_LINE         12
#define KEY_UP_GPIO_IRQn         EXTI4_15_IRQn

#ifdef __cplusplus
}
#endif
#endif /* __KEY_CONF_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
