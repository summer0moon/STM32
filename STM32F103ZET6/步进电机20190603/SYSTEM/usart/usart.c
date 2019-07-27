#include "sys.h"
#include "usart.h"
#include <math.h>
#include "timer.h"
#include "delay.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误 
u8 i=0,j=0;
u16 ReceiveCounter=0,SendCounter=0;
u8 USART_RX_BUF[12][USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA[12]={0};       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
extern u8 IsRun;
extern int X;
extern int Y;
extern u16 step;
u8 t0=1,t1=1;
void ReceiveWenHao()//接收到？
{

	if((IsRun&0x03)!=0)
	{
		t0=0;
		switch(t1)
		{
			case 0:printf("<Run|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);break;//打印正在活动,并显示坐标
			case 1:{printf("<Run|MPos:%d,%d,0.000|FS:%d,0>\r\n",X,Y,step);t1--;}break;
			default :break;
		}
		
		//printf("<Run|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);//打印正在活动,并显示坐标
	}
	else
	{
		switch(t0)
		{
			case 0:printf("<Idle|MPos:%d,%d,0.000|FS:0,255|Ov:100,100,100|A:S>\r\n",X,Y);break;//连接后第一次打印
			case 1:{printf("<Idle|MPos:%d,%d,0.000|FS:0,0|WCO:0.000,0.000,0.000>\r\n",X,Y);t0++;}break;//连接后第一次打印
			case 2:{printf("<Idle|MPos:%d,%d,0.000|FS:0,0|Ov:100,100,100>\r\n",X,Y);t0++;}break;//连接后第二次打印
			default :{
			printf("<Idle|MPos:%d,%d,0.000|FS:0,0>\r\n",X,Y);t0++;
				if(t0>=11)
					t0=1;
			}break;//其他
		}
		//printf("<Idle|MPos:%d,%d,0.000|FS:%d,255>\r\n",X,Y,step);//打印没有活动,并显示坐标
	}
}

//*********************************************$系列*********************************
void Receive$$()//接收到$$
{
				printf("$0=10\r\n$1=25\r\n$2=0\r\n$3=0\r\n$4=0\r\n$5=0\r\n$6=0\r\n$10=1\r\n");
				printf("$11=0.010\r\n$12=0.002\r\n$13=0\r\n$20=0\r\n$21=0\r\n$22=0\r\n$23=0\r\n");
				printf("$24=25.000\r\n$25=500.000\r\n$26=250\r\n$27=1.000\r\n$30=255\r\n");
				printf("$31=0\r\n$32=0\r\n$100=80.000\r\n$101=80.000\r\n$102=80.000\r\n");
				printf("$110=10000.000\r\n$111=10000.000\r\n$112=10000.000\r\n");
				printf("$120=500.000\r\n$121=500.000\r\n$122=500.000\r\n$130=200.000\r\n");
				printf("$131=200.000\r\n$132=200.000\r\nok\r\n");
}

void Receive$G()//接收到$G
{
				printf("[GC:G0 G54 G17 G21 G90 G94 M5 M9 T0 F0 S0]\r\nok\r\n");
}

void Receive$()//接收到$开头的代码
{
	switch(USART_RX_BUF[j][1])
	{
		case 'G':Receive$G();break;//接收到$G
		case '$':Receive$$();break;//接收到$$
		default:printf("OK\r\n");break;
	}
}
//*********************************************$系列*********************************

//*********************************************G系列*********************************
void ReceiveG0X()//接收到G0X
{
	u16 len,t,n=0;
	char x[20]="",y[20]="";
	double x0=0,y0=0;
	len=USART_RX_STA[j]&0x7fff;//得到此次接收到的数据长度
					t=3;
	        while(USART_RX_BUF[j][t]!='Y'){x[n]=USART_RX_BUF[j][t];t++;n++;}//循环判断是否读到Y
					x[n]='\0';
					n=0;
					t++;
					while(t<len){y[n]=USART_RX_BUF[j][t];t++;n++;}//循环判断是否读完
					y[n]='\0';
	
			x0 = atof(x);//得到x坐标
			y0 = atof(y);//得到y坐标		
			Turn_x0y0((int)x0*100,(int)y0*100);
			//Tap(1);
			printf("OK\r\n");
}

void ReceiveG0Z()//接收到G0Z0
{
		printf("OK\r\n");
}

void ReceiveG0()//接收到G0
{
	switch(USART_RX_BUF[j][2])
	{
		case 'X':ReceiveG0X();break;//接收到G0X...Y...
		case 'Z':ReceiveG0Z();break;//接收到G0Z0
		default:printf("OK\r\n");break;
	}
}

void ReceiveG1X()//接收到G1X
{
		u16 len,t,n=0;
	char x[20]="",y[20]="";
	double x0=0,y0=0;
	len=USART_RX_STA[j]&0x7fff;//得到此次接收到的数据长度
					t=3;
	        while(USART_RX_BUF[j][t]!='Y'){x[n]=USART_RX_BUF[j][t];t++;n++;}//循环判断是否读到Y
					x[n]='\0';
					n=0;
					t++;
					while(t<len){y[n]=USART_RX_BUF[j][t];t++;n++;}//循环判断是否读完
					y[n]='\0';
					
			x0 = atof(x);//得到x坐标
			y0 = atof(y);//得到y坐标
					//Tap(0);
			DrawLine(X,Y,(int)(x0*100),(int)(y0*100));
			printf("OK\r\n");
}

void ReceiveG1()//接收到G1
{
	switch(USART_RX_BUF[j][2])
	{
		case 'X':ReceiveG1X();break;//接收到G1X...Y...
		case 'F':printf("OK\r\n");break;//接收到G1F
		default:printf("OK\r\n");break;
	}
}

void ReceiveG()//接收到G开头的代码
{
	switch(USART_RX_BUF[j][1])
	{
		case '1':ReceiveG1();break;//接收到G1
		case '0':ReceiveG0();break;//接收到G0
		case '4':{IsRun=0;delay_ms(20);printf("OK\r\n");printf("OK\r\n");}break;//接收到G0
		default:{printf("OK\r\n");};break;
	}
}
//*********************************************G系列*********************************


void USART1_IRQHandler(void)                	//串口1中断服务程序，协议
	{
	u8 Res,n;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		if(Res=='?')//接收到？
		{
       ReceiveWenHao();//处理接收到？
			 return;
		}
			else if(Res==0x18)//接收到欢迎请求
			{
			printf("\r\nGrbl 1.1f ['$' for help]\r\n");//打印欢迎请求	
			return;
			}

		if((USART_RX_STA[i]&0x8000)==0)//接收未完成
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
						
					if(USART_RX_STA[i]>(USART_REC_LEN-1))//接收数据错误,重新开始接收	  
					{
					USART_RX_STA[i]=0;
					for(;n<USART_REC_LEN;n++)//对数组进行清零
					USART_RX_BUF[i][n]=0;
					USART_RX_STA[i]=0;
					ReceiveCounter-=2;
					printf("ok\r\n");
					printf("ok\r\n");
					}
					
					}		 

			}  
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}



void USART1_Receive()//串口接收函数
{
			u16 len,t;
			if(USART_RX_STA[j]&0x8000)
		{					   
			len=USART_RX_STA[j]&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[j][t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\n");//插入换行
			USART_RX_STA[j]=0;
		}
}

u8 GetLen(u8 *p)//获取字符串的长度
{
	u8 len=0;
	while(*p!='\0')p++,len++;
	return len;
}

/*
float TurnNumber(u8 *p)//将字符串转换为数字
{
	int FS=1;
	u8 ZS[20],XS[20],n=0,zs_len=0,xs_len=0;
	float zs=0.0,xs=0.0;
	if(*p=='-')
	{
		FS=-1;//发现负数
		p++;
		
		for(;*p!='.';p++)//寻找小数点
		ZS[n]=*p,n++;//整数部分
		ZS[n]='\0';
		n=0;
		p++;
		
		for(;*p!='\0';p++)
		XS[n]=*p,n++;//小数部分
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
	zs_len=GetLen(ZS);//求出整数部分个数
	xs_len=GetLen(XS);//求出小数部分个数
	n=0;
	for(;n<zs_len;n++)
	zs=zs+(ZS[n]-48)*pow(10,zs_len-n-1);//求出整数
	
	n=0;
	for(;n<xs_len;n++)
	xs=xs+(XS[n]-48)*pow(0.1,n+1);//求出小数
	return (zs+xs)*FS;
}
*/

void USART1_Deal()//串口数据处理*********************************************
{
	if(SendCounter<=ReceiveCounter)
{
	u8 n;
	if(USART_RX_STA[j]&0x8000)
	{
		//len=USART_RX_STA&0x7fff;//得到此次接收到的数据长度
		switch(USART_RX_BUF[j][0])
		{	
			case 'G':ReceiveG();break;//收到G开头的代码
			case '$':Receive$();break;//收到$开头的代码
			case 'M':printf("OK\r\n");break;//收到M开头的代码
			default:printf("OK\r\n");break;
		}
			for(;n<USART_REC_LEN;n++)//对数组进行清零
			USART_RX_BUF[j][n]=0;
			USART_RX_STA[j]=0;
		j++,SendCounter++;
			if(j>=12)
			j=0;
	}
}
}

#endif	

