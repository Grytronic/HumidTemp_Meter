/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        mock_lotto.h                                                                                                                   */
/* Created:     09.06.2024                                                                                                                     */
/* Description: Mock for Lotto module                                                                                                          */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef MOCK_LOTTO_H_
#define MOCK_LOTTO_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
#define LOTTO_5FROM50_OUT_RANGE 50U
#define LOTTO_2FROM12_OUT_RANGE 12U
#define DEFINED_VALUE_WHEN_RANDOM_VALUE_IS_0 3U

/* External types -----------------------------------------------------------------------------------------------------------------------------*/
typedef enum
	{
	  generated_numbers,
	  faked_numbers
	} genMode_t;

/* External consts ----------------------------------------------------------------------------------------------------------------------------*/
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/
int PERIPHERALS_get_generated_random_value(void);
void PERIPHERALS_set_value_for_generated_random_value(int val);
void PERIPHERALS_reset_start_value_for_generated_random_value(void);
void PERIPHERALS_use_random_generation_mode(genMode_t mode);


#endif /* MOCK_LOTTO_H_ */
