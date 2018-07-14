#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "beep.h"
#include "key.h"
#include "remote.h"
#include "smog.h"
#include "adc3.h"
#include "timer.h"

int showScreen(void);

int main(void)
{    
	short temp;
	u16 adcx;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(115200);	 //��ʼ�����ڲ�����Ϊ115200
	BEEP_Init();        //��ʼ��������
	LED_Init();					//��ʼ��LED 
 	LCD_Init();         //Һ����ʼ��
	Adc_Init();         //�ڲ��¶ȴ�����ADC��ʼ�� 
 	Remote_Init();				//������ճ�ʼ��		 
	Smog_Init();
 	TIM3_Int_Init(5000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms  
	LED1=0;				  	//�ȵ����̵�
  showScreen();	
	
	while(1)
	{
		temp=Get_Temprate();	//�õ��¶�ֵ 
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(30+10*8,140,16,16,16,"-");	    //��ʾ����
		}else LCD_ShowString(30+10*8,140,16,16,16," ");	//�޷���
		LCD_ShowxNum(30+11*8,140,temp/100,2,16,0);		//��ʾ��������
		LCD_ShowxNum(30+14*8,140,temp%100,2,16,0);		//��ʾС������ 
		adcx=Smog_Get_Vol();
		LCD_ShowxNum(30+10*8,160,adcx,5,16,0);//��ʾADC��ֵ 
	
		if(adcx>=1000){
			BEEP=1;
		}else{
			BEEP=0;
		}
	}
}

int showScreen(void)
{
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2018/7");	  
	POINT_COLOR=BLUE;//��������Ϊ��ɫ  
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//���ڹ̶�λ����ʾС����
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//���ڹ̶�λ����ʾС����
	
	LCD_ShowString(30,160,200,16,16,"smokescope:");//���ڹ̶�λ����ʾС����
	LCD_ShowString(30+16*8,160,200,16,16,"ppm");//���ڹ̶�λ����ʾС����
	return 0;
}