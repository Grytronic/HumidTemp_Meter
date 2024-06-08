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
/* Disp7SegLED mapping: */
#define TM1637_PORT_DATA			 	PORTD
#define TM1637_DIRECTION_REG_DATA 		DDRD
#define TM1637_PINREG_DATA				PIND

#define TM1637_PORT_CLK			 		PORTB
#define TM1637_DIRECTION_REG_CLK 		DDRB
#define TM1637_PINREG_CLK				PINB

#define DATA_PIN					    PD6  //D6
#define CLK_PIN						    PB4	 //D12






#define DHT_PIN 		(1<<PD4)
#define DHT_PORT 		PORTD
#define DHT_DIR			DDRD
#define DHT_PINREG 		PIND
#define USE_INTERNAL_PULLUP_FOR_SENSOR 1




#define SET_CLKPIN_TO_HIGH		TM1637_PORT_CLK |= (1<<CLK_PIN)
#define SET_CLKPIN_TO_LOW		TM1637_PORT_CLK &= ~(1<<CLK_PIN)

#define SET_DATAPIN_TO_HIGH		TM1637_PORT_DATA |= (1<<DATA_PIN)
#define SET_DATAPIN_TO_LOW		TM1637_PORT_DATA &= ~(1<<DATA_PIN)

#define CONFIG_TM1637_DATA_PIN_AS_INPUT  	TM1637_DIRECTION_REG_DATA &= ~(1<<DATA_PIN)
#define CONFIG_TM1637_DATA_PIN_AS_OUTPUT	TM1637_DIRECTION_REG_DATA |= (1<<DATA_PIN)
#define CONFIG_TM1637_CLK_PIN_AS_OUTPUT		TM1637_DIRECTION_REG_CLK |= (1<<CLK_PIN)

#define CONFIG_TM1637_DATA_PIN_SET_PULLUP 	TM1637_PORT_DATA |= (1<<DATA_PIN)


#define IS_DATAPIN_LOW			!(TM1637_PINREG_DATA & (1<<DATA_PIN))
#define IS_DATAPIN_HIGH			TM1637_PINREG_DATA & (1<<DATA_PIN)

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

void DIO_set_tm1637_clk_low(void);
void DIO_set_tm1637_clk_high(void);
void DIO_set_tm1637_dout_low(void);
void DIO_set_tm1637_dout_high(void);

void DIO_twi_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data);

uint8_t DIO_check_if_Button_MENU_is_pressed(void);
uint8_t DIO_check_if_Button_CHANGE_is_pressed(void);


#endif /* __DIO_H_ */
