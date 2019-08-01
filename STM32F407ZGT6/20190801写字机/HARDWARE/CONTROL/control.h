#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define Sep0 PAout(0)   	//PA0,���Ʋ������0��������
#define Sep0Turn PAout(1)   	//PA1,���Ʋ������0������ת
#define Sep1 PCout(0)   	//PC0,���Ʋ������1��������
#define Sep1Turn PCout(1)   	//PC1,���Ʋ������1������ת
#define ENA PEout(10)  //ʹ�ܶ˿�
#define MS1 PEout(7)	//���Ʋ���ϸ��
#define MS2 PEout(8)	//���Ʋ���ϸ��
#define MS3 PEout(9)	//���Ʋ���ϸ��
void TIM3_Int_Init(u16 arr,u16 psc);//���Ʋ������0
void TIM4_Int_Init(u16 arr,u16 psc);//���Ʋ������1
void Turn_Init(void);//��ʼ�������������,ȷ������ԭ��
void Control_Init(void);//�κ���Ϊ�������IO�ڳ�ʼ��
void Turn0(u16 a,u8 b);//������ת�ͷ�ת�Ĳ�����aΪ��������bΪ1��ת��bΪ0��ת
void Turn1(u16 a,u8 b);//������ת�ͷ�ת�Ĳ�����aΪ��������bΪ1��ת��bΪ0��ת
void DrawX(u16 counter,u8 b);//�ƶ�X����,b=1�����ƶ�,b=0�����ƶ�
void DrawY(u16 counter,u8 b);//�ƶ�Y����,b=1�����ƶ�,b=0�����ƶ�
void Turn_x0y0(int x0,int y0);//�ƶ���x0,y0��
void Tap(u8 a);//1̧�ʺ�0���
void DrawLine(int x0,int y0,int x1,int y1);//x0,y0Ϊ��ʼ���꣬x1��y1Ϊ�յ�����,����ֱ��
void DrawArc(int x0,int y0,int x_1,int y_1,int x_2,int y_2);//x0,y0ΪԲ�����꣬x1��y1Ϊ�������,x2��y2Ϊ�յ�����,����Բ��
void DrawCircle(int x0,int y0,u16 r);//��һ��Բ��x0,y0ΪԲ�ģ�rΪ�뾶
#endif
