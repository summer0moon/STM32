#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "lcd.h"

/************************************************
 ALIENTEK战舰STM32开发板实验8
 定时器中断实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

extern int temp;

 int main(void)
 {		
 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	TIM2_Int_Init(10000,7200-1);//10Khz的计数频率，计数到5000为500ms
	TIM4_Int_Init(0xffff,0); 
	TIM3_PWM_Init(100-1,7200-1);	 //不分频。PWM频率=72000000/900=80Khz
	TIM_SetCompare2(TIM3,50);

   	while(1)
	{
		//printf("%d\r\n",temp);
		//delay_ms(100);
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Frequency");
	  	LCD_ShowNum(30,40+30,temp,9,24);
		  LCD_ShowString(150,70,210,24,24,"Hz");
		
	}	 

 
}	 
 
