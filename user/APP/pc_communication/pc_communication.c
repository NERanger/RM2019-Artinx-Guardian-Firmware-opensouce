/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "pc_communication.h"
#include "usart.h"
#include "stdio.h"

//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
uint8_t pc_rx_buf[2][PC_RX_BUF_NUM];

debug_t pc_com_debug;

//初始化DMA，串口6
void pc_communication_init(void)
{
    usart_Init(pc_rx_buf[0], pc_rx_buf[1], PC_RX_BUF_NUM);
}

//串口中断
void USART6_IRQHandler(void)
{
	pc_com_debug.usart6_irq_flag = 1;
	
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART6);
    }
    else if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
    {
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART6);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = PC_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, PC_RX_BUF_NUM);
            DMA2_Stream1->CR |= DMA_SxCR_CT;
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == PC_FRAME_LENGTH)
            {
                //处理遥控器数据
                //SBUS_TO_RC(SBUS_rx_buf[0], &rc_ctrl);
                //记录数据接收时间
                //DetectHook(DBUSTOE);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = PC_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, PC_RX_BUF_NUM);
            DMA2_Stream1->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == PC_FRAME_LENGTH)
            {
                //处理遥控器数据
                //SBUS_TO_RC(SBUS_rx_buf[1], &rc_ctrl);
                //记录数据接收时间
                //DetectHook(DBUSTOE);
            }
        }
    }
}
