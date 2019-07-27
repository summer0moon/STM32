#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"


//ALIENTEK ̽����STM32F407������ ʵ��3
//��������ʵ��-�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 


int main(void)
{ 
 	u8 value=0;
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿� 
	BEEP_Init();      //��ʼ���������˿�
	KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�
	LED0=0;				  	//�ȵ������
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	while(1)
	{
		value=KEY_Scan(1);
		printf("KEY=%d\r\n",value);
		delay_ms(100);
		//KEY_D
	}

}
