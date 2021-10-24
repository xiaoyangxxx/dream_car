#include <stdarg.h>
#include "hal_usart.h"

#define DMA1_MEM_LEN 256//����DMAÿ�����ݴ��͵ĳ���
char _dbg_TXBuff[DMA1_MEM_LEN];

/***********************************************************************/
/*************************----- �����ض��� -----************************/
/***********************************************************************/
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/*******************************************************************************
* ������  : fputc
* ����    : �ض���putc����
* ����    : ��
* ����    : �� 
* ˵��    : �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*******************************************************************************/
int fputc(int ch, FILE *f)
{
	 USART1->SR;

	 /* e.g. ��USARTдһ���ַ� */
	 USART_SendData(USART1, (uint8_t) ch);

	 /* ѭ��ֱ��������� */
	 while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	 return ch;
}

/*******************************************************************************
* ������  : fgetc
* ����    : �ض���getc����
* ����    : ��
* ����    : �� 
* ˵��    : �ض���getc��������������ʹ��scanff�����Ӵ���1��������
*******************************************************************************/
int fgetc(FILE *f)
{
	 /* �ȴ�����1�������� */
	 while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	 return (int)USART_ReceiveData(USART1);
}	


/*
 *******************************************************************************
 *   ����1���ͺ���
 *	 ����ʹ�õ�����ͨģʽ,��ÿ�η�����ɺ�,��װ��ͨ��
 *	 װ�غ�ͨ����ȴ�������ɡ�
 *******************************************************************************
 */
uint16_t USART1_SendBuffer(const char* buffer, uint16_t length)
{
	if( (buffer==NULL) || (length==0) )
	{
		return 0;
	}
 
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, length);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	while(1)
	{
		if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET)	//�ж�ͨ��4�������
		{
			DMA_ClearFlag(DMA1_IT_TC4);//���ͨ��4������ɱ�־
			break;
		}
	}
	return length;
}


/*
 *******************************************************************************
 *		DMA��ʽ��printf
 *******************************************************************************
 */
void _dbg_printf(const char *format,...)
{
		uint32_t length;
		va_list args;
	 
		va_start(args, format);
		length = vsnprintf((char*)_dbg_TXBuff, sizeof(_dbg_TXBuff), (char*)format, args);
		va_end(args);
		USART1_SendBuffer((const char*)_dbg_TXBuff,length); 
}

void HalUASRT1_DMA_Send_init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
		DMA_InitTypeDef DMA_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
		//DMA1_MEM_LEN=cndtr;
		DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
		DMA_InitStructure.DMA_BufferSize = DMA1_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�������ȼ� 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		DMA_Init(DMA_CHx, &DMA_InitStructure); 
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
		DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
}


/*
 *******************************************************************************
 *		����1DMA��ʼ��
 *		�ر�RXNE��TC�ж�,����IDLE
 *		����RX,TX�����Ŀ�ĵ�ַ��Դ��ַ
 *******************************************************************************
 */
void HalUASRT1_DMA_Config(void)
{
		USART_ITConfig(USART1, USART_IT_TC,DISABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		
		HalUASRT1_DMA_Send_init(DMA1_Channel4,(u32)&USART1->DR,(u32)_dbg_TXBuff);//TX
}


void HalUASRT1_NVIC_Config(void)
{
		
}

void HalUASRT2_NVIC_Config(void)
{
	  //Usart2 NVIC ����
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


void HalUSART1_Init(u32 bound)
{
		USART_InitTypeDef USART_InitStructure;
	 
	   //USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	  USART_Init(USART1, &USART_InitStructure); //��ʼ������
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
}


void HalUSART2_Init(u32 bound)
{
		USART_InitTypeDef USART_InitStructure;
	 
	   //USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	  USART_Init(USART2, &USART_InitStructure); //��ʼ������
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
}


void HalUSART1_IO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
			//USART1_TX   GPIOA.9
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
	   
	  //USART1_RX	  GPIOA.10��ʼ��
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
}


void HalUSART2_IO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2��GPIOAʱ��
			//USART2_TX   GPIOA.2
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
	   
	  //USART2_RX	  GPIOA.3��ʼ��
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3
}

//void USART2_IRQHandler(void) 
//{
//		u8 Res;
//		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
//		{ 
//			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//			Res = USART_ReceiveData(USART2); //��ȡ���յ�������
//			_dbg_printf("%c\n",Res);	
//		}
//}

void HalUARTInit ( void )
{
		HalUSART1_IO_Init();
		HalUSART1_Init(115200);
		HalUASRT1_DMA_Config();
//		HalUASRT1_NVIC_Config();	
		HalUSART2_IO_Init();
		HalUSART2_Init(115200);
		HalUASRT2_NVIC_Config();	

}

