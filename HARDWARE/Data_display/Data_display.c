#include "Data_display.h"
uint8_t buf[32];
/**************************************************************************
�������ܣ���ʼ��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Initial_display(int word)
{
	 switch(word)		
							{ 
//		          case 1 ��ʾ��˾��  case 2 ��ʾ����ģʽ  case 3 ��ʾ����Ƕ��뵥λ
				        case 1: OLED_CLS();OLED_ShowCN(19,3,0);OLED_ShowCN(45,3,1);OLED_ShowCN(71,3,2);OLED_ShowCN(97,3,3);Delay_ms(2000);break;
								case 2: OLED_CLS();OLED_ShowCN(11,0,8);OLED_ShowCN(41,0,9);OLED_ShowCN(75,0,10);OLED_ShowCN(105,0,11);break;	
		            case 3:	OLED_ShowCN(16,3,6);OLED_ShowCN(32,3,7);OLED_ShowStr(48,3, ":", 2);OLED_ShowCN(88,3,13);
								        OLED_ShowCN(16,6,4);OLED_ShowCN(32,6,5);OLED_ShowStr(48,6, ":", 2);OLED_ShowCN(88,6,14);break;						
								default:break;
							}
	 
}
/**************************************************************************
�������ܣ���ʾ��ǰPDOA����ֵ�ͽǶ�ֵ
��ڲ�����Angle,distance
����  ֵ����
**************************************************************************/
void OLED_Data_display(float New_distance,int New_Angle)
{	
	
	if(New_distance>=0)
	{
		memset(buf, 0, sizeof(buf));
	  if(New_distance<=999) {sprintf(buf,"%0.2f",New_distance/100);}
		else if(New_distance<=9999) {sprintf(buf,"%0.1f",New_distance/100);}
		else  {sprintf(buf,"%0.2f",New_distance/100);}
		OLED_ShowStr(56,3,buf,2);	
	}
	 if(New_Angle>=0)
	{
    memset(buf, 0, sizeof(buf));
   	if(New_Angle>=-9) {sprintf(buf," %d ",New_Angle);}
		else if(New_Angle>=-99) {sprintf(buf," %d ",New_Angle);}
		else  {sprintf(buf,": %d",New_Angle);}
		OLED_ShowStr(56,6,buf,2);	
	}
	else 
	{
    memset(buf, 0, sizeof(buf));
   	if(New_Angle>=-9) {sprintf(buf,"%d  ",New_Angle);}
		else if(New_Angle>=-99) {sprintf(buf,"%d ",New_Angle);}
		else  {sprintf(buf,"%d",New_Angle);}
		OLED_ShowStr(56,6,buf,2);	
	}	
}