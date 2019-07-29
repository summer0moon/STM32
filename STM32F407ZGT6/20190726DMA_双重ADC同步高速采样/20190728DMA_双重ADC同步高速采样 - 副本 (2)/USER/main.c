#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"


//ALIENTEK ̽����STM32F407������ ʵ��18
//ADCģ��ת��ʵ��-�⺯���汾  
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


extern	u16 adc1[SENDBUFF_SIZE];
extern	u16 adc2[SENDBUFF_SIZE];
int main(void)
{
		//u32 adc1 = 0,adc2 = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	//LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC      
	while(1)
	{ 
		delay_ms(100);
		printf("%d     %d\r\n",adc1[0],adc2[0]);
	}
}



