#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"


//ALIENTEK 探索者STM32F407开发板 实验18
//ADC模数转换实验-库函数版本  
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


extern u32 adc_result;
u32 adc_result_0;
int main(void)
{
		u32 adc1 = 0,adc2 = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	//LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC      
	while(1)
	{ 
		//adc_result[0] = ADC->CDR ;
		delay_ms(100);
		adc_result_0 = adc_result;
		adc1 = adc_result_0 & 0x0000ffff;
		adc2 = adc_result_0>>16;
		printf("%d          %d\r\n",adc1,adc2);
	}
}



