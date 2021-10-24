#ifndef __BEEP_H__
#define __BEEP_H__
#include "sys.h"

#define ON  0   //�궨��
#define OFF 1
#define BEEP(a) if(a) \
											GPIO_SetBits(GPIOD,GPIO_Pin_2); \
								else \
											GPIO_ResetBits(GPIOD,GPIO_Pin_2)
								
#define	Beep PAout(8)  //ѡ��FLASH					

void Beep_Init(void);
void Beep_Test(void);
#endif
