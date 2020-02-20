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
#include "stm8s_i2c.h"
#include "stm8s.h"
#include "atom.h"
#include "atommutex.h"
#include "sc8815.h"

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
//static ATOM_MUTEX sc8815_mutex;


/**
  * @brief  void delay(unsigned int ms)     
  * @param  None
  * @retval None
  */
void delayms(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /* delay recyle */
    for(y = 3000 ; y > 0 ; y--);
}


/**
  * @brief  DeInitializes peripherals used by the I2C device.
  * @param  None
  * @retval None
  */
void IIC_Deinit(void)
{
  /* sc8815_I2C Peripheral Disable */
  I2C_Cmd(DISABLE);

  /* sc8815_I2C DeInit */
  I2C_DeInit();

  /*!< sc8815_I2C Peripheral clock disable */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sc8815_I2C pins: SCL */
  GPIO_Init(GPIOE, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_Init(GPIOE, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
}


/**
  * @brief  Initializes peripherals used by the I2C driver.
  * @param  None
  * @retval None
  */
void IIC_Init(void)
{
  /*!< sc8815_I2C Peripheral clock enable */
//  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  
  /* I2C configuration */
  
  /* sc8815_I2C Peripheral Enable */
  I2C_Cmd( ENABLE);
  
  /* sc8815_I2C configuration after enabling it */
  I2C_Init(I2C_SPEED, I2C_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}


/**
  * @brief  Write data to I2C device.
  * @param  subaddr :    slave device address
            Byte_addr :  offset in slave memory 
            *buffer   :  starting address to send
            num     :    number of bytes to send
  * @retval None
  */
void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{

    //发送起始信号/*!< Send STRAT condition */
    I2C_GenerateSTART( ENABLE);  
    
    /*!< Test on EV5 and clear it *///等待起始信号产生
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    
      /*!< Send EEPROM address for write *///发送器件地地址，并清除SB标志位
    I2C_Send7bitAddress((u8)subaddr, I2C_DIRECTION_TX);
    /*!< Test on EV6 and clear it *///等待器件地址发送完成并清除发送器件地址标志位
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /*!< Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData((u8)(Byte_addr));//发送器件存储首地址
    /*!< Test on EV8 and clear it *///等待移位发送器发送完成
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      
    while(num > 0)
    { /*!< Send the byte to be written *///发送器件存储首地址
        I2C_SendData(*buffer);
        /*!< Test on EV8 and clear it *///等待移位发送器发送完成
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));

        buffer++;
        num--;
    }	
    /*!< Send STOP condition *///发送停止信号结束数据传输
    I2C_GenerateSTOP(ENABLE);
}


/**
  * @brief  Read data from I2C device.
  * @param  subaddr :    slave device address
            Byte_addr :  offset in slave memory 
            *buffer   :  starting address to read
            num     :    number of bytes to read
  * @retval None
  */
void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{
  /*!< While the bus is busy */
  while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
  /*!< Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_ACK_CURR);//产生应答信号        
  
  /*!< Send START condition */
  I2C_GenerateSTART(ENABLE);//发送起始信号
  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));//等待起始信号产生
  
  /*!< Send EEPROM address for write *///发送器件地地址，并清除SB标志位
  I2C_Send7bitAddress((u8)subaddr, I2C_DIRECTION_TX);
  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待器件地址发送完成

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData((u8)(Byte_addr));//发送存储地址
  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));//等待移位发送器发送完成

  
   /*!< Send STRAT condition a second time */
  I2C_GenerateSTART(ENABLE); //重新发送起始信号
  /*!< Test on EV5 and clear it *///等待起始信号产生
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

   /*!< Send EEPROM address for read *///发送器件地地址，并清除SB标志位
  I2C_Send7bitAddress((u8)subaddr, I2C_DIRECTION_RX);

  /*!< Test on EV6 and clear it *///等待器件地址发送完成
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


  while(num)
  {
    if(num == 1)
    {   /*!< Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_ACK_NONE);//最后一个字节不产生应答信号         
        /*!< Send STOP Condition */
      I2C_GenerateSTOP(ENABLE);//发送停止信号结束数据传输         
    }
    /*!< Test on EV7 and clear it */ //等待数据接收完成
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));

    *buffer = I2C_ReceiveData();  //读出数据
    buffer++;   //读出数据缓存地址递加
    num--;      //接收数据数目减1
  }
}


/**
  * @brief  Read 28 byte data out from I2C device.
  * @param  

  * @retval None
  */
/*******************************************************************************
**函数名称:void I2C_Test(void)
**功能描述:向AT24C02里写满128个字节数据，再读出来，验证通过IIC接口写数据与读数据的正确性
**入口参数:
						无
**输出:无
*******************************************************************************/
void I2C_AT24C02_Test(void)
{
    unsigned int i;
    unsigned char I2c_Buf[128];
          
  
    printf("AT24C02EEPROM write in: \n\r");
    for(i = 0; i < 128; i++)
    {   
      I2c_Buf[i] = i ;
      printf("0x%x\t" , I2c_Buf[i]);
      if(i%16 == 15)
      {
        printf("\n\r");
      }
    }
    printf("\n\r");
    //AT24C02 每次最多只能写入8个字节的数据，所以要分段写
    IIC_Write(0xa0 , 0 , I2c_Buf , 8);
    delayms(5);
    IIC_Write(0xa0 , 8 ,  &I2c_Buf[8] , 8);
    delayms(5);
    IIC_Write(0xa0 , 16 , &I2c_Buf[16] , 8);
    delayms(5);
    IIC_Write(0xa0 , 24 , &I2c_Buf[24] , 8);
    delayms(5);
    IIC_Write(0xa0 , 32 , &I2c_Buf[32] , 8);
    delayms(5);
    IIC_Write(0xa0 , 40 , &I2c_Buf[40] , 8);
    delayms(5);
    IIC_Write(0xa0 , 48 , &I2c_Buf[48] , 8);
    delayms(5);
    IIC_Write(0xa0 , 56 , &I2c_Buf[56] , 8);
    delayms(5);
    IIC_Write(0xa0 , 64 , &I2c_Buf[64] , 8);
    delayms(5);
    IIC_Write(0xa0 , 72 , &I2c_Buf[72] , 8);
    delayms(5);
    IIC_Write(0xa0 , 80 , &I2c_Buf[80] , 8);
    delayms(5);
    IIC_Write(0xa0 , 88 , &I2c_Buf[88] , 8);
    delayms(5);
    IIC_Write(0xa0 , 96 , &I2c_Buf[96] , 8);
    delayms(5);
    IIC_Write(0xa0 , 104 , &I2c_Buf[104] , 8);
    delayms(5);
    IIC_Write(0xa0 , 112 , &I2c_Buf[112] , 8);
    delayms(5);
    IIC_Write(0xa0 , 120 , &I2c_Buf[120] , 8);
  
  
    for(i = 0 ; i < 128 ;i++)		//清空缓存数据
    {   
      I2c_Buf[i] = 0;
    }
  
    printf("AT24C02IIRPOM Read out: \n\r");  //从AT24C02里读出数据
    IIC_Read(0xa0 , 0 , I2c_Buf , 128);
  
    
    for(i = 0 ; i < 128 ; i++)
    {	
        if(I2c_Buf[i] != i)				//把读出的数据与写入的数据进行比较，看是否正确
        {
          printf("Error: I2C EEPROMAT24C02\n\r");
            while(1);			//EEPROM AT24C02读写失败，停止等待
        }
        printf("0x%X\t", I2c_Buf[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("\r\n\r\nEEPROM AT24C02\n\r");
}


/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
