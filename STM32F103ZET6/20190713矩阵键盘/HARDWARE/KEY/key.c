#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#define COM GPIOE		//ѡ���ʼ���˿ڣ��������˿ڣ��˴���Ҫ�޸�   
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��,�������˿ڣ��˴���Ҫ�޸�
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������COM0-COM7
 	GPIO_Init(COM, &GPIO_InitStructure);


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
	
	COM->CRL &=0XFFFF0000;//����λ��������
	COM->CRL|=0X00003333;//���ö˿ڵ���λΪͨ���������
	COM->BRR =0x000f; //����λ����
	
	COM->CRL &=0X0000ffff;//����λ��������
	COM->CRL|=0X88880000;//���ö˿ڸ���λΪ����/��������
	COM->ODR |=0x00f0; //��������λ
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
		
	COM->CRL &=0XFFFF0000;//����λ��������
	COM->CRL|=0X00008888;//���ö˿ڵ���λΪ������/����������
	COM->ODR|=0x000f; //��������λ
	
	COM->CRL &=0X0000ffff;//����λ��������
	COM->CRL|=0X33330000;//���ö˿ڸ���λΪ���ͨ���������
	COM->BRR =0x00f0; //����λ����͵�ƽ
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
