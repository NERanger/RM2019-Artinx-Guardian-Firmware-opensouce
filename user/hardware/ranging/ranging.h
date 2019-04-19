#ifndef RANGING_H
#define RANGING_H
#include "main.h"

#define RANGING_TIM_ARR 0xFFFFFFFF
#define RANGING_TIM_PSC 90

//PWM2 CH2
#define __RANGINGa_EDGE_CAP 			(TIM2->SR & 0x0004)
#define __RANGINGa_CAP_FLAG_CLEAR    (TIM2->SR &= ~(1<<2))
#define __RANGINGa_CONFIG_CAP_RISING_EDGE    (TIM2->CCER &= ~(1<<5))
#define __RANGINGa_CONFIG_CAP_FALLING_EDGE   (TIM2->CCER |= (1<<5))

//PWM5 CH1
#define __RANGINGb_EDGE_CAP 			(TIM5->SR & 0x0002)
#define __RANGINGb_CAP_FLAG_CLEAR    (TIM5->SR &= ~(1<<1))
#define __RANGINGb_CONFIG_CAP_RISING_EDGE    (TIM5->CCER &= ~(1<<1))
#define __RANGINGb_CONFIG_CAP_FALLING_EDGE   (TIM5->CCER |= (1<<1))

typedef enum
{
	NOW_CAP_FALLING_EDGE = 0,
	NOW_CAP_RISING_EDGE = 1
}capture_type_t;

void ranging_configuration(void);
uint32_t RANGINGa_Edge_Capture_Handler(void);
uint32_t RANGINGb_Edge_Capture_Handler(void);

#endif
