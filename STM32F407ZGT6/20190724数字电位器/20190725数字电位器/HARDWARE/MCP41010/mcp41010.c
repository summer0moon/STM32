#include "mcp41010.h"
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	
/*使用此文件需要spi.c
CS1 PB13
CS2 PB14
SPI SCK PB3
SPI MISO PB4
SPI MOSI PB5
*/
 void mcp41010_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

	  //GPIOB14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB14,PB13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	MCP41010_CS_1=1;			//SPI不选中,拉高，片选信号
	MCP41010_CS_2=1;
	SPI1_Init();		   			//初始化SPI
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_32);		//设置为2.652M时钟,32分频
}


void MCP41010_Write_SR(u8 Channel,u16 sr)  //向模块写入一个16位数据 
{
		if(Channel == 1)
		{
	MCP41010_CS_1=0;                            //使能器件      
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	MCP41010_CS_1=1;                            //取消片选 
		}
		else if(Channel == 2)
		{
	MCP41010_CS_2=0;                            //使能器件      
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	MCP41010_CS_2=1;                            //取消片选 
		}
} 

void MCP41010_Write_R(u8 Channel,u8 R)//向模块写入电阻值
{
	u16 Data_H,Data_L,Data;//数据的高低位
	Data_H = 0x13 ;
	Data_L = R;
	Data = (Data_H<<8) | Data_L;
	MCP41010_Write_SR(Channel,Data);//写入一个16位数据
}





