#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
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
#define LED0 PCout(11)// PB5
#define LED1 PCout(12)// PE5	

#define  LED_0N_B1    GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define  LED_OFF_B1   GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define  LED_0N_B0    GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define  LED_OFF_B0   GPIO_SetBits(GPIOB,GPIO_Pin_0)

void LED_Init(void);//��ʼ��

void led_breath(int freq,int led_No);
		 				    
#endif
