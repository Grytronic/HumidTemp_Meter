/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dio.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DIO driver                                                                                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "dio.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define Button1_MENU_pressed     !(PINC&(1<<PC2))
#define Button2_CHANGE_pressed   !(PINC&(1<<PC3))

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static void DIO_led_arduino_init(void);
static void DIO_led_orange_init(void);
static void DIO_led_green_init(void);
static void DIO_led_red_init(void);
static void DIO_buzzer_init(void);
static void DIO_button_int_init(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
/* Disp7SegLED HW init: */
void TM1637_DIO_init (void)
{
	CONFIG_TM1637_CLK_PIN_AS_OUTPUT;
	CONFIG_TM1637_DATA_PIN_AS_OUTPUT;
	SET_CLKPIN_TO_HIGH;
	SET_DATAPIN_TO_HIGH;
}


uint8_t DIO_check_if_Button_MENU_is_pressed(void)
{
	return Button1_MENU_pressed;
}


uint8_t DIO_check_if_Button_CHANGE_is_pressed(void)
{
	return Button2_CHANGE_pressed;
}


static void DIO_led_arduino_init(void)
{
	DDRB |= (1<<PB5);
	DIO_led_arduino_off();
}


void DIO_led_arduino_on(void)
{
	PORTB |= (1<<PB5);
}


void DIO_led_arduino_off(void)
{
	PORTB &= ~(1<<PB5);
}


void DIO_led_arduino_toggle(void)
{
	PORTB ^= (1<<PB5);
}


void DIO_led_arduino_blink(uint8_t how_many_times)
{
	for (uint8_t i = 0; i < how_many_times; i++)
	{
		DIO_led_arduino_on();
		PERIPHERALS_delay_ms(100);
		DIO_led_arduino_off();
	}
}

static void DIO_buzzer_init(void)
{
	DDRC |= (1<<PC1);
}

void DIO_buzzer_BEEP(void)
{
	DIO_buzzer_on();
	PERIPHERALS_delay_ms(100);
	DIO_buzzer_off();
}


void DIO_buzzer_off(void)
{
	PORTC |= (1<<PC1);
}

void DIO_buzzer_on(void)
{
	PORTC &= ~(1<<PC1);
}

static void DIO_led_orange_init(void)
{
	DDRB |= (1<<PB1);
	DIO_led_orange_off();
}

void DIO_led_orange_off(void)
{
	PORTB |= (1<<PB1);
}

void DIO_led_orange_on(void)
{
	PORTB &= ~(1<<PB1);
}

static void DIO_led_green_init(void)
{
	DDRB |= (1<<PB0);
	DIO_led_green_off();
}

void DIO_led_green_off(void)
{
	PORTB |= (1<<PB0);
}

void DIO_led_green_on(void)
{
	PORTB &= ~(1<<PB0);
}

static void DIO_led_red_init(void)
{
	DDRD |= (1<<PD7);
	DIO_led_red_off();
}

void DIO_led_red_off(void)
{
	PORTD |= (1<<PB7);
}

void DIO_led_red_on(void)
{
	PORTD &= ~(1<<PB7);
}

static void DIO_button_int_init(void)
{
	/* pull-up on buttons pins */
	PORTC |= (1<<PC3)|(1<<PC2);
	/* pin change int allowed*/
	PCICR |= (1<<PCIE1);
	/* pin change int enabled on corresponding pins */
	PCMSK1 |= ((1<<PCINT11)|(1<<PCINT10));
}

void DIO_board_init(void)
{
	DIO_buzzer_init();
	DIO_buzzer_off();
	DIO_led_arduino_init();
	DIO_led_orange_init();
	DIO_led_green_init();
	DIO_led_red_init();
	DIO_button_int_init();
}

void DIO_dht_setPIN_low(void)
{
	DHT_DIR  |= DHT_PIN; 		// pin jest wyj�ciem
	DHT_PORT &= ~DHT_PIN;		// ustawia 0
}

void DIO_dht_setPIN_high(void)
{
	DHT_DIR  &= ~DHT_PIN; 		// pin jest wejsciem
	DHT_PORT |= DHT_PIN;		// pull-up -ustawia stan 1
}

void DIO_set_tm1637_clk_low(void)
{

}

void DIO_set_tm1637_clk_high(void)
{

}

void DIO_set_tm1637_dout_low(void)
{

}

void DIO_set_tm1637_dout_high(void)
{

}

void DIO_twi_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data)
{
	TWI_write_byte(Slave_Addr, reg_adr, data);
}
