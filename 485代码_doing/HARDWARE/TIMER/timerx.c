#include "timerx.h"
#include "led.h"
#include "usart.h"
#include "master.h"
#include "delay.h"
#include "tcp_demo.h"
#include "uip.h"
//#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//u8 yewei_flag;

//u8 yewei_flag;
//u8 yewei;
extern int recend_flag;
u32 uip_timer=0;//uip 计时器，每10ms增加1.

//定时器6中断服务程序	 
void TIM2_IRQHandler(void)
{ if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
  	uip_timer++;//uip计时器增加1	
	} 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
				    		  			    	    
}
 
//基本定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr,u16 psc)
{	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 								 
}



void Timer3_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能	 
//	TIM_TimeBaseStructure.TIM_Period = 5000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM2, //TIM2
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_ClearFlag(TIM3,TIM_FLAG_Update);

//	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设							 
}


void Timer3_enable( void )
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除溢出中断标志
	TIM_SetCounter(TIM3,0x00);			//清零计数器值
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void Timer3_disable (void)
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); //时钟失能
//	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
	TIM_Cmd(TIM3, DISABLE);  //失能TIMx外设
}


void Timer4_enable(u16 arr ,u16 psc)	   	//TIM4使能
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设							 
}

void Timer4_disable (void)					   //TIM4失能
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); //时钟失能
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
	TIM_Cmd(TIM4, DISABLE);  //失能TIMx外设
}


void TIM3_IRQHandler(void)   //TIM3中断
{int i=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		LED0=!LED0;									   //接收完成闪灯
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
void TIM4_IRQHandler(void)   //TIM4中断
{
//	int t;
   //uip_polling();
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		i++;
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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
//					tcp_client_reconnect();	   		//尝试连接到TCP Server端,用于TCP Client
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
//			//tcp_client_sta|=1<<5;//标记有数据需要发送
//			i=0;
//		}
//	}
if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	printf("I am in enable Timer4\r\n");
}
	timer4_out=1;
	Timer4_disable();
}


//实时监测液位开关



//void Timer5_enable(u16 arr ,u16 psc)	   	//TIM5使能
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能	
//	TIM_TimeBaseStructure.TIM_Period =100 ; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler = 71; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//     TIM5, //TIM5
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//		);

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设							 
//}

//void Timer5_disable (void)					   //TIM4失能
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); //时钟失能
//	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_Trigger,DISABLE );
//	TIM_Cmd(TIM5, DISABLE);  //失能TIMx外设
//}



//void TIM5_IRQHandler(void)   //TIM4中断
//{
////	int t;

//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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
// 	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;  //72分频，1Mhz/100频率
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

// 	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

// 	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);

// 	/* 开启TIM1的中断 */
// 	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);     
// 			
// 	/* 选择TIM1的中断通道 */
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;      
// 		
// 	/* 抢占式中断优先级设置为0 */
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	
// 	/* 响应式中断优先级设置为0 */
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	
// 	/* 使能中断 */
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

// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
// 	
// 	//定时器TIM3初始化
// 	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
// 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
// 	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
//  
// 	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

// 	//中断优先级NVIC设置
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
// 	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


// 	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
// }

