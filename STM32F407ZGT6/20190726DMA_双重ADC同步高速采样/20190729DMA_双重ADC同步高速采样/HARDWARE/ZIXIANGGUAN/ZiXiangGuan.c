#include "ZiXiangGuan.h"
#include "adc.h"


void Get_Adc_Data(u16 * adc1,u16 * adc2)//��������ĵ�ַ
{
		extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA���ڴ������
		extern u32 counter;//��ʱ���жϵĿ��Ʊ���
		u16 j;
		counter = SENDBUFF_SIZE;//�����Ʊ�����ֵ
		while(counter);//�ȴ��ɼ����
		for(j=0;j<SENDBUFF_SIZE;j++)//�����ݱ�����������* adc1��* adc2
		{
					adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
					adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
}




float ZiXiangGuan_Deal()//��ȡADC���ݲ��Ҽ�����λ��
{
	u16 adc1[SENDBUFF_SIZE];//����ADC1������
	u16 adc2[SENDBUFF_SIZE];//����ADC2������
	u32 adc_sum[SENDBUFF_SIZE];//ADC�ĺ�
	u16 i;
	Get_Adc_Data(adc1,adc2);
	//for(i=0;i<SENDBUFF_SIZE;i++)
	
	
	return 0;
}



