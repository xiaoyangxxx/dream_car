#include "control.h"
#include "motor4.h"
#include "uwb.h"
#include "hal_usart.h"
//#include "servor.h"
#include "timer.h"
#include "math.h"

int car_stop=0,Angle_difference=0,car_starting,Car_move,Car_time;
/**************************************************************************
函数功能：卡尔曼滤波
入口参数：角度
返回  值：暂无
**************************************************************************/
float Kalman_filtering (int AOA_angle,int distance,AOA_DATA *AVG)
{
	static float Kalman_KA,Kalman_XA,Kalman_ZA,Kalman_ESTA=5,Kalman_MEAA=3;
	static float Kalman_KD,Kalman_XD,Kalman_ZD,Kalman_ESTD=5,Kalman_MEAD=3;//	
	
	Kalman_MEAA=fabs(Kalman_ZA-AOA_angle);
	Kalman_KA=Kalman_ESTA/(Kalman_ESTA+Kalman_MEAA);
	Kalman_XA=Kalman_XA+Kalman_KA*(AOA_angle-Kalman_XA);
	
	Kalman_MEAD=fabs(Kalman_ZD-distance);
	Kalman_KD=Kalman_ESTD/(Kalman_ESTD+Kalman_MEAD);
	Kalman_XD=Kalman_XD+Kalman_KD*(distance-Kalman_XD);
//	//更新
	Kalman_ZA=AOA_angle;//为下一次滤波做准备
	AVG->Angle_filter=Kalman_XA;
	AVG->Distance_filter=Kalman_XD;
}
/**************************************************************************
函数功能：角度过滤  试验型
入口参数：Angle_final  
返回  值：差异值
**************************************************************************/
int AOA_Angle_filtering(int Angle)
{
	static int Last_A,Deposit_A,difference_A,Output_value,Output_number;
	int Angle_limit_one=5,Angle_limit_two=20;//
	float AOA_Judge=(Angle*1.00)/(Last_A*1.00);
//	_dbg_printf("/AOA_Judge %0.2f\n",AOA_Judge);
	difference_A=Angle-Last_A;
	if(difference_A==Angle){Last_A=Angle;}//防止第一次进入函数时陷入死循环
	else if (AOA_Judge>0)//同相位角度变化
  {
		if(difference_A<-Angle_limit_two || difference_A>Angle_limit_two)
		{Output_value=Last_A;Output_number++;}
		else {Output_value=Angle;Last_A=Angle;Output_number=0;}
	}
	else if(difference_A<-Angle_limit_one || difference_A>Angle_limit_one)
	{
//		_dbg_printf("/difference_A %d\n",difference_A);
		Output_value=Last_A;Output_number++;
	}
	else {Last_A=Angle; Output_value=Angle;Output_number=0;}
	if(Output_number>=50){Output_number=0;Last_A=Angle;}
//	_dbg_printf("/Output_value %d\n",Output_value);
	return Output_value;
}
/**************************************************************************
函数功能：根据距离差判断小车是否需要拐弯
入口参数：距离  aoa.distance
返回  值：change_direction
**************************************************************************/
int car_distance (int AOA_distance)
{
	static int Last_distance,New_distance,distance_number,distance_mean,change_direction;
	distance_number++;
	if(distance_number>=5)//计算5次距离差的平均值小于零则代表小车离标签越来越远
	{
		New_distance=AOA_distance;
		change_direction=New_distance-Last_distance;
		Last_distance=AOA_distance;
		if(change_direction>10){distance_mean++;}else{distance_mean=0;}
	  distance_number=0;
//		_dbg_printf(" change_direction %d distance_mean %d\n",change_direction,distance_mean);
	}
	return distance_mean;
}
/**************************************************************************
函数功能：获取AOA具体数据并赋值,最后控制小车运动
入口参数：无
返回  值：暂无
**************************************************************************/
void follow_car_task(void)
{
		Message msg;
	  AOA_DATA AVG;
	  General_t aoa;
		static int catastrophe_data,Output_distance,Turn_around_number=0,Angle_final,OLED_number=100;
		if(get_AOA_data(&msg) == true)
		{
			
//			_dbg_printf("------------Get a new data:----------------");
//			for(int i = 0; i < msg.len; i++)
//			{
//				_dbg_printf(" %02X",msg.buf[i]);
//			}
//		_dbg_printf("\n");
			#ifdef HW_DEBUG			

#endif			
			  App_Module_uint8_conver_format(msg.buf, &aoa);	
				catastrophe_data=AOA_Angle_filtering(aoa.Angual_ONE);
        Kalman_filtering(catastrophe_data,aoa.distance_ONE,&AVG);//平滑当前符合条件的数据
				Output_distance=car_distance(AVG.Distance_filter);
			  car_stop=0;Car_move=Action;
		}
		if(Car_move==Action && Car_time>=16)//根据AOA数值驱动电机
		{
		    OLED_number++; Car_time=0;
				if(Turn_around_number>0)
				{Turn_around_number--;motor_speed_A(6);motor_speed_D(36);}	
        else if(Turn_around_number<0)
				{Turn_around_number++;motor_speed_A(36);motor_speed_D(6);}		
				else{car_motor_direction(AVG.Angle_filter,AVG.Distance_filter);}//主控制函数
				
				
				if(Output_distance>=turn_around)//判断是否满足掉头条件
				{
				  Output_distance=0;if(AVG.Angle_filter>=0){Turn_around_number=20;}else {Turn_around_number=-20;}
				}
				if(Turn_around_number!=0){LED1(ON);}else{LED1(OFF);}
				if(OLED_number>=20)//OLED 显示
        {OLED_number=0;LED2(ON);OLED_Data_display(AVG.Distance_filter,AVG.Angle_filter);}  else {LED2(OFF);}
	  }
}

