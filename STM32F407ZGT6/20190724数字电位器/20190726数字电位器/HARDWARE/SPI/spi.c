#include "spi.h"
#include "delay.h"
/*使用此文件需要spi.c
CS1 PB13
CS2 PB14
SPI SCK PA4
SPI MOSI PA5
SPI MISO PA6
*/
u16 Time = 1;//延时时间
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB14,PB13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
		MCP41010_CS_1=1;			//SPI不选中,拉高，片选信号
		MCP41010_CS_2=1;
	
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//PA4~6复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	SPI1_SetSpeed(1);
}   



void SPI1_SetSpeed(u8 time)
{
	Time = time;
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
//mode:设置时钟模式00,01,10,11
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
		u8 i;	
		SCK_0;
		delay_us(Time);
		MOSI_0;
		delay_us(Time);
		for (i = 0; i < 8; i++ )
		{
				MOSI_0;
				delay_us(Time);
				SCK_0;
				delay_us(Time);
				if (TxData & 0x80) //将data与0x80进行与预算,目的是取得date二进制首位数值,如果首位是1,那么MOSI就会变成高电平,否则就会保持低电平
				{
						MOSI_1;
					delay_us(Time);
				}
				SCK_0;
				delay_us(Time);
				SCK_1;
				delay_us(Time);
				TxData <<= 1 ;  //将date左移,继续循环进行比较
		}	   
}








