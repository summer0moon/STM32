#include "ads1115.h"
#include "delay.h"
#define sum_Max 60000
/*
IIC_SCL PB8
IIC_SDA    PB9
*/
void Ads1115Init(u8 Channel)
{
	IIC_Init();//���iic��ʼ��
	Ads1115_Config(Channel);//ʹ��ͨ��0���޸�ͨ���޸Ĵ˴�
}

void Ads1115_Config(u8 Channel)//ChannelΪͨ��
{
		u8 TAB[4]={0xC0,0xD0,0xE0,0xF0};//ͨ��0,1,2,3
	
		IIC_Start();//iic��ʼ�ź�
	
	IIC_Send_Byte(0x90);//����д������
	IIC_Wait_Ack();//�ȴ�Ӧ��
	
	IIC_Send_Byte(0x01);//�������üĴ���	
	IIC_Wait_Ack();
	
	IIC_Send_Byte(TAB[Channel]);//16λ�Ĵ�����λ
	IIC_Wait_Ack();
	
	IIC_Send_Byte(0xe3);//16λ�Ĵ�����λ
	IIC_Wait_Ack();
	
	IIC_Stop();//iic����
}

void Ads1115Init_Pointer()
{
		IIC_Start();//��ʼ
	
		IIC_Send_Byte(0x90);//����д������
	IIC_Wait_Ack();
	
		IIC_Send_Byte(0x00);//ָ��ָ��Ĵ���
	IIC_Wait_Ack();
	
		IIC_Stop();
}

float Ads1115Init_GetADC()
{
	u16 Vaule_L = 0,Vaule_H = 0;
	u16 sum = 0;
	Ads1115Init_Pointer();//ָ��ָ��
			IIC_Start();
		IIC_Send_Byte(0x91);//������
	IIC_Wait_Ack();
	
	Vaule_H = IIC_Read_Byte(1);//����8λ
	IIC_Ack();
	Vaule_L = IIC_Read_Byte(1);//���Ͱ�λ
	//IIC_NAck();
	IIC_Stop();
	sum = Vaule_H<<8 | Vaule_L;
	if(sum>sum_Max)
		sum = 0;
	return sum*(3.3/17645.0);
}
