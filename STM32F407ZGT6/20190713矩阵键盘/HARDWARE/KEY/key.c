#include "key.h"
#include "delay.h"
#include "stm32f4xx_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//������ʼ������
#define COM GPIOE		//ѡ���ʼ���˿ڣ��������˿ڣ��˴���Ҫ�޸�   
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��,�������˿ڣ��˴���Ҫ�޸�
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(COM, &GPIO_InitStructure);//����Ϊ��������


}

u8 TAB[4][4]={	//��ѯ���������������ı��
{1,2,3,4},
{5,6,7,8},
{9,10,11,12},
{13,14,15,16},
};
u8 KEY_Scan(u8 mode)
{
	int Key_X_Value=0,Key_Y_Value=0;//�������˿ڼĴ�����ֵ
	static u8 key_up=1;//�������ɿ���־
	int x=0,y=0;//�������������
	if(mode)key_up=1;  //֧������
	
	COM ->MODER &= 0xFFFFFF00;//����λ��������
	COM ->MODER |= 0x55;;//���ö˿ڵ���λΪͨ�����
	COM ->OTYPER &= 0xFFF0;//����λ����Ϊ����
	COM ->BSRRH |= 0x0F; //����λ����
	
	COM ->MODER &= 0xFFFF00FF;//����λ�������㣬����Ϊ����
	COM ->PUPDR |= 0x5500;//��������λ
	
	delay_us(10);
	Key_X_Value = COM->IDR&0xf0;//��ȡ�˿ڸ���λ��ֵ,��ɨ��
	Key_X_Value = Key_X_Value>>4;//������λ

	if(key_up&&(Key_X_Value!=0x0f))//��������
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		
		switch(Key_X_Value)
		{
			case 0x0e:x=4;break;//1110
			case 0x0d:x=3;break;//1101
			case 0x0b:x=2;break;//1011
			case 0x07:x=1;break;//0111
			default :break;
		}
		
	COM ->MODER &= 0xFFFFFF00;//����λ��������,����Ϊ����
	COM ->PUPDR |= 0x55; //��������λ
		
		
	
	COM ->MODER &= 0xFFFF00FF;//����λ��������
	COM ->MODER |= 0x5500;//���ö˿ڸ���λΪ���ͨ�����
	COM ->OTYPER &= 0xFF0F;//����λ����Ϊ����
	COM ->BSRRH |= 0xF0; //����λ����͵�ƽ
	delay_us(10);
	Key_Y_Value = COM->IDR&0x0f;//��ȡ�˿ڵ���λ��ֵ,��ɨ��
		
				switch(Key_Y_Value)
		{
			case 0x0e:y=4;break;//1110
			case 0x0d:y=3;break;//1101
			case 0x0b:y=2;break;//1011
			case 0x07:y=1;break;//0111
			default :break;
		}
		
		return TAB[x-1][y-1];//���ذ�����ֵ
	}else if(Key_X_Value==0x0f)key_up=1; 	    
 	return 0;// �ް�������
}

void KEY_Deal(u8 value)
{
	
	switch(value)
	{
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:break;
		case 5:break;
		case 6:break;
		case 7:break;
		case 8:break;
		case 9:break;
		case 10:break;
		case 11:break;
		case 12:break;
		case 13:break;
		case 14:break;
		case 15:break;
		case 16:break;
		default :break;
	}
	
	
}
