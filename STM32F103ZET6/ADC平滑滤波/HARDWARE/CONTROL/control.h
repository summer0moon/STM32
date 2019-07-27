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
float SetAngle; //�����趨ֵ
float ActualAngle; //����ʵ��ֵ
float err; //����ƫ��ֵ
float err_next; //������һ��ƫ��ֵ
float err_last; //��������ǰ��ƫ��ֵ
float Kp,Ki,Kd; //������������֡�΢��ϵ��
};

void Control(void);
void PID_init(void);
void Mode0_Deal(float angle);//ģʽ0������
void Mode1_Deal(float angle1,float angle2);//ģʽ1������
int PID_Deal(float angle);//PID������,angleΪʵ�ʵĽǶ�
int PID_Deal_1(float angle);//PID������,angleΪʵ�ʵĽǶ�
void Mode1(float Angle_0);//ģʽ0��������
#endif 
