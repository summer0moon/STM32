#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
#define ADC_CDR_ADDRESS    ((uint32_t)0x40012308) //������˫�غ�����ģʽ�� ADC ͨ�ù������ݼĴ����� ADC_CDR)
#define SENDBUFF_SIZE 10
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 							   
void Adc_Init(void); 				//ADCͨ����ʼ��
void DMA_Enable(DMA_Stream_TypeDef* DMA_CHx);
//u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
//u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
//void DMA_InIt(void);//DMA��ʼ��
#endif 















