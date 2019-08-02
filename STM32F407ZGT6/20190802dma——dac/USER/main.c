#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"


//ALIENTEK 探索者STM32F407开发板 实验21
//DAC实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	//LCD_Init();					//LCD初始化
	//Adc_Init(); 				//adc初始化
	//KEY_Init(); 				//按键初始化
	Dac_Init();		 		//DAC通道1初始化	
	DAC_Ch1_EscalatorConfig();
	while(1)
	{
			
	}	
}

