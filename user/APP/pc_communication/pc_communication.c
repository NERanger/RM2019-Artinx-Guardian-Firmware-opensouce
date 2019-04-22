/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "pc_communication.h"
#include "usart.h"
#include "stdio.h"

//����ԭʼ���ݣ�Ϊ18���ֽڣ�����36���ֽڳ��ȣ���ֹDMA����Խ��
uint8_t pc_rx_buf[2][PC_RX_BUF_NUM];

debug_t pc_com_debug;

//��ʼ��DMA������6
void pc_communication_init(void)
{
    usart_Init(pc_rx_buf[0], pc_rx_buf[1], PC_RX_BUF_NUM);
}

//�����ж�
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
            //��������DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = PC_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, PC_RX_BUF_NUM);
            DMA2_Stream1->CR |= DMA_SxCR_CT;
            //��DMA�жϱ�־
            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == PC_FRAME_LENGTH)
            {
                //����ң��������
                //SBUS_TO_RC(SBUS_rx_buf[0], &rc_ctrl);
                //��¼���ݽ���ʱ��
                //DetectHook(DBUSTOE);
            }
        }
        else
        {
            //��������DMA
            DMA_Cmd(DMA2_Stream1, DISABLE);
            this_time_rx_len = PC_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
            DMA_SetCurrDataCounter(DMA2_Stream1, PC_RX_BUF_NUM);
            DMA2_Stream1->CR &= ~(DMA_SxCR_CT);
            //��DMA�жϱ�־
            DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA2_Stream1, ENABLE);
            if(this_time_rx_len == PC_FRAME_LENGTH)
            {
                //����ң��������
                //SBUS_TO_RC(SBUS_rx_buf[1], &rc_ctrl);
                //��¼���ݽ���ʱ��
                //DetectHook(DBUSTOE);
            }
        }
    }
}
