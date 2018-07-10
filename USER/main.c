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

void remoteFunction(void);
void keyFunction(void);
int showScreen(void);

u8 screen=0;
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
		keyFunction();
		remoteFunction();
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

//����
void keyFunction() 
{
	u8 key=KEY_Scan(0);		//�õ���ֵ
	if(key)
	{						   
		switch(key)
		{				 
			case WKUP_PRES:
				if(screen==0){
					GPIO_SetBits(GPIOB,GPIO_Pin_15);   //�ر���Ļ
					screen=1;
				}else {
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					screen=0;
				}
				break;
			case KEY1_PRES:	//����LED0��ת	 
				if(LED0==0)
					GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
				else GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
				break; 
			case KEY2_PRES:	//���Ʒ�����
				if(BEEP==0)
					GPIO_SetBits(GPIOF,GPIO_Pin_8);   //BEEP�������ߣ� ��ͬBEEP=1;	
				else GPIO_ResetBits(GPIOF,GPIO_Pin_8); //BEEP�������ͣ� ��ͬBEEP=0;			
				break;
			default:break;
		}
	}else delay_ms(10); 
}

//����
void remoteFunction(void)
{	
	u8 key=Remote_Scan();	
	if(key)
	{  
		switch(key)
		{		    
			case 226:
				if(screen==0){
					GPIO_SetBits(GPIOB,GPIO_Pin_15);   //�ر���Ļ
					screen=1;
				}else {
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					screen=0;
				}
				break;	    
			case 104:
				if(LED0==0)
					LED0=1;	   //LED0��Ӧ����GPIOF.0���ߣ���
				else LED0=0;  //LED0��Ӧ����GPIOF.9���ͣ���
				break;		  
			case 152:
				if(BEEP==0)
					BEEP=1;   //BEEP��������	
				else BEEP=0; //BEEP��������
				break;	   	 
			default:break;
		}
	}
	delay_ms(10);
}