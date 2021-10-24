#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"	 
#include "oled_i2c.h"
#include "hal_usart.h"
#include "delay.h"

void  OLED_Initial_display(int word);
void OLED_Data_display(float New_Angle,int New_distance);
