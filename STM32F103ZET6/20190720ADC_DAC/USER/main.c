#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usart.h"	
#include "adc.h"
/************************************************
 ALIENTEKս��STM32������ʵ��1
 �����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

u16 x = 0;
 int main(void)
 {	
	delay_init();	    //��ʱ������ʼ��	  
	 Adc_Init();
	 	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	 uart_init(115200);
	 printf("564556\r\n");
	while(1)
	{

	x =	Get_Adc();
		delay_ms(10);
		printf("%d\r\n",x);
		delay_ms(10);
	}
 }
