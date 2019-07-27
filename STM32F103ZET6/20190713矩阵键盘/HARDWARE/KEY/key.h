#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数	
void KEY_Deal(u8);		//按键处理函数
#endif
