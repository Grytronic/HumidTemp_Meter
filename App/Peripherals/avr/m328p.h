/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        m328p.h                                                                                                                        */
/* Created:     21.04.2024                                                                                                                     */
/* Description: ATmega328p Interface HW Interface                                                                                              */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef M328P_H_
#define M328P_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 						// dla wyswietlania stalych z pamieci FLASH
#include "util/delay.h"
#include "adc.h"
#include "dio.h"
#include "i2c.h"
#include "tim.h"
#include "uart.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
#define use_m16m32_init_settings       0
 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void M328P_peripherals_init(void);
void M328P_peripherals_enable_global_interrupts(void);
int M328P_get_generated_random_value(void);
void M328P_delay_us(uint16_t delay);
void M328P_delay_ms(uint16_t delay);
/*
 * Arduino Nano ATmega328PB
 * Procesor 2 MHz (16Mhz/CDIV 8)
 * OLED Display
 * DTH11
 * Buzzer
 *
 *
//---------------------------------------------
//ATmega 328 PINOUT:
//---------------------------------------------
//			TX   PD1      		---     VIN
//	GPS	    RX   PD0      		---     GND
//			RST           		---     RST
//			GND           		---     +5V
//			D2   PD2  			---     A7 -ADC7
//			D3   PD3 			    ---	    A6
//  DTH11   D4   PD4   		    ---     A5  PC5                 I2C SSD1306
//	  	    D5   PD5      		---     A4  PC4                 I2C SSD1306
//     TM.. D6   PD6      		---     A3  PC3                 ButtonMenu (PCINT11)
//  	    D7   PD7    	 		---     A2  PC2             ButtonChoose (PCINT10)
//		    D8   PB0      		---     A1  PC1                 Buzzer
//		    D9   PB1      		---     A0  PC0                 ADC
//		    D10  PB2     			---     AREF
//		    D11  PB3      		---     3V3
// 		TM..D12  PB4      		---     D13 PB5  LED
 *
 * ------CONNECTIONS--------
 * 	OLED:
 * 	- SCL PC5 A5
 * 	- SDA PC4 A4
 *
 * 	DTH11:
 * 	- DIO PD4 D4 (Arduino 4)
 *
 * 	LED_TM1637:
 * 	- DIO PD6 D6 (Arduino )
 * 	- CLK PB4 D12 (Arduino 12)
 *
 * 	Buzzer:
 * 	- PC1
 * 
 * // ---I2C  Arduino NANO:
// ---SDA  A4 - PC4
// ---SCL  A5 - PC5
 */


#endif /* M328P_H_ */
