#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "ad9833.h"
//ALIENTEK ̽����STM32F407������ ʵ��13
//LCD��ʾʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{	
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					  //��ʼ��LED
 	LCD_Init();           //��ʼ��LCD FSMC�ӿ�	
	AD9833_Init_GPIO();//AD9833��ʼ��
		delay_ms(1);
	AD9833_WaveSeting(100000,0,SIN_WAVE,0);//100KHz,	Ƶ�ʼĴ���0�����Ҳ���� ,����λ0 */
  	while(1) 
	{
		
		
	} 
}



