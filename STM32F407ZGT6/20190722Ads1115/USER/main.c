#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "24cxx.h"
#include "key.h"  
#include "ads1115.h"

//ALIENTEK ̽����STM32F407������ ʵ��24
//IIC ʵ�� --�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK



//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"Explorer STM32F4 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
	
 int main(void)
 {	 
	delay_init(168);	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD 	
	KEY_Init();				//������ʼ��	
	Ads1115Init(0);//ѡ��ͨ��0
	while(1)
	{
		float asd = 0;  
		asd = Ads1115Init_GetADC();
		printf("%f\r\n",asd);
		delay_ms(100);
	}
}
