#include "ZiXiangGuan.h"
#include "adc.h"
#include <Filter.h>
#include "usart.h"
	u16 adc1[SENDBUFF_SIZE];//保存ADC1的数据
	u16 adc2[SENDBUFF_SIZE];//保存ADC2的数据
	u32 adc_sum[SENDBUFF_SIZE];//ADC的和
	extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA在内存的数组
	
extern u16 flag;	
void Get_Adc_Data()//传入数组的地址
{
		u16 j;
	//*****************************************************//dma重置
		while(flag);
		flag = 1;
		DMA_Cmd(DMA2_Stream0,DISABLE);//关闭dma
		//设置传输数据长度  
		DMA_SetCurrDataCounter(DMA2_Stream0,SENDBUFF_SIZE); 
		DMA_Cmd(DMA2_Stream0,ENABLE);//开启dma
		ADC1 ->SR &= 0xFFFFFFDF;//
		ADC2 ->SR &= 0xFFFFFFDF;//
		ADC_SoftwareStartConv(ADC1);
		while(flag);//等待传输完成
	//********************************************************//
	
		for(j=0;j<SENDBUFF_SIZE;j++)//将数据保存下来放入* adc1和* adc2
		{
					adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
					//printf("%d %d\r\n",j,adc1[j]);
					adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
		//printf("\r\n\r\n");
		//for(j=0;j<SENDBUFF_SIZE;j++)
		//{
			//printf("%d %d\r\n",j,adc2[j]);
			
	//	}
}




float ZiXiangGuan_Deal()//获取ADC数据并且计算相位差
{
	u16 i,j;
	u16 max = 0;
	Get_Adc_Data();
	for(i=0;i<SENDBUFF_SIZE;i++)//数组元素相乘求和
	{
		for(j=0;j<SENDBUFF_SIZE;j++)
		{
		if((j+i)<SENDBUFF_SIZE)
		adc_sum[i] = adc_sum[i]	+	adc1[j]	*	adc2[j+i] * 0.1;
		}
	}
	
	
	for(i=0;i<SENDBUFF_SIZE;i++)
	if(adc_sum[i]>=adc_sum[max])
		max = i;
	
	return max*0.000056*100000;
}



