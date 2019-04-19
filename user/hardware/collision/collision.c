/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/
#include "collision.h"

#include "stm32f4xx.h"

void collision_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = COLLISIONa_Pin | COLLISIONb_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_GPIO_Port, &GPIO_InitStructure);
}

collison_state_t get_collision_a_state(void)
{
	return (collison_state_t)GPIO_ReadInputDataBit(COLLISION_GPIO_Port, COLLISIONa_Pin);
}

collison_state_t get_collision_b_state(void)
{
	return (collison_state_t)GPIO_ReadInputDataBit(COLLISION_GPIO_Port, COLLISIONb_Pin);
}
