#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "lcd.h"

//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
extern int temp;
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	TIM2_Int_Init(10000,8400-1);//1s�ж�
	TIM4_Int_Init(0xffff,0); //PB7
 	TIM3_PWM_Init(10-1,0);	//PB4     
  TIM_SetCompare1(TIM3,5);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	LCD_Init();
	
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
