#include <stdio.h>
#include <math.h>
#include <Filter.h>

//struct TAdSample AdSample[1];

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


