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

/**
  * @brief Deinitializes the led GPIOs registers to their default reset
  * values.
  * @param[in] None
  * @retval None
  */
void Led_DeInit(void)
{
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOD);  
    GPIO_DeInit(GPIOE);  
}

/**
  * @brief initializes the led GPIOs registers to preset
  * values.
  * @param[in] None
  * @retval None
  */
void Led_Init(void)
{
     /* Configure GPIO for Charger work mode indication(D6/PE0/standard,D7/PD0/fast,D8/PD2/maintaining) */
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOD);  
    GPIO_DeInit(GPIOE);
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
    
    /* Configure GPIO for Charging status indication(D1/PA2,D2/PD7,D3/PD5,D4/PA6,D5/PA4,D6/PE0) */
    GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOA, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);

    /* Configure GPIO for Charging channel indication 3 color LED(Orange/PD4,Green/PD3) */
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/