#ifndef __HAL_UART_H
#define __HAL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

	
/**************************************************************************************************
 * 																				INCLUDES
 **************************************************************************************************/	
#include "stdio.h" 
#include "OSAL_Comdef.h"
#include "stm32f10x.h"	

	
	
/**************************************************************************************************
 * 																				CONSTANTS
 **************************************************************************************************/		

#define HAL_USART1					/*调试口*/	
#define HAL_USART3					/*uwb数据解析*/

#define HAL_USART1_DMA			/*调试口采用DMA形式*/

/***************************************************************************************************
 * 																				TYPEDEF
 ***************************************************************************************************/
typedef void (*halUARTCBack_t) (uint8_t port, uint8_t event);


/***************************************************************************************************
 * 																				GLOBAL VARIABLES
 ***************************************************************************************************/


/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/	
extern void _dbg_printf(const char *format,...);

extern void HalUARTInit ( void );	

static void HalUASRT1_NVIC_Config(void);

static void HalUASRT2_NVIC_Config(void);

static void HalUSART1_Init(u32 bound);

static void HalUSART2_Init(u32 bound);

static void HalUSART1_IO_Init(void);

static void HalUSART2_IO_Init(void);

#ifdef __cplusplus
}
#endif

#endif	//__HAL_UART_H
