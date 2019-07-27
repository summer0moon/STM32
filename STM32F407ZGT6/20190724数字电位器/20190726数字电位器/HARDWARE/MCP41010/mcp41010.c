#include "mcp41010.h"
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
/*ʹ�ô��ļ���Ҫspi.c
CS1 PB13
CS2 PB14
SPI SCK PA4
SPI MISO PA5
SPI MOSI PA6
*/
void mcp41010_Init()
{
	
	SPI1_Init();		   			//��ʼ��SPI
	
}


void MCP41010_Write_R(u8 Channel,u8 R)//��ģ��д�����ֵ
{
	u16 Data_H,Data_L;//���ݵĸߵ�λ
	Data_H = 0x13 ;
	Data_L = R;
			if(Channel == 1)
		{
	MCP41010_CS_1=0;                            //ʹ������      
	SPI1_ReadWriteByte(Data_H);               //д��һ���ֽ� 
	SPI1_ReadWriteByte(Data_L);               //д��һ���ֽ�
	MCP41010_CS_1=1;                            //ȡ��Ƭѡ 
		}
		else if(Channel == 2)
		{
	MCP41010_CS_2=0;                            //ʹ������      
	SPI1_ReadWriteByte(Data_H);               //д��һ���ֽ� 
	SPI1_ReadWriteByte(Data_L);               //д��һ���ֽ�
	MCP41010_CS_2=1;                            //ȡ��Ƭѡ 
		}
}





