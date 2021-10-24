#include "led.h"
#include "delay.h"


/**************************************************************************
函数功能：LED IO初始化
入口参数：无
返回  值：无 
**************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTB时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;//PC4蓝灯  PC5红灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOB14	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);				 //PC.4 输出高
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	
}
 /**************************************************************************
函数功能：LED 测试函数 点亮LED 500ms然后熄灭300ms 循环2次  LED1蓝灯,LED2红灯
入口参数：无
返回  值：无 
**************************************************************************/
void Led_Test(void)
{
//		LED1(OFF);LED2(OFF);
	  Delay_ms(300);
		LED1(ON);LED2(ON);
	  Delay_ms(300);
		LED1(OFF);LED2(OFF);
	  Delay_ms(300);
		LED1(ON);LED2(ON);
	  Delay_ms(300);
		LED1(OFF);LED2(OFF);
}
