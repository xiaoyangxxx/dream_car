#ifndef __CONTROL_H
#define __CONTROL_H
#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************************************
 * 																				INCLUDES
 **************************************************************************************************/
#include "stm32f10x.h"
#include "OSAL_Clock.h"
#include "math.h"
#include "Data_display.h"
#include "led.h"

#define Action      1     //允许车辆移动
#define FILTER_N 12       //过滤器数组长度设置
#define turn_around 6     //掉头判定
#define Angle_Buffering 100//当连续有15个差异信号时，取用该值
#define Angular_limit   10//当差异值大于15或小于-15是抛弃该值
typedef struct
{
  int	Angle_filter;
	int	Distance_filter;
}AOA_DATA;


void follow_car_task(void);
double Angle_transformation(int Xcm,int distance );//直角计算单次角度
double Angle_Trigonometric(int Xcm,int Ycm,int distance);//任意三角形三边计算
double Angle_average(int Xcm,int distance );//5次角度平均值
void car_motor_speed (int distance);

int AOA_Angle_difference(int Angle_final);
int AOA_Angle_difference(int Angle);
int AOA_final_angle (int Angle_difference,int Angle );
int car_distance (int AOA_distance);


#ifdef __cplusplus
}
#endif
#endif
