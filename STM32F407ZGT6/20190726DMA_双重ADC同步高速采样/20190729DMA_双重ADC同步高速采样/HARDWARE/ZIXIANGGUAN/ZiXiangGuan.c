#include "ZiXiangGuan.h"
#include "adc.h"


void Get_Adc_Data(u16 * adc1,u16 * adc2)//传入数组的地址
{
		extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA在内存的数组
		extern u32 counter;//定时器中断的控制变量
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
	u16 adc1[SENDBUFF_SIZE];//保存ADC1的数据
	u16 adc2[SENDBUFF_SIZE];//保存ADC2的数据
	u32 adc_sum[SENDBUFF_SIZE];//ADC的和
	u16 i;
	Get_Adc_Data(adc1,adc2);
	//for(i=0;i<SENDBUFF_SIZE;i++)
	
	
	return 0;
}



