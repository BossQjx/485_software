#include "tcp_demo.h"
#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	
#include "led.h"
#include "delay.h"
#include "enc28j60.h"
#include "wdg.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK战舰STM32开发板
//uIP TCP Client测试 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/28
//版本：V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////

//u8 tcp_client_databuf[100]={1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};   //发送数据缓存	
extern void uip_polling(void);
extern int recend_flag;
u8 tcp_client_databuf[100];   //发送数据缓存	
u8 tcp_client_sta;				//服务端状态
u8 tcp_client_receivebuf[22];  //	接受命令缓存
u8 ready=0;				//客户端状态
u8 connected=0;
//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//这是一个TCP 客户端应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1400),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
// void tcp_client_demo_appcall(void)
// {		  
//  	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
// 	if(uip_aborted())tcp_client_aborted();		//连接终止	   
// 	if(uip_timedout())tcp_client_timedout();	//连接超时   
// 	if(uip_closed())tcp_client_closed();		//连接关闭	   
//  	if(uip_connected())tcp_client_connected();	//连接成功	    
// 	if(uip_acked())tcp_client_acked();			//发送的数据成功送达 
//  	//接收到一个新的TCP数据包 
// 	if (uip_newdata())
// 	{
// 		if((tcp_client_sta&(1<<6))==0)//还未收到数据
// 		{
// 			if(uip_len>199)
// 			{		   
// 				((u8*)uip_appdata)[199]=0;
// 			}		    
// 	    	strcpy((char*)tcp_client_receivebuf,uip_appdata);				   	  		  
// 			tcp_client_sta|=1<<6;//表示收到客户端数据
// 		}				  
// 	}else if(tcp_client_sta&(1<<5))//有数据需要发送
// 	{
// 		s->textptr=tcp_client_databuf;
// 		s->textlen=strlen((const char*)tcp_client_databuf);
// 		tcp_client_sta&=~(1<<5);//清除标记
// 	}  
// 	//当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
// 	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
// 	{
// 		tcp_client_senddata();
// 	}											   
// }





//自己改的程序
void tcp_client_demo_appcall(void)
{		  
 	int t;
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//connected=1;
	if(uip_aborted())tcp_client_aborted();		//连接终止	   
	if(uip_timedout())tcp_client_timedout();	//连接超时   
	if(uip_closed())tcp_client_closed();		//连接关闭	   
 	if(uip_connected())tcp_client_connected();	//连接成功	    
	if(uip_acked())tcp_client_acked();			//发送的数据成功送达 
 	//接收到一个新的TCP数据包 
	if (uip_newdata())
	{
		if((tcp_client_sta&(1<<6))==0)//还未收到数据
		{
			if(uip_len>21)
			{		   
				((u8*)uip_appdata)[21]=0;
			}
        for(t=0;t<21;t++)
	      tcp_client_receivebuf[t]= ((u8*)uip_appdata)[t];
			
// 	    strcpy((char*)tcp_client_receivebuf,uip_appdata);				   	  		  
			tcp_client_sta|=1<<6;//表示收到客户端数据
		}				  
	}else if(tcp_client_sta&(1<<5))//有数据需要发送
	{
		s->textptr=&tcp_client_databuf[0];
		s->textlen=56;
	
		tcp_client_sta&=~(1<<5);//清除标记
		
	}  


	//当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
//	if(uip_rexmit()||uip_newdata()||uip_connected()||uip_poll())
	if((uip_rexmit()||uip_poll())/*&& recend_flag==1 */)
	{
		tcp_client_senddata();
		recend_flag=0; 
//		for(t=6;t<28;t++)					//测试用  记得改回
//		  tcp_client_databuf[t]=0x00;
//		if(uip_rexmit())
//		{
//			tcp_client_databuf[8]=0x11;
//		}
//		else if(uip_poll())
//		{
//			tcp_client_databuf[8]=0x12;
//		}
	}											   
}









//这里我们假定Server端的IP地址为:192.168.1.103
//这个IP必须根据Server端的IP修改.
//尝试重新连接
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	//while(tapdev_init())	//初始化ENC28J60错误
	//{								   
//		printf("EN28J60 Init Error");
		//delay_ms(200);
	//};
//	uip_ipaddr(ipaddr, 192,168,1,16);	//设置本地设置IP地址
//	uip_sethostaddr(ipaddr);					    
//	uip_ipaddr(ipaddr, 192,168,1,1); 	//设置网关IP地址(其实就是你路由器的IP地址)
//	uip_setdraddr(ipaddr);						 
//	uip_ipaddr(ipaddr, 255,255,255,0);	//设置网络掩码
//	uip_setnetmask(ipaddr);
		uip_init();				//uIP初始化	  

 	uip_ipaddr(ipaddr, 192,168,1,16);	//设置本地设置IP地址
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,1,1); 	//设置网关IP地址(其实就是你路由器的IP地址)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//设置网络掩码
	uip_setnetmask(ipaddr);
	
	uip_ipaddr(&ipaddr,192,168,1,192);	//设置IP为192.168.1.105
	uip_connect(&ipaddr,htons(1400)); 	//端口为1400
	
	//uip_connect(&ipaddr,htons(8018)); 	//端口为1400
}


//终止连接				    
void tcp_client_aborted(void)
{
	//tcp_client_sta&=~(1<<7);	//标志没有连接
//	connected=0;
	//tcp_client_reconnect();		//尝试重新连接
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1000);
//	uip_polling();
	connected=3;
	LED0=0;
	uip_log("tcp_client aborted!\r\n");//打印log
}


//连接超时
void tcp_client_timedout(void)
{
//	tcp_client_sta&=~(1<<7);	//标志没有连接	 
	//tcp_client_reconnect();		//尝试重新连接
	LED1=0;
	connected=2;
//	delay_ms(1800);
//	delay_ms(1800);
//	delay_ms(1800);
	uip_log("tcp_client timeout!\r\n");//打印log
}
//连接关闭
void tcp_client_closed(void)
{
//	tcp_client_sta&=~(1<<7);	//标志没有连接
	LED0=0;
	connected=4;
	//tcp_client_reconnect();		//尝试重新连接
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1000);
//	uip_polling();
	
	uip_log("tcp_client closed!\r\n");//打印log
}	 
//连接建立
void tcp_client_connected(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 //	tcp_client_sta|=1<<7;		//标志连接成功
	connected=1;
  	uip_log("tcp_client connected!\r\n");//打印log
	s->state=STATE_CMD; 		//指令状态
	s->textlen=0;
	//LED1=!LED1;
	//s->textptr="ALIENTEK STM32 Board Connected Successfully!\r\n";//回应消息
	//s->textlen=strlen((char *)s->textptr);	  
}
//发送的数据成功送达
void tcp_client_acked(void)
{											    
// 	int t;
	
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	led_breath(4,1);
	s->textlen=0;//发送清零
	
	//uip_log("tcp_client acked!\r\n");//表示成功发送		
}
//发送数据给服务端
void tcp_client_senddata(void)
{
	
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	led_breath(4,0);
	//s->textptr:发送的数据包缓冲区指针
	//s->textlen:数据包的大小（单位字节）		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//发送TCP数据包	 
}


















