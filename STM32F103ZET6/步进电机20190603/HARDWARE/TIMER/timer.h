#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);//arr为初值,psc为分频数
void TIM4_Int_Init(u16 arr,u16 psc);//arr为初值,psc为分频数
void Turn_Init(void);//初始化两个步进电机,确定坐标原点
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
