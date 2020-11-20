/**
  ******************************************************************************
  * @file    ili9341.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the ili9341.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ILI9341_H
#define ILI9341_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "ili9341_regs.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @defgroup ILI9341 ILI9341
  * @{
  */

/** @defgroup ILI9341_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*ILI9341_Init_Func)     (void);
typedef int32_t (*ILI9341_DeInit_Func)   (void);
typedef uint32_t (*ILI9341_GetTick_Func)  (void);
typedef int32_t (*ILI9341_Delay_Func)    (uint32_t);
typedef int32_t (*ILI9341_WriteReg_Func) (uint8_t*, uint32_t);
typedef int32_t (*ILI9341_ReadReg_Func)  (uint8_t*, uint32_t);
typedef int32_t (*ILI9341_SendData_Func) (uint8_t*, uint32_t);
typedef int32_t (*ILI9341_RecvData_Func) (uint8_t*, uint32_t);

typedef struct
{
  ILI9341_Init_Func          Init;
  ILI9341_DeInit_Func        DeInit;
  uint16_t                   Address;  
  ILI9341_WriteReg_Func      WriteReg;
  ILI9341_ReadReg_Func       ReadReg;
  ILI9341_SendData_Func      SendData;
  ILI9341_RecvData_Func      RecvData;
  ILI9341_GetTick_Func       GetTick; 
} ILI9341_IO_t;

 
typedef struct
{
  ILI9341_IO_t         IO;
  ili9341_ctx_t        Ctx;   
  uint8_t             IsInitialized;
} ILI9341_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(ILI9341_Object_t*, uint32_t, uint32_t);
  int32_t (*DeInit           )(ILI9341_Object_t*);
  int32_t (*ReadID           )(ILI9341_Object_t*, uint32_t*);
  int32_t (*DisplayOn        )(ILI9341_Object_t*);
  int32_t (*DisplayOff       )(ILI9341_Object_t*);
  int32_t (*SetDisplayWindow )(ILI9341_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*SetBrightness    )(ILI9341_Object_t*, uint32_t); 
  int32_t (*GetBrightness    )(ILI9341_Object_t*, uint32_t*);   
  int32_t (*SetOrientation   )(ILI9341_Object_t*, uint32_t);
  int32_t (*GetOrientation   )(ILI9341_Object_t*, uint32_t*);

  /* Drawing functions*/
  int32_t ( *SetCursor       ) (ILI9341_Object_t*, uint32_t, uint32_t); 
  int32_t ( *DrawBitmap      ) (ILI9341_Object_t*, uint32_t, uint32_t, uint8_t *);
  int32_t ( *FillRGBRect     ) (ILI9341_Object_t*, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t ( *DrawHLine       ) (ILI9341_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *DrawVLine       ) (ILI9341_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *FillRect        ) (ILI9341_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetPixel        ) (ILI9341_Object_t*, uint32_t, uint32_t, uint32_t*);
  int32_t ( *SetPixel        ) (ILI9341_Object_t*, uint32_t, uint32_t, uint32_t);
  int32_t ( *GetXSize        ) (ILI9341_Object_t*, uint32_t *);
  int32_t ( *GetYSize        ) (ILI9341_Object_t*, uint32_t *);
  
}ILI9341_LCD_Drv_t;
  
/**
  * @}
  */ 

/** @defgroup ILI9341_Exported_Constants Exported Constants
  * @{
  */

/** 
  * @brief  ILI9341 Size  
  */  
#define ILI9341_OK                (0)
#define ILI9341_ERROR             (-1)

/** 
  * @brief  ILI9341 ID  
  */  
#define  ILI9341_ID              0x9341U
  
/** 
  * @brief  ILI9341 Size  
  */  
#define  ILI9341_WIDTH           240U
#define  ILI9341_HEIGHT          320U

/** 
  * @brief  ILI9341 Timing  
  */     
/* Timing configuration  (Typical configuration from ILI9341 datasheet)
  HSYNC=10 (9+1)
  HBP=20 (29-10+1)
  ActiveW=240 (269-20-10+1)
  HFP=10 (279-240-20-10+1)

  VSYNC=2 (1+1)
  VBP=2 (3-2+1)
  ActiveH=320 (323-2-2+1)
  VFP=4 (327-320-2-2+1)
*/
#define  ILI9341_HSYNC            ((uint32_t)9)   /* Horizontal synchronization */
#define  ILI9341_HBP              ((uint32_t)29)    /* Horizontal back porch      */ 
#define  ILI9341_HFP              ((uint32_t)2)    /* Horizontal front porch     */
#define  ILI9341_VSYNC            ((uint32_t)1)   /* Vertical synchronization   */
#define  ILI9341_VBP              ((uint32_t)3)    /* Vertical back porch        */
#define  ILI9341_VFP              ((uint32_t)2)    /* Vertical front porch       */

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define ILI9341_ORIENTATION_PORTRAIT         0x00U /* Portrait orientation choice of LCD screen               */
#define ILI9341_ORIENTATION_PORTRAIT_ROT180  0x01U /* Portrait rotated 180° orientation choice of LCD screen  */
#define ILI9341_ORIENTATION_LANDSCAPE        0x02U /* Landscape orientation choice of LCD screen              */
#define ILI9341_ORIENTATION_LANDSCAPE_ROT180 0x03U /* Landscape rotated 180° orientation choice of LCD screen */

/**
 *  @brief  Possible values of pixel data format (ie color coding) 
 */ 
#define ILI9341_FORMAT_RBG565                0x05U /* Pixel format chosen is RGB565 : 16 bpp */
#define ILI9341_FORMAT_RBG666                0x06U /* Pixel format chosen is RGB666 : 18 bpp */
#define ILI9341_FORMAT_DEFAULT               ILI9341_FORMAT_RBG565
/**
  * @}
  */
  
/** @defgroup ILI9341_Exported_Functions Exported Functions
  * @{
  */ 
int32_t ILI9341_RegisterBusIO (ILI9341_Object_t *pObj, ILI9341_IO_t *pIO);
int32_t ILI9341_Init(ILI9341_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t ILI9341_DeInit(ILI9341_Object_t *pObj);
int32_t ILI9341_ReadID(ILI9341_Object_t *pObj, uint32_t *Id);
int32_t ILI9341_DisplayOn(ILI9341_Object_t *pObj);
int32_t ILI9341_DisplayOff(ILI9341_Object_t *pObj);
int32_t ILI9341_SetDisplayWindow(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
int32_t ILI9341_SetBrightness(ILI9341_Object_t *pObj, uint32_t Brightness);
int32_t ILI9341_GetBrightness(ILI9341_Object_t *pObj, uint32_t *Brightness);
int32_t ILI9341_SetOrientation(ILI9341_Object_t *pObj, uint32_t Orientation);
int32_t ILI9341_GetOrientation(ILI9341_Object_t *pObj, uint32_t *Orientation);

int32_t ILI9341_SetCursor(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t ILI9341_DrawBitmap(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t ILI9341_FillRGBRect(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
int32_t ILI9341_DrawHLine(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t ILI9341_DrawVLine(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t ILI9341_FillRect(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
int32_t ILI9341_SetPixel(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t ILI9341_GetPixel(ILI9341_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t ILI9341_GetXSize(ILI9341_Object_t *pObj, uint32_t *XSize);
int32_t ILI9341_GetYSize(ILI9341_Object_t *pObj, uint32_t *YSize);

extern ILI9341_LCD_Drv_t   ILI9341_LCD_Driver;

/**
  * @}
  */ 
      
#ifdef __cplusplus
}
#endif

#endif /* ILI9341_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
