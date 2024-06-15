/*
 * fake_lotto.c
 *
 *  Created on: 09.06.2024
 *      Author: Boss
 */
#include "fake_lotto.h"


const int PeripheralsFakeDefaultRandomReturnValue = 178U;


int PERIPHERALS_get_generated_random_value(void)
{
	int ret_val = PeripheralsFakeDefaultRandomReturnValue;

	return ret_val;
}

