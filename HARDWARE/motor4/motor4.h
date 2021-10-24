#include "stm32f10x.h"	
#include "encod.h"
#include "hal_usart.h" 
#define  Mode_selection_0//Mode_selection_0 调试模式  Mode_selection_1 工作模式 
#define TIM_FOUR_ARR 7200

#define PWMA1   TIM3->CCR3   //PB0
#define PWMA2   TIM3->CCR4   //PB1

#define PWMB1   TIM3->CCR1   //PA6
#define PWMB2   TIM3->CCR2   //Pa7

#define PWMC1   TIM4->CCR1   //PB6
#define PWMC2   TIM4->CCR2   //PB7

#define PWMD1   TIM4->CCR3   //PB8
#define PWMD2   TIM4->CCR4   //PB9
//距离判断 根据实际修改
#define Distance_speed_leve1  0   // 安全距离50~100 小于50厘米则后退
#define Distance_speed_leve2  100  
#define Distance_speed_leve3  200
#define Distance_speed_leve4  300
#define Distance_speed_leve5  400

 //直行速度 三挡可调
#define  gear_level_1    36
#define  gear_level_2    36
#define  gear_level_3    36
#define  gear_level_4    20  	//后退档位

//直行角度  不同发距离应该需要不同的直行角度
#define  Straight_angle 10
//                                  
//通过控制左轮或右轮速度来实现转弯 根据实际情况改变        
#define  Turning_angle_level_1  36  
#define  Turning_angle_level_2  30 
#define  Turning_angle_level_3  26 
#define  Turning_angle_level_4  20  
 
#define  Turning_angle_level_5  16  
#define  Turning_angle_level_6  10  
#define  Turning_angle_level_7  8          
#define  Turning_angle_level_8  6   
/**************************************************************************
各类主要函数体
**************************************************************************/
void Motor_Gpio_init(void);//设置马达IO口

//设置各马达的PWM值
void Set_Pwm_Motor1(int motor_a);
void Set_Pwm_Motor2(int motor_b);
void Set_Pwm_Motor3(int motor_c);
void Set_Pwm_Motor4(int motor_d);

//PID算法通过编码器的输出的脉冲数计算当前需要的PWM值
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
int Incremental_PI_C (int Encoder,int Target);
int Incremental_PI_D (int Encoder,int Target);

void Xianfu_Pwm(void);//pwm限制器

//通过编码器的输出的脉冲数设定马达PWM值
void motor_speed_A (int number);
void motor_speed_B (int number);
void motor_speed_C (int number);
void motor_speed_D (int number);

//根据角度值和距离值控制小车运动
void motor_direction(int Angual,int distance);
void Angle_judgement(int Angual);//角度判断
void car_motor_wheel_speed(int Angual ,int Gear1,int Gear2);
void car_motor_direction(int Angual,int distance);
/**************************************************************************
各类次要函数体
**************************************************************************/
//小车马达控制
void CAR_STOP(void);//小车停止
void ALL_motor_control (int A_speed,int B_speed,int C_speed,int D_speed);//用PID算法控制所有马达的转速
void ALL_motor_pwm_control (int A_Pwm,int B_Pwm,int C_Pwm,int D_Pwm);//直接设置PWM值改变马达速度
