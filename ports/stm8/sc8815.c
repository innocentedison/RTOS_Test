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

#if defined USE_HARDWARE_IIC


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
  I2C_Init(I2C_SPEED, SC8815_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
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
  I2C->CR2 |= I2C_CR2_ACK; //产生应答信号        
  
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
      I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);//最后一个字节不产生应答信号         
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



void I2C_SC8815_Test(void)
{
  //  unsigned int i;
    unsigned char b;
          
/*  
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
  */
    printf("SC8815 Read out: \n\r");  //从AT24C02里读出数据
    IIC_Read(SC8815_ADDRESS7 , 0x00 , &b , 1);
    printf("0x%X\t", &b);
    printf("\r\n\r\nSC8815 Tested\n\r");
}
#endif

#if defined USE_SOFTWARE_IIC
//模拟I2C------------------------------------------------------------------------//
void Delay_5us(void)
{
   uint8_t i;                   //fcpu 16MHz 时
   for (i=100; i>0; i--);
}


void IIC_Init(void)
{
	GPIO_Init(SCL_PORT, SCL_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SDA_PORT, SDA_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

	IIC_SCL_HIGH;
	IIC_SDA_HIGH;
}

//IIC Start signal
void IIC_Start(void)
{
	SDA_OUT;		//SDA output
	IIC_SCL_HIGH;
	IIC_SDA_HIGH;
	Delay_5us();
	IIC_SDA_LOW;	//START:when CLK is high,DATA change form high to low 
	Delay_5us();
	IIC_SCL_LOW;	//hold I2C, ready to send or receive data
	Delay_5us();
}	

//IIC Stop signal
void IIC_Stop(void)
{
	SDA_OUT;		//SDA output
	IIC_SDA_LOW;	//STOP:when CLK is high DATA change form low to high
	Delay_5us();
	IIC_SCL_HIGH;
	Delay_5us();
	IIC_SDA_HIGH;

}

u8 IIC_Wait_Ack(void)
{
	uint8_t re;

	IIC_SDA_HIGH;		////CPU释放SDA总线
	Delay_5us();
	IIC_SCL_HIGH;
	Delay_5us();

	SDA_IN;
	if(SDA_READ)
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	IIC_SCL_LOW;	
	Delay_5us();

	return re;
}

void IIC_Ack(void)
{
	SDA_OUT;
	IIC_SDA_LOW;
	Delay_5us();
	IIC_SCL_HIGH;
	Delay_5us();
	IIC_SCL_LOW;
	Delay_5us();
	IIC_SDA_HIGH;
}

void IIC_NAck(void)
{
	SDA_OUT;
	IIC_SDA_HIGH;
	Delay_5us();
	IIC_SCL_HIGH;
	Delay_5us();
	IIC_SCL_LOW;
	Delay_5us();
}


void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT;

	for(t=0;t<8;t++)
	{
		if(txd & 0x80)		/* transmit from MSB to LSB*/
		{
			IIC_SDA_HIGH;
		}
		else
		{
			IIC_SDA_LOW;
		}
		Delay_5us();	
		IIC_SCL_HIGH;
		Delay_5us();
		IIC_SCL_LOW;

		if(t == 7)
		{
			IIC_SDA_HIGH;	//8位数据发送完毕后，主机释放SDA，以检测从机应答
		}
		txd <<= 1;			//左移1个bit
		Delay_5us();
	}
}

u8 IIC_Read_Byte(void)
{
	uint8_t i,value = 0;
	
	SDA_IN;

	value = 0;
	for(i = 0;i < 8;i++)
	{
		value <<= 1;
		IIC_SCL_HIGH;
		Delay_5us();

		if(SDA_READ)
		{
			value++;
		}
		IIC_SCL_LOW;
		Delay_5us();
	
	}

	return value;
}



//初始化IIC接口
void SC_8815_Init(void)
{
      IIC_Init();
          /* Configure GPIO for SC8815 A Channel*/
      GPIO_Init(SC8815_1_int_GPIO,SC8815_1_int_PIN, GPIO_MODE_OUT_PP_LOW_FAST); 
      GPIO_Init(CE_1_GPIO,CE_1_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
      GPIO_Init(PSTOP_1_GPIO,PSTOP_1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
      GPIO_Init(VDIS_A_GPIO,VDIS_A_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
      
      PSTOP_1_h;
      CE_1_l;
      VDIS_A_l;
}

//在SC8815指定地址读出一个数据    
//ReadAddr:开始读数的地址     
//返回值  :读到的数据
u8 SC8815_ReadOneByte(u8 ReadAddr)
{
      u8 temp=0;
      IIC_Start();
      IIC_Send_Byte(I2C_SLAVE_ADDRES_WT);
      IIC_Wait_Ack();
      IIC_Send_Byte(ReadAddr);
      IIC_Wait_Ack();
      
      IIC_Start();
      IIC_Send_Byte(I2C_SLAVE_ADDRES_RD);
      IIC_Wait_Ack();
      temp = IIC_Read_Byte();
      IIC_NAck();
      IIC_Stop();
      
      return temp;
}

//在SC8815指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void SC8815_WriteOneByte(u8 WriteAddr, u8 DataToWrite)
{
      
      IIC_Start();
      IIC_Send_Byte(I2C_SLAVE_ADDRES_WT);
      IIC_Wait_Ack();
      IIC_Send_Byte(WriteAddr);
      IIC_Wait_Ack();
      IIC_Send_Byte(DataToWrite);
      IIC_Wait_Ack();
      IIC_Stop();
      
      for(int i = 0; i++; i < 2000) 
      {
         Delay_5us();
      }
      
}

#endif
/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
