#include "timer.h"
#include "led.h"
#include "adc.h"
void TIM4_PWM_Init(u16 arr,u16 psc)
{
				NVIC_InitTypeDef			NVIC_InitStruct;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
       TIM_TimeBaseStructure.TIM_Period = arr;
        TIM_TimeBaseStructure.TIM_Prescaler =psc;         
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;         
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     
     TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
       TIM_OCInitStructure.TIM_Pulse = 0;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OCInitStructure.TIM_Pulse=arr/2;
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM4, ENABLE);

		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
		NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//DMA2通道
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//子优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//抢占
		NVIC_Init(&NVIC_InitStruct);
	
    TIM_Cmd(TIM4, ENABLE); //ENABLE TIM4
	
}
extern u32	adc_result_IRQ[SENDBUFF_SIZE];
//定时器4中断服务函数
u16 adc1[SENDBUFF_SIZE];
u16 adc2[SENDBUFF_SIZE];
//DMA中断服务函数
u16 j;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
			DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
		for(j=0;j<SENDBUFF_SIZE;j++)
		{
			adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
			adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
			DMA_Enable(DMA2_Stream0);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
