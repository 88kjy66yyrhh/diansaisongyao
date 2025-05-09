#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x.h"                  // Device header

void PWM_Init(void);
void PWM_SetComparePWMB(int Compare);
void PWM_SetComparePWMA(int Compare);

#endif
