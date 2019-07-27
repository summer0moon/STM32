#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "control.h"
#include "lcd.h"
u16 speed=0,speed_L=0;
extern struct _pid pid;
float Angle=0,Angle_Start=0;
u8 Mode=0,Angle_Mode=0,Angle_0=65,Angle_1=110,Start=0,k=60;
 int main(void)
 { 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();	
	LCD_Init();	 //LED�˿ڳ�ʼ
	Adc_Init();  //��ʼ��ADC,PA.1
	KEY_Init();//PE.4
	TIM2_Int_Init(10000-1,72-1);				//ʹ�ö�ʱ��2���ж�72��Ƶ
	PID_init();
	speed_L=0;
	speed=0	;
 	TIM3_PWM_Init(2000,720-1);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM_SetCompare2(TIM3,speed_L);	//PA.6�����PWM�� ����
	TIM_SetCompare1(TIM3,speed);	//PA.7�����PWM�� �ҷ��
	Angle_Start=Get_Adc(ADC_Channel_1)*(3.3/4096);
	Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ
	 //Angle_0=70;
	 //Angle_1=120;
	 //Mode0_Deal(Angle_0);
	// Mode1_Deal(Angle_0,Angle_1);
   	while(1)
	{ 
		
		//delay_ms(100);
//		KEY_Deal();//��ֵ����,PE3-Mode,PE6-Add,PE7-Reduce,PE8-Start,PE9-Stop
//		DisPlay();
//	  Control();//����
		Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ
		printf("%f\r\n",Angle);
		/*
		Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ
		k++;
		printf("%f,%d\r\n",Angle,speed);
	delay_ms(100);	
				if(k>=3)
				{
			k=0;
			LED1 = ~LED1;
			speed--;
			if(speed>=1400)
			 {
				 TIM_SetCompare1(TIM3,1400);	//PB.0�����PWM��  �ҷ��	
			  speed_L = 1400-(speed-1400);
				 if(speed_L>=1400)
					 speed_L=1400;
				TIM_SetCompare2(TIM3,speed_L);	//PB.5�����PWM��  ����
	     }
			else
			 {
				TIM_SetCompare2(TIM3,1400);	//PB.5�����PWM��  ����
				TIM_SetCompare1(TIM3,speed);	//PB.0�����PWM��  �ҷ��
			 }
			}
		*/
		/*
						Angle=Get_Angle_Average();//��ȡ�Ƕȵ�ֵ

		printf("%f,%d,%d\r\n",Angle,speed,speed_L);
		delay_ms(100);
		if(KEY0==0){
			delay_ms(20);
			LED1 = ~LED1;
			speed+=10;
			if(speed>=1400)
			 {
				 TIM_SetCompare1(TIM3,1400);	//PB.0�����PWM��  �ҷ��	
			  speed_L = 1400-(speed-1400);
				 if(speed_L>=1400)
					 speed_L=1400;//speed_L = 1400+0.7*(speed_L-1400);;
				TIM_SetCompare2(TIM3,speed_L);	//PB.5�����PWM��  ����
	     }
			else
			 {
				TIM_SetCompare2(TIM3,1400);	//PB.5�����PWM��  ����
				TIM_SetCompare1(TIM3,speed);	//PB.0�����PWM��  �ҷ��	
		}
		*/
		//delay_ms(100);

		//printf("Mode=%d,Angle_0=%d,Angle_1=%d,Start=%d\r\n",Mode,Angle_0,Angle_1,Start);
		//Mode0_Deal(64);
		//printf("Mode=%d Angle_Mode=%d Angle_0=%d Angle_1=%d Start=%d\r\n\r\n",Mode,Angle_Mode,Angle_0,Angle_1,Start);
	
	//}
	
 }
}
