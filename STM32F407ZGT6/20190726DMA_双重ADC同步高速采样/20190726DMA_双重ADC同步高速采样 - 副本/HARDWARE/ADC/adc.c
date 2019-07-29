#include "adc.h"
#include "delay.h"	
#include "stm32f4xx_dma.h"
#include "misc.h"
#define ADC_CDR_ADDRESS    ((uint32_t)0x40012308) //������˫�غ�����ģʽ�� ADC ͨ�ù������ݼĴ����� ADC_CDR)


u32 adc_result[50];
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//��ʼ��ADC															   
void  Adc_Init(void)
{    
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
		NVIC_InitTypeDef			NVIC_InitStruct;
	DMA_Stream_TypeDef			DMAy_Streamx;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC1ʱ��
  //�ȳ�ʼ��ADC1ͨ��6	ADC2ͨ��7 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//PA6 ͨ��6	PA7 ͨ��7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	  DMA_DeInit(DMA2_Stream0);
    DMA_StructInit( &DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                     //ѡ��Channel_0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS; //���ݴ���������׵�ַ��������
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_result;   //�Լ����������������׵�ַ��Ҫǿ��ת��Ϊ32λ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       //���ݴ��䷽��ѡ��Ϊ����->�ڴ�
    DMA_InitStructure.DMA_BufferSize = 50;         //*             //�������ݸ�������λ������ȷ������СҪ��϶�����������ͺ�������������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ����Զ����ӽ�ֹ����Ϊ����ֻ�õ���DR���ݼĴ���
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //�ڴ��ַ������������Ϊֻ��ȡһ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //*	//��������ݴ�С
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;//*     //����ҲѡByte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA����ģʽΪNormal�����ΪCircular,����ѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //���ȼ�ΪVeryHigh
    DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
		DMA_Cmd(DMA2_Stream0, ENABLE);
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,ENABLE);	  //ADC1,ADC2��λ
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC);//����жϱ�־
		DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);	//��USART1 Tx DMA�ж�
		
		NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream0_IRQn;//DMA2ͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//�����ȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//��ռ
		
		NVIC_Init(&NVIC_InitStruct);


		
	//ADC1��ʼ��
  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;//ADC˫��ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;//* 	//DMAģʽ2
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//*	//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��������ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 


 
 	
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
 //����ָ��ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1,ADC_SampleTime_3Cycles);
	
	ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��
	//����ָ��ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1,ADC_SampleTime_3Cycles);
	
	  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
    
    /* Enable DMA request after last transfer (Multi-ADC mode)  */
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
	//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)&&ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));//�ȴ�ת�����
    /* ʹ�� ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /* ʹ�� ADC2 */
    ADC_Cmd(ADC2, ENABLE);
    
    /* �������ADCת�� */ 
	ADC_SoftwareStartConv(ADC1);
	//ADC_SoftwareStartConv(ADC1);
	
	
}	

void DMA_InIt(void)//DMA��ʼ��
{
 DMA_InitTypeDef DMA_InitStructure;
    /*���ȿ�DMA2ʱ�ӣ���407�ο��ֲ�-RM0090-Reference manual
     *165ҳ��֪��ADC��DMA2ӳ�䣬����DMA2������AHB1ʱ��������*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    DMA_DeInit(DMA2_Stream0);
    DMA_StructInit( &DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                     //ѡ��Channel_0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS; //���ݴ���������׵�ַ��������
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_result;   //�Լ����������������׵�ַ��Ҫǿ��ת��Ϊ32λ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       //���ݴ��䷽��ѡ��Ϊ����->�ڴ�
    DMA_InitStructure.DMA_BufferSize = 2;                      //�������ݴ�СΪ16����λ������ȷ������СҪ��϶�����������ͺ�������������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ����Զ����ӽ�ֹ����Ϊ����ֻ�õ���DR���ݼĴ���
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //�ڴ��ַ������������Ϊֻ��ȡһ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������ݴ�С
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     //����ҲѡByte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA����ģʽΪNormal�����ΪCircular,����ѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   //���ȼ�ΪVeryHigh
    DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    

//    NVIC_InitTypeDef NVIC_InitStructure;
//    /* DMA�ж����� */
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;      //Ƕ��ͨ��ΪDMA2_Stream0_IRQn
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ 1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //��Ӧ���ȼ�Ϊ 0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ͨ���ж�ʹ��
//    NVIC_Init(&NVIC_InitStructure);
    
    
    
    //ʹ��DMA2_Stream0ͨ��
    DMA_Cmd(DMA2_Stream0, ENABLE);
    

//    /* DMA�жϿ� */
//    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}


u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
	 









