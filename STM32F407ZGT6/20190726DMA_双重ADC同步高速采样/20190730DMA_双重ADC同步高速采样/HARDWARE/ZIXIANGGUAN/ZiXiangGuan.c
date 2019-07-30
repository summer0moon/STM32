#include "ZiXiangGuan.h"
#include "adc.h"
#include <Filter.h>
#include "usart.h"
	u16 adc1[SENDBUFF_SIZE];//����ADC1������
	u16 adc2[SENDBUFF_SIZE];//����ADC2������
	u32 adc_sum[SENDBUFF_SIZE];//ADC�ĺ�
	extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA���ڴ������
	
extern u16 flag;	
void Get_Adc_Data()//��������ĵ�ַ
{
		u16 j;
	//*****************************************************//dma����
		while(flag);
		flag = 1;
		DMA_Cmd(DMA2_Stream0,DISABLE);//�ر�dma
		//���ô������ݳ���  
		DMA_SetCurrDataCounter(DMA2_Stream0,SENDBUFF_SIZE); 
		DMA_Cmd(DMA2_Stream0,ENABLE);//����dma
		ADC1 ->SR &= 0xFFFFFFDF;//
		ADC2 ->SR &= 0xFFFFFFDF;//
		ADC_SoftwareStartConv(ADC1);
		while(flag);//�ȴ��������
	//********************************************************//
	
		for(j=0;j<SENDBUFF_SIZE;j++)//�����ݱ�����������* adc1��* adc2
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




float ZiXiangGuan_Deal()//��ȡADC���ݲ��Ҽ�����λ��
{
	u16 i,j;
	u16 max = 0;
	Get_Adc_Data();
	for(i=0;i<SENDBUFF_SIZE;i++)//����Ԫ��������
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



