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


extern u32 adc_result;
u32 adc_result_0;
int main(void)
{
		u32 adc1 = 0,adc2 = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	//LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC      
	while(1)
	{ 
		//adc_result[0] = ADC->CDR ;
		delay_ms(100);
		adc_result_0 = adc_result;
		adc1 = adc_result_0 & 0x0000ffff;
		adc2 = adc_result_0>>16;
		printf("%d          %d\r\n",adc1,adc2);
	}
}



