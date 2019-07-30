#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "ZiXiangGuan.h"
float x;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	//LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	//TIM3_Int_Init(10-1,42-1);	
	while(1)
	{ 
		delay_ms(100);
		//Get_Adc_Data();
	x = ZiXiangGuan_Deal();
		printf("%f\r\n",x);
		delay_ms(2000);
	} 
}

