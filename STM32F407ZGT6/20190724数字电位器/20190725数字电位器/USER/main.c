#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "spi.h"
#include "mcp41010.h"
#include "key.h"  


//ALIENTEK ̽����STM32F407������ ʵ��25
//SPIͨ�Žӿ�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
 
 
 
//Ҫд�뵽W25Q16���ַ�������
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
int main(void)
{ 
	u8 value = 0;
	u8 R1 = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	//LCD_Init();					//LCD��ʼ�� 
	//KEY_Init(); 				//������ʼ��  
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

