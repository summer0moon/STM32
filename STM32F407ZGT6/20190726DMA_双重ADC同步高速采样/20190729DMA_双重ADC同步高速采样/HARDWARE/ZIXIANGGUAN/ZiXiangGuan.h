#ifndef __ZiXiangGuan
#define __ZiXiangGuan
#include "sys.h"
 
void Get_Adc_Data(u16 * adc1,u16 * adc2);//传入数组的地址
float ZiXiangGuan_Deal(void);//获取ADC数据并且计算相位差 				    
#endif
