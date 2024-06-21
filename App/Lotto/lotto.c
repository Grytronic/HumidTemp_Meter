/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        lotto.c                                                                                                                        */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Lotto Jack-Pot numbers generator                                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "lotto.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define LOTTO_5FROM50_OUT_RANGE 50U
#define LOTTO_2FROM12_OUT_RANGE 12U

#define GENERETED_TWICE 1U

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
Lotto_t LOTTO_results;

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static uint8_t LOTTO_generate_5_from_50(void);
static uint8_t LOTTO_generate_2_from_12(void);
static uint8_t LOTTO_check_if_in_5from50_the_same_number_generated_twice(uint8_t value);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 

void LOTTO_generate_numbers(void)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		LOTTO_results.lottoTab5from50[i] = LOTTO_generate_5_from_50();
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		LOTTO_results.lottoTab2from12[i] = LOTTO_generate_2_from_12();
	}
}

static uint8_t LOTTO_generate_5_from_50(void)
{
	uint8_t random_value = (uint8_t)(PERIPHERALS_get_generated_random_value() % LOTTO_5FROM50_OUT_RANGE + 1U);

	while ((0 >= random_value) || (LOTTO_check_if_in_5from50_the_same_number_generated_twice(random_value) == GENERETED_TWICE))
	{
		random_value = (uint8_t)(PERIPHERALS_get_generated_random_value() % LOTTO_5FROM50_OUT_RANGE + 1U);
	}

	return random_value;
}

static uint8_t LOTTO_generate_2_from_12(void)
{
	uint8_t random_value = (uint8_t)(PERIPHERALS_get_generated_random_value() % LOTTO_2FROM12_OUT_RANGE + 1U);

	return random_value;
}

static uint8_t LOTTO_check_if_in_5from50_the_same_number_generated_twice(uint8_t value)
{
	uint8_t result = 0;

	for (uint8_t i = 0; i < 5; i++)
	{
		if (LOTTO_results.lottoTab5from50[i] == value) result = GENERETED_TWICE;
	}

	return result;
}
