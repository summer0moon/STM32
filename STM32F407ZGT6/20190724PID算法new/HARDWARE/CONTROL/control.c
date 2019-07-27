#include "sys.h"
#include "control.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "timer.h"
#include "key.h"
#include "usart.h"
#include "niming.h"
#include "adc.h"
#include "LED.h"
#include "lcd.h"

struct _pid pid[1];
						//	��С      0        ���     1       ֵ
float GetY(float x0,float y0,float x1,float y1,float x)//��һ������
{
	float y=y0+(y1-y0)*(x-x0)/(x1-x0);
	return y;
}

float ReturnY(float x0,float y0,float x1,float y1,float x)//��һ����ԭ
{
	float y = (x-y0)/(y1-y0)*(x1-x0)+x0;
	return y;
}

void PID_init(){
	speed = 0;
	speed_L = 0;
	Mode=0,Angle_Mode=0,Angle_0=65,Angle_1=110;
	TIM_SetCompare2(TIM3,speed_L);	//PB.5�����PWM��
	TIM_SetCompare1(TIM3,speed);	//PB.0�����PWM��
	Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ
	pid[0].SetAngle=0.0;
	pid[0].ActualAngle=0.0;
	pid[0].err=0.0;
	pid[0].err_last=0.0;
	pid[0].err_next=0.0;
	pid[0].Kp=0.6;
	pid[0].Ki=0.01;
	pid[0].Kd=1.8;
	pid[0].err_sum=0;
	pid[0].MaxActualAngle=145;//����ʵ�ʽǶ�
	pid[0].MinActualAngle=45;//��С��ʵ�ʽǶ�
	pid[0].Maxu=1.0;//�������
	pid[0].Minu=0;//��С�����
	pid[0].du=0;//���������
	pid[0].SetAngle_r=0;//�趨ֵ�Ĺ�һֵ
	pid[0].ActualAngle_r=0;//ʵ��ֵ�Ĺ�һֵ
	pid[0].err_r=0;//ƫ��Ĺ�һֵ
}

int PID_Deal(float angle){//angleΪʵ�ʵĽǶ�
	int math=0;
	float err_r;//��һ�����ƫ��
	float u_r;//��һ�������
	float OUT;//ԭ�����
	pid[0].SetAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,pid[0].SetAngle);//�趨ֵ��һ������
	pid[0].ActualAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,angle);//�趨ֵ��һ������
	err_r = pid[0].SetAngle_r - pid[0].ActualAngle_r;//ƫ���һ������
		
	if(pid[0].ActualAngle_r>pid[0].SetAngle_r)//�����ۻ�,�����ͻ���
		{
			if(err_r<0)
			{
			pid[0].err_sum = pid[0].err_sum + err_r;
			}
		}else if(pid[0].ActualAngle_r<pid[0].SetAngle_r)
			{
				if(err_r>0)
				{
					pid[0].err_sum = pid[0].err_sum + err_r;
				}
			}
			
	u_r = pid[0].Kp * err_r + pid[0].Ki * pid[0].err_sum + pid[0].Kd * (err_r-pid[0].err);//pid����
	pid[0].du = u_r - pid[0].du;//�������-�ϴεĿ������
	if(u_r>MAX) u_r=MAX;//������Ϸ�
	if(u_r<MIN) u_r=MIN;//������·�
	pid[0].err_r = err_r;//�����ε����д��ṹ��
	pid[0].du = u_r;//���������д��ṹ��
	OUT = ReturnY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,u_r);//��һ����ԭ
		math = (int)(16.031*OUT - 116.08);//���ƺ������
	return   math;//���ؿ������
}

int PID_Deal_1(float angle){//angleΪʵ�ʵĽǶ�
	int math;
	float err_r;//��һ�����ƫ��
	float u_r;//��һ�������
	float OUT;//ԭ�����
	angle=180-angle;//������ԽǶ�
	pid[0].SetAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,pid[0].SetAngle);//�趨ֵ��һ������
	pid[0].ActualAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,angle);//�趨ֵ��һ������
	err_r = pid[0].SetAngle_r - pid[0].ActualAngle_r;//ƫ���һ������
		
	if(pid[0].ActualAngle_r>pid[0].SetAngle_r)//�����ۻ�,�����ͻ���
		{
			if(err_r<0)
			{
			pid[0].err_sum = pid[0].err_sum + err_r;
			}
		}else if(pid[0].ActualAngle_r<pid[0].SetAngle_r)
			if(err_r>0)
			{
			pid[0].err_sum = pid[0].err_sum + err_r;
			}

	u_r = pid[0].Kp * err_r + pid[0].Ki * pid[0].err_sum + pid[0].Kd * (err_r-pid[0].err);//pid����
	pid[0].du = u_r - pid[0].du;//�������-�ϴεĿ������
	if(u_r>MAX) u_r=MAX;//������Ϸ�
	if(u_r<MIN) u_r=MIN;//������·�
	pid[0].err_r = err_r;//�����ε����д��ṹ��
	pid[0].du = u_r;//���������д��ṹ��
	OUT = ReturnY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,u_r);//��һ����ԭ
	OUT=180-OUT;
		math =(int)(15.31*OUT - 67.55);//���ƺ������
	OUT=180-OUT;
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
			pid[0].SetAngle = Angle_0;//���ó�ʼ�Ƕ�
		 	//OUT=Get_Angle_Average();
		  pid[0].ActualAngle=Angle;
		 }
		 else//�Ƕ�ֵ����90��
		 {
			 //OUT = 180 - Get_Angle_Average();
		  pid[0].ActualAngle = 180 - Angle;
			 pid[0].SetAngle = 180 - Angle_0;//���ó�ʼ�Ƕ�
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
			pid[0].SetAngle = Angle_0;//���ó�ʼ�Ƕ�
		 	//OUT=Get_Angle_Average();
		  pid[0].ActualAngle=Angle;
		 }
		 else//�Ƕ�ֵ����90��
		 {
			 //OUT = 180 - Get_Angle_Average();
		  pid[0].ActualAngle = 180 - Angle;
			 pid[0].SetAngle = 180 - Angle_0;//���ó�ʼ�Ƕ�
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


