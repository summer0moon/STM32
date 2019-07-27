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
						//	最小      0        最大     1       值
float GetY(float x0,float y0,float x1,float y1,float x)//归一化处理
{
	float y=y0+(y1-y0)*(x-x0)/(x1-x0);
	return y;
}

float ReturnY(float x0,float y0,float x1,float y1,float x)//归一化还原
{
	float y = (x-y0)/(y1-y0)*(x1-x0)+x0;
	return y;
}

void PID_init(){
	speed = 0;
	speed_L = 0;
	Mode=0,Angle_Mode=0,Angle_0=65,Angle_1=110;
	TIM_SetCompare2(TIM3,speed_L);	//PB.5，输出PWM波
	TIM_SetCompare1(TIM3,speed);	//PB.0，输出PWM波
	Angle=Get_Angle_Average();//获取角度的值
	pid[0].SetAngle=0.0;
	pid[0].ActualAngle=0.0;
	pid[0].err=0.0;
	pid[0].err_last=0.0;
	pid[0].err_next=0.0;
	pid[0].Kp=0.6;
	pid[0].Ki=0.01;
	pid[0].Kd=1.8;
	pid[0].err_sum=0;
	pid[0].MaxActualAngle=145;//最大的实际角度
	pid[0].MinActualAngle=45;//最小的实际角度
	pid[0].Maxu=1.0;//最大的输出
	pid[0].Minu=0;//最小的输出
	pid[0].du=0;//输出的增量
	pid[0].SetAngle_r=0;//设定值的归一值
	pid[0].ActualAngle_r=0;//实际值的归一值
	pid[0].err_r=0;//偏差的归一值
}

int PID_Deal(float angle){//angle为实际的角度
	int math=0;
	float err_r;//归一化后的偏差
	float u_r;//归一化的输出
	float OUT;//原本输出
	pid[0].SetAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,pid[0].SetAngle);//设定值归一化处理
	pid[0].ActualAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,angle);//设定值归一化处理
	err_r = pid[0].SetAngle_r - pid[0].ActualAngle_r;//偏差归一化处理
		
	if(pid[0].ActualAngle_r>pid[0].SetAngle_r)//误差的累积,抗饱和积分
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
			
	u_r = pid[0].Kp * err_r + pid[0].Ki * pid[0].err_sum + pid[0].Kd * (err_r-pid[0].err);//pid运算
	pid[0].du = u_r - pid[0].du;//控制输出-上次的控制输出
	if(u_r>MAX) u_r=MAX;//输出限上幅
	if(u_r<MIN) u_r=MIN;//输出限下幅
	pid[0].err_r = err_r;//将本次的误差写入结构体
	pid[0].du = u_r;//将本次输出写入结构体
	OUT = ReturnY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,u_r);//归一化还原
		math = (int)(16.031*OUT - 116.08);//控制函数输出
	return   math;//返回控制输出
}

int PID_Deal_1(float angle){//angle为实际的角度
	int math;
	float err_r;//归一化后的偏差
	float u_r;//归一化的输出
	float OUT;//原本输出
	angle=180-angle;//计算相对角度
	pid[0].SetAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,pid[0].SetAngle);//设定值归一化处理
	pid[0].ActualAngle_r = GetY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,angle);//设定值归一化处理
	err_r = pid[0].SetAngle_r - pid[0].ActualAngle_r;//偏差归一化处理
		
	if(pid[0].ActualAngle_r>pid[0].SetAngle_r)//误差的累积,抗饱和积分
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

	u_r = pid[0].Kp * err_r + pid[0].Ki * pid[0].err_sum + pid[0].Kd * (err_r-pid[0].err);//pid运算
	pid[0].du = u_r - pid[0].du;//控制输出-上次的控制输出
	if(u_r>MAX) u_r=MAX;//输出限上幅
	if(u_r<MIN) u_r=MIN;//输出限下幅
	pid[0].err_r = err_r;//将本次的误差写入结构体
	pid[0].du = u_r;//将本次输出写入结构体
	OUT = ReturnY(pid[0].MinActualAngle,0,pid[0].MaxActualAngle,1,u_r);//归一化还原
	OUT=180-OUT;
		math =(int)(15.31*OUT - 67.55);//控制函数输出
	OUT=180-OUT;
	return   math;
}

void Control()//控制函数
{
	if(Start==1)
	{
		Start=0;
		if(Mode==0)//运行第一种模式
		{

			Mode0_Deal(Angle_0);
		}
	
		else if(Mode==1)//运行第二种模式
		{

			Mode1_Deal(Angle_0,Angle_1);
		}
	}
}

extern u8 TIM2_Start;
void Mode0_Deal(float Angle_0)//模式0处理函数
{
	u8 counter=0;
	u16 time=0;
	if(45<=Angle_0<=145)//判断角度是否有误
	{
   if(Angle_0<=145)
	 {
		 if(Angle_0<=90)//角度值小于90°
		 {
			pid[0].SetAngle = Angle_0;//设置初始角度
		 	//OUT=Get_Angle_Average();
		  pid[0].ActualAngle=Angle;
		 }
		 else//角度值大于90°
		 {
			 //OUT = 180 - Get_Angle_Average();
		  pid[0].ActualAngle = 180 - Angle;
			 pid[0].SetAngle = 180 - Angle_0;//设置初始角度
		 }

		 
		 	TIM2_Start=1;  //开启定时器T2
		 while(counter<=5 && time<=900)//判断是否稳定，9s超时退出
		 {
			 if(Angle_0-Angle<0.01 && Angle_0-Angle>=-0.01)//误差在+-0.01
			 counter++;
			 time++; 
			 delay_ms(10);
		 }
		 BEEP=1;
		 delay_ms(500);
		 BEEP=0;
		 
		 time=5000;
		 while(time)//延时5s
		 {
			 time--;
			 delay_ms(1);
		 }
		 TIM2_Start=0;  //关闭定时器T2
		 
	TIM_SetCompare2(TIM3,0);	//PB.5，输出PWM波 左风机
	TIM_SetCompare1(TIM3,1000);	//PB.0，输出PWM波 右风机
		 delay_ms(1000);
		PID_init();
	 }		
	 else
	 { 
		 TIM2_Start=0;  //关闭定时器T2
	 }
	}
	else return;//角度错误返回
}


void Mode1(float Angle_0)//模式0辅助函数
{
	u8 counter=0;
	u16 time=0;
	if(45<=Angle_0<=145)//判断角度是否有误
	{
   if(Angle_0<=145)
	 {
		 if(Angle_0<=90)//角度值小于90°
		 {
			pid[0].SetAngle = Angle_0;//设置初始角度
		 	//OUT=Get_Angle_Average();
		  pid[0].ActualAngle=Angle;
		 }
		 else//角度值大于90°
		 {
			 //OUT = 180 - Get_Angle_Average();
		  pid[0].ActualAngle = 180 - Angle;
			 pid[0].SetAngle = 180 - Angle_0;//设置初始角度
		 }

		 
		 	TIM2_Start=1;  //开启定时器T2
		 while(counter<=5 && time<=500)//判断是否稳定，9s超时退出
		 {
			 if(Angle_0-Angle<0.01 && Angle_0-Angle>=-0.01)//误差在+-0.01
			 counter++;
			 time++;
			 delay_ms(10);
		 }
		 BEEP=1;
		 delay_ms(500);
		 BEEP=0;
		 //time=5000;
		// while(time)//延时5s
		 //{
			// time--;
			 //delay_ms(1);
		 //}
		 TIM2_Start=0;  //关闭定时器T2
		 
	//TIM_SetCompare2(TIM3,0);	//PB.5，输出PWM波 左风机
	//TIM_SetCompare3(TIM3,1000);	//PB.0，输出PWM波 右风机
		// delay_ms(1000);
		PID_init();
	 }		
	 else
	 { 
		 TIM2_Start=0;  //关闭定时器T2
	 }
	}
	else return;//角度错误返回
}



void Mode1_Deal(float angle1,float angle2)//模式1处理函数
{
	u8 i=0;
	if((45<=angle1<=145)&&(45<=angle2<=145))//判断角度是否有误
	{
		for(;i<=3;i++)
		{
			Angle_0=angle1;
		Mode1(angle1);
			Angle_0=angle2;
		Mode1(angle2);
		}
			TIM_SetCompare2(TIM3,1000);	//PB.5，输出PWM波 左风机
	TIM_SetCompare1(TIM3,0);	//PB.0，输出PWM波 右风机
		 delay_ms(1000);
		PID_init();
	}
	else return;//角度错误返回
}


