#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "spi.h"
#include "mcp41010.h"
#include "key.h"  


//ALIENTEK 探索者STM32F407开发板 实验25
//SPI通信接口实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
 
 
 
//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
int main(void)
{ 
	u8 value = 0;
	u8 R1 = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	//LED_Init();					//初始化LED 
 	//LCD_Init();					//LCD初始化 
	//KEY_Init(); 				//按键初始化  
	mcp41010_Init();
	MCP41010_Write_R(1,255);
	MCP41010_Write_R(2,1);
	while(1)
	{
	   //MCP41010_Write_R(1,255);
		delay_ms(1000);
		//value = KEY_Scan(0);
		//if(value == 2)
			R1++,MCP41010_Write_R(2,R1);
		//MCP41010_Write_R(2,0);
	}       
}

