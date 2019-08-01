#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "ZiXiangGuan.h"
extern float maxValue[2];
extern uint32_t testIndex[2];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	//LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	TIM3_Int_Init(period-1,1);//时钟42M
	TIM_SetCompare1(TIM3,period/2);
	while(1)
	{ 
		delay_ms(100);
		//Get_Adc_Data();
		ZiXiangGuan_Deal();
		printf("%f   %f\r\n",(float)testIndex[0]/SENDBUFF_SIZE*42000000.0f/period,(float)testIndex[1]/SENDBUFF_SIZE*42000000.0f/period);
		delay_ms(1000);
	} 
}

