#ifndef USART_H
#define USART_H

#include "main.h"
#include "stm32f4xx.h"

#define PC_COM_USART USART6
#define PC_COM_DMA_STREAM DMA2_Stream1

void usart_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);



#endif
