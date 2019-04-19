#ifndef COLLISION_H
#define COLLISION_H

#include "main.h"

typedef enum
{
	COLLISION_PRESSED = 0,
	COLLISION_NO_PRESSSED = 1
}collison_state_t;

void collision_configuration(void);
collison_state_t get_collision_a_state(void);
collison_state_t get_collision_b_state(void);

#endif
