#include "stm32f10x.h"	
#include "encod.h"
#include "hal_usart.h" 
#define  Mode_selection_0//Mode_selection_0 ����ģʽ  Mode_selection_1 ����ģʽ 
#define TIM_FOUR_ARR 7200

#define PWMA1   TIM3->CCR3   //PB0
#define PWMA2   TIM3->CCR4   //PB1

#define PWMB1   TIM3->CCR1   //PA6
#define PWMB2   TIM3->CCR2   //Pa7

#define PWMC1   TIM4->CCR1   //PB6
#define PWMC2   TIM4->CCR2   //PB7

#define PWMD1   TIM4->CCR3   //PB8
#define PWMD2   TIM4->CCR4   //PB9
//�����ж� ����ʵ���޸�
#define Distance_speed_leve1  0   // ��ȫ����50~100 С��50���������
#define Distance_speed_leve2  100  
#define Distance_speed_leve3  200
#define Distance_speed_leve4  300
#define Distance_speed_leve5  400

 //ֱ���ٶ� �����ɵ�
#define  gear_level_1    36
#define  gear_level_2    36
#define  gear_level_3    36
#define  gear_level_4    20  	//���˵�λ

//ֱ�нǶ�  ��ͬ������Ӧ����Ҫ��ͬ��ֱ�нǶ�
#define  Straight_angle 10
//                                  
//ͨ���������ֻ������ٶ���ʵ��ת�� ����ʵ������ı�        
#define  Turning_angle_level_1  36  
#define  Turning_angle_level_2  30 
#define  Turning_angle_level_3  26 
#define  Turning_angle_level_4  20  
 
#define  Turning_angle_level_5  16  
#define  Turning_angle_level_6  10  
#define  Turning_angle_level_7  8          
#define  Turning_angle_level_8  6   
/**************************************************************************
������Ҫ������
**************************************************************************/
void Motor_Gpio_init(void);//�������IO��

//���ø�����PWMֵ
void Set_Pwm_Motor1(int motor_a);
void Set_Pwm_Motor2(int motor_b);
void Set_Pwm_Motor3(int motor_c);
void Set_Pwm_Motor4(int motor_d);

//PID�㷨ͨ������������������������㵱ǰ��Ҫ��PWMֵ
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
int Incremental_PI_C (int Encoder,int Target);
int Incremental_PI_D (int Encoder,int Target);

void Xianfu_Pwm(void);//pwm������

//ͨ����������������������趨���PWMֵ
void motor_speed_A (int number);
void motor_speed_B (int number);
void motor_speed_C (int number);
void motor_speed_D (int number);

//���ݽǶ�ֵ�;���ֵ����С���˶�
void motor_direction(int Angual,int distance);
void Angle_judgement(int Angual);//�Ƕ��ж�
void car_motor_wheel_speed(int Angual ,int Gear1,int Gear2);
void car_motor_direction(int Angual,int distance);
/**************************************************************************
�����Ҫ������
**************************************************************************/
//С��������
void CAR_STOP(void);//С��ֹͣ
void ALL_motor_control (int A_speed,int B_speed,int C_speed,int D_speed);//��PID�㷨������������ת��
void ALL_motor_pwm_control (int A_Pwm,int B_Pwm,int C_Pwm,int D_Pwm);//ֱ������PWMֵ�ı�����ٶ�
