#include "ZiXiangGuan.h"
#include "adc.h"
#include <Filter.h>
#include "usart.h"
#include "arm_math.h"
#include "delay.h"
float32_t adc1[2048];//����ADC1������
float32_t adc2[2048];//����ADC2������
extern int32_t	adc_result_IRQ[SENDBUFF_SIZE];//DMA���ڴ������
extern u16 flag;	
void Get_Adc_Data()//��������ĵ�ַ
{
		u16 j;
		int32_t max;
		uint32_t place;
		float adc1_max,adc2_max;
	//*****************************************************//dma����
		j = 0;
		while(flag)//�ȴ��������
		{
			delay_ms(1);
			j++;
			if(j>=5000)//��ʱ�˳�
			return ;
		}
		flag = 1;
		DMA_Cmd(DMA2_Stream0,DISABLE);//�ر�dma
		//���ô������ݳ���  
		DMA_SetCurrDataCounter(DMA2_Stream0,SENDBUFF_SIZE+1); 
		DMA_Cmd(DMA2_Stream0,ENABLE);//����dma
		ADC1 ->SR &= 0xFFFFFFDF;//
		ADC2 ->SR &= 0xFFFFFFDF;//
		//ADC_SoftwareStartConv(ADC1);
				j = 0;
		while(flag)//�ȴ��������
		{
			delay_ms(1);
			j++;
			if(j>=5000)//��ʱ�˳�
			return;
		}
		
	//********************************************************//
		arm_max_q31(adc_result_IRQ,SENDBUFF_SIZE,&max,&place);//ȡ�����ֵ
	
		adc1_max = (u16)(max & 0x0000ffff)*3.3/4095;//�õ�ADC1�����ֵ
		adc2_max = (u16)((max & 0xffff0000)>>16)*3.3/4095;//�õ�ADC2�����ֵ
	
		for(j=0;j<SENDBUFF_SIZE;j++)//�����ݱ�����������* adc1��* adc2
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


float32_t fft_outputbuf[1024];//fftת����Ľ�� ��ֵ ʵ��
extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len1024;
arm_cfft_radix4_instance_f32 scfft;//����fft�ṹ��
float maxValue[2];
uint32_t testIndex[2];
float ZiXiangGuan_Deal()//��ȡADC���ݲ��Ҽ�����λ��
{
//	u16 i;
	Get_Adc_Data();//��ȡ����ͨ����ADCֵ

//             for(i=0;i<SENDBUFF_SIZE;i++)//�����ź�����
//						{
//                 adc1[2*i] = 3.3*arm_sin_f32(2*PI*i*100.0/1000.0);
//								 adc1[2*i+1] = 0; 
//            }
						arm_cfft_radix4_init_f32(&scfft,SENDBUFF_SIZE,0,1);//��ʼ���ṹ��
            arm_cfft_radix4_f32(&scfft,adc1);    //ADC1����FFT���㣨��4��
            arm_cfft_radix4_f32(&scfft,adc2);    //ADC2����FFT���㣨��4��
            arm_cmplx_mag_f32(adc1,fft_outputbuf,SENDBUFF_SIZE);    //��ADC1������������ģ�÷�ֵ
						arm_max_f32(fft_outputbuf,SENDBUFF_SIZE/2,&maxValue[0],&testIndex[0]);
						arm_cmplx_mag_f32(adc2,fft_outputbuf,SENDBUFF_SIZE);    //��ADC2������������ģ�÷�ֵ
						arm_max_f32(fft_outputbuf,SENDBUFF_SIZE/2,&maxValue[1],&testIndex[1]);
						
	
	return 0;
}


