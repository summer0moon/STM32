#ifndef __MCP41010_H
#define __MCP41010_H			    
#include "sys.h"  

void mcp41010_Init(void);//��ʼ��mcp41010
void MCP41010_Write_SR(u8 Channel,u16 sr);//д��һ������
void MCP41010_Write_R(u8 Channel,u8 R);//��ģ��д�����ֵ
#define	MCP41010_CS_1 		PBout(13)  		//MCP41010��CS1Ƭѡ�ź�
#define	MCP41010_CS_2 		PBout(14)  		//MCP41010��CS2Ƭѡ�ź�
#endif
















