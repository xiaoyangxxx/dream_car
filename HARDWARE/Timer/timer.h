#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"
#define TIM1_ARR 1999
#define TIM1_PSC	71
#define TIM4_ARR  7199
#define TIM4_PSC	0
#define TIM3_ARR  7199
#define TIM3_PSC	0

void Timer_Init(void);
void Timer_ON(void);
void Timer_OFF(void);
void HalIwdgFeed(void);

#endif	/* TIME_TEST_H */
