#ifndef USART_H
#define USART_H

#include "main.h"
#include "stm32f4xx.h"

void usart_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);



#endif
