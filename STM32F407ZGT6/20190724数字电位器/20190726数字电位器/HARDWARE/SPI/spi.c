#include "spi.h"
#include "delay.h"
/*ʹ�ô��ļ���Ҫspi.c
CS1 PB13
CS2 PB14
SPI SCK PA4
SPI MOSI PA5
SPI MISO PA6
*/
u16 Time = 1;//��ʱʱ��
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB14,PB13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
		MCP41010_CS_1=1;			//SPI��ѡ��,���ߣ�Ƭѡ�ź�
		MCP41010_CS_2=1;
	
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//PA4~6���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	SPI1_SetSpeed(1);
}   



void SPI1_SetSpeed(u8 time)
{
	Time = time;
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
//mode:����ʱ��ģʽ00,01,10,11
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
		u8 i;	
		SCK_0;
		delay_us(Time);
		MOSI_0;
		delay_us(Time);
		for (i = 0; i < 8; i++ )
		{
				MOSI_0;
				delay_us(Time);
				SCK_0;
				delay_us(Time);
				if (TxData & 0x80) //��data��0x80������Ԥ��,Ŀ����ȡ��date��������λ��ֵ,�����λ��1,��ôMOSI�ͻ��ɸߵ�ƽ,����ͻᱣ�ֵ͵�ƽ
				{
						MOSI_1;
					delay_us(Time);
				}
				SCK_0;
				delay_us(Time);
				SCK_1;
				delay_us(Time);
				TxData <<= 1 ;  //��date����,����ѭ�����бȽ�
		}	   
}








