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


//#define USE_HARDWARE_IIC
#define USE_SOFTWARE_IIC

/* Exported variables ------------------------------------------------------- */
#if defined USE_HARDWARE_IIC

#define I2C_SPEED              400000
#define SC8815_ADDRESS7           0x74

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
#endif

#if defined USE_SOFTWARE_IIC

#define I2C_SLAVE_ADDR_REF			0x74
#define I2C_SLAVE_ADDRES_RD			0xE9
#define I2C_SLAVE_ADDRES_WT			0xE8

#define SCL_PORT				GPIOE
#define SCL_PIN				GPIO_PIN_1
#define SDA_PORT			          GPIOE
#define SDA_PIN				GPIO_PIN_2

#define SDA_OUT				GPIO_Init(SDA_PORT, SDA_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
#define SDA_IN				GPIO_Init(SDA_PORT, SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
#define SDA_READ				GPIO_ReadInputPin(SDA_PORT,SDA_PIN)

#define IIC_SCL_HIGH			GPIO_WriteHigh(SCL_PORT,SCL_PIN)
#define IIC_SCL_LOW				GPIO_WriteLow(SCL_PORT,SCL_PIN)
#define IIC_SDA_HIGH			GPIO_WriteHigh(SDA_PORT,SDA_PIN)
#define IIC_SDA_LOW				GPIO_WriteLow(SDA_PORT,SDA_PIN)

#define SC8815_1_int_GPIO (GPIOE)
#define SC8815_1_int_PIN (GPIO_PIN_3)
#define CE_1_GPIO (GPIOE)
#define CE_1_PIN  (GPIO_PIN_6)
#define PSTOP_1_GPIO (GPIOE)
#define PSTOP_1_PIN  (GPIO_PIN_7)
#define VDIS_A_GPIO (GPIOB)
#define VDIS_A_PIN  (GPIO_PIN_4)
#define VBAT_1_GPIO (GPIOB)
#define VBAT_1_PIN (GPIO_PIN_0)

#define CE_1_h                                    GPIO_WriteHigh(CE_1_GPIO,CE_1_PIN)
#define CE_1_l                                    GPIO_WriteLow(CE_1_GPIO,CE_1_PIN)
#define PSTOP_1_h                                 GPIO_WriteHigh(PSTOP_1_GPIO,PSTOP_1_PIN)
#define PSTOP_1_l                                 GPIO_WriteLow(PSTOP_1_GPIO,PSTOP_1_PIN)
#define VDIS_A_h                                  GPIO_WriteHigh(VDIS_A_GPIO,VDIS_A_PIN)
#define VDIS_A_l                                  GPIO_WriteLow(VDIS_A_GPIO,VDIS_A_PIN)

//软件I2C
//IIC所有操作函数
void IIC_Delay(void);				//IIC延时函数
void IIC_Init(void);                                        //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			          //IIC发送一个字节
u8 IIC_Read_Byte(void);				//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void SC8815_WriteOneByte(u8 WriteAddr,u8 DataToWrite);
u8 SC8815_ReadOneByte(u8 ReadAddr);
void SC_8815_Init(void);

#endif
/**
  * @}
  */

#endif /* _SC8815_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
