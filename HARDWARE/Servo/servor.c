#include "servor.h"
#include "delay.h"
#include "common.h"
u8 count1;				 //每个变量用作8路舵机先后赋值控制
extern unsigned int S_PWM;


void Servor_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
}
/************************GPIO电平反转函数1*****************************************/ 

void Flip_GPIO_One(void)
{
	switch(count1)								 //将20ms的舵机控制周期分成8份，每2.5ms控制一个舵机运转
	{  											      //每个定时器控制8路舵机运转，3个定时器控制24路舵机运转
		case 1: 
			 TIM2->ARR=S_PWM;				 //将第一个舵机脉冲宽度值赋值给定时器2			
    	 GPIO_SetBits(GPIOC,GPIO_Pin_0);  //同时拉高控制舵机1的引脚的电
			 break;
		
		case 2:
   	    TIM2->ARR=20000-S_PWM; 		 //将2.5ms减去PWM脉宽值的数据赋值定时器2
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);//同时拉低控制舵机1引脚的电平 
		    count1=0;
				break;							 //控制舵机1的引脚在剩下20ms-CPM[0]时间内将一直保持低电平，舵机1按照CPWM值转动
		
		default:break;
	}	
}

/************************舵机控制函数1*********************************************/
void Servo1(void)
{		
	count1++; 
	Flip_GPIO_One();						 //反转IO电平

}


