/***************************************************************
@Author: NERanger
@email:  11712605@mail.sustech.edu.cn
***************************************************************/

#include "ranging.h"
#include "timer.h"

#include "stm32f4xx.h"

static capture_type_t RANGINGa_cap_type = NOW_CAP_RISING_EDGE;
static capture_type_t RANGINGb_cap_type = NOW_CAP_RISING_EDGE;

void ranging_configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOI, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = RANGINGa_TRIG_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RANGINGa_TRIG_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RANGINGb_TRIG_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(RANGINGb_TRIG_GPIO_Port, &GPIO_InitStructure);

	TIM2_Init(RANGING_TIM_ARR, RANGING_TIM_PSC);
	TIM5_Init(RANGING_TIM_ARR, RANGING_TIM_PSC);
}

uint32_t RANGINGa_Edge_Capture_Handler(void)
{
	static uint32_t diff;
	
	if(RANGINGa_cap_type == NOW_CAP_RISING_EDGE)
	{
		TIM_Cmd(TIM2, DISABLE);
		TIM2->CNT = 0;
		__RANGINGa_CONFIG_CAP_FALLING_EDGE;
		__RANGINGa_CAP_FLAG_CLEAR;
		TIM_Cmd(TIM2, ENABLE);
		
		RANGINGa_cap_type = NOW_CAP_FALLING_EDGE;
	}
	else if(RANGINGa_cap_type == NOW_CAP_FALLING_EDGE)
	{
		diff =TIM2->CCR2;
		
		 TIM_Cmd(TIM2, DISABLE);
		 __RANGINGa_CAP_FLAG_CLEAR;
		 __RANGINGa_CONFIG_CAP_RISING_EDGE;
		 TIM_Cmd(TIM2, ENABLE); 
		
		RANGINGa_cap_type = NOW_CAP_RISING_EDGE;
	}
	
	return diff;
}

uint32_t RANGINGb_Edge_Capture_Handler(void)
{
	static uint32_t diff;
	
	if(RANGINGb_cap_type == NOW_CAP_RISING_EDGE)
	{
		TIM_Cmd(TIM5, DISABLE);
		TIM5->CNT = 0;
		__RANGINGb_CONFIG_CAP_FALLING_EDGE;
		__RANGINGb_CAP_FLAG_CLEAR;
		TIM_Cmd(TIM5, ENABLE);
		
		RANGINGb_cap_type = NOW_CAP_FALLING_EDGE;
	}
	else if(RANGINGb_cap_type == NOW_CAP_FALLING_EDGE)
	{
		diff =TIM5->CCR2;
		
		 TIM_Cmd(TIM5, DISABLE);
		 __RANGINGb_CAP_FLAG_CLEAR;
		 __RANGINGb_CONFIG_CAP_RISING_EDGE;
		 TIM_Cmd(TIM5, ENABLE); 
		
		RANGINGb_cap_type = NOW_CAP_RISING_EDGE;
	}
	
	return diff;
}
