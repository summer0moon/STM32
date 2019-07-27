#ifndef __control
#define __control	
#include "sys.h"
extern u16 speed;
extern u16 speed_L;
extern float Angle;
extern u8 Mode;
extern u8 Angle_Mode;
extern u8 Angle_0;
extern u8 Angle_1;
extern u8 Start;

struct _pid{
float SetAngle; //定义设定值
float ActualAngle; //定义实际值
float err; //定义偏差值
float err_next; //定义上一个偏差值
float err_last; //定义最上前的偏差值
float Kp,Ki,Kd; //定义比例、积分、微分系数
};

void Control(void);
void PID_init(void);
void Mode0_Deal(float angle);//模式0处理函数
void Mode1_Deal(float angle1,float angle2);//模式1处理函数
int PID_Deal(float angle);//PID处理函数,angle为实际的角度
int PID_Deal_1(float angle);//PID处理函数,angle为实际的角度
void Mode1(float Angle_0);//模式0辅助函数
#endif 
