#ifndef __POWER_H
#define __POWER_H	 
#include "sys.h"
#define Capacity_V_High 12
#define Capacity_V_Low  5
#define I_0_High 12
#define I_0_Low  5
#define I_1_High 12
#define I_1_Low  5
#define Time 1
struct POWER
{	
	float Capacity;//电池容量
	float I_0;//0号设备的电流
	float I_1;//1号设备的电流
	float Sum_Power_0;//0号设备的充电总和
	float	Sum_Power_1;//1号设备的充电总和
	float Percent;//设备0充电百分比
};
void Power_Init(void);//初始化
float Get_Capacity(void);//获得电池容量,使用ADC1通道1
float Get_I_0(void);//获得设备0的电流,使用ADC1通道2
float Get_I_1(void);//获得设备1的电流,使用ADC1通道3
float Get_Percent(void);//获取设备0充电百分比
void Get_Sum_I(void);//获取电流总和
#endif
