#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
 
#define SendingDelay	6000 
 
void TIM2_Int_Init(u16 arr,u16 psc);
// void TIM1_Configuration(void);

// void TIM3_Int_Init(u16 arr,u16 psc);

void Timer3_Init(u16 arr ,u16 psc);
void Timer3_enable(void);
void Timer3_disable(void);

void Timer4_enable(u16 arr,u16 psc);
void Timer4_disable(void);

extern u8 modbus_com_over;

#endif
