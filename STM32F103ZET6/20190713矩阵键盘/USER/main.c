#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "beep.h"
#include "usart.h"
 int main(void)
 {
	 u8 value=0;
 	vu8 key=0;	
	delay_init();	    	 //延时函数初始化	  
 	//LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	//BEEP_Init();         	//初始化蜂鸣器端口
	//LED0=0;					//先点亮红灯
	uart_init(115200);	 //串口初始化为115200
	while(1)
	{
		
		value=KEY_Scan(1);
		printf("KEY=%d\r\n",value);
		delay_ms(100);
		//KEY_Deal(KEY_Scan(1));
		
	}	 
}

