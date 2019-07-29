#include "adc.h"
#include "delay.h"	
#include "stm32f4xx_dma.h"
#include "misc.h"
#define ADC_CDR_ADDRESS    ((uint32_t)0x40012308) //适用于双重和三重模式的 ADC 通用规则数据寄存器（ ADC_CDR)


u32 adc_result[50];
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//初始化ADC															   
void  Adc_Init(void)
{    
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
		NVIC_InitTypeDef			NVIC_InitStruct;
	DMA_Stream_TypeDef			DMAy_Streamx;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟
  //先初始化ADC1通道6	ADC2通道7 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//PA6 通道6	PA7 通道7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	  DMA_DeInit(DMA2_Stream0);
    DMA_StructInit( &DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                     //选择Channel_0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS; //数据传输的外设首地址，详解见上
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_result;   //自己定义待发送数组的首地址，要强制转换为32位
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       //数据传输方向选择为外设->内存
    DMA_InitStructure.DMA_BufferSize = 50;         //*             //传输数据个数，单位由以下确定，大小要配合定义的数组类型和外设数据类型
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //内存地址自增不允许，因为只存取一个数
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //*	//外设的数据大小
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;//*     //这里也选Byte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA传输模式为Normal，如果为Circular,将会循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //优先级为VeryHigh
    DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
		DMA_Cmd(DMA2_Stream0, ENABLE);
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,ENABLE);	  //ADC1,ADC2复位
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC);//清除中断标志
		DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);	//开USART1 Tx DMA中断
		
		NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream0_IRQn;//DMA2通道
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//子优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//抢占
		
		NVIC_Init(&NVIC_InitStruct);


		
	//ADC1初始化
  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;//ADC双重模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;//* 	//DMA模式2
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//*	//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//开启连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 


 
 	
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
 //设置指定ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1,ADC_SampleTime_3Cycles);
	
	ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
	//设置指定ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1,ADC_SampleTime_3Cycles);
	
	  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
    
    /* Enable DMA request after last transfer (Multi-ADC mode)  */
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
	//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)&&ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));//等待转换结果
    /* 使能 ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /* 使能 ADC2 */
    ADC_Cmd(ADC2, ENABLE);
    
    /* 软件启动ADC转换 */ 
	ADC_SoftwareStartConv(ADC1);
	//ADC_SoftwareStartConv(ADC1);
	
	
}	

void DMA_InIt(void)//DMA初始化
{
 DMA_InitTypeDef DMA_InitStructure;
    /*首先开DMA2时钟，由407参考手册-RM0090-Reference manual
     *165页可知，ADC与DMA2映射，而且DMA2挂载在AHB1时钟总线上*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    DMA_DeInit(DMA2_Stream0);
    DMA_StructInit( &DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                     //选择Channel_0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS; //数据传输的外设首地址，详解见上
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_result;   //自己定义待发送数组的首地址，要强制转换为32位
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       //数据传输方向选择为外设->内存
    DMA_InitStructure.DMA_BufferSize = 2;                      //传输数据大小为16，单位由以下确定，大小要配合定义的数组类型和外设数据类型
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //内存地址自增不允许，因为只存取一个数
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设的数据大小
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     //这里也选Byte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA传输模式为Normal，如果为Circular,将会循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //优先级为VeryHigh
    DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    

//    NVIC_InitTypeDef NVIC_InitStructure;
//    /* DMA中断配置 */
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;      //嵌套通道为DMA2_Stream0_IRQn
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级为 0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //通道中断使能
//    NVIC_Init(&NVIC_InitStructure);
    
    
    
    //使能DMA2_Stream0通道
    DMA_Cmd(DMA2_Stream0, ENABLE);
    

//    /* DMA中断开 */
//    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}


u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 









