#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "hal_usart.h"
#include "control.h"
#include "uwb.h"
#include "OSAL.h"
//#include "servor.h"
#include "motor4.h"
//#include "hal_timer.h"
#include "timer.h"
static Rx_Uart_Flag_t rx_uart_flag;
static QUEUE uart_queue;
u8 Res;

enum UART_FRAME{
	UART_FRAME_CMD =2	
};

/**************************************************************************

**************************************************************************/

void Uart_Queue_Init(void)
{
	memset(&rx_uart_flag.state,0,sizeof(rx_uart_flag));			

	osal_CreateQueue(&uart_queue, QUEUE_MSG_MAX);	
}


static uint8_t usart3_resv_msg_check(Rx_Uart_Flag_t rx_uart_flag)
{
		uint8_t ret = FALSE;

		if(IS_RIGTH_FOOT(rx_uart_flag.buf[rx_uart_flag.total_len - 1])
#ifdef HW_RELEASE
			&&get_Xor_CRC(rx_uart_flag.buf, 2, rx_uart_flag.total_len - 4) == rx_uart_flag.buf[rx_uart_flag.total_len - 2]
#endif
			&&rx_uart_flag.total_len >= 11
			)
			ret = TRUE;
		return ret;
}

static uint8_t usart3_recv_msg_handler(uint8_t ch)
{
		uint8_t ret = FALSE;
		switch (rx_uart_flag.state)
		{
			case SOP_STATE:
				if (ch == CMD_SOP)
				{
					rx_uart_flag.state = LEN_STATE;
					rx_uart_flag.buf[rx_uart_flag.tmp_len++] = ch;
					//rx_uart_flag.timer = true;
				}	
				break;
		
			case LEN_STATE:
				{
					rx_uart_flag.state = DATA_STATE;			
					rx_uart_flag.buf[rx_uart_flag.tmp_len++] = ch;
					rx_uart_flag.total_len = ch + 1 + 1 + 1 + 1;//head + len + check + footer
					if(rx_uart_flag.total_len > RX_UART_LEN)
					{
						//清零结构体
						memset(&rx_uart_flag.state,0,sizeof(rx_uart_flag)); 	
					}
				}
				break;
			case DATA_STATE:
				{
					rx_uart_flag.buf[rx_uart_flag.tmp_len++] = ch;
					if(rx_uart_flag.total_len - rx_uart_flag.tmp_len <= 0)
					{								
						if((ret = usart3_resv_msg_check(rx_uart_flag)) == TRUE)
						{
						}
						else
						{
#ifdef	HW_RTLS_ANC_UART_TEST				
							uint8 debug_buf[128];
							uint8 str_buf[128];
							osal_Hex2Str(rx_uart_flag.buf, str_buf, rx_uart_flag.total_len);
							sprintf(debug_buf, "error Ax:%04X, len:%d buf:%s\n", instancegetaddresses(),rx_uart_flag.total_len, str_buf);
							USART1_SendBuffer(debug_buf, strlen(debug_buf), TRUE);	
#endif
						}
						rx_uart_flag.state = SOP_STATE;
						rx_uart_flag.tmp_len = 0;
						//rx_uart_flag.timer = false;
					}
				}
				break;
			case FCS_STATE:
			case END_STATE:
			default:
			 break;
		}
		return ret;	
}

void USART2_IRQHandler(void) 
{
//		u8 Res;
//		TIM_SetCounter(TIM1, 0);
//    HalIwdgFeed();	
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{ 
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			Res = USART_ReceiveData(USART2); //读取接收到的数据
//			_dbg_printf("%c",Res);
			if(usart3_recv_msg_handler(Res))
			{
				//写入队列
				Message msg;
				msg.flag = UART_FRAME_CMD;
				msg.len = rx_uart_flag.total_len;
				memcpy(msg.buf, rx_uart_flag.buf, msg.len);
				osal_Enqueue(&uart_queue, msg);
		
				//清零结构体
				memset(&rx_uart_flag.state,0,sizeof(rx_uart_flag));			
			}
		}
}

/*
	函数功能：解析UWB数据 赋值range_T2A0 range_T2A1 rang_A2A angual
	入口参数：NULL
	返回  值：NULL
 */
bool get_AOA_data(Message *msg)
{		
		return osal_Dequeue(&uart_queue, msg);
}


void App_Module_uint8_conver_format(uint8_t *buf,General_t *aoa)
{		
	  uint32_t tmp;
		int index = 0,Mode=1;
	
	  aoa->head = buf[index++];
		aoa->len = buf[index++];
		aoa->sn = buf[index++];	
		aoa->Addr = BUILD_UINT16(buf[index], buf[index + 1]);
		index += 2;
	  if(Mode==0){aoa->Flag = buf[index++];}
		aoa->Angual_ONE = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);index += 4;
		aoa->distance_ONE = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]); index += 4;
		if(Mode==0)
		{
	  aoa->Angual_TWO = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);index += 4;
		aoa->distance_TWO = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]); index += 4;
	  aoa->Angual_THREE = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);index += 4;
		aoa->distance_THREE = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]); index += 4;
	  aoa->Angual_FOUR = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);index += 4;
		aoa->distance_FOUR = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]); index += 4;
		}
	  else if(Mode==1){aoa->Usercmd=aoa->Flag = BUILD_UINT16(buf[index], buf[index + 1]);index += 2;}
	
		tmp= BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);
	  aoa->F_Path_Power_Level= *(float*)&tmp;//
		index += 4;
	  tmp = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);
	  aoa->RX_Level= *(float*)&tmp;
		
		index += 4;
//		aoa->Xcm = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);
//		index += 4;
//		aoa->Ycm = BUILD_UINT32(buf[index],buf[index + 1],buf[index + 2],buf[index + 3]);
//		index += 4;
		aoa->Acc_X = BUILD_UINT16(buf[index], buf[index + 1]);
		index += 2;
		aoa->Acc_Y = BUILD_UINT16(buf[index], buf[index + 1]);
		index += 2;
		aoa->Acc_Z = BUILD_UINT16(buf[index], buf[index + 1]);
		index += 2;	
		aoa->check = buf[index++];
		aoa->foot = buf[index++];
//		  _dbg_printf("F_Path_Power_Level  %0.2f \n",aoa->F_Path_Power_Level);
//		_dbg_printf("Angual:%d\n",aoa->Angual);		
//		_dbg_printf("distance:%d\n",aoa->distance);		
		
//		_dbg_printf("head:%02X\n",aoa->head);
//		_dbg_printf("len:%d\n",aoa->len);
//		_dbg_printf("sn:%d\n",aoa->sn);				
//		_dbg_printf("Addr:%04X\n",aoa->Addr);		
//		_dbg_printf("Angual:%d\n",aoa->Angual);		
//		_dbg_printf("distance:%d\n",aoa->distance);		
////		_dbg_printf("Xcm:%d\n",aoa->Xcm);		
////		_dbg_printf("Ycm:%d\n",aoa->Ycm);			
//		_dbg_printf("Acc_X:%04X\n",aoa->Acc_X);		
//		_dbg_printf("Acc_Y:%04X\n",aoa->Acc_Y);				
//		_dbg_printf("Acc_Y:%04X\n",aoa->Acc_Z);	
//		_dbg_printf("check:%02X\n",aoa->check);
//		_dbg_printf("foot:%02X\n",aoa->foot);		
}
