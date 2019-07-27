#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#define COM GPIOE		//选择初始化端口，若更换端口，此处需要修改   
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTE时钟,若更换端口，此处需要修改
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入COM0-COM7
 	GPIO_Init(COM, &GPIO_InitStructure);


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
	
	COM->CRL &=0XFFFF0000;//低四位配置清零
	COM->CRL|=0X00003333;//设置端口低四位为通用推挽输出
	COM->BRR =0x000f; //低四位清零
	
	COM->CRL &=0X0000ffff;//高四位配置清零
	COM->CRL|=0X88880000;//设置端口高四位为上拉/下拉输入
	COM->ODR |=0x00f0; //上拉高四位
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
		
	COM->CRL &=0XFFFF0000;//低四位配置清零
	COM->CRL|=0X00008888;//设置端口低四位为带上拉/下拉的输入
	COM->ODR|=0x000f; //上拉低四位
	
	COM->CRL &=0X0000ffff;//高四位配置清零
	COM->CRL|=0X33330000;//设置端口高四位为输出通用推挽输出
	COM->BRR =0x00f0; //高四位输出低电平
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
