

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "rtc.h" 
#include "adc.h"
#include "tsensor.h"
#include "enc28j60.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timerx.h"				   
#include "math.h" 	
#include "string.h"
#include "timer.h"
#include "spi.h"
#include "suart.h"
#include "wdg.h"
#include "master.h"



//#define UIP_APPCALL tcp_demo_appcall //定义回调函数为 tcp_demo_appcall 

void uip_polling(void);	 	
void receive_data(void);
void delay_1s(void);
void delay_500ms(void);
void delay_50ms(void);
void reconnect(void);

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	 


u8 yewei_flag;
u8 Tn;

u8 modbus_com2_over;
u8 value[38];
u8 slaveraddr=1 ;			//子板从机地址
//int i;
int j,j1;
int z,z1;
u8 flag1;
u8 timer4_out=1;
int flag_1=0,recend_flag=0;

u8 tcp_client_tsta=0XFF;
u16 receivebuf = 0;
u8 channel;
u16 vl;

uint16_t debug_buf[50];


 int main(void)
 {	 u8 connected_ok=0;	
	 int p=0,k=1;
  
	 uip_ipaddr_t ipaddr;

	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart1_init(115200);   //串口初始化
	uart2_init(9600);    //485初始化
 	LED_Init();			     //LED端口初始化
	delay_ms(1000);
	//LED0 = !LED0;
	
	 IWDG_Init(6,625);  // 独立看门狗 4S   4 1s
	 	 
  //采集id   & data_len
   tcp_client_databuf[0]='D';
	 tcp_client_databuf[1]='A';
	 tcp_client_databuf[2]='T';
	 tcp_client_databuf[3]='A';
	 tcp_client_databuf[4]=0x00;
	 tcp_client_databuf[5]=0x16;
	

  while(tapdev_init())	//初始化ENC28J60错误
	{			
		printf("EN28J60 Init Error");
		delay_ms(200);
	};
	printf("ENC28J60 Init OK!\n\r");
	tcp_client_reconnect();	   		//尝试连接到TCP Server端,用于TCP Client
	receEnable();
	Timer3_Init(50,719);//1.75ms				
	while(1)
	{
		recend_flag=1;
		uip_polling();
		tcp_client_sta|=1<<5;
		//printf("%d\r\n",connected);
		
		if(connected>1)
		{
		 do
		 {		
			reconnect();
		 }while(connected>1);
		}
		
	}
	
}
 void reconnect(void)
 {
			printf("I am in 2th while route\r\n");	
			printf("I`m reconnecting , connected is :%d\r\n",connected);
			tcp_client_reconnect();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
			delay_ms(100);
			uip_polling();
 }


void delay_1s(void)
{
	for(j=0;j<1000;j++)
	{
	
		delay_ms(1);
		uip_polling();
		uip_polling();
		uip_polling();
		receive_data();
		
	}
}

void delay_500ms(void)
{
	for(j=0;j<500;j++)
	{
			delay_ms(1);
		  uip_polling();
		uip_polling();
		   uip_polling();
		  receive_data();
	}
}

void delay_50ms(void)
{
	for(z=0;z<150;z++)
	{
			delay_ms(1);
		  uip_polling(); 
		  uip_polling();
		uip_polling();
		  receive_data();
	}
}
void receive_data(void)
 {
	 	if(tcp_client_tsta!=tcp_client_sta)//TCP Client状态改变
		{															 
       uip_polling();
 			if(tcp_client_sta&(1<<6))	//收到新数据
			{
				if((tcp_client_receivebuf[0]=='D')&&(tcp_client_receivebuf[1]=='A')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='A'))
				{
				
					receivebuf = (((u16)tcp_client_receivebuf[5])<<8)|tcp_client_receivebuf[6];

					flag1=tcp_client_receivebuf[4];
					
					
					
//					rtu_set_hldreg(0x02,sendBuf,0,receivebuf);	   //发送开始测量命令
				}
				else if((tcp_client_receivebuf[0]=='S')&&(tcp_client_receivebuf[1]=='E')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='D'))
				{
					channel=tcp_client_receivebuf[6];
					vl = (u16)tcp_client_receivebuf[7];
					
					set_dout_value(channel,vl );
//					rtu_set_hldreg(0x02,sendBuf,0,receivebuf);	   //发送开始测量命令
				}
				else if((tcp_client_receivebuf[0]=='S')&&(tcp_client_receivebuf[1]=='E')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='T'))
				{
					Tn=tcp_client_receivebuf[6];
					
			}
				else if((tcp_client_receivebuf[0]=='S')&&(tcp_client_receivebuf[1]=='E')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='F'))
				{
					flag1=tcp_client_receivebuf[6];
					 uip_polling();
				}
				
				tcp_client_sta&=~(1<<6);		//标记数据已经被处理	
			}
			tcp_client_tsta=tcp_client_sta;
		}
 }

void uip_polling(void)
{
	u8 i;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
	IWDG_Feed();
	if(timer_ok==0)//仅初始化一次
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/5);  //创建1个0.5秒的定时器 
		timer_set(&arp_timer,CLOCK_SECOND*15);	   	//创建1个10秒的定时器 
	}				 
	uip_len=tapdev_read();	//从网络设备读取一个IP包,得到数据长度.uip_len在uip.c中定义
	if(uip_len>0) 			//有数据
	{  // tcp_client_databuf[6]=uip_len;
		//处理IP数据包(只有校验通过的IP包才会被接收) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//是否是IP包? 
		{
			uip_arp_ipin();	//去除以太网头结构，更新ARP表
			uip_input();   	//IP包处理
			//当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
			//需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)		
			if(uip_len>0)//需要回应数据
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
		else if (BUF->type==htons(UIP_ETHTYPE_ARP))//处理arp报文,是否是ARP请求包?
		{
			uip_arp_arpin();
 			//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len(这是2个全局变量)
 			if(uip_len>0)tapdev_send();//需要发送数据,则通过tapdev_send发送	 
		}
	}
	else if(timer_expired(&periodic_timer))	//0.5秒定时器超时
	{
	//	LED1=!LED1;
		timer_reset(&periodic_timer);		//复位0.5秒定时器 
		//轮流处理每个TCP连接, UIP_CONNS缺省是10个  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//处理TCP通信事件  
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
	 		if(uip_len>0)
			{	
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#if UIP_UDP	//UIP_UDP 
		//轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//处理UDP通信事件
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
			if(uip_len > 0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#endif 
		//每隔10秒调用1次ARP定时器函数 用于定期ARP处理,ARP表10秒更新一次，旧的条目会被抛弃
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}
