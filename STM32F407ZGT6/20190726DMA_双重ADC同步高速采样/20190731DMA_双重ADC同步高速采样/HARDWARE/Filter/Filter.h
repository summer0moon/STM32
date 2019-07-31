#ifndef __Filter_H
#define AD_LENGTH 1000

struct TAdSample//用结构描绘AD采样
{
 float avg;//AD平均值
 int buf[AD_LENGTH];//AD缓冲区
 int i;//缓冲区指针
 int len;//长度
 int bover;//是否满的标志位
 float max;//缓冲区的最大值
 float min;	//缓冲区的最小值
 float limit;//最大值和最小值的差值
 int sum;//buf的总和
};

float Avg_Filter2(int adm, struct TAdSample *p);

#endif
