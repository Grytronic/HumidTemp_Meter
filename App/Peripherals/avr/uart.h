/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        uart.h                                                                                                                         */
/* Created:     21.04.2024                                                                                                                     */
/* Description: UART driver                                                                                                                    */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef UART_H_
#define UART_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/ 
#include "peripherals.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
#define VT100_command_for_teminal_use  0

/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void UART_init( uint16_t baudrate ) ;

void UART_send_char (char data);
void UART_send_string (char *s);
void UART_send_string_progmem (const char *s);
void UART_send_int (int val);

void UART_send_Uinteger32 (uint32_t zmienna);
void UART_set_new_line (void);

char UART_Receive_one_char(void);
void UART_receive_string (unsigned char *serial_received_data);


#if VT100_command_for_teminal_use
//tu nazwy VT100_cursor_home

//	------------- obsluga komend vt100
void VT100_terminal_cursor_hide (uint8_t hide);
void VT100_terminal_cls (uint8_t cursor_off);
void VT100_terminal_locate (uint8_t y, uint8_t x);
void VT100_terminal_background_colour (uint8_t color);
void VT100_terminal_font_colour_basic (uint8_t color);
void VT100_terminal_font_colour_brigt (uint8_t colour);
void VT100_terminal_font_attr (uint8_t atr, uint8_t fg, uint8_t bg);
void VT100_terminal_print_line (char ascii, uint8_t cnt);
void VT100_terminal_cursor_home_1_1( uint8_t cur_onoff);
//void terminal_cursor_left( uint8_t cur_onoff);
#endif

#endif /* UART_H_ */

/*
 *  	Set text color - basic
 *
0 � black
1 � red
2 � green
3 � yellow
4 � blue
5 � magenta
6 � cyan
7 � white

 *  	Set text color - brigt
 *
0 �
1 � red
2 �
3 � yellow
4 �
5 � magneta (t-mobile)
6 � turkus
7 �
 */



/* od Atmela:
#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
*/





//PD0-RXD
//PD1-TXD
