#include<iostm8s105c6.h>
#include <stdint.h>
#include <stdio.h>
#include"usart3.h"


/*******************************************************************************
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
void Delayms(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
    for(y = 3000 ; y > 0 ; y--);
}
/*******************************************************************************
**��������:void IIC_Init() 
**��������:��ʼ��IIC1�ӿ�
**��ڲ���:
**���:��
*******************************************************************************/
void IIC_Init()
{					

    
    I2C_CR2_bit.SWRST = 1;             //I2C1->CR1 |= 1 << 15; 		//��λI2C1�Ĵ���
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    I2C_CR2_bit.SWRST = 0;            	//�����λ����
    
    //I2C1->CR2 &= ~I2C_CR2_FREQ;												//
    I2C_FREQR_bit.FREQ = 16;      	//Freq = 36M

    I2C_CCRH_bit.F_S = 1;   		//���ڿ���ģʽ
    I2C_CCRH_bit.DUTY = 0;
    I2C_CCRL = 40;           		//����I2C1��ʱ��Ƶ��Ϊ200KHZ  I2CFreq = PCLK1 / CCR / DUTY[λ]		=   16MHZ / 40 / 2 = 200KHZ
    
    I2C_TRISER_bit.TRISE = 16 + 1;								
    
   // I2C1->CR1 &= ~I2C_CR1_SMBUS; 	//����ΪI2Cģʽ
    I2C_CR1_bit.ENGC = 1;		//����Ϊ�㲥����ģʽ
    I2C_CR1_bit.PE = 1;		        //��I2C(ʹ��I2C)
	
}


/*******************************************************************************
**��������:void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
**��������:��IIC����д����
**��ڲ���:
          subaddr :  ��������ַ
          Byte_addr : ȷ������д��ַ����ʼ��ַ
          *buffer   : д���ݵ���ַ��ַ
          num				: Ҫд���ݵĸ���
**���:��
*******************************************************************************/
void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{
	unsigned char i2csr1;
	
	
	while(I2C_SR3_bit.BUSY == 1);          //�ж�I2Cģ���Ƿ�æ
	
	//������ʼ�ź�
        I2C_CR1_bit.PE = 1;
	I2C_CR2_bit.START = 1;    
	while(I2C_SR1_bit.SB == 0);	//�ȴ���ʼ�źŲ���
	i2csr1 = I2C_SR1; //SR1.AF
	I2C_DR = (subaddr);		//���������ص�ַ�������SB��־λ
	while(I2C_SR1_bit.ADDR == 0);	//�ȴ�������ַ�������
	i2csr1 = I2C_SR1;
	i2csr1 = I2C_SR3;		//��״̬�Ĵ���1��״̬�Ĵ���3�������������ַ��־λ
	

	I2C_DR = Byte_addr;             //���������洢�׵�ַ

	while(I2C_SR1_bit.BTF == 0);  //�ȴ���λ�������������
	i2csr1 = I2C_SR1;	      //���BIT��־λ

	while(num > 0)
	{
            I2C_DR = *buffer;		//���������洢�׵�ַ

            while(I2C_SR1_bit.BTF == 0);//�ȴ���λ�������������
            i2csr1 = I2C_SR1;		//���BIT��־λ
            buffer++;
            num--;
	}
	I2C_CR2_bit.STOP = 1;   	//����ֹͣ�źŽ������ݴ���
}


/*******************************************************************************
**��������:void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
**��������:��IIC����������
**��ڲ���:
          subaddr :  ��������ַ
          Byte_addr : ȷ������д��ַ����ʼ��ַ
          *buffer   : �����ݵĻ�������ʼ��ַ
          num				: Ҫ�����ݵĸ���
**���:��
*******************************************************************************/
void IIC_Read(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
{
  unsigned char i2csr1;
  
  
 
  
  I2C_CR2_bit.ACK = 1;	        //����Ӧ���ź�
  
  I2C_CR2_bit.START = 1;	//������ʼ�ź�
  while(I2C_SR1_bit.SB == 0);	//�ȴ���ʼ�źŲ���
  i2csr1 = I2C_SR1;		//SR1.AF??
  I2C_DR = subaddr;		//���������ص�ַ�������SB��־λ
  while(I2C_SR1_bit.ADDR == 0);	//�ȴ�������ַ�������
  i2csr1 = I2C_SR1;
  i2csr1 = I2C_SR3;		//��״̬�Ĵ���1��״̬�Ĵ���3�������������ַ��־λ

  I2C_DR = Byte_addr;
  while(I2C_SR1_bit.BTF == 0);//�ȴ���λ�������������
  i2csr1 = I2C_SR1;	//���BIT��־λ
  
  //���·�����ʼ�ź�
  I2C_CR2_bit.START = 1;//I2C1->CR1 |= I2C_CR1_START;
  while(I2C_SR1_bit.SB == 0);//�ȴ���ʼ�źŲ���

  i2csr1 = I2C_SR1;//SR1.AF??
  I2C_DR = (char)(subaddr | 0x01);	//���������ص�ַ�������SB��־λ
  while(I2C_SR1_bit.ADDR == 0);         //�ȴ�������ַ�������
  i2csr1 = I2C_SR1;
  i2csr1 = I2C_SR3;			//��״̬�Ĵ���1��״̬�Ĵ���2�������������ַ��־λ

  while(num)
  {
    if(num == 1)
    {
        I2C_CR2_bit.ACK = 0;          //���һ���ֽڲ�����Ӧ���ź�
        I2C_CR2_bit.STOP = 1;         //����ֹͣ�źŽ������ݴ���
    }

    while(I2C_SR1_bit.RXNE == 0);
    i2csr1 = I2C_SR1;

    *buffer = I2C_DR;	
    buffer++;
    num--;
  }
}


/*******************************************************************************
**��������:void I2C_Test(void)
**��������:��AT24C02��д��128���ֽ����ݣ��ٶ���������֤ͨ��IIC�ӿ�д����������ݵ���ȷ��
**��ڲ���:
						��
**���:��
*******************************************************************************/
void I2C_AT24C02_Test(void)
{
    unsigned int i;
    unsigned char I2c_Buf[128];
          
  
    printf("��AT24C02EEPROMд�������Ϊ��\n\r");
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
    //AT24C02 ÿ�����ֻ��д��8���ֽڵ����ݣ�����Ҫ�ֶ�д
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
  
  
    for(i = 0 ; i < 128 ;i++)		//��ջ�������
    {   
      I2c_Buf[i] = 0;
    }
  
    printf("��AT24C02IIRPOM����������Ϊ��\n\r");  //��AT24C02���������
    IIC_Read(0xa0 , 0 , I2c_Buf , 128);
  
    
    for(i = 0 ; i < 128 ; i++)
    {	
        if(I2c_Buf[i] != i)				//�Ѷ�����������д������ݽ��бȽϣ����Ƿ���ȷ
        {
            printf("����I2C EEPROMAT24C02д������������ݲ�һ��\n\r");
            while(1);			//EEPROM AT24C02��дʧ�ܣ�ֹͣ�ȴ�
        }
        printf("0x%X\t", I2c_Buf[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("\r\n\r\nEEPROM AT24C02��дһ�£��ɹ�������\n\r");
}