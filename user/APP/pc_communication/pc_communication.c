/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "pc_communication.h"
#include "usart.h"
#include "stdio.h"

void pc_data_handler(volatile const uint8_t *pc_rx_buf, pc_com_t *pc_com);

//����ԭʼ���ݣ�Ϊ18���ֽڣ�����36���ֽڳ��ȣ���ֹDMA����Խ��
uint8_t pc_rx_buf[2][PC_RX_BUF_NUM];
fp64_uchar_u gimbal_yaw_angle_fp64, gimbal_yaw_angle_uchar, gimbal_pitch_angle_fp64, gimbal_pitch_angle_uchar;
pc_com_t pc_com;

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
                //����pc����
                pc_data_handler(pc_rx_buf[0], &pc_com);
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
                //����pc����
                pc_data_handler(pc_rx_buf[1], &pc_com);
                //��¼���ݽ���ʱ��
                //DetectHook(DBUSTOE);
            }
        }
    }
}

void pc_data_handler(volatile const uint8_t *pc_rx_buf, pc_com_t *pc_com)
{
	pc_com->yaw_mode = pc_rx_buf[PC_CMD_GIMBAL_YAW_MODE_BUF_OFFSET];
	pc_com->pitch_mode = pc_rx_buf[PC_CMD_GIMBAL_PITCH_MODE_BUF_OFFSET];
	
	gimbal_yaw_angle_uchar.uchar_data[0] = pc_rx_buf[2];
	gimbal_yaw_angle_uchar.uchar_data[1] = pc_rx_buf[3];
	gimbal_yaw_angle_uchar.uchar_data[2] = pc_rx_buf[4];
	gimbal_yaw_angle_uchar.uchar_data[3] = pc_rx_buf[5];
	gimbal_yaw_angle_uchar.uchar_data[4] = pc_rx_buf[6];
	gimbal_yaw_angle_uchar.uchar_data[5] = pc_rx_buf[7];
	gimbal_yaw_angle_uchar.uchar_data[6] = pc_rx_buf[8];
	gimbal_yaw_angle_uchar.uchar_data[7] = pc_rx_buf[9];
	
	gimbal_pitch_angle_uchar.uchar_data[0] = pc_rx_buf[10];
	gimbal_pitch_angle_uchar.uchar_data[1] = pc_rx_buf[11];
	gimbal_pitch_angle_uchar.uchar_data[2] = pc_rx_buf[12];
	gimbal_pitch_angle_uchar.uchar_data[3] = pc_rx_buf[13];
	gimbal_pitch_angle_uchar.uchar_data[4] = pc_rx_buf[14];
	gimbal_pitch_angle_uchar.uchar_data[5] = pc_rx_buf[15];
	gimbal_pitch_angle_uchar.uchar_data[6] = pc_rx_buf[16];
	gimbal_pitch_angle_uchar.uchar_data[7] = pc_rx_buf[17];
	
	for(uint8_t i = 0; i < UNION_UCHAR_SIZE; i++)
	{
		gimbal_yaw_angle_fp64.uchar_data[i] = gimbal_yaw_angle_uchar.uchar_data[i];
		gimbal_pitch_angle_fp64.uchar_data[i] = gimbal_pitch_angle_uchar.uchar_data[i];
	}
	
	pc_com->yaw_angle_cmd =  gimbal_yaw_angle_fp64.fp64_data;
	pc_com->pitch_angle_cmd = gimbal_pitch_angle_fp64.fp64_data;
}
