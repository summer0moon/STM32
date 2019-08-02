#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"


//ALIENTEK ̽����STM32F407������ ʵ��21
//DACʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	//LCD_Init();					//LCD��ʼ��
	//Adc_Init(); 				//adc��ʼ��
	//KEY_Init(); 				//������ʼ��
	Dac_Init();		 		//DACͨ��1��ʼ��	
	DAC_Ch1_EscalatorConfig();
	while(1)
	{
			
	}	
}

