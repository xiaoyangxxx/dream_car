
#include "timer.h"
#include "stm32f10x_iwdg.h"
/*
 * 函数名：IWDG_Init
 * 描述  ：初始化独立看门狗
 * 输入  ：prer:分频数:0~7(只有低3位有效!),分频因子=4*2^prer.但最大值只能是256!
					 rlr:重装载寄存器值:低11位有效.时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 * 输出  ：无
 */
void HalIwdgInit(u8 prer,u16 rlr) 
{
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对寄存器IWDG_PR和IWDG_RLR的写操作	
		IWDG_SetPrescaler(prer);	//设置IWDG预分频值:设置IWDG预分频值为64
		IWDG_SetReload(rlr);			//设置IWDG重装载值
		IWDG_ReloadCounter(); 		//按照IWDG重装载寄存器的值重装载IWDG计数器
		IWDG_Enable();						//使能IWDG
}

void HalIwdgFeed(void)
{
	 	IWDG_ReloadCounter();			//reload		
}
/**********************************************************************************/

/**********************定时器4中断配置函数*****************************************/
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

/**********************定时器2中断配置函数*****************************************/
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
/**********************定时器3中断配置函数*********************************************/
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
/**********************定时器4中断配置函数*********************************************/
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

/************************定时器1配置函数***********************************************/
void TIM1_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
		
		TIM_DeInit(TIM1); 										/* 重新启动定时器 */
		
		TIM_TimeBaseStructure.TIM_Period=TIM1_ARR;						/* 自动重装载寄存器周期的值(计数值) */
		TIM_TimeBaseStructure.TIM_Prescaler=TIM1_PSC; 					/* 时钟预分频数 72M/72 */
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		
		TIM_ClearFlag(TIM1, TIM_FLAG_Update); 					/* 清除溢出中断标志 */
		
		TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					/* 开启中断触发*/ 	 
		
		TIM_Cmd(TIM1,DISABLE);											/* 关闭定时器 */ 

}

/**************************************************************************************/
/************************定时器3配置函数***********************************************/
void TIM3_Configuration(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		
		TIM_TimeBaseStructure.TIM_Period = TIM3_ARR;							//设置在下一个更新事件装入活动的自动重装载寄存器周期的值 80K
		TIM_TimeBaseStructure.TIM_Prescaler = TIM3_PSC;						//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//重复寄存器，用于自动更新
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//②初始化 TIMx

		
		TIM_ARRPreloadConfig(TIM3, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //脉宽调制模式 通道1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//使能互补端输出
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; //死区后输出状态
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//死区后互补端输出状态
		TIM_OCInitStructure.TIM_Pulse = 0; 							//设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //设置互补端输出极性
		
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_Cmd(TIM3, ENABLE); //④使能 TIM2
		
		TIM_CtrlPWMOutputs(TIM3,ENABLE); //⑤MOE 主输出使能
		
		TIM_SetAutoreload(TIM3, TIM3_ARR);
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0);
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare4(TIM3,0);
		
		return;
}
/**************************************************************************************/
/************************定时器4配置函数***********************************************/
void TIM4_Configuration(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
		
		TIM_TimeBaseStructure.TIM_Period = TIM4_ARR;							//设置在下一个更新事件装入活动的自动重装载寄存器周期的值 80K
		TIM_TimeBaseStructure.TIM_Prescaler = TIM4_PSC;						//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;					//重复寄存器，用于自动更新
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//②初始化 TIMx

		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //脉宽调制模式 通道1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;//使能互补端输出
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset; //死区后输出状态
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//死区后互补端输出状态
		TIM_OCInitStructure.TIM_Pulse = 0; 							//设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //设置互补端输出极性
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); 			//③初始化外设 TIMx	
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 	//CH1 预装载使能
		
		TIM_Cmd(TIM4, ENABLE); //④使能 TIM2
		
		TIM_CtrlPWMOutputs(TIM4,ENABLE); //⑤MOE 主输出使能
		
		TIM_SetAutoreload(TIM4, TIM4_ARR);
		TIM_SetCompare1(TIM4,0);
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
		
		return;
}
/**************************************************************************************/
/************************定时器配置函数************************************************/
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
    
	  TIM_DeInit(TIM5);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=65535;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);					/* 开启中断触发*/    
	
	  TIM_Cmd(TIM5,DISABLE);									    /* 关闭定时器 */ 
}
/**************************************************************************************/
/************************定时器初始化函数*********************************************/
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
/************************开启定时器函数************************************************/
void Timer_ON(void)
{	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	//TIM_Cmd(TIM3,ENABLE);
	//TIM_Cmd(TIM4,ENABLE);	
}
/**************************************************************************************/
/************************关闭定时器函数************************************************/
void Timer_OFF(void)
{	
	TIM_Cmd(TIM2,DISABLE);
	//TIM_Cmd(TIM3,DISABLE);
	//TIM_Cmd(TIM4,DISABLE);	
}
/**************************************************************************************/

