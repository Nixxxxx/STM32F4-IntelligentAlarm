#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc3.h"

//��ʼ����������
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			//PC0 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������

	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	Adc3_Init();
}
//��ȡ���������ĵ�ѹֵ
u16 Smog_Get_Vol(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(ADC_Channel_5);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//�õ�ƽ��ֵ
	printf("Smog_ADC_Val:%d\r\n", temp_val);
    return (u16)temp_val;
}
//����������������ȡ�ĵ�ѹֵ��ͨ����ʽ�������ȼ�����Ũ��ֵ
//��RsΪ������������裬��������Ũ��������������Rs�½�����Rs���½���ᵼ�£�MQ-2��4�š�6�ŶԵ�����ĵ�ѹ����
//��������Ũ������������ĵ�ѹҲ��������Rs�ڲ�ͬ�������в�ͬ��Ũ��ֵ�����Ըú�������Ϊ�ο�.
u16 Smog_Trans_Concentration(void)
{
    u16 ulVal = 0;
    u16 temp_val = Smog_Get_Vol();
	u16 Rs;
	Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
	printf("Smog_Rs_Val:%d\r\n", Rs);
	
	ulVal = 5000+15000.0/4096.0*temp_val;//������д��ʽ����Rs��������Ũ��
	
    return ulVal;
}

