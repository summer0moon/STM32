#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "lcd.h"

//ALIENTEK 探索者STM32F407开发板 实验9
//PWM输出实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
extern int temp;
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	TIM2_Int_Init(10000,8400-1);//1s中断
	TIM4_Int_Init(0xffff,0); //PB7
 	TIM3_PWM_Init(10-1,0);	//PB4     
  TIM_SetCompare1(TIM3,5);	//修改比较值，修改占空比
	LCD_Init();
	
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
