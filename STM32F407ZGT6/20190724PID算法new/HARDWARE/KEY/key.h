#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY_Mode PBin(5)   	//ģʽ���ð���PE3
#define KEY0 PEin(4)   	//PE4
#define KEY_Angle_Change PBin(6)   	//�Ƕ��л�PE6
#define KEY_Add PBin(7)   	//����ֵ����PE7
#define KEY_Reduce PBin(8)   	//������ֵ����PE8
#define KEY_Start PBin(9)   	//��ʼ����PE9


#define KEY_Mode_PRES 	1	//KEY_Mode����
#define KEY_Angle_Change_PRES   2	//KEY_Stop����(��WK_UP/KEY_UP)
#define KEY_Add_PRES	3	//KEY_Add����
#define KEY_Reduce_PRES	4	//KEY_Reduce����
#define KEY_Start_PRES	5	//KEY_Start����

extern u8 Mode;
extern u8 Angle_Mode;
extern u8 Angle_0;
extern u8 Angle_1;
extern u8 Start;
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��		
void KEY_Deal(void);//��ֵ����
#endif
