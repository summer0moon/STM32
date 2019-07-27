#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "control.h"
#include "lcd.h"
u16 speed=0,speed_L=0;
extern struct _pid pid;
float Angle=0,Angle_Start=0;
u8 Mode=0,Angle_Mode=0,Angle_0=65,Angle_1=110,Start=0,k=60;
int main(void)
{ 
	delay_init(168);	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	//LED_Init();	
	LCD_Init();	 //LCD端口初始
	Adc_Init();  //初始化ADC,PA.1
	KEY_Init();//PE.4
	TIM2_Int_Init(10000-1,72-1);				//使用定时器2的中断72分频
	PID_init();
	speed_L=0;
	speed=0	;
 	TIM3_PWM_Init(2000,720-1);	 //不分频。PWM频率=72000000/900=80Khz
	TIM_SetCompare2(TIM3,speed_L);	//PA.6，输出PWM波 左风机
	TIM_SetCompare1(TIM3,speed);	//PA.7，输出PWM波 右风机
	Angle_Start=Get_Adc(ADC_Channel_1)*(3.3/4096);
	Angle=Get_Angle_Average();//获取角度的值
	 //Angle_0=70;
	 //Angle_1=120;
	 Mode0_Deal(Angle_0);
	// Mode1_Deal(Angle_0,Angle_1);   
   while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
		delay_ms(100);
		KEY_Deal();//键值处理,PE3-Mode,PE6-Add,PE7-Reduce,PE8-Start,PE9-Stop
		DisPlay();
	  Control();//控制
	}
}
