/**
  ******************************************************************************
  * @file led.h
  * @brief This file contains all functions prototype and macros for the LEDs
  * @author Edison Chen
  * @version V0.1
  * @date 02/19/2020
  ***************************************************************************/


#ifndef _LED_H
#define _LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_gpio.h"

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup GPIO_Exported_Types
  * @{
  */

/**/

/* Exported functions ------------------------------------------------------- */
/** @addtogroup GPIO_Exported_Functions
  * @{
  */

void Led_DeInit(void);
void Led_Init(void);
/**
  * @}
  */

#endif /* _LED_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
