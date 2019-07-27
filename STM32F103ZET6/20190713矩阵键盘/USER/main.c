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
	delay_init();	    	 //��ʱ������ʼ��	  
 	//LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	//BEEP_Init();         	//��ʼ���������˿�
	//LED0=0;					//�ȵ������
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	while(1)
	{
		
		value=KEY_Scan(1);
		printf("KEY=%d\r\n",value);
		delay_ms(100);
		//KEY_Deal(KEY_Scan(1));
		
	}	 
}

