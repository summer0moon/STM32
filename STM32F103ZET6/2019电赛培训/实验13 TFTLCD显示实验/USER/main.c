#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "math.h"
 
/************************************************
 ALIENTEK ս��STM32F103������ʵ��13
 TFTLCD��ʾʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 float xz=0,yz=0;	
 
 u8 a=50;
	
 int main(void)
 {	 
 	u8 x=0;
	u8 lcd_id[12];			//���LCD ID�ַ���
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	
  	while(1) 
	{
		yz=10*sin(xz);
		xz+=0.001;
		
		if(xz>300)
			a+=30,xz=0;
		LCD_Fast_DrawPoint(xz,yz+a,RED);
	} 
}
