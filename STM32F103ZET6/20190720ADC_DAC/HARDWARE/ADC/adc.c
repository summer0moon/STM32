#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define COM GPIOA//修改端口需修改此处

void  Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能时钟，修改端口需修改此处
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed  =GPIO_Speed_50MHz; 
	GPIO_Init(COM,&GPIO_InitStruct);
}


u16 Get_Adc(void)   
{
	u16 value = 0;//读取的值
	value = COM->IDR;//读取IO口 
	return value;
}

