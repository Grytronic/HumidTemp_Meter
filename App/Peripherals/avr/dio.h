/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dio.h                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DIO driver                                                                                                                     */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef __DIO_H_
#define __DIO_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../../Main/main.h"
#include "peripherals.h"
 
/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void DIO_board_init(void);

void DIO_led_arduino_on(void);
void DIO_led_arduino_off(void);
void DIO_led_arduino_toggle(void);
void DIO_led_arduino_blink(uint8_t how_many_times);

void DIO_led_orange_on(void);
void DIO_led_orange_off(void);

void DIO_led_green_on(void);
void DIO_led_green_off(void);

void DIO_led_red_on(void);
void DIO_led_red_off(void);

void DIO_buzzer_on(void);
void DIO_buzzer_off(void);
void DIO_buzzer_BEEP(void);

void DIO_dht_setPIN_low(void);
void DIO_dht_setPIN_high(void);
void DIO_dht_pullup_on(void);
uint8_t DIO_check_if_dht_PIN_is_high(void);

void DIO_set_tm1637_clk_low(void);
void DIO_set_tm1637_clk_high(void);
void DIO_set_tm1637_dout_low(void);
void DIO_set_tm1637_dout_high(void);

void DIO_twi_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data);

uint8_t DIO_check_if_Button_MENU_is_pressed(void);
uint8_t DIO_check_if_Button_CHANGE_is_pressed(void);


#endif /* __DIO_H_ */
