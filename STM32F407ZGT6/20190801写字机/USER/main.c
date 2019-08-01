#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "control.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
  Control_Init();//�κ���Ϊ�������IO�ڳ�ʼ��
	TIM3_Int_Init(600-1,84-1);//���Ʋ������0
	TIM3_Int_Init(600-1,84-1);//���Ʋ������1
	Turn_Init();
	while(1)
	{
		DrawX(100,1);	
	}
}
