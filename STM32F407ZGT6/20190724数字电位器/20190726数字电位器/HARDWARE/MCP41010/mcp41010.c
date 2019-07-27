#include "mcp41010.h"
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
/*使用此文件需要spi.c
CS1 PB13
CS2 PB14
SPI SCK PA4
SPI MISO PA5
SPI MOSI PA6
*/
void mcp41010_Init()
{
	
	SPI1_Init();		   			//初始化SPI
	
}


void MCP41010_Write_R(u8 Channel,u8 R)//向模块写入电阻值
{
	u16 Data_H,Data_L;//数据的高低位
	Data_H = 0x13 ;
	Data_L = R;
			if(Channel == 1)
		{
	MCP41010_CS_1=0;                            //使能器件      
	SPI1_ReadWriteByte(Data_H);               //写入一个字节 
	SPI1_ReadWriteByte(Data_L);               //写入一个字节
	MCP41010_CS_1=1;                            //取消片选 
		}
		else if(Channel == 2)
		{
	MCP41010_CS_2=0;                            //使能器件      
	SPI1_ReadWriteByte(Data_H);               //写入一个字节 
	SPI1_ReadWriteByte(Data_L);               //写入一个字节
	MCP41010_CS_2=1;                            //取消片选 
		}
}





