#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define Sep0 PAout(0)   	//PA0
#define Sep0Turn PAout(1)   	//PA1
#define Sep1 PCout(0)   	//PC0
#define Sep1Turn PCout(1)   	//PC1
#define KEY0 PEin(4)   	//PE4
#define LED0 PEout(5)   	//PE5
#define ENA PEout(10)
#define MS1 PEout(7)
#define MS2 PEout(8)
#define MS3 PEout(9)
void LED_Init(void);//初始化
	 				    
#endif
