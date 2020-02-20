/**
  ******************************************************************************
  * @file led.h
  * @brief This file contains all functions prototype and macros for the LEDs
  * @author Edison Chen
  * @version V0.1
  * @date 02/19/2020
  ***************************************************************************/


#ifndef _SC8815_H
#define _SC8815_H

/* Includes ------------------------------------------------------------------*/
#include "atom.h"

/* Exported variables ------------------------------------------------------- */


#define I2C_SPEED              400000
#define I2C_SLAVE_ADDRESS7     0x74
/* Exported types ------------------------------------------------------------*/



/** @addtogroup GPIO_Exported_Types
  * @{
  */

/**/

/* Exported functions ------------------------------------------------------- */
/** @addtogroup GPIO_Exported_Functions
  * @{
  */
void Delayms(unsigned int ms);
void IIC_Deinit(void);
void IIC_Init(void);
void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num);
void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num);
void I2C_AT24C02_Test(void);
/**
  * @}
  */

#endif /* _SC8815_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
