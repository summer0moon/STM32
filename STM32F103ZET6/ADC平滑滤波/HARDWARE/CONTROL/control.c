#include "sys.h"
#include "control.h"
#include "delay.h"
#include "stm32f10x.h"
#include "timer.h"
#include "key.h"
#include "usart.h"
#include "niming.h"
#include "adc.h"
#include "LED.h"
#include "lcd.h"
#define OUT_Max 145
#define OUT_Min	45

struct _pid pid;



void PID_init(){
speed = 0;
speed_L = 0;
Mode=0,Angle_Mode=0,Angle_0=65,Angle_1=110;
TIM_SetCompare2(TIM3,speed_L);	//PB.5�����PWM��
TIM_SetCompare1(TIM3,speed);	//PB.0�����PWM��
Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ
pid.SetAngle=0.0;
pid.ActualAngle=0.0;
pid.err=0.0;
pid.err_last=0.0;
pid.err_next=0.0;
pid.Kp=0.6;
//pid.Ki=0.015;
//pid.Kd=0.005;
//pid.Kp=0;
pid.Ki=0.01;
pid.Kd=1.8;
}

float OUT=0;
int PID_Deal(float angle){//angleΪʵ�ʵĽǶ�
	float incrementAngle;
	int math=0;
	pid.ActualAngle=angle;
	pid.err=pid.SetAngle-pid.ActualAngle;
	incrementAngle=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
	OUT+=incrementAngle;
	pid.err_last=pid.err_next;
	pid.err_next=pid.err;
		if(OUT>OUT_Max) OUT=OUT_Max;  //�����޴���
	if(OUT<=OUT_Min) OUT=OUT_Min;
		math = (int)(16.031*OUT - 116.08);//(int)(0.000002*OUT*OUT*OUT*OUT + 0.004*OUT*OUT*OUT - 1.3821*OUT*OUT + 147.96*OUT - 3741.4);

		//if(OUT<=60.78)//�ֶ�pwm����45-72
		//math = (int)( 1.1975*OUT + 291.16);
	//else if(OUT>60.78 && OUT<=107.38)//�ֶ�pwm����72-110
		//math = (int)( -0.0241*OUT*OUT*OUT  + 6.5938*OUT*OUT - 544.45*OUT + 14502);
	//else if(OUT>107.78 && OUT<=120.38)//�ֶ�pwm����110-138
		//math = (int)(37.554*OUT - 2044.3);
	//else if(OUT>120.38)//�ֶ�pwm����110-135
		//math = (int)( -0.0012*OUT*OUT*OUT*OUT + 0.6346*OUT*OUT*OUT - 122.59*OUT*OUT + 10523*OUT - 336187);
	return   math;
}

int PID_Deal_1(float angle){//angleΪʵ�ʵĽǶ�
	float incrementAngle;
	int math;
	angle=180-angle;
	pid.ActualAngle=angle;
	pid.err=pid.SetAngle-pid.ActualAngle;
	incrementAngle=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
	OUT+=incrementAngle;
	pid.err_last=pid.err_next;
	pid.err_next=pid.err;
		if(OUT>OUT_Max) OUT=OUT_Max;  //�����޴���
	if(OUT<=OUT_Min) OUT=OUT_Min;
	OUT=180-OUT;
		math =(int)(15.31*OUT - 67.55);
	OUT=180-OUT;
	//if(OUT<=50.78)//�ֶ�pwm����45-72
		//math = (int)( -0.0007*OUT*OUT*OUT*OUT + 0.1686*OUT*OUT*OUT - 14.801*OUT*OUT + 574.79*OUT - 7976.6);
	//else if(OUT>50.78 && OUT<=130.38)//�ֶ�pwm����72-110
		//math = (int)( -0.0241*OUT*OUT*OUT  + 6.5938*OUT*OUT - 544.45*OUT + 14502);
	//else if(OUT>130.38)//�ֶ�pwm����110-135
		//math = (int)( -0.0012*OUT*OUT*OUT*OUT + 0.6346*OUT*OUT*OUT - 122.59*OUT*OUT + 10523*OUT - 336187);
	return   math;
}

void Control()//���ƺ���
{
	if(Start==1)
	{
		Start=0;
		if(Mode==0)//���е�һ��ģʽ
		{

			Mode0_Deal(Angle_0);
		}
	
		else if(Mode==1)//���еڶ���ģʽ
		{

			Mode1_Deal(Angle_0,Angle_1);
		}
	}
}

extern u8 TIM2_Start;
void Mode0_Deal(float Angle_0)//ģʽ0������
{
	u8 counter=0;
	u16 time=0;
	if(45<=Angle_0<=145)//�жϽǶ��Ƿ�����
	{
   if(Angle_0<=145)
	 {
		 if(Angle_0<=90)//�Ƕ�ֵС��90��
		 {
			pid.SetAngle = Angle_0;//���ó�ʼ�Ƕ�
		 	OUT=Get_Angle_Average();
		  pid.ActualAngle=Angle;
		 }
		 else//�Ƕ�ֵ����90��
		 {
			 OUT = 180 - Get_Angle_Average();
		  pid.ActualAngle = 180 - Angle;
			 pid.SetAngle = 180 - Angle_0;//���ó�ʼ�Ƕ�
		 }

		 
		 	TIM2_Start=1;  //������ʱ��T2
		 while(counter<=5 && time<=900)//�ж��Ƿ��ȶ���9s��ʱ�˳�
		 {
			 if(Angle_0-Angle<0.01 && Angle_0-Angle>=-0.01)//�����+-0.01
			 counter++;
			 time++; 
			 delay_ms(10);
		 }
		 BEEP=1;
		 delay_ms(500);
		 BEEP=0;
		 
		 time=5000;
		 while(time)//��ʱ5s
		 {
			 time--;
			 delay_ms(1);
		 }
		 TIM2_Start=0;  //�رն�ʱ��T2
		 
	TIM_SetCompare2(TIM3,0);	//PB.5�����PWM�� ����
	TIM_SetCompare1(TIM3,1000);	//PB.0�����PWM�� �ҷ��
		 delay_ms(1000);
		PID_init();
	 }		
	 else
	 { 
		 TIM2_Start=0;  //�رն�ʱ��T2
	 }
	}
	else return;//�Ƕȴ��󷵻�
}


void Mode1(float Angle_0)//ģʽ0��������
{
	u8 counter=0;
	u16 time=0;
	if(45<=Angle_0<=145)//�жϽǶ��Ƿ�����
	{
   if(Angle_0<=145)
	 {
		 if(Angle_0<=90)//�Ƕ�ֵС��90��
		 {
			pid.SetAngle = Angle_0;//���ó�ʼ�Ƕ�
		 	OUT=Get_Angle_Average();
		  pid.ActualAngle=Angle;
		 }
		 else//�Ƕ�ֵ����90��
		 {
			 OUT = 180 - Get_Angle_Average();
		  pid.ActualAngle = 180 - Angle;
			 pid.SetAngle = 180 - Angle_0;//���ó�ʼ�Ƕ�
		 }

		 
		 	TIM2_Start=1;  //������ʱ��T2
		 while(counter<=5 && time<=500)//�ж��Ƿ��ȶ���9s��ʱ�˳�
		 {
			 if(Angle_0-Angle<0.01 && Angle_0-Angle>=-0.01)//�����+-0.01
			 counter++;
			 time++;
			 delay_ms(10);
		 }
		 BEEP=1;
		 delay_ms(500);
		 BEEP=0;
		 //time=5000;
		// while(time)//��ʱ5s
		 //{
			// time--;
			 //delay_ms(1);
		 //}
		 TIM2_Start=0;  //�رն�ʱ��T2
		 
	//TIM_SetCompare2(TIM3,0);	//PB.5�����PWM�� ����
	//TIM_SetCompare3(TIM3,1000);	//PB.0�����PWM�� �ҷ��
		// delay_ms(1000);
		PID_init();
	 }		
	 else
	 { 
		 TIM2_Start=0;  //�رն�ʱ��T2
	 }
	}
	else return;//�Ƕȴ��󷵻�
}



void Mode1_Deal(float angle1,float angle2)//ģʽ1������
{
	u8 i=0;
	if((45<=angle1<=145)&&(45<=angle2<=145))//�жϽǶ��Ƿ�����
	{
		for(;i<=3;i++)
		{
			Angle_0=angle1;
		Mode1(angle1);
			Angle_0=angle2;
		Mode1(angle2);
		}
			TIM_SetCompare2(TIM3,1000);	//PB.5�����PWM�� ����
	TIM_SetCompare1(TIM3,0);	//PB.0�����PWM�� �ҷ��
		 delay_ms(1000);
		PID_init();
	}
	else return;//�Ƕȴ��󷵻�
}


