

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



//#define UIP_APPCALL tcp_demo_appcall //����ص�����Ϊ tcp_demo_appcall 

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
u8 slaveraddr=1 ;			//�Ӱ�ӻ���ַ
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

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart1_init(115200);   //���ڳ�ʼ��
	uart2_init(9600);    //485��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	delay_ms(1000);
	//LED0 = !LED0;
	
	 IWDG_Init(6,625);  // �������Ź� 4S   4 1s
	 	 
  //�ɼ�id   & data_len
   tcp_client_databuf[0]='D';
	 tcp_client_databuf[1]='A';
	 tcp_client_databuf[2]='T';
	 tcp_client_databuf[3]='A';
	 tcp_client_databuf[4]=0x00;
	 tcp_client_databuf[5]=0x16;
	

  while(tapdev_init())	//��ʼ��ENC28J60����
	{			
		printf("EN28J60 Init Error");
		delay_ms(200);
	};
	printf("ENC28J60 Init OK!\n\r");
	tcp_client_reconnect();	   		//�������ӵ�TCP Server��,����TCP Client
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
	 	if(tcp_client_tsta!=tcp_client_sta)//TCP Client״̬�ı�
		{															 
       uip_polling();
 			if(tcp_client_sta&(1<<6))	//�յ�������
			{
				if((tcp_client_receivebuf[0]=='D')&&(tcp_client_receivebuf[1]=='A')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='A'))
				{
				
					receivebuf = (((u16)tcp_client_receivebuf[5])<<8)|tcp_client_receivebuf[6];

					flag1=tcp_client_receivebuf[4];
					
					
					
//					rtu_set_hldreg(0x02,sendBuf,0,receivebuf);	   //���Ϳ�ʼ��������
				}
				else if((tcp_client_receivebuf[0]=='S')&&(tcp_client_receivebuf[1]=='E')&&(tcp_client_receivebuf[2]=='T')&&(tcp_client_receivebuf[3]=='D'))
				{
					channel=tcp_client_receivebuf[6];
					vl = (u16)tcp_client_receivebuf[7];
					
					set_dout_value(channel,vl );
//					rtu_set_hldreg(0x02,sendBuf,0,receivebuf);	   //���Ϳ�ʼ��������
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
				
				tcp_client_sta&=~(1<<6);		//��������Ѿ�������	
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
	if(timer_ok==0)//����ʼ��һ��
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/5);  //����1��0.5��Ķ�ʱ�� 
		timer_set(&arp_timer,CLOCK_SECOND*15);	   	//����1��10��Ķ�ʱ�� 
	}				 
	uip_len=tapdev_read();	//�������豸��ȡһ��IP��,�õ����ݳ���.uip_len��uip.c�ж���
	if(uip_len>0) 			//������
	{  // tcp_client_databuf[6]=uip_len;
		//����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//�Ƿ���IP��? 
		{
			uip_arp_ipin();	//ȥ����̫��ͷ�ṹ������ARP��
			uip_input();   	//IP������
			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
			//��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)		
			if(uip_len>0)//��Ҫ��Ӧ����
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
		else if (BUF->type==htons(UIP_ETHTYPE_ARP))//����arp����,�Ƿ���ARP�����?
		{
			uip_arp_arpin();
 			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len(����2��ȫ�ֱ���)
 			if(uip_len>0)tapdev_send();//��Ҫ��������,��ͨ��tapdev_send����	 
		}
	}
	else if(timer_expired(&periodic_timer))	//0.5�붨ʱ����ʱ
	{
	//	LED1=!LED1;
		timer_reset(&periodic_timer);		//��λ0.5�붨ʱ�� 
		//��������ÿ��TCP����, UIP_CONNSȱʡ��10��  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//����TCPͨ���¼�  
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 		if(uip_len>0)
			{	
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#if UIP_UDP	//UIP_UDP 
		//��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10��
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//����UDPͨ���¼�
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
			if(uip_len > 0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#endif 
		//ÿ��10�����1��ARP��ʱ������ ���ڶ���ARP����,ARP��10�����һ�Σ��ɵ���Ŀ�ᱻ����
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}
