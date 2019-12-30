#include "timerx.h"
#include "led.h"
#include "usart.h"
#include "master.h"
#include "delay.h"
#include "tcp_demo.h"
#include "uip.h"
//#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//u8 yewei_flag;

//u8 yewei_flag;
//u8 yewei;
extern int recend_flag;
u32 uip_timer=0;//uip ��ʱ����ÿ10ms����1.

//��ʱ��6�жϷ������	 
void TIM2_IRQHandler(void)
{ if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
  	uip_timer++;//uip��ʱ������1	
	} 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
				    		  			    	    
}
 
//������ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Int_Init(u16 arr,u16 psc)
{	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܶ�ʱ��6���´����ж�
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 								 
}



void Timer3_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��	 
//	TIM_TimeBaseStructure.TIM_Period = 5000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM2, //TIM2
//		TIM_IT_Update  |  //TIM �ж�Դ
//		TIM_IT_Trigger,   //TIM �����ж�Դ 
//		ENABLE  //ʹ��
//		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ClearFlag(TIM3,TIM_FLAG_Update);

//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����							 
}


void Timer3_enable( void )
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); //�������жϱ�־
	TIM_SetCounter(TIM3,0x00);			//���������ֵ
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void Timer3_disable (void)
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); //ʱ��ʧ��
//	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
	TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����
}


void Timer4_enable(u16 arr ,u16 psc)	   	//TIM4ʹ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����							 
}

void Timer4_disable (void)					   //TIM4ʧ��
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); //ʱ��ʧ��
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
	TIM_Cmd(TIM4, DISABLE);  //ʧ��TIMx����
}


void TIM3_IRQHandler(void)   //TIM3�ж�
{int i=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		LED0=!LED0;									   //�����������
		recend_flag=1;
		Timer3_disable();
		modbus_com2_over=2;
		for(i=6;i<56;i++)
		tcp_client_databuf[i]=receBuf[i-6];
//		printf("%x,%x,%x,%d",receBuf[0],receBuf[1],receBuf[2],receCount);
		receCount=0;
	}
}
extern u8 timer4_out;
u8 i=0;
u8 t=0;
void TIM4_IRQHandler(void)   //TIM4�ж�
{
//	int t;
   //uip_polling();
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//	{
//		i++;
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//		if((i==1)||(i==2))
//		{
//			//modbus_rtu();
//			

//		}
//		else if(i==3)
//		{
//			if(connected==0)
//			{
//				t++;
//				if(t==8)
//				{
////					if(uip_closed())
//					tcp_client_reconnect();	   		//�������ӵ�TCP Server��,����TCP Client
//					t=0;
//				}
//			}
//			else
//			{
//				//connected=0;
//				t=0;
//			}
//		}
//		else 
//		{
//			//tcp_client_sta|=1<<5;//�����������Ҫ����
//			i=0;
//		}
//	}
if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	printf("I am in enable Timer4\r\n");
}
	timer4_out=1;
	Timer4_disable();
}


//ʵʱ���Һλ����



//void Timer5_enable(u16 arr ,u16 psc)	   	//TIM5ʹ��
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��	
//	TIM_TimeBaseStructure.TIM_Period =100 ; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
//	TIM_TimeBaseStructure.TIM_Prescaler = 71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//     TIM5, //TIM5
//		TIM_IT_Update  |  //TIM �ж�Դ
//		TIM_IT_Trigger,   //TIM �����ж�Դ 
//		ENABLE  //ʹ��
//		);

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����							 
//}

//void Timer5_disable (void)					   //TIM4ʧ��
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); //ʱ��ʧ��
//	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
//	TIM_Cmd(TIM5, DISABLE);  //ʧ��TIMx����
//}



//void TIM5_IRQHandler(void)   //TIM4�ж�
//{
////	int t;

//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//	{
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//      uip_polling();
//		
//	}
//		
//	}


// void TIM1_Configuration(void)
// {
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

// 	/* Time base configuration */
// 	TIM_TimeBaseStructure.TIM_Period = SendingDelay -1;    //1s
// 	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;  //72��Ƶ��1Mhz/100Ƶ��
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

// 	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

// 	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);

// 	/* ����TIM1���ж� */
// 	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);     
// 			
// 	/* ѡ��TIM1���ж�ͨ�� */
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;      
// 		
// 	/* ��ռʽ�ж����ȼ�����Ϊ0 */
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	
// 	/* ��Ӧʽ�ж����ȼ�����Ϊ0 */
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	
// 	/* ʹ���ж� */
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	
// 	TIM_Cmd(TIM1, ENABLE);
// 	
// }



// void TIM3_Int_Init(u16 arr,u16 psc)
// {
//   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
// 	
// 	//��ʱ��TIM3��ʼ��
// 	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
// 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
// 	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//  
// 	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

// 	//�ж����ȼ�NVIC����
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
// 	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


// 	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
// }

