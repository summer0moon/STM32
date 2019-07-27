#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			30  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[12][USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA[12];         		//����״̬���	
extern u8 i;
extern u8 j;
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART1_Receive(void);//���ڽ��պ���
u8 GetLen(u8 *p);//��ȡ�ַ����ĳ���
void USART1_Deal(void);//�������ݴ���
//float TurnNumber(u8 *p);//���ַ���ת��Ϊ����
void ReceiveWenHao(void);//���յ���
void Receive$$(void);//���յ�$$
void Receive$G(void);//���յ�$G
void Receive$(void);//���յ�$��ͷ�Ĵ���
void ReceiveG0X(void);//���յ�G0X
void ReceiveG0Z(void);//���յ�G0Z0
void ReceiveG0(void);//���յ�G0
void ReceiveG1(void);//���յ�G1
void ReceiveG(void);//���յ�G��ͷ�Ĵ���
#endif


