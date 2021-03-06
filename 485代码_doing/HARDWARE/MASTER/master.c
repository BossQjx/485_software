/*
*
*	主机服务程序
*
*/

#include"master.h"
#include"usart.h"
#include"delay.h"
#include"tcp_demo.h"

u8 FLAG_caiji;	   //等于0时，采集电压，等于1时,采集内阻
extern u8 index_qd;
u8 n=0;

extern u8 yewei_flag;

void construct_rtu_frm ( u8 *dst_buf,u8 *src_buf,u8 lenth)
{
    unsigned short  crc_tmp;
    crc_tmp = crc16(src_buf, lenth);	
    *(src_buf+lenth) = crc_tmp >> 8 ;
    *(src_buf+lenth+1) = crc_tmp & 0xff;
    lenth++;
    lenth++;
    while ( lenth--)
    {
       *dst_buf = *src_buf;
       dst_buf++;
       src_buf++;     
    }
}



u16  rtu_read_hldreg ( u8 board_adr,u8 *com_buf,u16 start_address,u16 lenth) //03
{

    unsigned char tmp[32], tmp_lenth;   
    tmp[0] = board_adr;
    tmp[1] = READ_HLD_REG;
    tmp[2] = HI(start_address);
    tmp[3] = LOW(start_address);
    tmp[4] = HI(lenth);
    tmp[5] = LOW(lenth);
    tmp_lenth = 6;
    construct_rtu_frm ( com_buf,tmp,tmp_lenth);
	  sendCount=8;
	  beginSend();
    return 8;//为了防止编译器报错//
}

u16 rtu_set_hldreg( u8 board_adr,u8 *com_buf, u16 start_address, u16 value )//06
{
    unsigned char tmp[32], tmp_lenth;   
    tmp[0] = board_adr;
    tmp[1] = SET_HLD_REG;
    tmp[2] = HI(start_address);
    tmp[3] = LOW(start_address);
    tmp[4] = HI(value);
    tmp[5] = LOW(value);  
    tmp_lenth = 6;
    construct_rtu_frm ( com_buf, tmp, tmp_lenth);
	  sendCount=8;
	  beginSend();
    return 8 ;

}
//
void set_dout_value(u8 channel,u16 vl)//channel是通道 value是电平值
{
	rtu_set_hldreg(0x02, sendBuf, channel, vl);
}
//void set_time(u8 channel,u16 vl)
//{
//	
//}
//void set_all_dout_value(u16 vl)//channel是通道 value是电平值
//{ 
////	int i;
////	for(int i= 0;i<16;i++)
////	{
////		 if(channel==i) 
//	u8 channel;
//		 rtu_set_hldreg(0x02, sendBuf, channel, vl);
////	}
//		
//}
int rtu_data_anlys( u8  *dest_p, u8 *source_p, u16 data_start_address, u16 fr_lenth)//rtu 接收分析//
{
    u16 crc_result, crc_tmp;
    u8 tmp1; 
		 
	crc_tmp = *(source_p + fr_lenth-2); // crc  第一字节//
 	crc_tmp = crc_tmp * 256 + *( source_p+fr_lenth-1); // CRC 值// 
	crc_result = crc16(source_p, fr_lenth-2); // 计算CRC 值
//  if ( crc_tmp != crc_result ) // CRC 校验正确//
	if ( crc_tmp == crc_result ) // CRC 校验正确//
    {
// 		hld_reg[0x31]++;
// 		return -1;
//	}
		switch ( *(source_p+1) ) // 功能码//
		{
// 		   case READ_COIL:                   //读取继电器状态//
// 		   for ( tmp1=0; tmp1<*( source_p+2); tmp1++)
// 		   {
// 				shift = 1;
// 				for ( tmp2=0; tmp2<8; tmp2++)
// 				{ 
// 					 *(dest_p+data_start_address+tmp1*8+tmp2) = shift & *( source_p+3);
// 					 *( source_p+3) >>= 1;          
// 				}
// 		   }
// 		   break;
// 		   case READ_DI: //读取开关量输入//
// 		   for ( tmp1=0; tmp1<*( source_p+2); tmp1++)
// 		   {
// 				shift = 1;
// 				for (tmp2=0; tmp2<8; tmp2 ++)
// 				{ 
// 					*(dest_p+data_start_address+tmp1*8+tmp2) = shift & *( source_p+3);
// 					*( source_p+3)>>=1;             
// 				}
// 		   }
// 		   break;
		   case READ_HLD_REG:  //读取保持寄存器//
		   for ( tmp1=0; tmp1<*( source_p+2); tmp1++)
		   {
				//relay17_he;
				*(dest_p + data_start_address+ tmp1)= *( source_p+tmp1+3) ;
				//relay17_he;        
		   }
		   break ;
// 		   case 4:      //读取模拟量输入//
// 		   for ( tmp1=0; tmp1<*( source_p+2); tmp1+=2)
// 		   {
// 				*(dest_p + data_start_address+ tmp1/2) = *( source_p+tmp1+3)*256 +  *( source_p+tmp1+4) ;      
// 		   }
// 		   break;
		   case PROTOCOL_EXCEPTION:
		   return -1*PROTOCOL_ERR;     //整数转换导致类型变化的迹象
		   //break;
		   default:
		   return -1*PROTOCOL_ERR;		//整数转换导致类型变化的迹象
		   // break;
		}
	}
	return 0;	
}


void modbus_rtu(void) //电压读取命令
{
	u8 t;
	if(modbus_com2_over==2&&receBuf[0]==1)
	{
		modbus_com2_over=0;
		rtu_data_anlys(value,receBuf,0,21);	
		for(t=0;t<16;t++)
    {
			tcp_client_databuf[t+6]=value[t];
//			printf("%d",t);
		}	
		ready =1;
	}
	else if(modbus_com2_over==2&&receBuf[0]==3)
	{
		modbus_com2_over=0;
		rtu_data_anlys(value,receBuf,16,11);
    for(t=16;t<22;t++)
    {
			tcp_client_databuf[t+6]=value[t];
			
			yewei_flag=value[21];
			
//			printf("%x",value[t]);
//			printf("%d",t);
		}
		ready =1;
	}
// 	else
// 	{
// 		 for(t=0;t<38;t++)
// 			tcp_client_databuf[t+4]=0x55;
// 		ready =1;
// 	}
	receCount=0;
   
	if(slaveraddr==1)
	{
		rtu_read_hldreg(1,sendBuf,0,8);
		slaveraddr=3;
	}
	else if(slaveraddr==3) 
	{
		rtu_read_hldreg(3,sendBuf,0,3);
		slaveraddr=1;
	}
	
}


const u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ; 


const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;
u16 crc16(u8 *puchMsg, u16 usDataLen) 
{ 
	u8 uchCRCHi = 0xFF ; 
	u8 uchCRCLo = 0xFF ; 
	u32 uIndex ; 
	while (usDataLen--) 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}//uint16 crc16(uint8 *puchMsg, uint16 usDataLen)

















