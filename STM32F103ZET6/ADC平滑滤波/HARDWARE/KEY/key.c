#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4
	//初始化 WK_UP-->GPIOA.0	  下拉输入
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	//GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1	//KEY_Mode按下
//2	//KEY_Angle_Change按下
//3	//KEY_Add按下
//4	//KEY_Reduce按下
//5	//KEY_Start按下

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_Mode==0||KEY_Add==0||KEY_Reduce==0||KEY_Start==0||KEY_Angle_Change==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY_Mode==0)return KEY_Mode_PRES;
		else if(KEY_Angle_Change==0)return KEY_Angle_Change_PRES;
		else if(KEY_Add==0)return KEY_Add_PRES;
		else if(KEY_Reduce==0)return KEY_Reduce_PRES;
		else if(KEY_Start==0)return KEY_Start_PRES;
	}else if(KEY_Mode==1&&KEY_Add==1&&KEY_Reduce==1&&KEY_Start==1&&KEY_Angle_Change_PRES==1)key_up=1; 	    
 	return 0;// 无按键按下
}


void KEY_Deal()//键值处理
{
		switch(KEY_Scan(1))//键值处理
		{
			case 1:Mode^=0x01;break;//模式转换
			case 2:Angle_Mode^=0x01;break;
			case 3:{
				if(Angle_Mode==0)//加角度值
					Angle_0++;
				else
					Angle_1++;
			};break;
			case 4:{
				if(Angle_Mode==0)//减角度值
					Angle_0--;
				else
					Angle_1--;
			};break;
			case 5:Start^=0x01;break;//开始
			default :break;
		}
}
