#include "control.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
/*
	Sep0 PAout(0)   	//PA0,���Ʋ������0��������
	Sep0Turn PAout(1) //PA1,���Ʋ������0������ת
	Sep1 PCout(0)   	//PC0,���Ʋ������1��������
	Sep1Turn PCout(1) //PC1,���Ʋ������1������ת
	ENA PEout(10)  		//PE10,ʹ�ܶ˿�
	MS1 PEout(7)			//PE7,���Ʋ���ϸ��
	MS2 PEout(8)			//PE8,���Ʋ���ϸ��
	MS3 PEout(9)			//PE9,���Ʋ���ϸ��
*/
int X=0,Y=0;//����ϵ������
u16 step=0;
u8 TAP;
void Control_Init()//�κ���Ϊ�������IO�ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz ;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//���ò���ϸ��Ϊ���
	MS1=1;//���Ʋ���ϸ��
	MS2=1;//���Ʋ���ϸ��
	MS3=1;//���Ʋ���ϸ��
	ENA=1;//ʹ��
	Sep0=0;//��������
	Sep1=0;//��������
}

void TIM3_Int_Init(u16 arr,u16 psc)//���Ʋ������0
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM4_Int_Init(u16 arr,u16 psc)//���Ʋ������1
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��3�жϷ�����
u8 j_0=0,j_1=0;
u16 Counter_0=0,Counter_1=0;
u8 IsRun=0;//��ֱ�ߵ�״̬������Ϊ1��ʾ����û�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		
		if(Counter_0){//��һ�������������
		Sep0Turn=j_0;
		Sep0=~Sep0;	
		Counter_0--;
		}

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{

		if(Counter_1){//�ڶ��������������
			
			Sep1Turn=j_1;
		  Sep1=~Sep1;	
			Counter_1--;

		}
		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}


void Turn_Init(void)//��ʼ�������������,ȷ������ԭ��
{
	ENA=1;
	Sep0=0;
	Sep1=0;
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


