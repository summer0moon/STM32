#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//DAC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
 DAC_InitTypeDef  DAC_InitStructure;
const uint16_t aSine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
//DAC通道1输出初始化
void Dac_Init(void)
{  
  /*使用DAC之前的预配置---------------------------------------- */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DMA1时钟使能*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
  /* GPIOA时钟使能（与DAC一起使用）*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);                         
  /* DAC Periph时钟使能*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

  /* DAC通道1和2（DAC_OUT1 = PA.4）（DAC_OUT2 = PA.5）配置*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  /* TIM6配置---------------------------------------------- -------- */
  TIM6_Config(); 
}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

/**             
  * @brief TIM6配置
  * @note TIM6配置基于APB1频率
  * @note TIM6更新事件发生在每个TIM6CLK / 256   
  * @param无
  * @retval无
  */
void TIM6_Config(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  /* ------------------------------------------------ --------
  TIM3输入时钟（TIM6CLK）设置为2 * APB1时钟（PCLK1）， 
  因为APB1预分频器与1不同。   
    TIM6CLK = 2 * PCLK1  
    TIM6CLK = HCLK / 2 = SystemCoreClock / 2 
          
  每个TIM6CLK / 256发生TIM6更新事件 

  注意： 
   SystemCoreClock变量保存HCLK频率，并在system_stm32f4xx.c文件中定义。
   核心时钟（HCLK）每次更改时，用户都必须调用SystemCoreClockUpdate（）
   函数更新SystemCoreClock变量值。否则，任何配置
   基于此变量将是不正确的。    

  -------------------------------------------------- --------- * /
  / *时基配置*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 0xFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO选择*/
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* TIM6启用计数器*/
  TIM_Cmd(TIM6, ENABLE);
}


void DAC_Ch2_SineWaveConfig(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  /* DAC channel2配置*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* DMA1_Stream6 channel7配置**************************************/
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

 /*启用DMA1_Stream6 */
  DMA_Cmd(DMA1_Stream6, ENABLE);

   /*启用DAC通道2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /*为DAC通道2启用DMA */
  DAC_DMACmd(DAC_Channel_2, ENABLE);
}


void DAC_Ch1_EscalatorConfig(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* DAC channel1配置*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DMA1_Stream5 channel7配置**************************************/  
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

  /*启用DMA1_Stream5 */
  DMA_Cmd(DMA1_Stream5, ENABLE);
  
  /*启用DAC通道1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /*为DAC通道1启用DMA */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}


void DAC_Ch2_TriangleConfig(void)
{
 /* DAC channel2配置*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /*启用DAC通道2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /*设置DAC通道2 DHR12RD寄存器*/
  DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);
}

void DAC_Ch1_NoiseConfig(void)
{
 /* DAC channel1配置*/
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /*启用DAC通道1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

 /*设置DAC通道1 DHR12L寄存器*/
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
}



























































