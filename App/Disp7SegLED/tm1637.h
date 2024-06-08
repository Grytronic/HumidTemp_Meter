/* -------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                   */
/* File:        tm1637.h                                                                                                                      */
/* Created:     18.04.2024                                                                                                                    */
/* Description: TM1637 7seg 4-digit LED Driver                                                                                                */
/* Comments:                                                                                                                                  */
/*  ------------------------------------
 * 	TM1637 7seg display segment mapping: 
 *  ------------------------------------
 * 			 1 (0x01)
 * 		    	 --
 * 	32 (0x0x20)	|  |   2 (0x02)
 * 				 --   64 (0x40)
 *  16 (0x0x10)	|  |   4 (0x04)
 *  	 		 --
 *    		 8 (0x08)
 *
 *    DP  :    128 (0x80)
 *
 *
 *	128  64  32  16  8  4  2  1
 *	 b8	 b7  b6  b5  b4 b3 b2 b0
 *   0
 */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef TM1637_H_
#define TM1637_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/ 

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void TM1637_init(uint8_t brightness);

void TM1637_display_on(void);
void TM1637_display_off(void);
void TM1637_display_clear(void);
void TM1637_set_brightness(uint8_t brightness);

void TM1637_set_dp_or_colon(uint8_t place);
void TM1637_reset_dp_or_colon(uint8_t place);

void TM1637_display_digit(uint8_t position, uint8_t digit);

void TM1637_display_hours(uint8_t hours);
void TM1637_display_minutes(uint8_t minutes);
void TM1637_display_seconds(uint8_t seconds);

void TM1637_display_u8_as_2_digits(uint8_t value);
void TM1637_display_u16(uint16_t value);
void TM1637_display_int16(int16_t value);
void TM1637_display_float(float value, uint8_t precision);

#endif /* TM1637_H_ */
