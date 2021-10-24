#ifndef __UWB_H
#define __UWB_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * 																				INCLUDES
 **************************************************************************************************/
#include "stm32f10x.h"	
#include "string.h"	
#include "stdbool.h"	
#include "OSAL.h"
#include "OSAL_Comdef.h"

#define RX_UART_LEN 128	

#define CMD_SOP                    	 0x2A  
#define CMD_FOOT                     0x23 

#define SOP_STATE      0x00
#define LEN_STATE      0x01
#define DATA_STATE     0x02
#define FCS_STATE      0x03
#define END_STATE      0x04

#define IS_RIGTH_FOOT(x)	(x == CMD_FOOT)
	
typedef struct
{
	uint8_t state;
  uint8_t total_len;    //统计接受总的个数 
  uint8_t tmp_len;			//目前接受个数
  uint8_t buf[RX_UART_LEN];
	bool timer;
}Rx_Uart_Flag_t;


typedef struct
{
	uint8_t head;
	uint8_t len;
  uint8_t sn;
  uint16_t Addr;
	int Flag;
  int Angual_ONE;
  int distance_ONE;
	int Angual_TWO;
  int distance_TWO;
	int Angual_THREE;
  int distance_THREE;
	int Angual_FOUR;
  int distance_FOUR;
	int Usercmd;
	float F_Path_Power_Level;
	float RX_Level;
//  int Xcm;	
//  int Ycm;
	uint16_t Acc_X;
	uint16_t Acc_Y;
	uint16_t Acc_Z;
	uint8_t check;
	uint8_t foot;
}General_t;//General_variable;


/**************************************************************************************************
 * 																				CONSTANTS
 **************************************************************************************************/		

#define USART3_MAX_DATA_LEN 128



/***************************************************************************************************
 * 																				TYPEDEF
 ***************************************************************************************************/

/***************************************************************************************************
 * 																				GLOBAL VARIABLES
 ***************************************************************************************************/

extern u8  uwb_usart_datarate;

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/	
extern void Uart_Queue_Init(void);

extern bool get_AOA_data(Message *msg);

extern void App_Module_uint8_conver_format(uint8_t *buf,General_t *aoa);

extern void uwb_test(bool test);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif
#endif
