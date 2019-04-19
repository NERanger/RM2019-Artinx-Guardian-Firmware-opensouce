#ifndef TIMER_H
#define TIMER_H
#include "main.h"

extern void TIM1_Init(uint16_t arr, uint16_t psc);
extern void TIM3_Init(uint16_t arr, uint16_t psc);
extern void TIM6_Init(uint16_t arr, uint16_t psc);
extern void TIM12_Init(uint16_t arr, uint16_t psc);
//Added by NERanger 29100418
extern void TIM2_Init(uint32_t arr, uint16_t psc);
extern void TIM5_Init(uint32_t arr, uint16_t psc);
#endif
