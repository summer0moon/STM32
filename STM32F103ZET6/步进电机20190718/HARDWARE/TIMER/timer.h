#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);//arrΪ��ֵ,pscΪ��Ƶ��
void TIM4_Int_Init(u16 arr,u16 psc);//arrΪ��ֵ,pscΪ��Ƶ��
void Turn_Init(void);//��ʼ�������������,ȷ������ԭ��
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
