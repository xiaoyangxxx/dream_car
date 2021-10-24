#include "led.h"
#include "delay.h"


/**************************************************************************
�������ܣ�LED IO��ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTBʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;//PC4����  PC5���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOB14	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);				 //PC.4 �����
	GPIO_SetBits(GPIOC,GPIO_Pin_5);	
}
 /**************************************************************************
�������ܣ�LED ���Ժ��� ����LED 500msȻ��Ϩ��300ms ѭ��2��  LED1����,LED2���
��ڲ�������
����  ֵ���� 
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
