#include <string.h>
#include "stm32f10x.h"	    				//����stm32��ϵͳͷ�ļ�						
#include "delay.h"						//����ϵͳ��ʱ����ͷ�ļ�
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
	SysTick_Init();		//ϵͳ�δ�ʱ����ʼ�� 		
	LED_Init();	      //LED ��ʼ������
	Beep_Init();      //��������ʼ������
	Beep_Test();      //����������
	Led_Test();       //Ĭ�Ϻ�����ƿ�����˸,�û����������� 
	Timer_Init();     //��ʱ����ʼ��
	Timer_ON();       //������ʱ��
	Uart_Queue_Init();//AOA���ݶ��г�ʼ��
	HalUARTInit();    //USART��ʼ��
	Motor_Gpio_init();//���IO�ڳ�ʼ��
	EXTI_ALL_Init();  //�ⲿ�жϳ�ʼ��
	I2C_Configuration();//I2C���߳�ʼ��
	OLED_Initial_display(1);//������������ʱȷ��OLED���Ѱ�װ�ã��������Ῠ��������
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