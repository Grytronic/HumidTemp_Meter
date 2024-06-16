/*
 * fake_lotto.c
 *
 *  Created on: 09.06.2024
 *      Author: Boss
 */
#include "fake_lotto.h"


int FAKE_RandomReturnValue;


int PERIPHERALS_get_generated_random_value(void)
{
	int ret_val = FAKE_RandomReturnValue;

	return ret_val;
}

void PERIPHERALS_set_value_for_generated_random_value(int val)
{
	FAKE_RandomReturnValue = val;
}
