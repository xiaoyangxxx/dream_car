#include "stm32f10x.h"	
#include "stm32f10x_exti.h"
#include "sys.h"
#include "hal_usart.h"
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�


void Encoder_Init_TIM2(void);
void EXTI_ALL_Init(void);
int Read_Encoder(u8 TIMX);
int Read_Encoder1(u8 TIMX);
