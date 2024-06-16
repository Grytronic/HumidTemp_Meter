/*
 * fake_lotto.h
 *
 *  Created on: 15.06.2024
 *      Author: Boss
 */

#ifndef FAKE_LOTTO_H_
#define FAKE_LOTTO_H_


#define LOTTO_5FROM50_OUT_RANGE 51U
#define LOTTO_2FROM12_OUT_RANGE 13U
#define FAKE_PERIPHERALS_NUMBER_WHEN_RANDOM_VALUE_IS_0 3U


int PERIPHERALS_get_generated_random_value(void);
void PERIPHERALS_set_value_for_generated_random_value(int val);
void PERIPHERALS_reset_start_value_for_generated_random_value(void);

#endif /* FAKE_LOTTO_H_ */
