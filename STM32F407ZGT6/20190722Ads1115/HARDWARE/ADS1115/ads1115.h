#ifndef __ads1115
#define __ads1115
#include "myiic.h"   
void Ads1115Init(u8 Channel);//ads1115��ʼ����ChannelΪѡ��ͨ��
void Ads1115_Config(u8 Channel);//ads1115���üĴ���,ChannelΪѡ��ͨ��
void Ads1115Init_Pointer(void);//Ads1115ָ��ָ��Ĵ���,Ϊ��������׼��
float Ads1115Init_GetADC(void);//��ȡADC��ֵ
#endif
















