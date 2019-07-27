#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
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
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	//GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1	//KEY_Mode����
//2	//KEY_Angle_Change����
//3	//KEY_Add����
//4	//KEY_Reduce����
//5	//KEY_Start����

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_Mode==0||KEY_Add==0||KEY_Reduce==0||KEY_Start==0||KEY_Angle_Change==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_Mode==0)return KEY_Mode_PRES;
		else if(KEY_Angle_Change==0)return KEY_Angle_Change_PRES;
		else if(KEY_Add==0)return KEY_Add_PRES;
		else if(KEY_Reduce==0)return KEY_Reduce_PRES;
		else if(KEY_Start==0)return KEY_Start_PRES;
	}else if(KEY_Mode==1&&KEY_Add==1&&KEY_Reduce==1&&KEY_Start==1&&KEY_Angle_Change_PRES==1)key_up=1; 	    
 	return 0;// �ް�������
}


void KEY_Deal()//��ֵ����
{
		switch(KEY_Scan(1))//��ֵ����
		{
			case 1:Mode^=0x01;break;//ģʽת��
			case 2:Angle_Mode^=0x01;break;
			case 3:{
				if(Angle_Mode==0)//�ӽǶ�ֵ
					Angle_0++;
				else
					Angle_1++;
			};break;
			case 4:{
				if(Angle_Mode==0)//���Ƕ�ֵ
					Angle_0--;
				else
					Angle_1--;
			};break;
			case 5:Start^=0x01;break;//��ʼ
			default :break;
		}
}
