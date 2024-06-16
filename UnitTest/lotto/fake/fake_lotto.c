/*
 * fake_lotto.c
 *
 *  Created on: 09.06.2024
 *      Author: Boss
 */
#include "fake_lotto.h"

int FAKE_RandomReturnValue;
int entry_counter = 0;

int PERIPHERALS_get_generated_random_value(void)
{
	int ret_val = FAKE_RandomReturnValue + entry_counter;

	if ((0 >= ret_val) || (LOTTO_5FROM50_OUT_RANGE == ret_val) || (LOTTO_2FROM12_OUT_RANGE == ret_val))
	{
		ret_val = FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0;
		FAKE_RandomReturnValue = ret_val;
		entry_counter = 0;
	}
	entry_counter++;

	return ret_val;
}

void PERIPHERALS_set_value_for_generated_random_value(int val)
{
	FAKE_RandomReturnValue = val;
}

void PERIPHERALS_reset_start_value_for_generated_random_value(void)
{
	FAKE_RandomReturnValue = 0;
	entry_counter = 0;
}
