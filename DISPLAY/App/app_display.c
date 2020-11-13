/**
  ******************************************************************************
  * File Name          : app_display.c
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
#include "app_display.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "mem_io.h"
#include "lcd_io.h"
/* USER CODE BEGIN Includes */
#include "key_conf.h"
#include "string.h"
#include "Image.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static uint8_t TransferAllowed = 0;
static uint8_t BlockBuffer[240*2*4];
static uint16_t posy = 20;
static uint8_t key = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Send Data */
  if(BSP_LCD_WriteData(0, pData, (Width*Height))!= BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;  
}

static void BSP_LCD_Clear(uint32_t Instance)
{
  uint32_t sizex, sizey, size;
  uint32_t BlockLines;
  uint32_t offset = 0, line_cnt = 0;

  if((BSP_LCD_GetXSize(0, &sizex) != BSP_ERROR_NONE) || (BSP_LCD_GetYSize(0, &sizey) != BSP_ERROR_NONE))
  {
    Error_Handler();
  }

  if(BSP_LCD_SetDisplayWindow(0, 0, 0, sizex, sizey)!= BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  size = (2*sizex*sizey);
  BlockLines = (sizeof(BlockBuffer) / (2*sizex));
  memset(BlockBuffer, 0, sizeof(BlockBuffer));

  while(1)
  {
    if(BSP_LCD_FillRGBRect(0, 0, line_cnt, BlockBuffer, sizex, BlockLines) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    line_cnt += BlockLines;
    offset += sizeof(BlockBuffer);
    /* Check remaining data size */
    if(offset == size)
    {
      /* last block transfer was done, so exit */
      break;
    }
    else if((offset + sizeof(BlockBuffer)) > size)
    {
      /* Transfer last block and exit */
      BlockLines = ((size - offset)/ (2*sizex));
      if(BSP_LCD_FillRGBRect(0, 0, line_cnt, BlockBuffer, sizex, BlockLines) != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      break;
    }
  }
}

/**
  * @brief  Refresh the display.
  * @param  None.
  * @retval None.
  */
static void Display_Image(uint8_t *image, uint8_t bpp, uint16_t posx, uint16_t posy, uint16_t sizex, uint16_t sizey)
{
    uint32_t size = (bpp*sizex*sizey);
    uint32_t BlockLines = (sizeof(BlockBuffer) / (bpp*sizex));
    uint32_t line_cnt = 0;

    if(BSP_LCD_SetDisplayWindow(0, posx, posy, sizex, sizey)!= BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    // Send the frambuffer n times
    if((uint32_t )image >= 0x90000000)
    {
        uint32_t offset = 0;
        /* One block read */
        if(size < sizeof(BlockBuffer))
        {
          if(BSP_MEM_ReadData(0, (uint32_t )image, BlockBuffer, size) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }
          if(BSP_LCD_FillRGBRect(0, posx, posy, BlockBuffer, sizex, sizey) != BSP_ERROR_NONE)
          {
            Error_Handler();
          }
        }
        else
        {
          while(1)
          {
            /* Multi-block read/write */
            if(BSP_MEM_ReadData(0, (uint32_t )(image+offset), BlockBuffer, sizeof(BlockBuffer)) != BSP_ERROR_NONE)
            {
              Error_Handler();
            }
            if(BSP_LCD_FillRGBRect(0, posx, posy+line_cnt, BlockBuffer, sizex, BlockLines) != BSP_ERROR_NONE)
            {
              Error_Handler();
            }
            line_cnt += BlockLines;
            offset += sizeof(BlockBuffer);
            /* Check remaining data size */
            if(offset == size)
            {
              /* last block transfer was done, so exit */
              break;
            }
            else if((offset + sizeof(BlockBuffer)) > size)
            {
              /* Transfer last block and exit */
              if(BSP_MEM_ReadData(0, (uint32_t )(image+offset), BlockBuffer, (size - offset)) != BSP_ERROR_NONE)
              {
                Error_Handler();
              }
              BlockLines = ((size - offset)/ (bpp*sizex));
              if(BSP_LCD_FillRGBRect(0, posx, posy+line_cnt, BlockBuffer, sizex, BlockLines) != BSP_ERROR_NONE)
              {
                Error_Handler();
              }
              break;
            }
          }
        }
    }
    else
    {
      if(BSP_LCD_FillRGBRect(0, posx, posy, (uint8_t *)image, sizex, sizey) != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
    }
}

static uint8_t Joystick_State(void)
{
  uint8_t state = 0;

  if (HAL_GPIO_ReadPin(KEY_CENTER_GPIO_PORT, KEY_CENTER_GPIO_PIN) == GPIO_PIN_RESET)
    state = 1;
  else
    if (HAL_GPIO_ReadPin(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN) == GPIO_PIN_RESET)
      state = 2;
    else
      if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN) == GPIO_PIN_RESET)
        state = 3;
  
  return state;
}
/* USER CODE END 0 */

/**
 * Initialize DISPLAY application
 */
void MX_DISPLAY_Init(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Init 0 */

  /* USER CODE END MX_DISPLAY_Init 0 */
  if(BSP_MEM_Init(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if(BSP_LCD_Init(0, 0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if(BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN MX_DISPLAY_Init 1 */
  BSP_LCD_Clear(0);
  /* USER CODE END MX_DISPLAY_Init 1 */
}

/**
 * DISPLAY application entry function
 */
void MX_DISPLAY_Process(void)
{
  /* USER CODE BEGIN MX_DISPLAY_Process 0 */

  /* USER CODE END MX_DISPLAY_Process 0 */
  /* Wait for TE */
  //while(TransferAllowed == 0);
  //TransferAllowed = 0;
  /* Display something */
  /* USER CODE BEGIN MX_DISPLAY_Process 1 */
  if(key)
  {
    uint32_t line;
    uint32_t BlockLines = (sizeof(BlockBuffer) / (2*240));
    memset(BlockBuffer, 0, sizeof(BlockBuffer));
    for(line = 0; line < posy; line+=BlockLines)
    {
      Display_Image(BlockBuffer, 2, 0, line, 240, BlockLines);
    }
    Display_Image((uint8_t* )Image, 2, 0, posy, 240, 240);
    memset(BlockBuffer, 0, sizeof(BlockBuffer));
    for(line = (240 + posy); line < 320; line+=BlockLines)
    {
      Display_Image(BlockBuffer, 2, 0, line, 240, BlockLines);
    }
  }
  key = Joystick_State();
  switch(key)
  {
    case 1:
      posy = 20;
      break;
    case 2:
      if (posy > 20)
        posy=posy-2;
      break;
    case 3:
      if (posy <60)
        posy=posy+2;
          break;
    default:
      break;
  }
  /* USER CODE END MX_DISPLAY_Process 1 */
}

/**
 * DISPLAY application task
 */
void DISPLAY_Task(void *argument)
{
  /* USER CODE BEGIN DISPLAY_Task 0 */

  /* USER CODE END DISPLAY_Task 0 */
  /* Wait for TE */
  while(TransferAllowed == 0);
  TransferAllowed = 0;
  /* Display something */
  /* USER CODE BEGIN DISPLAY_Task 1 */

  /* USER CODE END DISPLAY_Task 1 */
}

/* USER CODE BEGIN 1 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  if(GPIO_Pin == LCD_TE_GPIO_PIN)
  {
    TransferAllowed = 1;
  }
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Rising_Callback could be implemented in the user file
   */
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
