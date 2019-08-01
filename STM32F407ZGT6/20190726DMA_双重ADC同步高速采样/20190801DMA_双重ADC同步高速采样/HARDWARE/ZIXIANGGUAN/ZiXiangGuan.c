#include "ZiXiangGuan.h"
#include "adc.h"
#include <Filter.h>
#include "usart.h"
#include "arm_math.h"
#include "delay.h"
float32_t adc1[2048];//保存ADC1的数据
float32_t adc2[2048];//保存ADC2的数据
extern int32_t	adc_result_IRQ[SENDBUFF_SIZE];//DMA在内存的数组
extern u16 flag;	
void Get_Adc_Data()//传入数组的地址
{
		u16 j;
		int32_t max;
		uint32_t place;
		float adc1_max,adc2_max;
	//*****************************************************//dma重置
		j = 0;
		while(flag)//等待传输完成
		{
			delay_ms(1);
			j++;
			if(j>=5000)//超时退出
			return ;
		}
		flag = 1;
		DMA_Cmd(DMA2_Stream0,DISABLE);//关闭dma
		//设置传输数据长度  
		DMA_SetCurrDataCounter(DMA2_Stream0,SENDBUFF_SIZE+1); 
		DMA_Cmd(DMA2_Stream0,ENABLE);//开启dma
		ADC1 ->SR &= 0xFFFFFFDF;//
		ADC2 ->SR &= 0xFFFFFFDF;//
		//ADC_SoftwareStartConv(ADC1);
				j = 0;
		while(flag)//等待传输完成
		{
			delay_ms(1);
			j++;
			if(j>=5000)//超时退出
			return;
		}
		
	//********************************************************//
		arm_max_q31(adc_result_IRQ,SENDBUFF_SIZE,&max,&place);//取出最大值
	
		adc1_max = (u16)(max & 0x0000ffff)*3.3/4095;//得到ADC1的最大值
		adc2_max = (u16)((max & 0xffff0000)>>16)*3.3/4095;//得到ADC2的最大值
	
		for(j=0;j<SENDBUFF_SIZE;j++)//将数据保存下来放入* adc1和* adc2
		{
					adc1[j*2] = (u16)(adc_result_IRQ[j] & 0x0000ffff)*3.3/4095 - adc1_max/2.0f;
					adc1[j*2+1] = 0;
					//printf("%d %f\r\n",j,adc1[j*2]);
					adc2[j*2] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16)*3.3/4095 - adc2_max/2.0f;
					adc2[j*2+1] = 0;
					//printf("%d %f\r\n",j,adc2[j*2]);
		}
		
//		for(j=0;j<SENDBUFF_SIZE;j++)
//		{
//			printf("%d %f\r\n",j,adc1[j*2]);
//		}
//		
//		printf("\r\n\r\n");
//		
//				for(j=0;j<SENDBUFF_SIZE;j++)
//		{
//			printf("%d %f\r\n",j,adc2[j*2]);
//		}
		

}


float32_t fft_outputbuf[1024];//fft转换后的结果 幅值 实数
extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len1024;
arm_cfft_radix4_instance_f32 scfft;//定义fft结构体
float maxValue[2];
uint32_t testIndex[2];
float ZiXiangGuan_Deal()//获取ADC数据并且计算相位差
{
//	u16 i;
	Get_Adc_Data();//获取两个通道的ADC值

//             for(i=0;i<SENDBUFF_SIZE;i++)//生成信号序列
//						{
//                 adc1[2*i] = 3.3*arm_sin_f32(2*PI*i*100.0/1000.0);
//								 adc1[2*i+1] = 0; 
//            }
						arm_cfft_radix4_init_f32(&scfft,SENDBUFF_SIZE,0,1);//初始化结构体
            arm_cfft_radix4_f32(&scfft,adc1);    //ADC1进行FFT计算（基4）
            arm_cfft_radix4_f32(&scfft,adc2);    //ADC2进行FFT计算（基4）
            arm_cmplx_mag_f32(adc1,fft_outputbuf,SENDBUFF_SIZE);    //把ADC1运算结果复数求模得幅值
						arm_max_f32(fft_outputbuf,SENDBUFF_SIZE/2,&maxValue[0],&testIndex[0]);
						arm_cmplx_mag_f32(adc2,fft_outputbuf,SENDBUFF_SIZE);    //把ADC2运算结果复数求模得幅值
						arm_max_f32(fft_outputbuf,SENDBUFF_SIZE/2,&maxValue[1],&testIndex[1]);
						
	
	return 0;
}


