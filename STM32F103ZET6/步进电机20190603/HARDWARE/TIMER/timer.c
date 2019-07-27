#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
int X=0,Y=0;//坐标系的坐标
u16 step=0;
u8 TAP;
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}




//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
   GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM3
	

}



void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}



//定时器3中断服务程序
u8 j_0=0,j_1=0;
u16 Counter_0=0,Counter_1=0;
u8 IsRun=0;//画直线的状态变量，为1表示画线没有完成

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
if(Counter_0){//第一个电机的脉冲数
	
		Sep0Turn=j_0;
		Sep0=~Sep0;	
		Counter_0--;

		}

}
}


void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
if(Counter_1){//第二个电机的脉冲数
			
			Sep1Turn=j_1;
		  Sep1=~Sep1;	
			Counter_1--;

		}

}
}


void Turn_Init(void)//初始化两个步进电机,确定坐标原点
{
	//DrawX(8,1);
	//while(Counter_0!=0||Counter_1!=0);
	//DrawY(8,1);
	//while(Counter_0!=0||Counter_1!=0);
//	X=0,Y=0;
	ENA=1;
	Sep0=0;
	Sep1=0;
	Tap(1);
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
/*
void DrawPoint(u16 x,u16 y)
{
	Tap(1);
	delay_ms(20);
	Turn_x0y0(x,y);
	Tap(0);
	delay_ms(20);
	Tap(1);
	delay_ms(20);
}
*/

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

void Drawarc(int x1,int y1,int x2,int y2,int r)//x1,y1为起点，x2，y2为终点，r为半径
{
	
	
	
	
	
	
}
	



















