#include <stdio.h>
#include <math.h>
#include <Filter.h>

//struct TAdSample AdSample[1];
//�ṹ�帳��ֵ
//struct TAdSample AdSample[1]=
//{
// 0,
// {0,},
// 0,
// 50,
// 0,
// 0,
// 0,
// 0, 
//};	
////��ȡADC��ֵ�ĺ���
//int Get_Adc()
//{	
//	return 2048+rand()%10;	
//}

////ƽ���˲�
//float Avg_Filter(struct TAdSample *p)//����N�μ���ƽ��ֵ
//{
//  for(int i=0;i<p[0].len;i++)
//  {
//  }
// return 0;
//}

float Avg_Filter2(int adm, struct TAdSample *p)//ÿ�β�������ƽ��ֵ,����ƽ���˲�
{
	p[0].sum=p[0].sum+adm-p[0].buf[p[0].i];//���ɾ��ԭ����ֵ
	p[0].buf[p[0].i]=adm;//���µ�ֵ��buf
	p[0].i++;
	if(p[0].i>=p[0].len) //������ѭ��
	{
		p[0].i=0;
		p[0].bover=1;//�����������־λ
	}
	
	p[0].avg=p[0].sum/p[0].len;//��ǰ��ƽ��ֵ
	
	if(!p[0].bover)//���û�������ǰ��ADCֵ����Ϊƽ��ֵ
	{
		p[0].avg=adm;
	}
	
	return p[0].avg;
}


//void main()
//{
//	ADC_Init();
//	while(1)
//	{
//		//0.1s
//	int i;
//	//float avg =	Avg_Filter2(Get_Adc(),&AdSample[0]);
//	for(i=0;i<sizeof(AdSample)/sizeof(AdSample[0]);i++)//�õ�����ĳ���
//		Avg_Filter2(Get_Adc(),&AdSample[0]);//
//	}  
//}

