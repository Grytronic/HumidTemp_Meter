/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        lotto.c                                                                                                                        */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Lotto Jack-Pot numbers generator                                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "lotto.h"
#include "../Peripherals/avr/peripherals.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
Lotto_t LOTTO_results;

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static uint8_t LOTTO_generate_5_from_50(void);
static uint8_t LOTTO_generate_2_from_12(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 

void LOTTO_generate_numbers(void)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		LOTTO_results.lottoTab5from50[i] = LOTTO_generate_5_from_50();
		printf("\n %d", LOTTO_results.lottoTab5from50[i]);
	}
	printf("\n");

	for (uint8_t i = 0; i < 2; i++)
	{
		LOTTO_results.lottoTab2from12[i] = LOTTO_generate_2_from_12();

		printf("\n %d", LOTTO_results.lottoTab2from12[i]);
	}
	printf("\n");
}

static uint8_t LOTTO_generate_5_from_50(void)
{
	uint8_t random_value = (uint8_t)((PERIPHERALS_get_generated_random_value() % 51U));

	return random_value;
}

static uint8_t LOTTO_generate_2_from_12(void)
{
	uint8_t random_value = (uint8_t)((PERIPHERALS_get_generated_random_value() % 13U));

	return random_value;
}
