#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
// void LED_Init(void)
// {
//  
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  	
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
// 	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PB�˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
//  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);						 //PB0 PB1�����

// }
 

void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED1-->PC.11 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.11
 GPIO_SetBits(GPIOC,GPIO_Pin_11);						 //PC.11 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED2-->PC.12 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_12); 						 //PC.12 ����� 
}

void led_breath(int freq,int led_No)
{
	int i=0;
	if(led_No==0)
	{
		for(i=0;i<freq;i++)
		{	
			LED0=!LED0;
			delay_ms(30);
		}
	}
	if(led_No==1)
	{
		for(i=0;i<freq;i++)
		{	
			LED1=!LED1;
			delay_ms(30);
		}
	}
}