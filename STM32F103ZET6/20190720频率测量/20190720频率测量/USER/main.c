#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "lcd.h"

/************************************************
 ALIENTEKս��STM32������ʵ��8
 ��ʱ���ж�ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

extern int temp;

 int main(void)
 {		
 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	TIM2_Int_Init(10000,7200-1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	TIM4_Int_Init(0xffff,0); 
	TIM3_PWM_Init(100-1,7200-1);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM_SetCompare2(TIM3,50);

   	while(1)
	{
		//printf("%d\r\n",temp);
		//delay_ms(100);
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Frequency");
	  	LCD_ShowNum(30,40+30,temp,9,24);
		  LCD_ShowString(150,70,210,24,24,"Hz");
		
	}	 

 
}	 
 
