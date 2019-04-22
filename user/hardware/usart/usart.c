/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "usart.h"
#include "stdio.h"


void usart_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	    /* -------------- Enable Module Clock Source ----------------------------*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART6, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART6, DISABLE);
	
		GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6); //PG9  usart6 rx
	
		/* -------------- Configure GPIO ---------------------------------------*/
        {
                GPIO_InitTypeDef GPIO_InitStructure;
                USART_InitTypeDef USART_InitStructure;
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOG, &GPIO_InitStructure);

                USART_DeInit(PC_COM_USART);

                USART_InitStructure.USART_BaudRate = 115200;
                USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Even;
                USART_InitStructure.USART_Mode = USART_Mode_Rx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init(PC_COM_USART, &USART_InitStructure);

                USART_DMACmd(PC_COM_USART, USART_DMAReq_Rx, ENABLE);

                USART_ClearFlag(PC_COM_USART, USART_FLAG_IDLE);
                USART_ITConfig(PC_COM_USART, USART_IT_IDLE, ENABLE);

                USART_Cmd(PC_COM_USART, ENABLE);
        }
		
		/* -------------- Configure NVIC ---------------------------------------*/
        {
                NVIC_InitTypeDef NVIC_InitStructure;
                NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART6_NVIC;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
        }
		
		//DMA2 stream1 ch5  or (DMA2 stream2 ch5)    !!!!!!! P206 of the datasheet
        /* -------------- Configure DMA -----------------------------------------*/
        {
                DMA_InitTypeDef DMA_InitStructure;
                DMA_DeInit(PC_COM_DMA_STREAM);

                DMA_InitStructure.DMA_Channel = DMA_Channel_5;
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = dma_buf_num;
                DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
                DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
                DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
                DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
                DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
                DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(PC_COM_DMA_STREAM, &DMA_InitStructure);
                DMA_DoubleBufferModeConfig(PC_COM_DMA_STREAM, (uint32_t)rx2_buf, DMA_Memory_0);
			
                DMA_DoubleBufferModeCmd(PC_COM_DMA_STREAM, ENABLE);
                DMA_Cmd(PC_COM_DMA_STREAM, DISABLE); //Add a disable
                DMA_Cmd(PC_COM_DMA_STREAM, ENABLE);
        }
}

//输出重定向
#ifdef __GNUC__
	 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else 
	 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
	 
PUTCHAR_PROTOTYPE
{
	USART_SendData(USART6, (uint8_t)ch);
	return ch;

}
