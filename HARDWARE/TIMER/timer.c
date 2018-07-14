#include "timer.h"
#include "led.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "beep.h"
#include "key.h"
#include "remote.h"

void remoteFunction(void);
void keyFunction(void);

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u8 screen=0;
//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
		keyFunction();
		remoteFunction();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}



//����
void keyFunction() 
{
	u8 key=KEY_Scan(0);		//�õ���ֵ
	if(key)
	{						   
		switch(key)
		{				 
			case KEY0_PRES:
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
	}
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
}