#ifndef __MCP41010_H
#define __MCP41010_H			    
#include "sys.h"  

void mcp41010_Init(void);//初始化mcp41010
void MCP41010_Write_SR(u8 Channel,u16 sr);//写入一个数据
void MCP41010_Write_R(u8 Channel,u8 R);//向模块写入电阻值
#define	MCP41010_CS_1 		PBout(13)  		//MCP41010的CS1片选信号
#define	MCP41010_CS_2 		PBout(14)  		//MCP41010的CS2片选信号
#endif
















