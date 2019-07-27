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
	float SetAngle; //�����趨ֵ
	float ActualAngle; //����ʵ��ֵ
	float err; //����ƫ��ֵ
	float err_next; //������һ��ƫ��ֵ
	float err_last; //��������ǰ��ƫ��ֵ
	float Kp,Ki,Kd; //������������֡�΢��ϵ��z
	float err_sum;
	float u;//pid���
	float MaxActualAngle;//����ʵ�ʽǶ�
	float MinActualAngle;//��С��ʵ�ʽǶ�
	float Maxu;//�������
	float Minu;//��С�����
	float du;//���������
	float SetAngle_r;//�趨ֵ�Ĺ�һֵ
	float ActualAngle_r;//ʵ��ֵ�Ĺ�һֵ
	float err_r;//ƫ��Ĺ�һֵ
	float umax;//�����ͻ�����������ֵ
	float umin;//�����ͻ����������Сֵ
};

void Control(void);
void PID_init(void);
void Mode0_Deal(float angle);//ģʽ0������
void Mode1_Deal(float angle1,float angle2);//ģʽ1������
int PID_Deal(float angle);//PID������,angleΪʵ�ʵĽǶ�
int PID_Deal_1(float angle);//PID������,angleΪʵ�ʵĽǶ�
void Mode1(float Angle_0);//ģʽ0��������
#endif 
