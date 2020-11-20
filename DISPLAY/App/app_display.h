/**
  ******************************************************************************
  * File Name          : app_display.h
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
#include "../../Utilities/Fonts/fonts.h"

typedef enum
{
  LCD_OK = 0,
  LCD_ERROR = 1,
  LCD_TIMEOUT = 2
}LCD_StatusTypeDef;

typedef struct
{
  uint32_t  TextColor;
  uint32_t  BackColor;
  sFONT     *pFont;
}LCD_DrawPropTypeDef;

typedef struct
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
  CENTER_MODE             = 0x01,    /* center mode */
  RIGHT_MODE              = 0x02,    /* right mode  */
  LEFT_MODE               = 0x03,    /* left mode   */
}Text_AlignModeTypdef;

/**
  * @brief  LCD color
  * RGB 565 - (beware usage is little endian)
  * 1111 11
  * 5432 1098 7654 3210
  * rrrr rggg gggb bbbb
  */
#define RGB565(r, g, b) ((((r >> 3) & 0x1F) << 11) | (((g >> 2) & 0x3F) << 5) | ((b >> 3) & 0x1F))
#define LCD_COLOR_BLUE          RGB565(0x00, 0x00, 0xFF)
#define LCD_COLOR_GREEN         RGB565(0x00, 0xFF, 0x00)
#define LCD_COLOR_RED           RGB565(0xFF, 0x00, 0x00)
#define LCD_COLOR_CYAN          RGB565(0x00, 0xFF, 0xFF)
#define LCD_COLOR_MAGENTA       RGB565(0xFF, 0x00, 0xFF)
#define LCD_COLOR_YELLOW        RGB565(0xFF, 0xFF, 0x00)
#define LCD_COLOR_LIGHTBLUE     RGB565(0x80, 0x80, 0xFF)
#define LCD_COLOR_LIGHTGREEN    RGB565(0x80, 0xFF, 0x80)
#define LCD_COLOR_LIGHTRED      RGB565(0xFF, 0x80, 0x80)
#define LCD_COLOR_LIGHTCYAN     RGB565(0x80, 0xFF, 0xFF)
#define LCD_COLOR_LIGHTMAGENTA  RGB565(0xFF, 0x80, 0xFF)
#define LCD_COLOR_LIGHTYELLOW   RGB565(0xFF, 0xFF, 0x80)
#define LCD_COLOR_DARKBLUE      RGB565(0x00, 0x00, 0x80)
#define LCD_COLOR_DARKGREEN     RGB565(0x00, 0x80, 0x00)
#define LCD_COLOR_DARKRED       RGB565(0x80, 0x00, 0x00)
#define LCD_COLOR_DARKCYAN      RGB565(0x00, 0x80, 0x80)
#define LCD_COLOR_DARKMAGENTA   RGB565(0x80, 0x00, 0x80)
#define LCD_COLOR_DARKYELLOW    RGB565(0x80, 0x80, 0x00)
#define LCD_COLOR_WHITE         RGB565(0xFF, 0xFF, 0xFF)
#define LCD_COLOR_LIGHTGRAY     RGB565(0xD3, 0xD3, 0xD3)
#define LCD_COLOR_GRAY          RGB565(0x80, 0x80, 0x80)
#define LCD_COLOR_DARKGRAY      RGB565(0x40, 0x40, 0x40)
#define LCD_COLOR_BLACK         RGB565(0x00, 0x00, 0x00)
#define LCD_COLOR_BROWN         RGB565(0xA5, 0x2A, 0x2A)
#define LCD_COLOR_ORANGE        RGB565(0xFF, 0xA5, 0x00)
/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font24

void     BSP_LCD_SetTextColor(uint32_t Color);
void     BSP_LCD_SetBackColor(uint32_t Color);
uint32_t BSP_LCD_GetTextColor(void);
uint32_t BSP_LCD_GetBackColor(void);
void     BSP_LCD_SetFont(sFONT *pFonts);
sFONT    *BSP_LCD_GetFont(void);

void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
//void     BSP_LCD_ClearStringLine(uint32_t Line);
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr, Text_AlignModeTypdef mode);
void     BSP_LCD_DisplayStringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode);

void MX_DISPLAY_Init(void);
void MX_DISPLAY_Process(void);

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
