#ifndef __Filter_H
#define AD_LENGTH 1000

struct TAdSample//�ýṹ���AD����
{
 float avg;//ADƽ��ֵ
 int buf[AD_LENGTH];//AD������
 int i;//������ָ��
 int len;//����
 int bover;//�Ƿ����ı�־λ
 float max;//�����������ֵ
 float min;	//����������Сֵ
 float limit;//���ֵ����Сֵ�Ĳ�ֵ
 int sum;//buf���ܺ�
};

float Avg_Filter2(int adm, struct TAdSample *p);

#endif
