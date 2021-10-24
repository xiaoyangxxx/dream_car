#include "encod.h"
int Encoder_A_EXTI=0,Encoder_B_EXTI=0,Encoder_D_EXTI=0;
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure; 
	
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD-1; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}
/**************************************************************************
�������ܣ��ⲿ�жϲɼ������� ���D  IO�� PC0 PC1
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM_Exit0(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Encoder_Init_TIM_Exit1(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);		
}
/**************************************************************************
�������ܣ��ⲿ�жϲɼ������� ���B  IO�� PC2 PC3
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM_Exit2(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
}
void Encoder_Init_TIM_Exit3(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
}
/**************************************************************************
�������ܣ��ⲿ�жϲɼ������� ���A  IO�� PC6 PC7
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM_Exit6(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

void Encoder_Init_TIM_Exit7(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PA�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	            //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
		GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	  
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

/**************************************************************************
�������ܣ��ⲿ�ж�ʹ�ܺ���
��ڲ�������
����  ֵ����
**************************************************************************/
void EXTI_ALL_Init(void)
{
	Encoder_Init_TIM2();
	Encoder_Init_TIM_Exit1();
	Encoder_Init_TIM_Exit0();
	Encoder_Init_TIM_Exit2();
	Encoder_Init_TIM_Exit3();
	Encoder_Init_TIM_Exit6();
	Encoder_Init_TIM_Exit7();
}
/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
		 case 1:  Encoder_TIM=(short)Encoder_A_EXTI;  Encoder_A_EXTI=0; break;
		 case 2:  Encoder_TIM=(short)Encoder_B_EXTI;  Encoder_B_EXTI=0; break;
		 case 3:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	
		 case 4:  Encoder_TIM=(short)Encoder_D_EXTI;  Encoder_D_EXTI=0; break;
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}


 int Read_Encoder1(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 1:  Encoder_TIM=(short)Encoder_A_EXTI;break;
		 case 2:  Encoder_TIM=(short)Encoder_B_EXTI;break;
		 case 3:  Encoder_TIM= (short)TIM2 -> CNT;break;	
		 case 4:  Encoder_TIM=(short)Encoder_D_EXTI;break;
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
/**************************************************************************
�������ܣ�EXTI�жϷ�����  ͨ���ⲿ�ж�ʵ�ֱ���������
��ڲ�������
����  ֵ����
**************************************************************************/
/**************************************************************************
���D  EXTI�ⲿ�ж�
**************************************************************************/
void EXTI1_IRQHandler(void)//PC1
{			
		EXTI->PR=1<<1;  //���LINE�ϵ��жϱ�־λ
	if(PCin(1)==1)
	{
	if(PCin(0)==0)  Encoder_D_EXTI++;   
	else            Encoder_D_EXTI--;
	}
	else
	{
	if(PCin(0)==0)  Encoder_D_EXTI--;
	else            Encoder_D_EXTI++;
	}		
}
void EXTI0_IRQHandler(void)  //PC0
{
		EXTI->PR=1<<0;  //���LINE�ϵ��жϱ�־λ
		if(PCin(0)==0)
	{
	if(PCin(1)==0)   Encoder_D_EXTI++;
	else             Encoder_D_EXTI--;
	}
	else
	{
	if(PCin(1)==0)   Encoder_D_EXTI--;
	else             Encoder_D_EXTI++;
	}		
}
/**************************************************************************
���B  EXTI�ⲿ�ж�
**************************************************************************/
void EXTI3_IRQHandler(void)//PC3
{			
		EXTI->PR=1<<3;  //���LINE�ϵ��жϱ�־λ
	if(PCin(3)==1)
	{
	if(PCin(2)==0)  Encoder_B_EXTI++;   
	else            Encoder_B_EXTI--;
	}
	else
	{
	if(PCin(2)==0)  Encoder_B_EXTI--;
	else            Encoder_B_EXTI++;
	}		
}
void EXTI2_IRQHandler(void)  //PC2
{
		EXTI->PR=1<<2;  //���LINE�ϵ��жϱ�־λ
		if(PCin(2)==0)
	{
	if(PCin(3)==0)   Encoder_B_EXTI++;
	else             Encoder_B_EXTI--;
	}
	else
	{
	if(PCin(3)==0)   Encoder_B_EXTI--;
	else             Encoder_B_EXTI++;
	}		
}
/**************************************************************************
���A  EXTI�ⲿ�ж�
**************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line6)!=RESET)
	{
		 EXTI->PR=1<<6;
			if(PCin(6)==0)
			{
			if(PCin(7)==0)  Encoder_A_EXTI--;   
			else            Encoder_A_EXTI++;
			}
			else
			{
			if(PCin(7)==0)  Encoder_A_EXTI++;
			else            Encoder_A_EXTI--;
			}	
	}
	if(EXTI_GetFlagStatus(EXTI_Line7)!=RESET)
	{
		 EXTI->PR=1<<7;
				if(PCin(7)==1)
			{
			if(PCin(6)==0)  Encoder_A_EXTI--;
			else            Encoder_A_EXTI++;
			}
			else
			{
			if(PCin(6)==0)  Encoder_A_EXTI++;
			else            Encoder_A_EXTI--;
			}	
	}
//	EXTI->PR=1<<7;
//  EXTI->PR=1<<6;	
}

