#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
/************************************************
 ALIENTEK战舰STM32开发板实验8
 定时器中断实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 a=0;
 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();			     //LED端口初始化
	TIM2_PWM_Init(200-1,7200-1);	 //不分频。PWM频率=72000000/900=80Khz
	TIM3_Int_Init(599,71);//10Khz的计数频率，计数到5000为500ms
	TIM4_Int_Init(599,71);//10Khz的计数频率，计数到5000为500ms
	uart_init(115200);	 //串口初始化为115200
	// DrawX(1,1);
	 //TIM_SetCompare3(TIM2,15);
	printf("\r\nGrbl 1.1f ['$' for help]\r\n");
	 Turn_Init();

   	while(1)
	{
		//if(KEY0==0){
		//	delay_ms(20);
			//Tap(a);
		//	a^=0x01;
	 	 //	DrawLine(0,0,100,0);
			//delay_ms(10);
			//DrawLine(1000,0,1000,1000);
			//DrawLine(1000,1000,0,1000);
			//DrawLine(0,1000,0,0);
			//DrawLine(0,0,1000,1000);
			//DrawLine(1000,0,0,1000);
		//USART_RX_STA[0]=USART_RX_STA[0]|0x8000;
		//USART_RX_STA[0]+=6;
			USART1_Deal();//串口数据处理
		//DrawLine(0,0,0,0);
			//USART1_Receive();
			//DrawLine(0,0,1446,-85);
			//DrawCircle(0,0,1000);
			//DrawX(100,1);
	//}
		}
	}	 

 
 
