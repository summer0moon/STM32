#include "ZiXiangGuan.h"
#include "adc.h"

	u16 adc1[SENDBUFF_SIZE];//����ADC1������
	u16 adc2[SENDBUFF_SIZE];//����ADC2������
	u32 adc_sum[SENDBUFF_SIZE];//ADC�ĺ�
	extern u32	adc_result_IRQ[SENDBUFF_SIZE];//DMA���ڴ������
	extern u32 counter;//��ʱ���жϵĿ��Ʊ���
void Get_Adc_Data()//��������ĵ�ַ
{

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
	u16 i,j;
	u16 max = 0;
	Get_Adc_Data();
	for(i=0;i<SENDBUFF_SIZE;i++)//����Ԫ��������
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



