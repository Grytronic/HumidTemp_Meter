/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        mock_lotto.c                                                                                                                   */
/* Created:     09.06.2024                                                                                                                     */
/* Description: Mock for Lotto module                                                                                                          */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "mock_lotto.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
static genMode_t E_generationMode = faked_numbers;
static uint8_t FAKE_RandomReturnValue = 0;
static uint8_t entry_counter = 0;
static uint8_t test;

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static int PERIPHERALS_return_fake_random_value(void);
static int PERIPHERALS_return_generated_random_value(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/
void PERIPHERALS_use_random_generation_mode(genMode_t mode)
{
	switch(mode)
	{
		case generated_numbers:
		{
			E_generationMode = generated_numbers;
			break;
		}
		case faked_numbers:
		{
			E_generationMode = faked_numbers;
		    srand(time(0));
			break;
		}
		default:
		{
			E_generationMode = faked_numbers;
			break;
		}
	}
}

int PERIPHERALS_get_generated_random_value(void)
{
	int genValue;

	if (generated_numbers == E_generationMode)
	{
		genValue = PERIPHERALS_return_generated_random_value();
	}
	else if (faked_numbers == E_generationMode)
	{
		genValue = PERIPHERALS_return_fake_random_value();
	}
	else
	{
		genValue = 0;
	}

	return genValue;
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

static int PERIPHERALS_return_fake_random_value(void)
{
	int ret_val = FAKE_RandomReturnValue + entry_counter;

	if ((0 >= ret_val) || (LOTTO_5FROM50_OUT_RANGE == ret_val) || (LOTTO_2FROM12_OUT_RANGE == ret_val))
	{
		ret_val = DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0;
		FAKE_RandomReturnValue = ret_val;
		entry_counter = 0;
	}
	entry_counter++;

	return ret_val - 1;
}

static int PERIPHERALS_return_generated_random_value(void)
{
	int ret_val = rand();

	if ((0 >= ret_val) || (LOTTO_5FROM50_OUT_RANGE == ret_val) || (LOTTO_2FROM12_OUT_RANGE == ret_val))
	{
		ret_val = rand();
	}

	return ret_val;
}





