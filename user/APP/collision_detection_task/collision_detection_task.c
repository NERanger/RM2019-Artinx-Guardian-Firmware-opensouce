/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "collision_detection_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "ranging.h"
#include "collision.h"

#include "stm32f4xx.h"

static collision_sensor_t collision_sensor;
guardian_collision_state  guard_collision_flag = GUARDIAN_COLLISION_FALSE;

void collision_detection_task(void *pvParameters)
{
	//����һ��ʱ��
    vTaskDelay(COLLISION_DETECTION_TASK_INIT_TIME);
	
	while(1)
	{
		//����TRIG��ƽ
		GPIO_SetBits(RANGINGa_TRIG_GPIO_Port, RANGINGa_TRIG_Pin);
		GPIO_SetBits(RANGINGb_TRIG_GPIO_Port, RANGINGb_TRIG_Pin);
		
		//�ߵ�ƽ����1ms
		vTaskDelay(RANGING_TRIG_SET_TIME_MS);
		
		//����TRIG��ƽ����ʼ�������
		GPIO_SetBits(RANGINGa_TRIG_GPIO_Port, RANGINGa_TRIG_Pin);
		GPIO_SetBits(RANGINGb_TRIG_GPIO_Port, RANGINGb_TRIG_Pin);
		
		//�������벶��
		if(__RANGINGa_EDGE_CAP)
		{
			collision_sensor.ranging_distance_a = RANGINGa_Edge_Capture_Handler();
		}
		
		if(__RANGINGb_EDGE_CAP)
		{
			collision_sensor.ranging_distance_b = RANGINGb_Edge_Capture_Handler();
		}
		
		//���´�������״̬
		collision_sensor.collison_state_a = get_collision_a_state();
		collision_sensor.collison_state_b = get_collision_b_state();
		
		if(collision_sensor.collison_state_a == COLLISION_PRESSED || collision_sensor.collison_state_a == COLLISION_PRESSED)
		{
			guard_collision_flag = GUARDIAN_COLLISION_TRUE;
		}
		else if(collision_sensor.ranging_distance_a < RANGINGa_COLLISION_LIMIT || collision_sensor.ranging_distance_b < RANGINGb_COLLISION_LIMIT)
		{
			guard_collision_flag = GUARDIAN_COLLISION_TRUE;
		}
		else
		{
			guard_collision_flag = GUARDIAN_COLLISION_FALSE;
		}
		
		vTaskDelay(COLLISION_CONTROL_TIME_MS);
		
	}
}
