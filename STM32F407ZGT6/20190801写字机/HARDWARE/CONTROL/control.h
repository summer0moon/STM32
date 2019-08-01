#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define Sep0 PAout(0)   	//PA0,控制步进电机0的脉冲数
#define Sep0Turn PAout(1)   	//PA1,控制步进电机0的正反转
#define Sep1 PCout(0)   	//PC0,控制步进电机1的脉冲数
#define Sep1Turn PCout(1)   	//PC1,控制步进电机1的正反转
#define ENA PEout(10)  //使能端口
#define MS1 PEout(7)	//控制步进细分
#define MS2 PEout(8)	//控制步进细分
#define MS3 PEout(9)	//控制步进细分
void TIM3_Int_Init(u16 arr,u16 psc);//控制步进电机0
void TIM4_Int_Init(u16 arr,u16 psc);//控制步进电机1
void Turn_Init(void);//初始化两个步进电机,确定坐标原点
void Control_Init(void);//次函数为步进电机IO口初始化
void Turn0(u16 a,u8 b);//控制正转和反转的步数，a为脉冲数，b为1正转，b为0反转
void Turn1(u16 a,u8 b);//控制正转和反转的步数，a为脉冲数，b为1正转，b为0反转
void DrawX(u16 counter,u8 b);//移动X坐标,b=1正向移动,b=0反向移动
void DrawY(u16 counter,u8 b);//移动Y坐标,b=1正向移动,b=0反向移动
void Turn_x0y0(int x0,int y0);//移动到x0,y0点
void Tap(u8 a);//1抬笔和0落笔
void DrawLine(int x0,int y0,int x1,int y1);//x0,y0为起始坐标，x1，y1为终点坐标,绘制直线
void DrawArc(int x0,int y0,int x_1,int y_1,int x_2,int y_2);//x0,y0为圆心坐标，x1，y1为起点坐标,x2，y2为终点坐标,绘制圆弧
void DrawCircle(int x0,int y0,u16 r);//画一个圆，x0,y0为圆心，r为半径
#endif
