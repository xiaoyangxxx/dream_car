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

#define Action      1     //�������ƶ�
#define FILTER_N 12       //���������鳤������
#define turn_around 6     //��ͷ�ж�
#define Angle_Buffering 100//��������15�������ź�ʱ��ȡ�ø�ֵ
#define Angular_limit   10//������ֵ����15��С��-15��������ֵ
typedef struct
{
  int	Angle_filter;
	int	Distance_filter;
}AOA_DATA;


void follow_car_task(void);
double Angle_transformation(int Xcm,int distance );//ֱ�Ǽ��㵥�νǶ�
double Angle_Trigonometric(int Xcm,int Ycm,int distance);//�������������߼���
double Angle_average(int Xcm,int distance );//5�νǶ�ƽ��ֵ
void car_motor_speed (int distance);

int AOA_Angle_difference(int Angle_final);
int AOA_Angle_difference(int Angle);
int AOA_final_angle (int Angle_difference,int Angle );
int car_distance (int AOA_distance);


#ifdef __cplusplus
}
#endif
#endif
