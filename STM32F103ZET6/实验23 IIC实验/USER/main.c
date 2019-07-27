#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usmart.h"	 
#include "24cxx.h"	 

 
/************************************************
 ALIENTEKս��STM32������ʵ��23
 IIC ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 				 	
//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
	int buff=0;
 int main(void)
 {	 
	u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD 	
	KEY_Init();				//������ʼ��		 	 	
	AT24CXX_Init();			//IIC��ʼ��   
	while(1)
	{
		buff=PCF8591_RcvByte(0x90);
		printf("buff=%d\r\n",buff);
		//PCF8591_SendByte(0x90,255);
		delay_ms(100);
	}
}
