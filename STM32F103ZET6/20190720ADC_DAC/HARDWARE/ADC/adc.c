#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define COM GPIOA//�޸Ķ˿����޸Ĵ˴�

void  Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��ʱ�ӣ��޸Ķ˿����޸Ĵ˴�
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed  =GPIO_Speed_50MHz; 
	GPIO_Init(COM,&GPIO_InitStruct);
}


u16 Get_Adc(void)   
{
	u16 value = 0;//��ȡ��ֵ
	value = COM->IDR;//��ȡIO�� 
	return value;
}

