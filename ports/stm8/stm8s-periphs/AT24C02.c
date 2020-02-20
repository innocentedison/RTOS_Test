#include<iostm8s105c6.h>
#include <stdint.h>
#include <stdio.h>
#include"usart3.h"


/*******************************************************************************
**函数名称：void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**功能描述：大概延时
**入口参数：unsigned int ms   输入大概延时数值
**输出：无
*******************************************************************************/
void Delayms(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 3000 ; y > 0 ; y--);
}
/*******************************************************************************
**函数名称:void IIC_Init() 
**功能描述:初始化IIC1接口
**入口参数:
**输出:无
*******************************************************************************/
void IIC_Init()
{					

    
    I2C_CR2_bit.SWRST = 1;             //I2C1->CR1 |= 1 << 15; 		//复位I2C1寄存器
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    I2C_CR2_bit.SWRST = 0;            	//清除复位设置
    
    //I2C1->CR2 &= ~I2C_CR2_FREQ;												//
    I2C_FREQR_bit.FREQ = 16;      	//Freq = 36M

    I2C_CCRH_bit.F_S = 1;   		//用于快速模式
    I2C_CCRH_bit.DUTY = 0;
    I2C_CCRL = 40;           		//设置I2C1的时钟频率为200KHZ  I2CFreq = PCLK1 / CCR / DUTY[位]		=   16MHZ / 40 / 2 = 200KHZ
    
    I2C_TRISER_bit.TRISE = 16 + 1;								
    
   // I2C1->CR1 &= ~I2C_CR1_SMBUS; 	//设置为I2C模式
    I2C_CR1_bit.ENGC = 1;		//设置为广播呼叫模式
    I2C_CR1_bit.PE = 1;		        //打开I2C(使能I2C)
	
}


/*******************************************************************************
**函数名称:void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
**功能描述:向IIC器件写数据
**入口参数:
          subaddr :  从器件地址
          Byte_addr : 确定器件写地址的起始地址
          *buffer   : 写数据的起址地址
          num				: 要写数据的个数
**输出:无
*******************************************************************************/
void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{
	unsigned char i2csr1;
	
	
	while(I2C_SR3_bit.BUSY == 1);          //判断I2C模块是否忙
	
	//发送起始信号
        I2C_CR1_bit.PE = 1;
	I2C_CR2_bit.START = 1;    
	while(I2C_SR1_bit.SB == 0);	//等待起始信号产生
	i2csr1 = I2C_SR1; //SR1.AF
	I2C_DR = (subaddr);		//发送器件地地址，并清除SB标志位
	while(I2C_SR1_bit.ADDR == 0);	//等待器件地址发送完成
	i2csr1 = I2C_SR1;
	i2csr1 = I2C_SR3;		//读状态寄存器1和状态寄存器3清除发送器件地址标志位
	

	I2C_DR = Byte_addr;             //发送器件存储首地址

	while(I2C_SR1_bit.BTF == 0);  //等待移位发送器发送完成
	i2csr1 = I2C_SR1;	      //清除BIT标志位

	while(num > 0)
	{
            I2C_DR = *buffer;		//发送器件存储首地址

            while(I2C_SR1_bit.BTF == 0);//等待移位发送器发送完成
            i2csr1 = I2C_SR1;		//清除BIT标志位
            buffer++;
            num--;
	}
	I2C_CR2_bit.STOP = 1;   	//发送停止信号结束数据传输
}


/*******************************************************************************
**函数名称:void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
**功能描述:向IIC器件读数据
**入口参数:
          subaddr :  从器件地址
          Byte_addr : 确定器件写地址的起始地址
          *buffer   : 读数据的缓冲区起始地址
          num				: 要读数据的个数
**输出:无
*******************************************************************************/
void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{
  unsigned char i2csr1;
  
  
 
  
  I2C_CR2_bit.ACK = 1;	        //产生应答信号
  
  I2C_CR2_bit.START = 1;	//发送起始信号
  while(I2C_SR1_bit.SB == 0);	//等待起始信号产生
  i2csr1 = I2C_SR1;		//SR1.AF??
  I2C_DR = subaddr;		//发送器件地地址，并清除SB标志位
  while(I2C_SR1_bit.ADDR == 0);	//等待器件地址发送完成
  i2csr1 = I2C_SR1;
  i2csr1 = I2C_SR3;		//读状态寄存器1和状态寄存器3清除发送器件地址标志位

  I2C_DR = Byte_addr;
  while(I2C_SR1_bit.BTF == 0);//等待移位发送器发送完成
  i2csr1 = I2C_SR1;	//清除BIT标志位
  
  //重新发送起始信号
  I2C_CR2_bit.START = 1;//I2C1->CR1 |= I2C_CR1_START;
  while(I2C_SR1_bit.SB == 0);//等待起始信号产生

  i2csr1 = I2C_SR1;//SR1.AF??
  I2C_DR = (char)(subaddr | 0x01);	//发送器件地地址，并清除SB标志位
  while(I2C_SR1_bit.ADDR == 0);         //等待器件地址发送完成
  i2csr1 = I2C_SR1;
  i2csr1 = I2C_SR3;			//读状态寄存器1和状态寄存器2清除发送器件地址标志位

  while(num)
  {
    if(num == 1)
    {
        I2C_CR2_bit.ACK = 0;          //最后一个字节不产生应答信号
        I2C_CR2_bit.STOP = 1;         //发送停止信号结束数据传输
    }

    while(I2C_SR1_bit.RXNE == 0);
    i2csr1 = I2C_SR1;

    *buffer = I2C_DR;	
    buffer++;
    num--;
  }
}


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
          
  
    printf("向AT24C02EEPROM写入的数据为：\n\r");
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
    Delayms(5);
    IIC_Write(0xa0 , 8 ,  &I2c_Buf[8] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 16 , &I2c_Buf[16] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 24 , &I2c_Buf[24] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 32 , &I2c_Buf[32] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 40 , &I2c_Buf[40] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 48 , &I2c_Buf[48] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 56 , &I2c_Buf[56] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 64 , &I2c_Buf[64] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 72 , &I2c_Buf[72] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 80 , &I2c_Buf[80] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 88 , &I2c_Buf[88] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 96 , &I2c_Buf[96] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 104 , &I2c_Buf[104] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 112 , &I2c_Buf[112] , 8);
    Delayms(5);
    IIC_Write(0xa0 , 120 , &I2c_Buf[120] , 8);
  
  
    for(i = 0 ; i < 128 ;i++)		//清空缓存数据
    {   
      I2c_Buf[i] = 0;
    }
  
    printf("从AT24C02IIRPOM读出的数据为：\n\r");  //从AT24C02里读出数据
    IIC_Read(0xa0 , 0 , I2c_Buf , 128);
  
    
    for(i = 0 ; i < 128 ; i++)
    {	
        if(I2c_Buf[i] != i)				//把读出的数据与写入的数据进行比较，看是否正确
        {
            printf("错误：I2C EEPROMAT24C02写入与读出的数据不一致\n\r");
            while(1);			//EEPROM AT24C02读写失败，停止等待
        }
        printf("0x%X\t", I2c_Buf[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("\r\n\r\nEEPROM AT24C02读写一致，成功！！！\n\r");
}