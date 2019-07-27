#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define	MCP41010_CS_1 		PBout(13)  		//MCP41010的CS1片选信号
#define	MCP41010_CS_2 		PBout(14)  		//MCP41010的CS2片选信号
#define SCK_1 GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SCK_0 GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define MOSI_1 GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define MOSI_0 GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define MISO	PAin(6)	

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 time); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
		 
#endif

