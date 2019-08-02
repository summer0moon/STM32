#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DAC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
 DAC_InitTypeDef  DAC_InitStructure;
const uint16_t aSine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
//DACͨ��1�����ʼ��
void Dac_Init(void)
{  
  /*ʹ��DAC֮ǰ��Ԥ����---------------------------------------- */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DMA1ʱ��ʹ��*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
  /* GPIOAʱ��ʹ�ܣ���DACһ��ʹ�ã�*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);                         
  /* DAC Periphʱ��ʹ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

  /* DACͨ��1��2��DAC_OUT1 = PA.4����DAC_OUT2 = PA.5������*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  /* TIM6����---------------------------------------------- -------- */
  TIM6_Config(); 
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

/**             
  * @brief TIM6����
  * @note TIM6���û���APB1Ƶ��
  * @note TIM6�����¼�������ÿ��TIM6CLK / 256   
  * @param��
  * @retval��
  */
void TIM6_Config(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  /* ------------------------------------------------ --------
  TIM3����ʱ�ӣ�TIM6CLK������Ϊ2 * APB1ʱ�ӣ�PCLK1���� 
  ��ΪAPB1Ԥ��Ƶ����1��ͬ��   
    TIM6CLK = 2 * PCLK1  
    TIM6CLK = HCLK / 2 = SystemCoreClock / 2 
          
  ÿ��TIM6CLK / 256����TIM6�����¼� 

  ע�⣺ 
   SystemCoreClock��������HCLKƵ�ʣ�����system_stm32f4xx.c�ļ��ж��塣
   ����ʱ�ӣ�HCLK��ÿ�θ���ʱ���û����������SystemCoreClockUpdate����
   ��������SystemCoreClock����ֵ�������κ�����
   ���ڴ˱������ǲ���ȷ�ġ�    

  -------------------------------------------------- --------- * /
  / *ʱ������*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGOѡ��*/
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* TIM6���ü�����*/
  TIM_Cmd(TIM6, ENABLE);
}


void DAC_Ch2_SineWaveConfig(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  /* DAC channel2����*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* DMA1_Stream6 channel7����**************************************/
  DMA_DeInit(DMA1_Stream6);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC_DHR12R2_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aSine12bit;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 32;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);

 /*����DMA1_Stream6 */
  DMA_Cmd(DMA1_Stream6, ENABLE);

   /*����DACͨ��2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /*ΪDACͨ��2����DMA */
  DAC_DMACmd(DAC_Channel_2, ENABLE);
}


void DAC_Ch1_EscalatorConfig(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* DAC channel1����*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DMA1_Stream5 channel7����**************************************/  
  DMA_DeInit(DMA1_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aEscalator8bit;
  DMA_InitStructure.DMA_BufferSize = 6;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_InitStructure);    

  /*����DMA1_Stream5 */
  DMA_Cmd(DMA1_Stream5, ENABLE);
  
  /*����DACͨ��1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /*ΪDACͨ��1����DMA */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}


void DAC_Ch2_TriangleConfig(void)
{
 /* DAC channel2����*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /*����DACͨ��2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /*����DACͨ��2 DHR12RD�Ĵ���*/
  DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);
}

void DAC_Ch1_NoiseConfig(void)
{
 /* DAC channel1����*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /*����DACͨ��1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

 /*����DACͨ��1 DHR12L�Ĵ���*/
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
}



























































