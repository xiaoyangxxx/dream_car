#include "beep.h"
#include "delay.h"
/**************************************************************************
函数功能：蜂鸣器IO初始化
入口参数：无
返回  值：无 
**************************************************************************/

void Beep_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //使能PD端口时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  //蜂鸣器-->PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHZ
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//根据参数初始化PB12	
	GPIO_SetBits(GPIOD,GPIO_Pin_2);				 //PB12 输出高

}
/**************************************************************************
函数功能：蜂鸣器测试函数
入口参数：无
返回  值：无 
**************************************************************************/ 
void Beep_Test(void)
{
		BEEP(ON);  
	  Delay_ms(500);
		BEEP(OFF);
	  Delay_ms(500);

}