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
static int LOTTO_generate_5_from_50(void);
static int LOTTO_generate_2_from_12(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 

void LOTTO_generate_numbers(void)
{
	for (uint8_t i = 0; i < 5; i++)
	{
		LOTTO_results.lottoTab5from50[i] = LOTTO_generate_5_from_50();
		for (uint8_t y = 0; y < i; y++)
		{
			if (LOTTO_results.lottoTab5from50[y] == LOTTO_results.lottoTab5from50[i])
			{
				i-=1;
			}
		}
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		LOTTO_results.lottoTab2from12[i] = LOTTO_generate_2_from_12();
		for (uint8_t y = 0; y < i; y++)
		{
			if (LOTTO_results.lottoTab2from12[y] == LOTTO_results.lottoTab2from12[i])
			{
				i-=1;
			}
		}
	}
}

static int LOTTO_generate_5_from_50(void)
{
	return ((PERIPHERALS_get_generated_random_value() % 49U) + 1U);
}

static int LOTTO_generate_2_from_12(void)
{
	return ((PERIPHERALS_get_generated_random_value() % 11U) + 1U);
}
