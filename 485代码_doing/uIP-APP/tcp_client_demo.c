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
//ALIENTEKս��STM32������
//uIP TCP Client���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/28
//�汾��V1.0			   								  
//////////////////////////////////////////////////////////////////////////////////

//u8 tcp_client_databuf[100]={1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};   //�������ݻ���	
extern void uip_polling(void);
extern int recend_flag;
u8 tcp_client_databuf[100];   //�������ݻ���	
u8 tcp_client_sta;				//�����״̬
u8 tcp_client_receivebuf[22];  //	���������
u8 ready=0;				//�ͻ���״̬
u8 connected=0;
//[7]:0,������;1,�Ѿ�����;
//[6]:0,������;1,�յ��ͻ�������
//[5]:0,������;1,��������Ҫ����

//����һ��TCP �ͻ���Ӧ�ûص�������
//�ú���ͨ��UIP_APPCALL(tcp_demo_appcall)����,ʵ��Web Client�Ĺ���.
//��uip�¼�����ʱ��UIP_APPCALL�����ᱻ����,���������˿�(1400),ȷ���Ƿ�ִ�иú�����
//���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
// void tcp_client_demo_appcall(void)
// {		  
//  	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
// 	if(uip_aborted())tcp_client_aborted();		//������ֹ	   
// 	if(uip_timedout())tcp_client_timedout();	//���ӳ�ʱ   
// 	if(uip_closed())tcp_client_closed();		//���ӹر�	   
//  	if(uip_connected())tcp_client_connected();	//���ӳɹ�	    
// 	if(uip_acked())tcp_client_acked();			//���͵����ݳɹ��ʹ� 
//  	//���յ�һ���µ�TCP���ݰ� 
// 	if (uip_newdata())
// 	{
// 		if((tcp_client_sta&(1<<6))==0)//��δ�յ�����
// 		{
// 			if(uip_len>199)
// 			{		   
// 				((u8*)uip_appdata)[199]=0;
// 			}		    
// 	    	strcpy((char*)tcp_client_receivebuf,uip_appdata);				   	  		  
// 			tcp_client_sta|=1<<6;//��ʾ�յ��ͻ�������
// 		}				  
// 	}else if(tcp_client_sta&(1<<5))//��������Ҫ����
// 	{
// 		s->textptr=tcp_client_databuf;
// 		s->textlen=strlen((const char*)tcp_client_databuf);
// 		tcp_client_sta&=~(1<<5);//������
// 	}  
// 	//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
// 	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
// 	{
// 		tcp_client_senddata();
// 	}											   
// }





//�Լ��ĵĳ���
void tcp_client_demo_appcall(void)
{		  
 	int t;
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//connected=1;
	if(uip_aborted())tcp_client_aborted();		//������ֹ	   
	if(uip_timedout())tcp_client_timedout();	//���ӳ�ʱ   
	if(uip_closed())tcp_client_closed();		//���ӹر�	   
 	if(uip_connected())tcp_client_connected();	//���ӳɹ�	    
	if(uip_acked())tcp_client_acked();			//���͵����ݳɹ��ʹ� 
 	//���յ�һ���µ�TCP���ݰ� 
	if (uip_newdata())
	{
		if((tcp_client_sta&(1<<6))==0)//��δ�յ�����
		{
			if(uip_len>21)
			{		   
				((u8*)uip_appdata)[21]=0;
			}
        for(t=0;t<21;t++)
	      tcp_client_receivebuf[t]= ((u8*)uip_appdata)[t];
			
// 	    strcpy((char*)tcp_client_receivebuf,uip_appdata);				   	  		  
			tcp_client_sta|=1<<6;//��ʾ�յ��ͻ�������
		}				  
	}else if(tcp_client_sta&(1<<5))//��������Ҫ����
	{
		s->textptr=&tcp_client_databuf[0];
		s->textlen=56;
	
		tcp_client_sta&=~(1<<5);//������
		
	}  


	//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
//	if(uip_rexmit()||uip_newdata()||uip_connected()||uip_poll())
	if((uip_rexmit()||uip_poll())/*&& recend_flag==1 */)
	{
		tcp_client_senddata();
		recend_flag=0; 
//		for(t=6;t<28;t++)					//������  �ǵøĻ�
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









//�������Ǽٶ�Server�˵�IP��ַΪ:192.168.1.103
//���IP�������Server�˵�IP�޸�.
//������������
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	//while(tapdev_init())	//��ʼ��ENC28J60����
	//{								   
//		printf("EN28J60 Init Error");
		//delay_ms(200);
	//};
//	uip_ipaddr(ipaddr, 192,168,1,16);	//���ñ�������IP��ַ
//	uip_sethostaddr(ipaddr);					    
//	uip_ipaddr(ipaddr, 192,168,1,1); 	//��������IP��ַ(��ʵ������·������IP��ַ)
//	uip_setdraddr(ipaddr);						 
//	uip_ipaddr(ipaddr, 255,255,255,0);	//������������
//	uip_setnetmask(ipaddr);
		uip_init();				//uIP��ʼ��	  

 	uip_ipaddr(ipaddr, 192,168,1,16);	//���ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,1,1); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//������������
	uip_setnetmask(ipaddr);
	
	uip_ipaddr(&ipaddr,192,168,1,192);	//����IPΪ192.168.1.105
	uip_connect(&ipaddr,htons(1400)); 	//�˿�Ϊ1400
	
	//uip_connect(&ipaddr,htons(8018)); 	//�˿�Ϊ1400
}


//��ֹ����				    
void tcp_client_aborted(void)
{
	//tcp_client_sta&=~(1<<7);	//��־û������
//	connected=0;
	//tcp_client_reconnect();		//������������
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1000);
//	uip_polling();
	connected=3;
	LED0=0;
	uip_log("tcp_client aborted!\r\n");//��ӡlog
}


//���ӳ�ʱ
void tcp_client_timedout(void)
{
//	tcp_client_sta&=~(1<<7);	//��־û������	 
	//tcp_client_reconnect();		//������������
	LED1=0;
	connected=2;
//	delay_ms(1800);
//	delay_ms(1800);
//	delay_ms(1800);
	uip_log("tcp_client timeout!\r\n");//��ӡlog
}
//���ӹر�
void tcp_client_closed(void)
{
//	tcp_client_sta&=~(1<<7);	//��־û������
	LED0=0;
	connected=4;
	//tcp_client_reconnect();		//������������
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1500);
//	uip_polling();
//	delay_ms(1000);
//	uip_polling();
	
	uip_log("tcp_client closed!\r\n");//��ӡlog
}	 
//���ӽ���
void tcp_client_connected(void)
{ 
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 //	tcp_client_sta|=1<<7;		//��־���ӳɹ�
	connected=1;
  	uip_log("tcp_client connected!\r\n");//��ӡlog
	s->state=STATE_CMD; 		//ָ��״̬
	s->textlen=0;
	//LED1=!LED1;
	//s->textptr="ALIENTEK STM32 Board Connected Successfully!\r\n";//��Ӧ��Ϣ
	//s->textlen=strlen((char *)s->textptr);	  
}
//���͵����ݳɹ��ʹ�
void tcp_client_acked(void)
{											    
// 	int t;
	
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	led_breath(4,1);
	s->textlen=0;//��������
	
	//uip_log("tcp_client acked!\r\n");//��ʾ�ɹ�����		
}
//�������ݸ������
void tcp_client_senddata(void)
{
	
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	led_breath(4,0);
	//s->textptr:���͵����ݰ�������ָ��
	//s->textlen:���ݰ��Ĵ�С����λ�ֽڣ�		   
	if(s->textlen>0)uip_send(s->textptr, s->textlen);//����TCP���ݰ�	 
}


















