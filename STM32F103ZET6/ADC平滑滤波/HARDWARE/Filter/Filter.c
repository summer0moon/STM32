#include <stdio.h>
#include <math.h>
#include <Filter.h>

//struct TAdSample AdSample[1];
//结构体赋初值
//struct TAdSample AdSample[1]=
//{
// 0,
// {0,},
// 0,
// 50,
// 0,
// 0,
// 0,
// 0, 
//};	
////获取ADC初值的函数
//int Get_Adc()
//{	
//	return 2048+rand()%10;	
//}

////平均滤波
//float Avg_Filter(struct TAdSample *p)//采样N次计算平均值
//{
//  for(int i=0;i<p[0].len;i++)
//  {
//  }
// return 0;
//}

float Avg_Filter2(int adm, struct TAdSample *p)//每次采样计算平均值,滑动平均滤波
{
	p[0].sum=p[0].sum+adm-p[0].buf[p[0].i];//求和删除原来的值
	p[0].buf[p[0].i]=adm;//赋新的值给buf
	p[0].i++;
	if(p[0].i>=p[0].len) //缓冲区循环
	{
		p[0].i=0;
		p[0].bover=1;//缓冲区溢出标志位
	}
	
	p[0].avg=p[0].sum/p[0].len;//求当前的平均值
	
	if(!p[0].bover)//如果没有溢出当前的ADC值设置为平均值
	{
		p[0].avg=adm;
	}
	
	return p[0].avg;
}


//void main()
//{
//	ADC_Init();
//	while(1)
//	{
//		//0.1s
//	int i;
//	//float avg =	Avg_Filter2(Get_Adc(),&AdSample[0]);
//	for(i=0;i<sizeof(AdSample)/sizeof(AdSample[0]);i++)//得到数组的长度
//		Avg_Filter2(Get_Adc(),&AdSample[0]);//
//	}  
//}

