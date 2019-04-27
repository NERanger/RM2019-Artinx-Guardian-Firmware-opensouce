#ifndef PC_COMMUNICATION_H
#define PC_COMMUNICATION_H

#include "main.h"

#define PC_RX_BUF_NUM 36u
#define PC_FRAME_LENGTH 18u

#define PC_CMD_GIMBAL_YAW_MODE_BUF_OFFSET 0
#define PC_CMD_GIMBAL_PITCH_MODE_BUF_OFFSET 1

#define UNION_UCHAR_SIZE 8

typedef union
{
	fp64 fp64_data;
	uint8_t uchar_data[UNION_UCHAR_SIZE];

}fp64_uchar_u;

typedef struct
{
	uint8_t  yaw_mode;
	uint8_t  pitch_mode;
	fp64    yaw_angle_cmd;
	fp64    pitch_angle_cmd;
}pc_com_t;

void pc_communication_init(void);
const pc_com_t *get_pc_com_point(void);

#endif
