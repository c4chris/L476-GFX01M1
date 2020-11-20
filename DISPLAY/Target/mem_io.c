
/**
  ******************************************************************************
  * File Name          : Target/mem_io.c
  * Description        : This file provides code for the configuration
  *                      of the MEM IO instances.
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
#include "mem_io.h"

/* Number of MEM instances */
#define   MEM_INSTANCES_NBR     1U

/* MEM Read command ID */
#define   MEM_CMD_READ          0x03

/**
  * @brief  Read Data from Serial Periphiral Memory Interface.
  * @param  Instance MEM Instance
  * @param  pData pointer to Data Memory to be filled
  * @param  Size pointer to Data Memory to be filled
  * @retval BSP status
  */
int32_t BSP_MEM_ReadData(uint32_t Instance, uint32_t Address, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= MEM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint8_t cmd[4] = { MEM_CMD_READ
                     , ((Address >> 16) & 0xFF)
                     , ((Address >>  8) & 0xFF)
                     , (Address & 0xFF)
                     };

    /* Reset the nCS pin */
    MEM_CS_LOW();

    ret = MEM_SPI_Send(cmd, 4);
    if(ret == BSP_ERROR_NONE)
    {
      ret = MEM_SPI_Recv(pData, Size);
    }

    /* Set the nCS */
    MEM_CS_HIGH();
  }

  return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
