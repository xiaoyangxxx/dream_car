#include <string.h>
#include "stm32f10x.h"	    				//包含stm32库系统头文件						
#include "delay.h"						//包含系统定时器库头文件
#include "led.h"
#include "beep.h"
#include "common.h"
#include "timer.h"
#include "motor4.h"
#include "hal_usart.h"
#include "control.h"
#include "uwb.h"
#include "oled_i2c.h"
#include "Data_display.h"
extern int car_stop;
int main(void)
{   
	SysTick_Init();		//系统滴答定时器初始化 		
	LED_Init();	      //LED 初始化函数
	Beep_Init();      //蜂鸣器初始化函数
	Beep_Test();      //蜂鸣器测试
	Led_Test();       //默认红灯蓝灯开机闪烁,用户可自行设置 
	Timer_Init();     //定时器初始化
	Timer_ON();       //开启定时器
	Uart_Queue_Init();//AOA数据队列初始化
	HalUARTInit();    //USART初始化
	Motor_Gpio_init();//马达IO口初始化
	EXTI_ALL_Init();  //外部中断初始化
	I2C_Configuration();//I2C总线初始化
	OLED_Initial_display(1);//运行这条程序时确认OLED屏已安装好，否则程序会卡死在这里
	OLED_Initial_display(2);
	OLED_Initial_display(3);	
	while (1)
	{
	  follow_car_task();
//	 ALL_motor_control (50,50,50,50);
//	 car_stop=0;
//	 Delay_ms(40);
	}
} 