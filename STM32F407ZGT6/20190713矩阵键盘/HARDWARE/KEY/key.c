#include "key.h"
#include "delay.h"
#include "stm32f4xx_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//按键初始化函数
#define COM GPIOE		//选择初始化端口，若更换端口，此处需要修改   
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTE时钟,若更换端口，此处需要修改
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(COM, &GPIO_InitStructure);//设置为上拉输入


}

u8 TAB[4][4]={	//查询按键行数和列数的表格
{1,2,3,4},
{5,6,7,8},
{9,10,11,12},
{13,14,15,16},
};
u8 KEY_Scan(u8 mode)
{
	int Key_X_Value=0,Key_Y_Value=0;//存放输入端口寄存器的值
	static u8 key_up=1;//按键按松开标志
	int x=0,y=0;//存放行数和列数
	if(mode)key_up=1;  //支持连按
	
	COM ->MODER &= 0xFFFFFF00;//低四位配置清零
	COM ->MODER |= 0x55;;//设置端口低四位为通用输出
	COM ->OTYPER &= 0xFFF0;//低四位设置为推挽
	COM ->BSRRH |= 0x0F; //低四位清零
	
	COM ->MODER &= 0xFFFF00FF;//高四位配置清零，设置为输入
	COM ->PUPDR |= 0x5500;//上拉高四位
	
	delay_us(10);
	Key_X_Value = COM->IDR&0xf0;//读取端口高四位的值,行扫描
	Key_X_Value = Key_X_Value>>4;//右移四位

	if(key_up&&(Key_X_Value!=0x0f))//按键按下
	{
		delay_ms(10);//去抖动 
		key_up=0;
		
		switch(Key_X_Value)
		{
			case 0x0e:x=4;break;//1110
			case 0x0d:x=3;break;//1101
			case 0x0b:x=2;break;//1011
			case 0x07:x=1;break;//0111
			default :break;
		}
		
	COM ->MODER &= 0xFFFFFF00;//低四位配置清零,设置为输入
	COM ->PUPDR |= 0x55; //上拉低四位
		
		
	
	COM ->MODER &= 0xFFFF00FF;//高四位配置清零
	COM ->MODER |= 0x5500;//设置端口高四位为输出通用输出
	COM ->OTYPER &= 0xFF0F;//高四位设置为推挽
	COM ->BSRRH |= 0xF0; //高四位输出低电平
	delay_us(10);
	Key_Y_Value = COM->IDR&0x0f;//读取端口低四位的值,列扫描
		
				switch(Key_Y_Value)
		{
			case 0x0e:y=4;break;//1110
			case 0x0d:y=3;break;//1101
			case 0x0b:y=2;break;//1011
			case 0x07:y=1;break;//0111
			default :break;
		}
		
		return TAB[x-1][y-1];//返回按键的值
	}else if(Key_X_Value==0x0f)key_up=1; 	    
 	return 0;// 无按键按下
}

void KEY_Deal(u8 value)
{
	
	switch(value)
	{
		case 1:break;
		case 2:break;
		case 3:break;
		case 4:break;
		case 5:break;
		case 6:break;
		case 7:break;
		case 8:break;
		case 9:break;
		case 10:break;
		case 11:break;
		case 12:break;
		case 13:break;
		case 14:break;
		case 15:break;
		case 16:break;
		default :break;
	}
	
	
}
