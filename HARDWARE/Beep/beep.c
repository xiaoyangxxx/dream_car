#include "beep.h"
#include "delay.h"
/**************************************************************************
�������ܣ�������IO��ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/

void Beep_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //ʹ��PD�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  //������-->PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //����Ϊ�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHZ
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//���ݲ�����ʼ��PB12	
	GPIO_SetBits(GPIOD,GPIO_Pin_2);				 //PB12 �����

}
/**************************************************************************
�������ܣ����������Ժ���
��ڲ�������
����  ֵ���� 
**************************************************************************/ 
void Beep_Test(void)
{
		BEEP(ON);  
	  Delay_ms(500);
		BEEP(OFF);
	  Delay_ms(500);

}