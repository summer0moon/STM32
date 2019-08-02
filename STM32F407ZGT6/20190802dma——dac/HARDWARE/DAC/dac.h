#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	     			    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DAC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
#if defined (USE_STM324xG_EVAL)
  #define DAC_DHR12R2_ADDRESS    0x40007414
  #define DAC_DHR8R1_ADDRESS     0x40007410

#else /* defined (USE_STM324x7I_EVAL)*/ 
  #define DAC_DHR12R2_ADDRESS    0x40007414
  #define DAC_DHR8R1_ADDRESS     0x40007410

#endif
void Dac_Init(void);		//DAC通道1初始化	 	 
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压
void TIM6_Config(void);
void DAC_Ch2_SineWaveConfig(void);
void DAC_Ch1_EscalatorConfig(void);
void DAC_Ch2_TriangleConfig(void);
void DAC_Ch1_NoiseConfig(void);
#endif

















