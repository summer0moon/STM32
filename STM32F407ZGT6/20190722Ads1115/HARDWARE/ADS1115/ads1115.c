#include "ads1115.h"
#include "delay.h"
#define sum_Max 60000
/*
IIC_SCL PB8
IIC_SDA    PB9
*/
void Ads1115Init(u8 Channel)
{
	IIC_Init();//软件iic初始化
	Ads1115_Config(Channel);//使用通道0，修改通道修改此处
}

void Ads1115_Config(u8 Channel)//Channel为通道
{
		u8 TAB[4]={0xC0,0xD0,0xE0,0xF0};//通道0,1,2,3
	
		IIC_Start();//iic起始信号
	
	IIC_Send_Byte(0x90);//发送写的命令
	IIC_Wait_Ack();//等待应答
	
	IIC_Send_Byte(0x01);//操作配置寄存器	
	IIC_Wait_Ack();
	
	IIC_Send_Byte(TAB[Channel]);//16位寄存器高位
	IIC_Wait_Ack();
	
	IIC_Send_Byte(0xe3);//16位寄存器低位
	IIC_Wait_Ack();
	
	IIC_Stop();//iic结束
}

void Ads1115Init_Pointer()
{
		IIC_Start();//开始
	
		IIC_Send_Byte(0x90);//发出写的命令
	IIC_Wait_Ack();
	
		IIC_Send_Byte(0x00);//指向指针寄存器
	IIC_Wait_Ack();
	
		IIC_Stop();
}

float Ads1115Init_GetADC()
{
	u16 Vaule_L = 0,Vaule_H = 0;
	u16 sum = 0;
	Ads1115Init_Pointer();//指向指针
			IIC_Start();
		IIC_Send_Byte(0x91);//读命令
	IIC_Wait_Ack();
	
	Vaule_H = IIC_Read_Byte(1);//读高8位
	IIC_Ack();
	Vaule_L = IIC_Read_Byte(1);//读低八位
	//IIC_NAck();
	IIC_Stop();
	sum = Vaule_H<<8 | Vaule_L;
	if(sum>sum_Max)
		sum = 0;
	return sum*(3.3/17645.0);
}
