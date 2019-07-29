#include "ZiXiangGuan.h"
#include "adc.h"

	u16 adc1[SENDBUFF_SIZE];//保存ADC1的数据
	u16 adc2[SENDBUFF_SIZE];//保存ADC2的数据
	u32 adc_sum[SENDBUFF_SIZE];//ADC的和
	extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA在内存的数组
	extern u32 counter;//定时器中断的控制变量
void Get_Adc_Data()//传入数组的地址
{

		u16 j;
		counter = SENDBUFF_SIZE;//给控制变量赋值
		while(counter);//等待采集完成
		for(j=0;j<SENDBUFF_SIZE;j++)//将数据保存下来放入* adc1和* adc2
		{
					adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
					adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
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
		adc_sum[i] = adc_sum[i]	+	adc1[j]	*	adc2[j+i];
		}
	}
	
	
	for(i=0;i<SENDBUFF_SIZE;i++)
	if(adc_sum[i]>=adc_sum[max])
		max = i;
	
	return max*0.000002*50000;
}



