#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY_Mode PBin(5)   	//模式设置按键PE3
#define KEY0 PEin(4)   	//PE4
#define KEY_Angle_Change PBin(6)   	//角度切换PE6
#define KEY_Add PBin(7)   	//加数值按键PE7
#define KEY_Reduce PBin(8)   	//减少数值按键PE8
#define KEY_Start PBin(9)   	//开始按键PE9


#define KEY_Mode_PRES 	1	//KEY_Mode按下
#define KEY_Angle_Change_PRES   2	//KEY_Stop按下(即WK_UP/KEY_UP)
#define KEY_Add_PRES	3	//KEY_Add按下
#define KEY_Reduce_PRES	4	//KEY_Reduce按下
#define KEY_Start_PRES	5	//KEY_Start按下

extern u8 Mode;
extern u8 Angle_Mode;
extern u8 Angle_0;
extern u8 Angle_1;
extern u8 Start;
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数		
void KEY_Deal(void);//键值处理
#endif
