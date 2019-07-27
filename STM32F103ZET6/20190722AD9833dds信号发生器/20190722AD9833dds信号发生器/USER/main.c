#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ad9833.h"
 
/************************************************
 ALIENTEK 战舰STM32F103开发板实验13
 TFTLCD显示实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 	
 int main(void)
 {	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200		
	AD9833_Init_GPIO();//AD9833初始化
		delay_ms(1);
	AD9833_WaveSeting(20000,0,SIN_WAVE,0 );//100KHz,	频率寄存器0，正弦波输出 ,初相位0 */
  	while(1) 
	{		
		
		//AD9833_WaveSeting(100000,0,SQU_WAVE,0 );
		
	} 
}
