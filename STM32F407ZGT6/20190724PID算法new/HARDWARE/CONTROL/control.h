#ifndef __control
#define __control	
#include "sys.h"
#define MAX 1
#define MIN 0
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
	float Kp,Ki,Kd; //定义比例、积分、微分系数z
	float err_sum;
	float u;//pid输出
	float MaxActualAngle;//最大的实际角度
	float MinActualAngle;//最小的实际角度
	float Maxu;//最大的输出
	float Minu;//最小的输出
	float du;//输出的增量
	float SetAngle_r;//设定值的归一值
	float ActualAngle_r;//实际值的归一值
	float err_r;//偏差的归一值
	float umax;//抗饱和积分输出的最大值
	float umin;//抗饱和积分输出的最小值
};

void Control(void);
void PID_init(void);
void Mode0_Deal(float angle);//模式0处理函数
void Mode1_Deal(float angle1,float angle2);//模式1处理函数
int PID_Deal(float angle);//PID处理函数,angle为实际的角度
int PID_Deal_1(float angle);//PID处理函数,angle为实际的角度
void Mode1(float Angle_0);//模式0辅助函数
#endif 
