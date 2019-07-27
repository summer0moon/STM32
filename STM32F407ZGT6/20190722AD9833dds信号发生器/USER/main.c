#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "ad9833.h"
//ALIENTEK 探索者STM32F407开发板 实验13
//LCD显示实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{	
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					  //初始化LED
 	LCD_Init();           //初始化LCD FSMC接口	
	AD9833_Init_GPIO();//AD9833初始化
		delay_ms(1);
	AD9833_WaveSeting(100000,0,SIN_WAVE,0);//100KHz,	频率寄存器0，正弦波输出 ,初相位0 */
  	while(1) 
	{
		
		
	} 
}



