#include "power.h"
#include "adc.h"
struct POWER Power;//定义结构体变量

void Power_Init()
{
	Power.Capacity=0;
	Power.I_0=0;
	Power.I_1=0;
	Power.Percent=0;
	Power.Sum_Power_0=0;
	Power.Sum_Power_1=0;

}

float Get_Capacity()//获得电池容量,使用ADC1通道1
{
	float val = Get_Adc_Average(ADC_Channel_1,5);
	
	Power.Capacity = ((val/4095) - Capacity_V_Low)/(Capacity_V_High - Capacity_V_Low)*100;
	return Power.Capacity;//返回电池容量,显示百分之多少
}


float Get_I_0()//获得设备0的电流,使用ADC1通道2
{
	float val = Get_Adc_Average(ADC_Channel_2,5);
	
	Power.I_0 = (val/4095)*(I_0_High-I_0_Low)+I_0_Low;
	return Power.I_0;//返回设备0电流大小	
}


float Get_I_1()//获得设备1的电流,使用ADC1通道3
{
	float val = Get_Adc_Average(ADC_Channel_3,5);
	
	Power.I_1 = (val/4095)*(I_1_High-I_1_Low)+I_1_Low;
	return Power.I_1;//返回设备1电流大小	
}



float Get_Percent()//获取设备0充电百分比
{
	return Power.Percent;
}


void Get_Sum_I()//获取电流总和
{
	Power.Sum_Power_0 += Power.Sum_Power_0 + Power.I_0 * Time;
	Power.Sum_Power_1 += Power.Sum_Power_0 + Power.I_1 * Time;
}



