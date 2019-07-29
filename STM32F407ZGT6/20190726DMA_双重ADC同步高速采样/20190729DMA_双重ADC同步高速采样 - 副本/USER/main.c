#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "ZiXiangGuan.h"
	extern u32 counter;//定时器中断的控制变量
extern		u16 adc1[SENDBUFF_SIZE];//保存ADC1的数据
extern	u16 adc2[SENDBUFF_SIZE];//保存ADC2的数据
float x = 0;
u16 j;
extern u32	adc_result_IRQ[SENDBUFF_SIZE];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	TIM3_Int_Init(168-1,0);	
	while(1)
	{ 
		delay_ms(100);
//		x = ZiXiangGuan_Deal();//计算相位差
//		LCD_ShowNum(70,150,(u32)x,5,24);
		counter = SENDBUFF_SIZE;//给控制变量赋值
		while(counter);//等待采集完成
		for(j=0;j<SENDBUFF_SIZE;j++)//将数据保存下来放入* adc1和* adc2
		{
					adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
					adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
		for(j=0;j<SENDBUFF_SIZE;j++)
		{
			LCD_ShowNum(70,150,adc1[j],5,24);
			delay_ms(1000);
		} 
	}


}
