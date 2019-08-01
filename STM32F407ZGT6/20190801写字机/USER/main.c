#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);	 //串口初始化为115200
  Control_Init();//次函数为步进电机IO口初始化
	TIM3_Int_Init(600-1,84-1);//控制步进电机0
	TIM3_Int_Init(600-1,84-1);//控制步进电机1
	Turn_Init();
	while(1)
	{
		DrawX(100,1);	
	}
}
