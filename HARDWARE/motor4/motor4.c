#include "motor4.h"

float Velocity_KP=40,Velocity_KI=60,Velocity_KD=0.9;//30ms 7200预装载值  

int Bias_A,Pwm_A,Last_bias_A,Integeal_bias_A;
int Bias_B,Pwm_B,Last_bias_B,Integeal_bias_B;
int Bias_C,Pwm_C,Last_bias_C,Integeal_bias_C;
int Bias_D,Pwm_D,Last_bias_D,Integeal_bias_D;

int Motor_A,Motor_B,Motor_C,Motor_D;
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;
/**************************************************************************
函数功能：设置马达各个电机IO口
入口参数：自定义PWM
返回  值：无
**************************************************************************/
void Motor_Gpio_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);// 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
   //设置该引脚为复用输出功能,输出TIM3 CH3 CH4 PWM脉冲波形  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_6|GPIO_Pin_7 |GPIO_Pin_8|GPIO_Pin_9; //TIM3_CH3 //TIM3_CH4
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
//	 Motor_Gpio();
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**************************************************************************
函数功能：赋值给PWM寄存器  预装载值7200 预分频值0
入口参数：自定义PWM
返回  值：无
**************************************************************************/
void Set_Pwm_Motor1(int motor_a)
{
    	if(motor_a<0)			PWMA1=TIM_FOUR_ARR,PWMA2=TIM_FOUR_ARR+motor_a;
			else 	            PWMA2=TIM_FOUR_ARR,PWMA1=TIM_FOUR_ARR-motor_a;
}

void Set_Pwm_Motor2(int motor_b)
{
		  if(motor_b<0)			PWMB2=TIM_FOUR_ARR,PWMB1=TIM_FOUR_ARR+motor_b;
			else 	            PWMB1=TIM_FOUR_ARR,PWMB2=TIM_FOUR_ARR-motor_b;
}
void Set_Pwm_Motor3(int motor_c)
{
    	if(motor_c<0)			PWMC1=TIM_FOUR_ARR,PWMC2=TIM_FOUR_ARR+motor_c;
			else 	            PWMC2=TIM_FOUR_ARR,PWMC1=TIM_FOUR_ARR-motor_c;
}
void Set_Pwm_Motor4(int motor_d)
{
		  if(motor_d<0)			PWMD2=TIM_FOUR_ARR,PWMD1=TIM_FOUR_ARR+motor_d;
			else 	            PWMD1=TIM_FOUR_ARR,PWMD2=TIM_FOUR_ARR-motor_d;
}
/**************************************************************************
函数功能：PID数据重置
入口参数：无
返回  值：无
**************************************************************************/
void PID_Reset_Data(void)
{
	 Pwm_A=0;Last_bias_A=0;Integeal_bias_A=0;Set_Pwm_Motor1(0);
	 Pwm_B=0;Last_bias_B=0;Integeal_bias_B=0;Set_Pwm_Motor2(0);
	 Pwm_C=0;Last_bias_C=0;Integeal_bias_C=0;Set_Pwm_Motor3(0);
	 Pwm_D=0;Last_bias_D=0;Integeal_bias_D=0;Set_Pwm_Motor4(0);
}
	/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/

int Incremental_PI_A (int Encoder,int Target)
{ 	
   Bias_A=Target-Encoder;
	 Integeal_bias_A+=Bias_A;
	 if(Integeal_bias_A>700) {Integeal_bias_A=700;}
	 else if(Integeal_bias_A<-700) {Integeal_bias_A=-700;}
	 Pwm_A+=Velocity_KP*(Bias_A-Last_bias_A)+Velocity_KI*Bias_A+Velocity_KD*Integeal_bias_A;   //增量式PI控制器
	 if(Integeal_bias_A>=300 || Integeal_bias_A<=-300) {PID_Reset_Data();}
	 Last_bias_A=Bias_A;	                   //保存上一次偏差 
	 return Pwm_A;   	 //增量输出
//	_dbg_printf("Integeal_bias_A  %d \n",Motor_B);	 
}

 int Incremental_PI_B (int Encoder,int Target)
{ 	
   Bias_B=Target-Encoder;
	 Integeal_bias_B+=Bias_B;
	 if(Integeal_bias_B>700) {Integeal_bias_B=700;}
	 else if(Integeal_bias_B<-700) {Integeal_bias_B=-700;}
	 Pwm_B+=Velocity_KP*(Bias_B-Last_bias_B)+Velocity_KI*Bias_B+Velocity_KD*Integeal_bias_B;   //增量式PI控制器
	 if(Integeal_bias_B>=300 || Integeal_bias_B<=-300) {PID_Reset_Data();}
	 Last_bias_B=Bias_B;	                   //保存上一次偏差 
	 return Pwm_B;                         //增量输出
//	 _dbg_printf("Integeal_bias_B  %d \n",Integeal_bias_B);	 
}

 int Incremental_PI_C (int Encoder,int Target)
{ 	
   Bias_C=Target-Encoder;
	 Integeal_bias_C+=Bias_C;
	 if(Integeal_bias_C>700) {Integeal_bias_C=700;}
	 else if(Integeal_bias_C<-700) {Integeal_bias_C=-700;}
	 Pwm_C+=Velocity_KP*(Bias_C-Last_bias_C)+Velocity_KI*Bias_C+Velocity_KD*Integeal_bias_C;   //增量式PI控制器
	 if(Integeal_bias_C>=300 || Integeal_bias_C<=-300) {PID_Reset_Data();}
	 Last_bias_C=Bias_C;	                   //保存上一次偏差 
	 return Pwm_C;                         //增量输出
//	 _dbg_printf("Integeal_bias_C  %d \n",Integeal_bias_C);	 
} 

int Incremental_PI_D (int Encoder,int Target)
{ 	
   Bias_D=Target-Encoder;
	 Integeal_bias_D+=Bias_D;
	 if(Integeal_bias_D>700){Integeal_bias_D=700;}
	 else if(Integeal_bias_D<-700){Integeal_bias_D=-700;}
	 Pwm_D+=Velocity_KP*(Bias_D-Last_bias_D)+Velocity_KI*Bias_D+Velocity_KD*Integeal_bias_D;   //增量式PI控制器
	 if(Integeal_bias_D>=300 || Integeal_bias_D<=-300) {PID_Reset_Data();}
	 Last_bias_D=Bias_D;	                   //保存上一次偏差 
//	  _dbg_printf("Integeal_bias_D  %d \n",Integeal_bias_D);	 
	 return Pwm_D;                         //增量输出
}
/**************************************************************************
函数功能：1.限制PWM赋值
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	 
	  int Amplitude=7100;    //===PWM满幅是7199 限制在7100
    if(Motor_A<-Amplitude) Motor_A=-Amplitude;	if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	if(Motor_B>Amplitude)  Motor_B=Amplitude;	
		if(Motor_C<-Amplitude) Motor_C=-Amplitude;	if(Motor_C>Amplitude)  Motor_C=Amplitude;	
	  if(Motor_D<-Amplitude) Motor_D=-Amplitude;	if(Motor_D>Amplitude)  Motor_D=Amplitude;	
}
/**************************************************************************
函数功能：通过限制编码器的值来限制轮胎转速 A前左 D前右  B后左 C后右
入口参数：int number
返回  值：无
**************************************************************************/

void motor_speed_A(int number)
{
		  Encoder_A=Read_Encoder(1);
			Motor_A=Incremental_PI_A(Encoder_A,number);                   //===速度闭环控制计算电机A最终PWM      
			Xianfu_Pwm();                                                      //===PWM限幅
			Set_Pwm_Motor1(Motor_A);
//	    _dbg_printf("Motor_A  %d \n",Motor_A);
}
void motor_speed_B(int number)
{
			Encoder_B=Read_Encoder(2);          
		  Motor_B=Incremental_PI_B(Encoder_B,number);                  //===速度闭环控制计算电机B最终PWM 
	    Xianfu_Pwm();                                                      //===PWM限幅
			Set_Pwm_Motor2(Motor_B);
}
void motor_speed_C(int number)
{
		  Encoder_C=Read_Encoder(3);
			Motor_C=Incremental_PI_C(Encoder_C,number);                   //===速度闭环控制计算电机A最终PWM      
			Xianfu_Pwm();                                                      //===PWM限幅
			Set_Pwm_Motor3(Motor_C);
}
void motor_speed_D(int number)
{
		  Encoder_D=Read_Encoder(4);
			Motor_D=Incremental_PI_D(Encoder_D,number);                   //===速度闭环控制计算电机A最终PWM      
			Xianfu_Pwm();                                                      //===PWM限幅
			Set_Pwm_Motor4(Motor_D);
//	    _dbg_printf("Motor_D  %d \n",Motor_D);
}
/**************************************************************************
函数功能：控制所有的轮胎
入口参数：speed
返回  值：无
**************************************************************************/
void ALL_motor_control (int A_speed,int B_speed,int C_speed,int D_speed)
{
	motor_speed_A(A_speed);
  motor_speed_B(B_speed);
	motor_speed_C(C_speed);
	motor_speed_D(D_speed);
}
void ALL_motor_pwm_control (int A_Pwm,int B_Pwm,int C_Pwm,int D_Pwm)
{
	Set_Pwm_Motor1(A_Pwm);
	Set_Pwm_Motor2(B_Pwm);
	Set_Pwm_Motor3(C_Pwm);
	Set_Pwm_Motor4(D_Pwm);
}
/**************************************************************************
函数功能：使小车停止
入口参数：int Xcm
返回  值：无
**************************************************************************/
void CAR_STOP(void)
{
	Read_Encoder(1);Read_Encoder(2);Read_Encoder(3);Read_Encoder(4);
	Set_Pwm_Motor1(0);Set_Pwm_Motor2(0);Set_Pwm_Motor3(0);Set_Pwm_Motor4(0);
}
/**************************************************************************
函数功能：根据角度值判断小车向左还是向右转
入口参数：Angual,Gear1,Gear2
返回  值：无
**************************************************************************/	
void car_motor_wheel_speed(int Angual ,int Gear1,int Gear2)
{
					if(Angual>0)//x<0时方向右 x>0时方向为左。
		{
			motor_speed_A (Gear2);
			motor_speed_D (Gear1);
//			_dbg_printf("/ → → → → → → / \n",Gear2);
		}
		else if(Angual<0)
		{
			motor_speed_A (Gear1);
			motor_speed_D (Gear2);
//			_dbg_printf("/ ← ← ← ← ← ← / \n",Gear2);
		}	
}
/**************************************************************************
函数功能：角度判断小车的转向速度
入口参数：Angual
返回  值：无
**************************************************************************/	
void Angle_judgement(int Angual)
{
	if(Angual<=-80 || Angual>=80)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_8);}	
	else if(Angual<=-70 || Angual>=70)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_8);}	
	else if(Angual<=-60 || Angual>=60)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_7);}	
	else if(Angual<=-50 || Angual>=50)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_6);}
	else if(Angual<=-40 || Angual>=40)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_5);}
	else if(Angual<=-30 || Angual>=30)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_4);}
  else if(Angual<=-20 || Angual>=20)
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_3);}
	else if(Angual<=-Straight_angle || Angual>=Straight_angle)//Straight_angle=8;
	{car_motor_wheel_speed( Angual,Turning_angle_level_1,Turning_angle_level_2);} 
}
/**************************************************************************
函数功能：通过distance与Angual判断的运动方向 
入口参数：int Angual
返回  值：无
**************************************************************************/
void motor_direction(int Angual,int distance)//小车方向判断
{
	if(distance>=Distance_speed_leve2)//距离大于Distance_speed_leve2 小车才拐弯
	{Angle_judgement(Angual);}
	
	else if (distance>=Distance_speed_leve1)
	{CAR_STOP();}
	else if (distance>=0)
	{
		motor_speed_A(gear_level_4); 
		motor_speed_D(gear_level_4);
//		_dbg_printf("/ ↓ ↓ ↓ ↓ ↓ ↓  / \n");
	}
}
/**************************************************************************
函数功能：根据距离调节车速
入口参数：int Xcm
返回  值：无
**************************************************************************/
void car_motor_speed (int distance)
{

	 if(distance>=Distance_speed_leve4)//500
	{ motor_speed_A(gear_level_3); motor_speed_D(gear_level_3);}
	
	else if(distance>=Distance_speed_leve3)//300
	{motor_speed_A(gear_level_2);  motor_speed_D(gear_level_2);}
	
	else if( distance>=Distance_speed_leve2)//100
	{motor_speed_A(gear_level_1);  motor_speed_D(gear_level_1);}
	
	else if(distance>=Distance_speed_leve1)//30
	{CAR_STOP();}
	else if(distance>=0)
	{
		motor_speed_A(-gear_level_4); 
		motor_speed_D(-gear_level_4);
//		_dbg_printf("/ ↓ ↓ ↓ ↓ ↓ ↓  / \n");
	}
}

/**************************************************************************
函数功能：通过角度判断小车是转弯还是直行
入口参数：int Xcm
返回  值：无
**************************************************************************/
void car_motor_direction(int Angual,int distance)
{ 
	static int Angle_correction,TRA_DMG=Distance_speed_leve2,back_up;
	if(distance<TRA_DMG )
	{
		Angle_correction++;back_up++;
		TRA_DMG=Distance_speed_leve3;
    if(distance<Distance_speed_leve1 && back_up>10){motor_speed_A (-5);motor_speed_D (-5);}//后退		
	  else if (Angual>(Straight_angle+15) && Angle_correction>5){ motor_speed_A (-7);motor_speed_D (7);}
	  else if(Angual<-(Straight_angle+15) && Angle_correction>5){ motor_speed_A (7);motor_speed_D (-7);}
		else if(distance>=Distance_speed_leve1){back_up=0;CAR_STOP();}//停止
	}
	else if(Angual<Straight_angle && Angual>-Straight_angle )//直行
  {
		Angle_correction=0;
		TRA_DMG=Distance_speed_leve2;
		car_motor_speed(distance);
	}
	else 
  {
	 Angle_judgement(Angual);
	 Angle_correction=0;TRA_DMG=Distance_speed_leve2;
	}
//	if(Angual<Straight_angle && Angual>-Straight_angle)//直行
//  {
//		car_motor_speed(distance);
////	  _dbg_printf("/ ↑↑↑↑↑↑↑↑/ \n");
//	}
//	else{motor_direction(Angual,distance);}	//拐弯
}