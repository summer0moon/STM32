#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
/************************************************
 ALIENTEKս��STM32������ʵ��8
 ��ʱ���ж�ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 a=0;
 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();			     //LED�˿ڳ�ʼ��
	TIM2_PWM_Init(200-1,7200-1);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM3_Int_Init(599,71);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	TIM4_Int_Init(599,71);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	// DrawX(1,1);
	 //TIM_SetCompare3(TIM2,15);
	printf("\r\nGrbl 1.1f ['$' for help]\r\n");
	 Turn_Init();

   	while(1)
	{
		//if(KEY0==0){
		//	delay_ms(20);
			//Tap(a);
		//	a^=0x01;
	 	 //	DrawLine(0,0,100,0);
			//delay_ms(10);
			//DrawLine(1000,0,1000,1000);
			//DrawLine(1000,1000,0,1000);
			//DrawLine(0,1000,0,0);
			//DrawLine(0,0,1000,1000);
			//DrawLine(1000,0,0,1000);
		//USART_RX_STA[0]=USART_RX_STA[0]|0x8000;
		//USART_RX_STA[0]+=6;
			USART1_Deal();//�������ݴ���
		//DrawLine(0,0,0,0);
			//USART1_Receive();
			//DrawLine(0,0,1446,-85);
			//DrawCircle(0,0,1000);
			//DrawX(100,1);
	//}
		}
	}	 

 
 
