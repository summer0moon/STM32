#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define Sep0 PAout(0)   	//PA0
#define Sep0Turn PAout(1)   	//PA1
#define Sep1 PCout(0)   	//PC0
#define Sep1Turn PCout(1)   	//PC1
#define KEY0 PEin(4)   	//PE4
#define LED0 PEout(5)   	//PE5
#define ENA PEout(10)
#define MS1 PEout(7)
#define MS2 PEout(8)
#define MS3 PEout(9)
void LED_Init(void);//��ʼ��
	 				    
#endif
