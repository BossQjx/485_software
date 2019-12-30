#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
// void LED_Init(void)
// {
//  
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  	
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
// 	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //LED0-->PB端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
//  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);						 //PB0 PB1输出高

// }
 

void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED1-->PC.11 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.11
 GPIO_SetBits(GPIOC,GPIO_Pin_11);						 //PC.11 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED2-->PC.12 端口配置, 推挽输出
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_12); 						 //PC.12 输出高 
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