
#include "timer.h"
#include "stm32f10x_iwdg.h"
/*
 * ��������IWDG_Init
 * ����  ����ʼ���������Ź�
 * ����  ��prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!),��Ƶ����=4*2^prer.�����ֵֻ����256!
					 rlr:��װ�ؼĴ���ֵ:��11λ��Ч.ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
 * ���  ����
 */
void HalIwdgInit(u8 prer,u16 rlr) 
{
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����	
		IWDG_SetPrescaler(prer);	//����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
		IWDG_SetReload(rlr);			//����IWDG��װ��ֵ
		IWDG_ReloadCounter(); 		//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
		IWDG_Enable();						//ʹ��IWDG
}

void HalIwdgFeed(void)
{
	 	IWDG_ReloadCounter();			//reload		
}
/**********************************************************************************/

/**********************��ʱ��4�ж����ú���*****************************************/
void TIM1_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**********************************************************************************/

/**********************��ʱ��2�ж����ú���*****************************************/
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************��ʱ��3�ж����ú���*********************************************/
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/
/**********************��ʱ��4�ж����ú���*********************************************/
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************/

/************************��ʱ��1���ú���***********************************************/
void TIM1_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
		
		TIM_DeInit(TIM1); 										/* ����������ʱ�� */
		
		TIM_TimeBaseStructure.TIM_Period=TIM1_ARR;						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
		TIM_TimeBaseStructure.TIM_Prescaler=TIM1_PSC; 					/* ʱ��Ԥ��Ƶ�� 72M/72 */
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		
		TIM_ClearFlag(TIM1, TIM_FLAG_Update); 					/* �������жϱ�־ */
		
		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					/* �����жϴ���*/ 	 
		
		TIM_Cmd(TIM1,DISABLE);											/* �رն�ʱ�� */ 

}

/**************************************************************************************/
/************************��ʱ��3���ú���***********************************************/
void TIM3_Configuration(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		
		TIM_TimeBaseStructure.TIM_Period = TIM3_ARR;							//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ 80K
		TIM_TimeBaseStructure.TIM_Prescaler = TIM3_PSC;						//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ����Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//�ظ��Ĵ����������Զ�����
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//�ڳ�ʼ�� TIMx

		
		TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //�������ģʽ ͨ��1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//ʹ�ܻ��������
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; //���������״̬
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//�����󻥲������״̬
		TIM_OCInitStructure.TIM_Pulse = 0; 							//���ô�װ�벶��ȽϼĴ���������ֵ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //���û������������
		
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_Cmd(TIM3, ENABLE); //��ʹ�� TIM2
		
		TIM_CtrlPWMOutputs(TIM3,ENABLE); //��MOE �����ʹ��
		
		TIM_SetAutoreload(TIM3, TIM3_ARR);
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0);
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare4(TIM3,0);
		
		return;
}
/**************************************************************************************/
/************************��ʱ��4���ú���***********************************************/
void TIM4_Configuration(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
		
		TIM_TimeBaseStructure.TIM_Period = TIM4_ARR;							//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ 80K
		TIM_TimeBaseStructure.TIM_Prescaler = TIM4_PSC;						//����������Ϊ TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ ����Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//�ظ��Ĵ����������Զ�����
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//�ڳ�ʼ�� TIMx

		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //�������ģʽ ͨ��1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//ʹ�ܻ��������
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; //���������״̬
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//�����󻥲������״̬
		TIM_OCInitStructure.TIM_Pulse = 0; 							//���ô�װ�벶��ȽϼĴ���������ֵ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //���û������������
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); 			//�۳�ʼ������ TIMx	
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 Ԥװ��ʹ��
		
		TIM_Cmd(TIM4, ENABLE); //��ʹ�� TIM2
		
		TIM_CtrlPWMOutputs(TIM4,ENABLE); //��MOE �����ʹ��
		
		TIM_SetAutoreload(TIM4, TIM4_ARR);
		TIM_SetCompare1(TIM4,0);
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
		
		return;
}
/**************************************************************************************/
/************************��ʱ�����ú���************************************************/
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
    
	  TIM_DeInit(TIM5);											/* ����������ʱ�� */

    TIM_TimeBaseStructure.TIM_Period=65535;		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);						/* �������жϱ�־ */

    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);					/* �����жϴ���*/    
	
	  TIM_Cmd(TIM5,DISABLE);									    /* �رն�ʱ�� */ 
}
/**************************************************************************************/
/************************��ʱ����ʼ������*********************************************/
void Timer_Init(void)
{
	HalIwdgInit(7,625);
	TIM1_NVIC_Configuration();
  TIM1_Configuration();

   //TIM3_NVIC_Configuration();
	TIM3_Configuration();

	//TIM4_NVIC_Configuration();
	TIM4_Configuration();

}
/**************************************************************************************/
/************************������ʱ������************************************************/
void Timer_ON(void)
{	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	//TIM_Cmd(TIM3,ENABLE);
	//TIM_Cmd(TIM4,ENABLE);	
}
/**************************************************************************************/
/************************�رն�ʱ������************************************************/
void Timer_OFF(void)
{	
	TIM_Cmd(TIM2,DISABLE);
	//TIM_Cmd(TIM3,DISABLE);
	//TIM_Cmd(TIM4,DISABLE);	
}
/**************************************************************************************/

