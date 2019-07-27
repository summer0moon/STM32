#include "power.h"
#include "adc.h"
struct POWER Power;//����ṹ�����

void Power_Init()
{
	Power.Capacity=0;
	Power.I_0=0;
	Power.I_1=0;
	Power.Percent=0;
	Power.Sum_Power_0=0;
	Power.Sum_Power_1=0;

}

float Get_Capacity()//��õ������,ʹ��ADC1ͨ��1
{
	float val = Get_Adc_Average(ADC_Channel_1,5);
	
	Power.Capacity = ((val/4095) - Capacity_V_Low)/(Capacity_V_High - Capacity_V_Low)*100;
	return Power.Capacity;//���ص������,��ʾ�ٷ�֮����
}


float Get_I_0()//����豸0�ĵ���,ʹ��ADC1ͨ��2
{
	float val = Get_Adc_Average(ADC_Channel_2,5);
	
	Power.I_0 = (val/4095)*(I_0_High-I_0_Low)+I_0_Low;
	return Power.I_0;//�����豸0������С	
}


float Get_I_1()//����豸1�ĵ���,ʹ��ADC1ͨ��3
{
	float val = Get_Adc_Average(ADC_Channel_3,5);
	
	Power.I_1 = (val/4095)*(I_1_High-I_1_Low)+I_1_Low;
	return Power.I_1;//�����豸1������С	
}



float Get_Percent()//��ȡ�豸0���ٷֱ�
{
	return Power.Percent;
}


void Get_Sum_I()//��ȡ�����ܺ�
{
	Power.Sum_Power_0 += Power.Sum_Power_0 + Power.I_0 * Time;
	Power.Sum_Power_1 += Power.Sum_Power_0 + Power.I_1 * Time;
}



