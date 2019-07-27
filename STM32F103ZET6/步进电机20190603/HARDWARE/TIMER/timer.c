#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
int X=0,Y=0;//����ϵ������
u16 step=0;
u8 TAP;
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}




//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
   GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM3
	

}



void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}



//��ʱ��3�жϷ������
u8 j_0=0,j_1=0;
u16 Counter_0=0,Counter_1=0;
u8 IsRun=0;//��ֱ�ߵ�״̬������Ϊ1��ʾ����û�����

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
if(Counter_0){//��һ�������������
	
		Sep0Turn=j_0;
		Sep0=~Sep0;	
		Counter_0--;

		}

}
}


void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
if(Counter_1){//�ڶ��������������
			
			Sep1Turn=j_1;
		  Sep1=~Sep1;	
			Counter_1--;

		}

}
}


void Turn_Init(void)//��ʼ�������������,ȷ������ԭ��
{
	//DrawX(8,1);
	//while(Counter_0!=0||Counter_1!=0);
	//DrawY(8,1);
	//while(Counter_0!=0||Counter_1!=0);
//	X=0,Y=0;
	ENA=1;
	Sep0=0;
	Sep1=0;
	Tap(1);
}


void Turn0(u16 counter,u8 b)//������ת�ͷ�ת�Ĳ�����aΪ��������bΪ1˳ʱ��ת��bΪ0��ʱ��
{	
	Counter_0=counter;
	j_0=b;
}

void Turn1(u16 counter,u8 b)//������ת�ͷ�ת�Ĳ�����aΪ��������bΪ1˳ʱ��ת��bΪ0��ʱ��ת
{	
	Counter_1=counter;
	j_1=b;
}

void DrawX(u16 counter,u8 b)//�ƶ�X����,b=1�����ƶ�,b=0�����ƶ�
{
	//���������ת����ͬ����ΪX�ƶ�
	j_0=b;
	j_1=b;
	
	Counter_0=counter;
	Counter_1=counter;
	if(b)
	X=X+counter;
	else
	X=X-counter;	
}

void DrawY(u16 counter,u8 b)//�ƶ�Y����,b=1�����ƶ�,b=0�����ƶ�
{
	//���������ת���򷴷���ΪY�ƶ�
	j_0=b;
	j_1=b^1;
	Counter_0=counter*2;
	Counter_1=counter*2;
	if(b)
	Y=Y+counter;
	else
	Y=Y-counter;	
}

void Turn_x0y0(int x0,int y0)//�ƶ���x0,y0��
{
	if(x0>10000||y0>10000)
		return;
	IsRun|=0x02;
	ENA=0;
	Tap(1);
		if(x0-X>=0)//�ƶ����굽���
	DrawX(x0-X,1);
	else
	DrawX(X-x0,0);

	while(Counter_0!=0||Counter_1!=0);
	if(y0-Y>=0)//�ƶ����굽���
	DrawY(y0-Y,1);
	else
	DrawY(Y-y0,0);
		while(Counter_0!=0||Counter_1!=0);
	
	//delay_ms(100);
	//delay_ms(5000);
	//Tap(0);
	//delay_ms(500);
	ENA=1;
	IsRun&=0xfd;
}

void Tap(u8 a)//1̧�ʺ�0���
{
	if(a==1){
		TAP=16;
		TIM_SetCompare3(TIM2,TAP);
	delay_ms(500);}
	else{
		TAP=30;
		TIM_SetCompare3(TIM2,TAP);
	delay_ms(500);}
}
/*
void DrawPoint(u16 x,u16 y)
{
	Tap(1);
	delay_ms(20);
	Turn_x0y0(x,y);
	Tap(0);
	delay_ms(20);
	Tap(1);
	delay_ms(20);
}
*/

void DrawLine(int x0,int y0,int x1,int y1)//x0,y0Ϊ��ʼ���꣬x1��y1Ϊ�յ�����,����ֱ��
{
	
	int Fm=0,x=0,y=0;
	step=0;
	IsRun|=0x01;
	if(TAP==16)
		Tap(0);//���
	//Tap(1);//̧��
	//Turn_x0y0(x0,y0);//�ƶ����굽���	
	//Tap(0);//���
	ENA=0;
	x=x1-x0;//�ó��������
	y=y1-y0;
		if(x>10000||y>10000)
		return;
	if(x>0)
			step=step+x;
	else
			step=step-x;//�����ܲ���
	
	if(y>0)
			step=step+y;
	else
			step=step-y;//�����ܲ���
	if(x>0 && y>=0){//��һ����
	for(;step!=0;step--)//ѭ���ж��Ƿ񵽴��յ�
	{
		if(Fm>=0){//�ж�ƫ��
			DrawX(1,1),Fm=Fm-y;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
		else{
			DrawY(1,1),Fm=Fm+x;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
		}//Tap(1);//delay_ms(50);//̧��
			//Sep0=0;
			//Sep1=0;
			ENA=1;
		//IsRun&=0xfe;
	return;
	}

	if(x<=0 && y>0){//�ڶ�����
			for(;step!=0;step--)//ѭ���ж��Ƿ񵽴��յ�
			{
			if(Fm>=0){//�ж�ƫ��
			DrawX(1,0),Fm=Fm-y;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
			else{
			DrawY(1,1),Fm=Fm-x;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
		}//Tap(1);//delay_ms(50);//̧��
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}

	if(x<0 && y<=0){//��������
			for(;step!=0;step--)//ѭ���ж��Ƿ񵽴��յ�
			{
			if(Fm>=0){//�ж�ƫ��
			DrawX(1,0),Fm=Fm+y;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
			else{
			DrawY(1,0),Fm=Fm-x;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
		}//Tap(1);//delay_ms(50);//̧��
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}
	
	if(x>=0 && y<0){//��������
			for(;step!=0;step--)//ѭ���ж��Ƿ񵽴��յ�
			{
			if(Fm>=0){//�ж�ƫ��
			DrawX(1,1),Fm=Fm+y;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
			else{
			DrawY(1,0),Fm=Fm+x;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ��
		}//Tap(1);//delay_ms(50);//̧��
			//Sep0=0;
			//Sep1=0;
		ENA=1;
		//IsRun&=0xfe;
			return;
	}
	//Tap(1);
	//Sep0=0;
//	Sep1=0;
	ENA=1;
	//IsRun&=0xfe;
}



void DrawArc(int x0,int y0,int x_1,int y_1,int x_2,int y_2)//x0,y0ΪԲ�����꣬x1��y1Ϊ�������,x2��y2Ϊ�յ�����,����Բ��
{
	u16 step=0;
	int Fm=0,x1=0,y1=0,x2=0,y2=0,xm=0,ym=0;
		IsRun|=0x01;
	if(TAP==16)
		Tap(0);//���
	//�ó��������
	x1=x_1-x0;
	y1=y_1-y0;
	x2=x_2-x0;
	y2=y_2-y0;
	xm=x1;
	ym=y1;
	if(x2-x1>0)
			step=step+(x2-x1);
	else
			step=step-(x2-x1);//�����ܲ���
	
	if(y2-y1>0)
			step=step+(y2-y1);
	else
			step=step-(y2-y1);//�����ܲ���
	
	if(x1>=0 && y1>=0 && x2>=0 && y2>=0){//��һ����
		if(x2>=x1){//�ж�˳ʱ�� SR1
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ��
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;
		}
		else      {   //�ж���ʱ�� NR1
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ��
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;	
		}
	}

	if(x1<=0 && y1>=0 && x2<=0 && y2>=0){//�ڶ�����
		
		if(x2>=x1){//�ж�˳ʱ�� SR2
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ��
				DrawX(1,1),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;
		}
		else      {   //�ж���ʱ�� NR2
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ��
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawX(1,0),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;	
		}
	}
	
	
		if(x1<=0 && y1<=0 && x2<=0 && y2<=0){//��������
		
		if(x2<=x1){//�ж�˳ʱ�� SR3
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ�� 0,-10
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;
		}
		else      {   //�ж���ʱ�� NR3
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ�� -10,0
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;	
		}
	}
		
	
		if(x1>=0 && y1<=0 && x2>=0 && y2<=0){//��������
		
		if(x2<=x1){//�ж�˳ʱ�� SR4
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ��  10,0
				DrawX(1,0),Fm=Fm-2*xm+1,xm=xm-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawY(1,0),Fm=Fm-2*ym+1,ym=ym-1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;
		}
		else      {   //�ж���ʱ�� NR4
			for(;step!=0;step--){//ѭ���ж��Ƿ񵽴��յ�		
				if(Fm>=0){//�ж�ƫ�� 0,-10
				DrawY(1,1),Fm=Fm+2*ym+1,ym=ym+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
				else{
				DrawX(1,1),Fm=Fm+2*xm+1,xm=xm+1;while(Counter_0!=0||Counter_1!=0);}//���Ʋ�������߲�,���Ҽ���ƫ�������
			}
				ENA=1;
				return;	
		}
	}
	ENA=1;
}


void DrawCircle(int x0,int y0,u16 r)//��һ��Բ��x0,y0ΪԲ�ģ�rΪ�뾶
{
			DrawArc(x0,y0,x0+r,0,0,y0+r);
			DrawArc(x0,y0,0,y0+r,x0-r,0);
			DrawArc(x0,y0,x0-r,0,0,y0-r);
			DrawArc(x0,y0,0,y0-r,x0+r,0);
}

void Drawarc(int x1,int y1,int x2,int y2,int r)//x1,y1Ϊ��㣬x2��y2Ϊ�յ㣬rΪ�뾶
{
	
	
	
	
	
	
}
	



















