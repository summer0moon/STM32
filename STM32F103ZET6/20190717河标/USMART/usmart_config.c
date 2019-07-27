#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"		
#include "sys.h"
#include "lcd.h"
#include "gps.h"
												 
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
 
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)Ublox_Cfg_Cfg_Save,"u8 Ublox_Cfg_Cfg_Save(void)",	 
	(void*)Ublox_Cfg_Msg,"u8 Ublox_Cfg_Msg(u8 msgid,u8 uart1set)",	 
	(void*)Ublox_Cfg_Prt,"u8 Ublox_Cfg_Prt(u32 baudrate)",	 
	(void*)Ublox_Cfg_Tp,"u8 Ublox_Cfg_Tp(u32 interval,u32 length,signed char status)",	 
	(void*)Ublox_Cfg_Rate,"u8 Ublox_Cfg_Rate(u16 measrate,u8 reftime)",	 	 
};					  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















