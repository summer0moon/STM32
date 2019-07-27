#ifndef __ads1115
#define __ads1115
#include "myiic.h"   
void Ads1115Init(u8 Channel);//ads1115初始化，Channel为选择通道
void Ads1115_Config(u8 Channel);//ads1115配置寄存器,Channel为选择通道
void Ads1115Init_Pointer(void);//Ads1115指向指针寄存器,为读数据做准备
float Ads1115Init_GetADC(void);//获取ADC的值
#endif
















