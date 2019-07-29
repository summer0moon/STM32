#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "timer.h"
#include "ZiXiangGuan.h"
	extern u32 counter;//��ʱ���жϵĿ��Ʊ���
extern		u16 adc1[SENDBUFF_SIZE];//����ADC1������
extern	u16 adc2[SENDBUFF_SIZE];//����ADC2������
float x = 0;
u16 j;
extern u32	adc_result_IRQ[SENDBUFF_SIZE];
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	//LED_Init();					//��ʼ��LED 
 	LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
	TIM3_Int_Init(168-1,0);	
	while(1)
	{ 
		delay_ms(100);
//		x = ZiXiangGuan_Deal();//������λ��
//		LCD_ShowNum(70,150,(u32)x,5,24);
		counter = SENDBUFF_SIZE;//�����Ʊ�����ֵ
		while(counter);//�ȴ��ɼ����
		for(j=0;j<SENDBUFF_SIZE;j++)//�����ݱ�����������* adc1��* adc2
		{
					adc1[j] = (u16)(adc_result_IRQ[j] & 0x0000ffff);
					adc2[j] = (u16)((adc_result_IRQ[j] & 0xffff0000)>>16);
		}
		for(j=0;j<SENDBUFF_SIZE;j++)
		{
			LCD_ShowNum(70,150,adc1[j],5,24);
			delay_ms(1000);
		} 
	}


}
