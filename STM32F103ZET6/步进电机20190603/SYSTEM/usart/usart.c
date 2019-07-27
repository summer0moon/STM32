#include "sys.h"
#include "usart.h"
#include <math.h>
#include "timer.h"
#include "delay.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
u8 i=0,j=0;
u16 ReceiveCounter=0,SendCounter=0;
u8 USART_RX_BUF[12][USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA[12]={0};       //����״̬���	  
  
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
extern u8 IsRun;
extern int X;
extern int Y;
extern u16 step;
u8 t0=1,t1=1;
void ReceiveWenHao()//���յ���
{

	if((IsRun&0x03)!=0)
	{
		t0=0;
		switch(t1)
		{
			case 0:printf("<Run|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);break;//��ӡ���ڻ,����ʾ����
			case 1:{printf("<Run|MPos:%d,%d,0.000|FS:%d,0>\r\n",X,Y,step);t1--;}break;
			default :break;
		}
		
		//printf("<Run|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);//��ӡ���ڻ,����ʾ����
	}
	else
	{
		switch(t0)
		{
			case 0:printf("<Idle|MPos:%d,%d,0.000|FS:0,255|Ov:100,100,100|A:S>\r\n",X,Y);break;//���Ӻ��һ�δ�ӡ
			case 1:{printf("<Idle|MPos:%d,%d,0.000|FS:0,0|WCO:0.000,0.000,0.000>\r\n",X,Y);t0++;}break;//���Ӻ��һ�δ�ӡ
			case 2:{printf("<Idle|MPos:%d,%d,0.000|FS:0,0|Ov:100,100,100>\r\n",X,Y);t0++;}break;//���Ӻ�ڶ��δ�ӡ
			default :{
			printf("<Idle|MPos:%d,%d,0.000|FS:0,0>\r\n",X,Y);t0++;
				if(t0>=11)
					t0=1;
			}break;//����
		}
		//printf("<Idle|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);//��ӡû�л,����ʾ����
	}
}

//*********************************************$ϵ��*********************************
void Receive$$()//���յ�$$
{
				printf("$0=10\r\n$1=25\r\n$2=0\r\n$3=0\r\n$4=0\r\n$5=0\r\n$6=0\r\n$10=1\r\n");
				printf("$11=0.010\r\n$12=0.002\r\n$13=0\r\n$20=0\r\n$21=0\r\n$22=0\r\n$23=0\r\n");
				printf("$24=25.000\r\n$25=500.000\r\n$26=250\r\n$27=1.000\r\n$30=255\r\n");
				printf("$31=0\r\n$32=0\r\n$100=80.000\r\n$101=80.000\r\n$102=80.000\r\n");
				printf("$110=10000.000\r\n$111=10000.000\r\n$112=10000.000\r\n");
				printf("$120=500.000\r\n$121=500.000\r\n$122=500.000\r\n$130=200.000\r\n");
				printf("$131=200.000\r\n$132=200.000\r\nok\r\n");
}

void Receive$G()//���յ�$G
{
				printf("[GC:G0 G54 G17 G21 G90 G94 M5 M9 T0 F0 S0]\r\nok\r\n");
}

void Receive$()//���յ�$��ͷ�Ĵ���
{
	switch(USART_RX_BUF[j][1])
	{
		case 'G':Receive$G();break;//���յ�$G
		case '$':Receive$$();break;//���յ�$$
		default:printf("OK\r\n");break;
	}
}
//*********************************************$ϵ��*********************************

//*********************************************Gϵ��*********************************
void ReceiveG0X()//���յ�G0X
{
	u16 len,t,n=0;
	char x[20]="",y[20]="";
	double x0=0,y0=0;
	len=USART_RX_STA[j]&0x7fff;//�õ��˴ν��յ������ݳ���
					t=3;
	        while(USART_RX_BUF[j][t]!='Y'){x[n]=USART_RX_BUF[j][t];t++;n++;}//ѭ���ж��Ƿ����Y
					x[n]='\0';
					n=0;
					t++;
					while(t<len){y[n]=USART_RX_BUF[j][t];t++;n++;}//ѭ���ж��Ƿ����
					y[n]='\0';
	
			x0 = atof(x);//�õ�x����
			y0 = atof(y);//�õ�y����		
			Turn_x0y0((int)x0*100,(int)y0*100);
			//Tap(1);
			printf("OK\r\n");
}

void ReceiveG0Z()//���յ�G0Z0
{
		printf("OK\r\n");
}

void ReceiveG0()//���յ�G0
{
	switch(USART_RX_BUF[j][2])
	{
		case 'X':ReceiveG0X();break;//���յ�G0X...Y...
		case 'Z':ReceiveG0Z();break;//���յ�G0Z0
		default:printf("OK\r\n");break;
	}
}

void ReceiveG1X()//���յ�G1X
{
		u16 len,t,n=0;
	char x[20]="",y[20]="";
	double x0=0,y0=0;
	len=USART_RX_STA[j]&0x7fff;//�õ��˴ν��յ������ݳ���
					t=3;
	        while(USART_RX_BUF[j][t]!='Y'){x[n]=USART_RX_BUF[j][t];t++;n++;}//ѭ���ж��Ƿ����Y
					x[n]='\0';
					n=0;
					t++;
					while(t<len){y[n]=USART_RX_BUF[j][t];t++;n++;}//ѭ���ж��Ƿ����
					y[n]='\0';
					
			x0 = atof(x);//�õ�x����
			y0 = atof(y);//�õ�y����
					//Tap(0);
			DrawLine(X,Y,(int)(x0*100),(int)(y0*100));
			printf("OK\r\n");
}

void ReceiveG1()//���յ�G1
{
	switch(USART_RX_BUF[j][2])
	{
		case 'X':ReceiveG1X();break;//���յ�G1X...Y...
		case 'F':printf("OK\r\n");break;//���յ�G1F
		default:printf("OK\r\n");break;
	}
}

void ReceiveG()//���յ�G��ͷ�Ĵ���
{
	switch(USART_RX_BUF[j][1])
	{
		case '1':ReceiveG1();break;//���յ�G1
		case '0':ReceiveG0();break;//���յ�G0
		case '4':{IsRun=0;delay_ms(20);printf("OK\r\n");printf("OK\r\n");}break;//���յ�G0
		default:{printf("OK\r\n");};break;
	}
}
//*********************************************Gϵ��*********************************


void USART1_IRQHandler(void)                	//����1�жϷ������Э��
	{
	u8 Res,n;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if(Res=='?')//���յ���
		{
       ReceiveWenHao();//������յ���
			 return;
		}
			else if(Res==0x18)//���յ���ӭ����
			{
			printf("\r\nGrbl 1.1f ['$' for help]\r\n");//��ӡ��ӭ����	
			return;
			}

		if((USART_RX_STA[i]&0x8000)==0)//����δ���
			{
				if(Res==0x0a)
			{
				USART_RX_STA[i]|=0x8000,i++,ReceiveCounter++;
				if(i>=12)
				i=0;
			}
				else
					{
					USART_RX_BUF[i][USART_RX_STA[i]]=Res ;
					USART_RX_STA[i]++;
						
					if(USART_RX_STA[i]>(USART_REC_LEN-1))//�������ݴ���,���¿�ʼ����	  
					{
					USART_RX_STA[i]=0;
					for(;n<USART_REC_LEN;n++)//�������������
					USART_RX_BUF[i][n]=0;
					USART_RX_STA[i]=0;
					ReceiveCounter-=2;
					printf("ok\r\n");
					printf("ok\r\n");
					}
					
					}		 

			}  
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}



void USART1_Receive()//���ڽ��պ���
{
			u16 len,t;
			if(USART_RX_STA[j]&0x8000)
		{					   
			len=USART_RX_STA[j]&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[j][t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\n");//���뻻��
			USART_RX_STA[j]=0;
		}
}

u8 GetLen(u8 *p)//��ȡ�ַ����ĳ���
{
	u8 len=0;
	while(*p!='\0')p++,len++;
	return len;
}

/*
float TurnNumber(u8 *p)//���ַ���ת��Ϊ����
{
	int FS=1;
	u8 ZS[20],XS[20],n=0,zs_len=0,xs_len=0;
	float zs=0.0,xs=0.0;
	if(*p=='-')
	{
		FS=-1;//���ָ���
		p++;
		
		for(;*p!='.';p++)//Ѱ��С����
		ZS[n]=*p,n++;//��������
		ZS[n]='\0';
		n=0;
		p++;
		
		for(;*p!='\0';p++)
		XS[n]=*p,n++;//С������
		XS[n]='\0';
	}
	else
	{
		for(;*p!='.';p++)
		ZS[n]=*p,n++;
		ZS[n]='\0';
		n=0;
		p++;
		
		for(;*p!='\0';p++)
		XS[n]=*p,n++;
		XS[n]='\0';
	}
	zs_len=GetLen(ZS);//����������ָ���
	xs_len=GetLen(XS);//���С�����ָ���
	n=0;
	for(;n<zs_len;n++)
	zs=zs+(ZS[n]-48)*pow(10,zs_len-n-1);//�������
	
	n=0;
	for(;n<xs_len;n++)
	xs=xs+(XS[n]-48)*pow(0.1,n+1);//���С��
	return (zs+xs)*FS;
}
*/

void USART1_Deal()//�������ݴ���*********************************************
{
	if(SendCounter<=ReceiveCounter)
{
	u8 n;
	if(USART_RX_STA[j]&0x8000)
	{
		//len=USART_RX_STA&0x7fff;//�õ��˴ν��յ������ݳ���
		switch(USART_RX_BUF[j][0])
		{	
			case 'G':ReceiveG();break;//�յ�G��ͷ�Ĵ���
			case '$':Receive$();break;//�յ�$��ͷ�Ĵ���
			case 'M':printf("OK\r\n");break;//�յ�M��ͷ�Ĵ���
			default:printf("OK\r\n");break;
		}
			for(;n<USART_REC_LEN;n++)//�������������
			USART_RX_BUF[j][n]=0;
			USART_RX_STA[j]=0;
		j++,SendCounter++;
			if(j>=12)
			j=0;
	}
}
}

#endif	

