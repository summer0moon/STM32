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
	float Capacity;//�������
	float I_0;//0���豸�ĵ���
	float I_1;//1���豸�ĵ���
	float Sum_Power_0;//0���豸�ĳ���ܺ�
	float	Sum_Power_1;//1���豸�ĳ���ܺ�
	float Percent;//�豸0���ٷֱ�
};
void Power_Init(void);//��ʼ��
float Get_Capacity(void);//��õ������,ʹ��ADC1ͨ��1
float Get_I_0(void);//����豸0�ĵ���,ʹ��ADC1ͨ��2
float Get_I_1(void);//����豸1�ĵ���,ʹ��ADC1ͨ��3
float Get_Percent(void);//��ȡ�豸0���ٷֱ�
void Get_Sum_I(void);//��ȡ�����ܺ�
#endif
