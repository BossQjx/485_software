#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
void SPI1_Init(void);
void SPI1_SetSpeed(u16 SPI_BaudRatePrescaler);
u16 SPI1_ReadWriteByte(u16 TxData);
void SPI1_WriteByte(u16 TxData);
u16 SPI1_ReadByte(u16 TxData);
void NVIC_Configuration_Spi(void);
 				  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
#define SIZE 2		 
		 
#endif

