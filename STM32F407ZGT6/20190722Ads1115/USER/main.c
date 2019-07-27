#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "24cxx.h"
#include "key.h"  
#include "ads1115.h"

//ALIENTEK 探索者STM32F407开发板 实验24
//IIC 实验 --库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK



//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"Explorer STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
 int main(void)
 {	 
	delay_init(168);	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD 	
	KEY_Init();				//按键初始化	
	Ads1115Init(0);//选择通道0
	while(1)
	{
		float asd = 0;  
		asd = Ads1115Init_GetADC();
		printf("%f\r\n",asd);
		delay_ms(100);
	}
}
