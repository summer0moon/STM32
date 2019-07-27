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
	delay_init(168);	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	//LED_Init();	
	LCD_Init();	 //LCD�˿ڳ�ʼ
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
	 Mode0_Deal(Angle_0);
	// Mode1_Deal(Angle_0,Angle_1);   
   while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
		delay_ms(100);
		KEY_Deal();//��ֵ����,PE3-Mode,PE6-Add,PE7-Reduce,PE8-Start,PE9-Stop
		DisPlay();
	  Control();//����
	}
}
