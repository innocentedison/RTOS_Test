/**
  ******************************************************************************
  * @file led.c
  * @brief This file contains all the functions for the leds.
  * @author Edison Chen
  * @version V0.1
  * @date 02/19/2020
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "stm8s_gpio.h"
#include "atom.h"
#include "atommutex.h"
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup Led_Public_Functions
  * @{
  */

/*
 * Semaphore for single-threaded access to led device
 */
static ATOM_MUTEX led_mutex;

/**
  * @brief initializes the led GPIOs registers to preset
  * values.
  * @param[in] None
  * @retval None
  */
int led_init(void)
{
   int status;
  
  /**
   * Set up Led GPIOs for indicate status and indicators.
   */
  Led_GPIOs_Init();


  /* Create a mutex for single-threaded putchar() access */
  if (atomMutexCreate (&led_mutex) != ATOM_OK)
  {
    status = -1;
  }
  else
  {
    status = 0;
  }

  /* Finished */
  return (status);

}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/