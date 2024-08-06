/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dio.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DIO driver                                                                                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "dio.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
/* Disp7SegLED mapping: */
#define TM1637_PORT_DATA			 	PORTD
#define TM1637_DIRREG_DATA 		        DDRD
#define TM1637_PINREG_DATA				PIND
#define TM1637_DATA_PIN					PD6

#define TM1637_PORT_CLK			 		PORTB
#define TM1637_DIRREG_CLK 		        DDRB
#define TM1637_PINREG_CLK				PINB
#define TM1637_CLK_PIN					PB4

// this is better to be functions with signle principle responsibility
#define CONFIG_TM1637_DATA_PIN_AS_INPUT  	TM1637_DIRREG_DATA &= ~(1<<TM1637_DATA_PIN)
#define CONFIG_TM1637_DATA_PIN_AS_OUTPUT	TM1637_DIRREG_DATA |= (1<<TM1637_DATA_PIN)
#define CONFIG_TM1637_CLK_PIN_AS_OUTPUT		TM1637_DIRREG_CLK |= (1<<TM1637_CLK_PIN)
#define IS_TM1637_DATAPIN_LOW			    (!(TM1637_PINREG_DATA & (1<<TM1637_DATA_PIN)))
#define IS_TM1637_DATAPIN_HIGH			    (TM1637_PINREG_DATA & (1<<TM1637_DATA_PIN))

/* TempRH sensor: */
#define DHT_PORT 		PORTD
#define DHT_DIRREG		DDRD
#define DHT_PINREG 		PIND
#define DHT_PIN 		PD4

/* Buttons */
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
static void DIO_TM1637_init(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
/* Disp7SegLED HW init: */
static void DIO_TM1637_init(void)
{
	CONFIG_TM1637_CLK_PIN_AS_OUTPUT;
	CONFIG_TM1637_DATA_PIN_AS_OUTPUT;
	DIO_set_tm1637_clk_high();
	DIO_set_tm1637_dout_high();
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

void DIO_buzzer_off(void)
{
	PORTC |= (1<<PC1);
}

void DIO_buzzer_on(void)
{
	PORTC &= ~(1<<PC1);
}

void DIO_buzzer_BEEP(void)
{
	DIO_buzzer_on();
	PERIPHERALS_delay_ms(100);
	DIO_buzzer_off();
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
	DDRD |= (1<<PB7);
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
	DIO_TM1637_init();
#if (ACTIVE == USE_INTERNAL_PULLUP_FOR_DHT_SENSOR)
	DIO_dht_pullup_on();
#endif
}

/* TempRH sensor */
void DIO_dht_setPIN_low(void)
{
	DHT_DIRREG |= (1<<DHT_PIN); 	// pin jest wyj�ciem
	DHT_PORT &= ~ (1<<DHT_PIN);		// ustawia 0
}

void DIO_dht_setPIN_high(void)
{
	DIO_dht_pullup_on();
}

void DIO_dht_pullup_on(void)
{
	DHT_DIRREG  &= ~(1<<DHT_PIN); 	// pin jest wejsciem
	DHT_PORT |= (1<<DHT_PIN);		// pull-up -ustawia stan 1
}

uint8_t DIO_check_if_dht_PIN_is_high(void)
{
	return (DHT_PINREG & (1<<DHT_PIN));
}

/* LED display */
void DIO_set_tm1637_clk_low(void)
{
	TM1637_PORT_CLK &= ~(1<<TM1637_CLK_PIN);
}

void DIO_set_tm1637_clk_high(void)
{
	TM1637_PORT_CLK |= (1<<TM1637_CLK_PIN);
}

void DIO_set_tm1637_dout_low(void)
{
	TM1637_PORT_DATA &= ~(1<<TM1637_DATA_PIN);
}

void DIO_set_tm1637_dout_high(void)
{
	TM1637_PORT_DATA |= (1<<TM1637_DATA_PIN);
}

/* OLED display */
void DIO_twi_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data)
{
	TWI_write_byte(Slave_Addr, reg_adr, data);
}
