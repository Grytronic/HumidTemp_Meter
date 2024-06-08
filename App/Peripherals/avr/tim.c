
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        tim.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Timers driver                                                                                                                  */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "tim.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
/* timer1 16bit init */
void TIM_init(void)
{
	/* Timer1 Init - makes interrupt about 250ms for 1s count */
	TCCR1B |= (1<<CS11);  // preskaler 8: 2000000/65535/64= about 250ms;
	TIMSK1 |= (1<<TOIE1); // timer-overflow interrupr

	/* Timer0 Init - calculate sensor time of low/high level */
	TCCR0B |= (1<<CS00); // no prescaling: 1 imp = 500ns, count in TCNT0
}

uint8_t TIM0_time_calculated_us(void)
{
	return TCNT0 >> 1;  //need to be divided by 2 by 2Mhz CPU clock
}

void TIM0_reset (void)
{
	TCNT0 = 0;
}

void TIM0_wait_for_low_level(uint8_t timeout_us)
{
	TIM0_reset();
	while (DHT_PINREG & DHT_PIN) {
		if ( TIM0_time_calculated_us() > timeout_us) return;
	}
}

void TIM0_wait_for_high_level(uint8_t timeout_us)
{
	TIM0_reset();
	while (!(DHT_PINREG & DHT_PIN)) {
		if ( TIM0_time_calculated_us() > timeout_us) return;
	}
}

int TIM1_get_generated_random_value(void)
{
	srand(TCNT1);

	return rand();
}
