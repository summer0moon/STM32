#include "control.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
/*
	Sep0 PAout(0)   	//PA0,控制步进电机0的脉冲数
	Sep0Turn PAout(1) //PA1,控制步进电机0的正反转
	Sep1 PCout(0)   	//PC0,控制步进电机1的脉冲数
	Sep1Turn PCout(1) //PC1,控制步进电机1的正反转
	ENA PEout(10)  		//PE10,使能端口
	MS1 PEout(7)			//PE7,控制步进细分
	MS2 PEout(8)			//PE8,控制步进细分
	MS3 PEout(9)			//PE9,控制步进细分
*/
int X=0,Y=0;//坐标系的坐标
u16 step=0;
u8 TAP;
void Control_Init()//次函数为步进电机IO口初始化
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz ;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//设置步进细分为最高
	MS1=1;//控制步进细分
	MS2=1;//控制步进细分
	MS3=1;//控制步进细分
	ENA=1;//使能
	Sep0=0;//脉冲拉低
	Sep1=0;//脉冲拉低
}

void TIM3_Int_Init(u16 arr,u16 psc)//控制步进电机0
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM4_Int_Init(u16 arr,u16 psc)//控制步进电机1
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
u8 j_0=0,j_1=0;
u16 Counter_0=0,Counter_1=0;
u8 IsRun=0;//画直线的状态变量，为1表示画线没有完成
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		
		if(Counter_0){//第一个电机的脉冲数
		Sep0Turn=j_0;
		Sep0=~Sep0;	
		Counter_0--;
		}

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{

		if(Counter_1){//第二个电机的脉冲数
			
			Sep1Turn=j_1;
		  Sep1=~Sep1;	
			Counter_1--;

		}
		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}


void Turn_Init(void)//初始化两个步进电机,确定坐标原点
{
	ENA=1;
	Sep0=0;
	Sep1=0;
}


void Turn0(u16 counter,u8 b)//控制正转和反转的步数，a为脉冲数，b为1顺时针转，b为0逆时针
{	
	Counter_0=counter;
	j_0=b;
}

void Turn1(u16 counter,u8 b)//控制正转和反转的步数，a为脉冲数，b为1顺时针转，b为0逆时针转
{	
	Counter_1=counter;
	j_1=b;
}

void DrawX(u16 counter,u8 b)//移动X坐标,b=1正向移动,b=0反向移动
{
	//两个电机旋转方向同方向为X移动
	j_0=b;
	j_1=b;
	
	Counter_0=counter;
	Counter_1=counter;
	if(b)
	X=X+counter;
	else
	X=X-counter;	
}

void DrawY(u16 counter,u8 b)//移动Y坐标,b=1正向移动,b=0反向移动
{
	//两个电机旋转方向反方向为Y移动
	j_0=b;
	j_1=b^1;
	Counter_0=counter*2;
	Counter_1=counter*2;
	if(b)
	Y=Y+counter;
	else
	Y=Y-counter;	
}

void Turn_x0y0(int x0,int y0)//移动到x0,y0点
{
	if(x0>10000||y0>10000)
		return;
	IsRun|=0x02;
	ENA=0;
	Tap(1);
		if(x0-X>=0)//移动坐标到起点
	DrawX(x0-X,1);
	else
	DrawX(X-x0,0);

	while(Counter_0!=0||Counter_1!=0);
	if(y0-Y>=0)//移动坐标到起点
	DrawY(y0-Y,1);
	else
	DrawY(Y-y0,0);
		while(Counter_0!=0||Counter_1!=0);
	
	//delay_ms(100);
	//delay_ms(5000);
	//Tap(0);
	//delay_ms(500);
	ENA=1;
	IsRun&=0xfd;
}

void Tap(u8 a)//1抬笔和0落笔
{
	if(a==1){
		TAP=16;
		TIM_SetCompare3(TIM2,TAP);
	delay_ms(500);}
	else{
		TAP=30;
		TIM_SetCompare3(TIM2,TAP);
	delay_ms(500);}
}

void DrawLine(int x0,int y0,int x1,int y1)//x0,y0为起始坐标，x1，y1为终点坐标,绘制直线
{
	
	int Fm=0,x=0,y=0;
	step=0;
	IsRun|=0x01;
	if(TAP==16)
		Tap(0);//落笔
	//Tap(1);//抬笔
	//Turn_x0y0(x0,y0);//移动坐标到起点	
	//Tap(0);//落笔
	ENA=0;
	x=x1-x0;//得出相对坐标
	y=y1-y0;
		if(x>10000||y>10000)
		return;
	if(x>0)
			step=step+x;
	else
			step=step-x;//计算总步数
	
	if(y>0)
			step=step+y;
	else
			step=step-y;//计算总步数
	if(x>0 && y>=0){//第一象限
	for(;step!=0;step--)//循环判断是否到达终点
	{
		if(Fm>=0){//判断偏差
			DrawX(1,1),Fm=Fm-y;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
		else{
			DrawY(1,1),Fm=Fm+x;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
		}//Tap(1);//delay_ms(50);//抬笔
			//Sep0=0;
			//Sep1=0;
			ENA=1;
		//IsRun&=0xfe;
	return;
	}

	if(x<=0 && y>0){//第二象限
			for(;step!=0;step--)//循环判断是否到达终点
			{
			if(Fm>=0){//判断偏差
			DrawX(1,0),Fm=Fm-y;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
			else{
			DrawY(1,1),Fm=Fm-x;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
		}//Tap(1);//delay_ms(50);//抬笔
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}

	if(x<0 && y<=0){//第三象限
			for(;step!=0;step--)//循环判断是否到达终点
			{
			if(Fm>=0){//判断偏差
			DrawX(1,0),Fm=Fm+y;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
			else{
			DrawY(1,0),Fm=Fm-x;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
		}//Tap(1);//delay_ms(50);//抬笔
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}
	
	if(x>=0 && y<0){//第四象限
			for(;step!=0;step--)//循环判断是否到达终点
			{
			if(Fm>=0){//判断偏差
			DrawX(1,1),Fm=Fm+y;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
			else{
			DrawY(1,0),Fm=Fm+x;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差
		}//Tap(1);//delay_ms(50);//抬笔
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}
	//Tap(1);
	//Sep0=0;
//	Sep1=0;
	ENA=1;
	//IsRun&=0xfe;
}

void DrawArc(int x0,int y0,int x_1,int y_1,int x_2,int y_2)//x0,y0为圆心坐标，x1，y1为起点坐标,x2，y2为终点坐标,绘制圆弧
{
	u16 step=0;
	int Fm=0,x1=0,y1=0,x2=0,y2=0,xm=0,ym=0;
		IsRun|=0x01;
	if(TAP==16)
		Tap(0);//落笔
	//得出相对坐标
	x1=x_1-x0;
	y1=y_1-y0;
	x2=x_2-x0;
	y2=y_2-y0;
	xm=x1;
	ym=y1;
	if(x2-x1>0)
			step=step+(x2-x1);
	else
			step=step-(x2-x1);//计算总步数
	
	if(y2-y1>0)
			step=step+(y2-y1);
	else
			step=step-(y2-y1);//计算总步数
	
	if(x1>=0 && y1>=0 && x2>=0 && y2>=0){//第一象限
		if(x2>=x1){//判断顺时针 SR1
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;
		}
		else      {   //判断逆时针 NR1
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;	
		}
	}

	if(x1<=0 && y1>=0 && x2<=0 && y2>=0){//第二象限
		
		if(x2>=x1){//判断顺时针 SR2
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差
				DrawX(1,1),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;
		}
		else      {   //判断逆时针 NR2
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawX(1,0),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;	
		}
	}
	
	
		if(x1<=0 && y1<=0 && x2<=0 && y2<=0){//第三象限
		
		if(x2<=x1){//判断顺时针 SR3
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差 0,-10
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;
		}
		else      {   //判断逆时针 NR3
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差 -10,0
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;	
		}
	}
		
	
		if(x1>=0 && y1<=0 && x2>=0 && y2<=0){//第四象限
		
		if(x2<=x1){//判断顺时针 SR4
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差  10,0
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;
		}
		else      {   //判断逆时针 NR4
			for(;step!=0;step--){//循环判断是否到达终点		
				if(Fm>=0){//判断偏差 0,-10
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
				else{
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//控制步进电机走步,并且计算偏差和坐标
			}
				ENA=1;
				return;	
		}
	}
	ENA=1;
}


void DrawCircle(int x0,int y0,u16 r)//画一个圆，x0,y0为圆心，r为半径
{
			DrawArc(x0,y0,x0+r,0,0,y0+r);
			DrawArc(x0,y0,0,y0+r,x0-r,0);
			DrawArc(x0,y0,x0-r,0,0,y0-r);
			DrawArc(x0,y0,0,y0-r,x0+r,0);
}


