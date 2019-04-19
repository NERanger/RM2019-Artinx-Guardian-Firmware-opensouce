#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "main.h"
#include "collision.h"

//任务开始空闲一段时间
#define COLLISION_DETECTION_TASK_INIT_TIME 357

#define COLLISION_CONTROL_TIME_MS 100
#define RANGING_TRIG_SET_TIME_MS 1
#define RANGINGa_COLLISION_LIMIT 1500
#define RANGINGb_COLLISION_LIMIT 1500

typedef enum
{
	GUARDIAN_COLLISION_FALSE = 0,
	GUARDIAN_COLLISION_TRUE = 1
} guardian_collision_state;

typedef struct
{
	uint32_t ranging_distance_a;
	uint32_t ranging_distance_b;
	
	collison_state_t collison_state_a;
	collison_state_t collison_state_b;
	
}collision_sensor_t;

extern guardian_collision_state  guard_collision_flag;

void collision_detection_task(void *pvParameters);

#endif
