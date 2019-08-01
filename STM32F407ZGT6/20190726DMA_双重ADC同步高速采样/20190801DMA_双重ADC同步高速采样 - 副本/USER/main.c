#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "ZiXiangGuan.h"
extern float maxValue[2];
extern uint32_t testIndex[2];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	//LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
	TIM3_Int_Init(period-1,1);//ʱ��42M
	TIM_SetCompare1(TIM3,period/2);
	while(1)
	{ 
		delay_ms(100);
		//Get_Adc_Data();
		ZiXiangGuan_Deal();
		printf("%f   %f\r\n",(float)testIndex[0]/SENDBUFF_SIZE*42000000.0f/period,(float)testIndex[1]/SENDBUFF_SIZE*42000000.0f/period);
		delay_ms(1000);
	} 
}

