#include "mcp41010.h"
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
/*ʹ�ô��ļ���Ҫspi.c
CS1 PB13
CS2 PB14
SPI SCK PB3
SPI MISO PB4
SPI MOSI PB5
*/
 void mcp41010_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB14,PB13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	MCP41010_CS_1=1;			//SPI��ѡ��,���ߣ�Ƭѡ�ź�
	MCP41010_CS_2=1;
	SPI1_Init();		   			//��ʼ��SPI
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_32);		//����Ϊ2.652Mʱ��,32��Ƶ
}


void MCP41010_Write_SR(u8 Channel,u16 sr)  //��ģ��д��һ��16λ���� 
{
		if(Channel == 1)
		{
	MCP41010_CS_1=0;                            //ʹ������      
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	MCP41010_CS_1=1;                            //ȡ��Ƭѡ 
		}
		else if(Channel == 2)
		{
	MCP41010_CS_2=0;                            //ʹ������      
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	MCP41010_CS_2=1;                            //ȡ��Ƭѡ 
		}
} 

void MCP41010_Write_R(u8 Channel,u8 R)//��ģ��д�����ֵ
{
	u16 Data_H,Data_L,Data;//���ݵĸߵ�λ
	Data_H = 0x13 ;
	Data_L = R;
	Data = (Data_H<<8) | Data_L;
	MCP41010_Write_SR(Channel,Data);//д��һ��16λ����
}





