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
#include "../../../Utilities/Fonts/fonts.h"
#include "../../../Utilities/Fonts/font24.c"
#include "../../../Utilities/Fonts/font20.c"
#include "../../../Utilities/Fonts/font16.c"
#include "../../../Utilities/Fonts/font12.c"
#include "../../../Utilities/Fonts/font8.c"
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
static uint8_t BlockBuffer[240*2*24];
static uint16_t posy = 20;
static uint8_t key = 1;
static uint8_t hb;
static uint8_t lb;
/* Default LCD configuration with LCD Layer 1 */
static uint32_t ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void DrawChar(uint16_t Xpos, const uint8_t *c);
static void BSP_LCD_DisplayChar(uint16_t Xpos, uint8_t Ascii);
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

uint32_t BSP_LCD_GetTextColor(void)
{
  return DrawProp[ActiveLayer].TextColor;
}

uint32_t BSP_LCD_GetBackColor(void)
{
  return DrawProp[ActiveLayer].BackColor;
}

void BSP_LCD_SetTextColor(uint32_t Color)
{
  DrawProp[ActiveLayer].TextColor = Color;
}

void BSP_LCD_SetBackColor(uint32_t Color)
{
  DrawProp[ActiveLayer].BackColor = Color;
}

void BSP_LCD_SetFont(sFONT *pFonts)
{
  DrawProp[ActiveLayer].pFont = pFonts;
}

sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp[ActiveLayer].pFont;
}

#if 0
void BSP_LCD_ClearStringLine(uint32_t Line)
{
  uint32_t sizex;

  if(BSP_LCD_GetXSize(0, &sizex) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  uint32_t colorbackup = DrawProp[ActiveLayer].TextColor;
  DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;

  /* Draw rectangle with background color */
  BSP_LCD_FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), sizex, DrawProp[ActiveLayer].pFont->Height);

  DrawProp[ActiveLayer].TextColor = colorbackup;
  BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
}
#endif
/**
  * @brief  Displays one character.
  * @param  Xpos: start column address
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E
  */
void BSP_LCD_DisplayChar(uint16_t Xpos, uint8_t Ascii)
{
  DrawChar(Xpos, &DrawProp[ActiveLayer].pFont->table[(Ascii-' ') *
              DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays a maximum of 60 char on the LCD.
  * @param  X: pointer to x position (in pixel)
  * @param  Y: pointer to y position (in pixel)
  * @param  pText: pointer to string to display on LCD
  * @param  mode: The display mode
  *    This parameter can be one of the following values:
  *                @arg CENTER_MODE
  *                @arg RIGHT_MODE
  *                @arg LEFT_MODE
  */
void BSP_LCD_DisplayStringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode)
{
  uint16_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0;
  uint8_t  *ptr = pText;

  uint32_t sizex;

  if(BSP_LCD_GetXSize(0, &sizex) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Get the text size */
  while (*ptr++) size ++ ;

  /* Characters number per line */
  xsize = (sizex/DrawProp[ActiveLayer].pFont->Width);

  switch (mode)
  {
  case CENTER_MODE:
    {
      refcolumn = X+ ((xsize - size)* DrawProp[ActiveLayer].pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = X;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn = X + ((xsize - size)*DrawProp[ActiveLayer].pFont->Width);
      break;
    }
  default:
    {
      refcolumn = X;
      break;
    }
  }

  /* Send the string character by character in the buffer */
  while ((*pText != 0) & (((sizex - (i*DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width))
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(refcolumn, *pText);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp[ActiveLayer].pFont->Width;
    /* Point on the next character */
    pText++;
    i++;
  }

  uint32_t sizey = DrawProp[ActiveLayer].pFont->Height;

  if(BSP_LCD_SetDisplayWindow(0, X, Y, sizex, sizey)!= BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  // Send the frambuffer
  {
    if(BSP_LCD_FillRGBRect(0, X, Y, BlockBuffer, sizex, sizey) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape
  * @param  ptr: pointer to string to display on LCD
  */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr, Text_AlignModeTypdef mode)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, mode);
}

/**
  * @brief  Writes Pixel.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  RGB_Code: the pixel color in ARGB mode (8-8-8-8)
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
  /* Write data value to all SDRAM memory, in little endian mode */
  BlockBuffer[2*(Ypos*240 + Xpos)] = RGB_Code & 0xFF;
  BlockBuffer[(2*(Ypos*240 + Xpos)) + 1] = (RGB_Code >> 8) & 0xFF;
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the position where to display the character shape
  * @param  c: pointer to the character data
  */
static void DrawChar(uint16_t Xpos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint32_t line=0;

  height = DrawProp[ActiveLayer].pFont->Height;
  width  = DrawProp[ActiveLayer].pFont->Width;

  offset = 8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {
    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }

    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        BSP_LCD_DrawPixel((Xpos + j), i, DrawProp[ActiveLayer].TextColor);
      }
      else
      {
        BSP_LCD_DrawPixel((Xpos + j), i, DrawProp[ActiveLayer].BackColor);
      }
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
      else
        if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_PORT, KEY_LEFT_GPIO_PIN) == GPIO_PIN_RESET)
          state = 4;
        else
          if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_PORT, KEY_RIGHT_GPIO_PIN) == GPIO_PIN_RESET)
            state = 5;
  
  return state;
}
/* USER CODE END 0 */

/**
 * Initialize DISPLAY application
 */
void MX_DISPLAY_Init(void)
{
//  if(BSP_MEM_Init(0) != BSP_ERROR_NONE)
//  {
//    Error_Handler();
//  }
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
  DrawProp[ActiveLayer].pFont = &Font24;
  DrawProp[ActiveLayer].BackColor = LCD_COLOR_BLACK;
  DrawProp[ActiveLayer].TextColor = LCD_COLOR_CYAN;
  /* USER CODE END MX_DISPLAY_Init 1 */
}

/**
 * DISPLAY application entry function
 */
void MX_DISPLAY_Process(void)
{
	typedef struct {uint16_t f; uint8_t n[14];} Col_t;
#if 1
	Col_t col[] = {
			{LCD_COLOR_BLUE, "BLUE"},
			{LCD_COLOR_GREEN, "GREEN"},
			{LCD_COLOR_RED, "RED"},
			{LCD_COLOR_CYAN, "CYAN"},
			{LCD_COLOR_MAGENTA, "MAGENTA"},
			{LCD_COLOR_YELLOW, "YELLOW"},
			{LCD_COLOR_LIGHTBLUE, "LIGHTBLUE"},
			{LCD_COLOR_LIGHTGREEN, "LIGHTGREEN"},
			{LCD_COLOR_LIGHTRED, "LIGHTRED"},
			{LCD_COLOR_LIGHTCYAN, "LIGHTCYAN"},
			{LCD_COLOR_LIGHTMAGENTA, "LIGHTMAGENTA"},
			{LCD_COLOR_LIGHTYELLOW, "LIGHTYELLOW"},
			{LCD_COLOR_DARKBLUE, "DARKBLUE"},
			{LCD_COLOR_DARKGREEN, "DARKGREEN"},
			{LCD_COLOR_DARKRED, "DARKRED"},
			{LCD_COLOR_DARKCYAN, "DARKCYAN"},
			{LCD_COLOR_DARKMAGENTA, "DARKMAGENTA"},
			{LCD_COLOR_DARKYELLOW, "DARKYELLOW"},
			{LCD_COLOR_WHITE, "WHITE"},
			{LCD_COLOR_LIGHTGRAY, "LIGHTGRAY"},
			{LCD_COLOR_GRAY, "GRAY"},
			{LCD_COLOR_DARKGRAY, "DARKGRAY"},
			{LCD_COLOR_BLACK, "BLACK"},
			{LCD_COLOR_BROWN, "BROWN"},
			{LCD_COLOR_ORANGE, "ORANGE"}
	};
#endif
#if 0
	Col_t col[] = {
#if 0
			{0x8000, "0x8000"},
			{0x4000, "0x4000"},
			{0x2000, "0x2000"},
			{0x1000, "0x1000"},
			{0x0800, "0x0800"},
			{0x0400, "0x0400"},
			{0x0200, "0x0200"},
			{0x0100, "0x0100"},
			{0x0080, "0x0080"},
			{0x0040, "0x0040"},
			{0x0020, "0x0020"},
			{0x0010, "0x0010"},
			{0x0008, "0x0008"},
			{0x0004, "0x0004"},
			{0x0002, "0x0002"},
			{0x0001, "0x0001"},
			{0x0000, "0x0000"},
#endif
#if 1
			{0xDFFF, "0xDFFF"},
			{0xFFFF, "0xFFFF"},
#endif
#if 0
			{0x7FFF, "0x7FFF"},
			{0xBFFF, "0xBFFF"},
			{0xDFFF, "0xDFFF"},
			{0xEFFF, "0xEFFF"},
			{0xF7FF, "0xF7FF"},
			{0xFBFF, "0xFBFF"},
			{0xFDFF, "0xFDFF"},
			{0xFEFF, "0xFEFF"},
			{0xFF7F, "0xFF7F"},
			{0xFFBF, "0xFFBF"},
			{0xFFDF, "0xFFDF"},
			{0xFFEF, "0xFFEF"},
			{0xFFF7, "0xFFF7"},
			{0xFFFB, "0xFFFB"},
			{0xFFFD, "0xFFFD"},
			{0xFFFE, "0xFFFE"},
			{0xFFFF, "0xFFFF"},
#endif
	};
#endif
	static int cc = 0;
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
    for (unsigned int i = 0; i < sizeof(BlockBuffer); i += 2)
    {
    	BlockBuffer[i] = hb;
    	BlockBuffer[i + 1] = lb;
    }
    //memset(BlockBuffer, 0, sizeof(BlockBuffer));
    for(line = 0; line < posy; line+=BlockLines)
    {
      Display_Image(BlockBuffer, 2, 0, line, 240, BlockLines);
    }
    Display_Image((uint8_t* )Image, 2, 0, posy, 240, 240);
    for (unsigned int i = 0; i < sizeof(BlockBuffer); i += 2)
    {
    	BlockBuffer[i] = hb;
    	BlockBuffer[i + 1] = lb;
    }
    //memset(BlockBuffer, 0, sizeof(BlockBuffer));
    for(line = (240 + posy); line < 320; line+=BlockLines)
    {
      Display_Image(BlockBuffer, 2, 0, line, 240, BlockLines);
    }
    BSP_LCD_DisplayStringAtLine(0, (uint8_t *) "Hello World!", CENTER_MODE);
  }
  else
  {
		uint16_t f = BSP_LCD_GetTextColor();
		uint16_t b = BSP_LCD_GetBackColor();
		BSP_LCD_SetTextColor(col[cc].f);
		BSP_LCD_SetBackColor(0);
		memset(BlockBuffer, 0, sizeof(BlockBuffer));
		BSP_LCD_DisplayStringAtLine(11, col[cc].n, CENTER_MODE);
		memset(BlockBuffer, 0xFF, sizeof(BlockBuffer));
		BSP_LCD_SetBackColor(0xFFFF);
		BSP_LCD_DisplayStringAtLine(12, col[cc].n, CENTER_MODE);
		cc += 1;
		if (cc >= sizeof(col) / sizeof(Col_t))
			cc = 0;
		BSP_LCD_SetTextColor(f);
		BSP_LCD_SetBackColor(b);
		HAL_Delay(2000);
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
    case 4:
    	if (hb != 0 && lb == 0)
    	{
    		hb = 0x03;
    		lb = 0xe0;
    	}
    	else if (hb != 0 && lb != 0)
    	{
    		hb = 0x00;
    		lb = 0x1f;
    	}
    	else {
    		hb = 0xfc;
    		lb = 0x00;
    	}
    	HAL_Delay(500);
    	break;
    case 5:
    	if (hb == 0 && lb == 0)
    	{
    		hb = 0xff;
    		lb = 0xff;
    	}
    	if (hb >= 0x04)
    		hb = (hb >> 1) & 0xfc;
    	else if (hb > 0)
    		hb >>= 1;
    	else if (lb > 0x8)
    		lb = (lb >> 1) & 0xe;
    	else
    		lb >>= 1;
    	HAL_Delay(500);
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
