/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        tm1637.c                                                                                                                       */
/* Created:     18.04.2024                                                                                                                     */
/* Description: TM1637 7seg 4-digit LED Driver                                                                                                 */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "tm1637.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define TM1637_DP_CLOCK_MASK  0x80U
#define TM1637_BRIGHTNESS_MAX 7U
#define TM1637_EMPTY_DIGIT 10U
#define TM1637_MINUS_SIGN 11U

// segment addressing from the left to the right
#define DIGIT1_ADDRESS 0xC0U
#define DIGIT2_ADDRESS 0xC1U
#define DIGIT3_ADDRESS 0xC2U
#define DIGIT4_ADDRESS 0xC3U
/* segment addressing from the right to the left
#define DIGIT1_ADDRESS 0xC3U
#define DIGIT2_ADDRESS 0xC2U
#define DIGIT3_ADDRESS 0xC1U
#define DIGIT4_ADDRESS 0xC0U
*/

#define TM1637_MEMORY_REG_START_ADDRESS 0xC0U
#define TM1637_MEMORY_REG_STOP_ADDRESS 0xC4U

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/

/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/
/* display 7seg digit codes defined in ROM, order is: 0-9, empty, -, o, C, H, L */
#if (USE_HW_PLATFORM_AVR == SET) 
	const uint8_t Digits[16] PROGMEM = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40, 0x63, 0x39,0b01110110, 0b00111000 };
#endif
#if (USE_HW_PLATFORM_STM32 == SET)
	const uint8_t Digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40, 0x63, 0x39,0b01110110, 0b00111000 };
#endif


/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
volatile uint8_t TxBuffer;

volatile uint8_t Digit1 = 10;
volatile uint8_t Digit2 = 10;
volatile uint8_t Digit3 = 10;
volatile uint8_t Digit4 = 10;

volatile uint8_t Dp1Flag = FLAG_IS_RESET;
volatile uint8_t Dp2Flag = FLAG_IS_RESET;
volatile uint8_t Dp3Flag = FLAG_IS_RESET;
volatile uint8_t Dp4Flag = FLAG_IS_RESET;

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static void TM1637_send_START(void);
static void TM1637_send_STOP(void);
static void TM1637_transmit_byte(uint8_t byte);
static void TM1637_send_command(uint8_t command);
static void TM1637_send_command_with_data(uint8_t command, uint8_t data);
static void TM1637_set_address_mode_incrementation(void);
static void TM1637_set_address_mode_fixed(void);
static void TM1637_set_display_normal_mode(void);
static void TM1637_set_display_test_mode(void);
static void TM1637_clear_display_memory(void);
static void TM1637_write_to_display_memory_fixed_addr(uint8_t address, uint8_t byte);
static void TM1637_clear_display_memory(void);
static void TM1637_tx_all_digits(void);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
static void TM1637_send_START(void)	
{
    DIO_set_tm1637_clk_high();
    PERIPHERALS_delay_us(1);
	DIO_set_tm1637_dout_high();
	PERIPHERALS_delay_us(1);
    DIO_set_tm1637_dout_low();
    PERIPHERALS_delay_us(1);
}

static void TM1637_send_STOP(void)
{
	DIO_set_tm1637_clk_low();
	PERIPHERALS_delay_us(1);
	DIO_set_tm1637_clk_high();
	PERIPHERALS_delay_us(1);
	DIO_set_tm1637_dout_low(); 		
	PERIPHERALS_delay_us(1);
	DIO_set_tm1637_dout_high();
	PERIPHERALS_delay_us(1);
}

static void TM1637_transmit_byte(uint8_t byte)	
{
    for (uint8_t i = 8; i; --i, byte >>= 1)
    {
    	DIO_set_tm1637_clk_low();
        if (byte & 1U)
        	DIO_set_tm1637_dout_high();
        else
		{
        	DIO_set_tm1637_dout_low();
		}
        PERIPHERALS_delay_us(1);
        DIO_set_tm1637_clk_high();
        PERIPHERALS_delay_us(1);
    }
    //check ACK:
    DIO_set_tm1637_clk_low();       
	DIO_set_tm1637_dout_low();
	PERIPHERALS_delay_us(1);
    DIO_set_tm1637_clk_high();
    PERIPHERALS_delay_us(1);
}

static void TM1637_send_command(uint8_t command)
{
	TM1637_send_START();
	TM1637_transmit_byte(command);
	TM1637_send_STOP();
	PERIPHERALS_delay_us(1);
}

static void TM1637_send_command_with_data(uint8_t command, uint8_t data)
{
	TM1637_send_START();
	TM1637_transmit_byte(command);
	TM1637_transmit_byte(data);
	TM1637_send_STOP();
	PERIPHERALS_delay_us(1);
}

void TM1637_init(uint8_t brightness)	
{
	PERIPHERALS_delay_ms(200);
	TM1637_display_off();
	TM1637_set_address_mode_fixed();
	//TM1637_set_address_mod_incrementation();
	TM1637_set_display_normal_mode();
	//TM1637_set_display_test_mode();
	TM1637_clear_display_memory();
	TM1637_set_brightness(brightness);
	TM1637_display_on();
}

void TM1637_display_on(void)	
{
	TxBuffer |= 0b10001000; //turn on with actual brighness
	TxBuffer &= 0b10001111; //write data to display register
	TM1637_send_command(TxBuffer);
}

void TM1637_display_off(void)	
{
	TxBuffer |= 0b10000000; //write actual brighness
	TxBuffer &= 0b10000111;
	TM1637_send_command(TxBuffer);
}

void TM1637_set_brightness(uint8_t brightness)  
{	
	if (brightness <= TM1637_BRIGHTNESS_MAX)
	{
		TxBuffer &=  0b10001000;
		TxBuffer |= (0b10000000 | (0b00000111 & brightness));
		TM1637_send_command(TxBuffer);
	}
}

void TM1637_set_address_mode_incrementation(void)	
{
	TxBuffer |= 0b01000000;
	TxBuffer &= 0b01001000;
	TM1637_send_command(TxBuffer);
}

void TM1637_set_address_mode_fixed(void)	
{
	TxBuffer |= 0b01000100;
	TxBuffer &= 0b01001100;
	TM1637_send_command(TxBuffer);
}

void TM1637_set_display_normal_mode(void)	
{
	TxBuffer |= 0b01000000;
	TxBuffer &= 0b01000100;
	TM1637_send_command(TxBuffer);
}

void TM1637_set_display_test_mode(void)	
{
	TxBuffer |= 0b01001000;
	TxBuffer &= 0b01001100;
	TM1637_send_command(TxBuffer);
}

static void TM1637_write_to_display_memory_fixed_addr(uint8_t address, uint8_t byte)	
{
	//if (byte> maxCountofArrayElements) byte = maxCountofArrayElements;
	if      (address == DIGIT1_ADDRESS && Dp1Flag == 1 ) TM1637_send_command_with_data(address, (pgm_read_byte(&Digits[byte]) | TM1637_DP_CLOCK_MASK));
	else if (address == DIGIT2_ADDRESS && Dp2Flag == 1 ) TM1637_send_command_with_data(address, (pgm_read_byte(&Digits[byte]) | TM1637_DP_CLOCK_MASK));
	else if (address == DIGIT3_ADDRESS && Dp3Flag == 1 ) TM1637_send_command_with_data(address, (pgm_read_byte(&Digits[byte]) | TM1637_DP_CLOCK_MASK));
	else if (address == DIGIT4_ADDRESS && Dp4Flag == 1 ) TM1637_send_command_with_data(address, (pgm_read_byte(&Digits[byte]) | TM1637_DP_CLOCK_MASK));
	else TM1637_send_command_with_data(address, pgm_read_byte(&Digits[byte]) );
}

void TM1637_display_digit(uint8_t position, uint8_t digit)	
{
	uint8_t positionAddress;

	if (position == 1)	
	{
		positionAddress = DIGIT1_ADDRESS;
		Digit1 = digit;
	}
	else if (position == 2)	
	{
		positionAddress = DIGIT2_ADDRESS;
		Digit2 = digit;
	}
	else if (position == 3)	
	{
		positionAddress = DIGIT3_ADDRESS;
		Digit3 = digit;
	}
	else if (position == 4)	
	{
		positionAddress = DIGIT4_ADDRESS;
		Digit4 = digit;
	}
	else
	{
		return;
	}
	TM1637_write_to_display_memory_fixed_addr(positionAddress, digit);
}

static void TM1637_clear_display_memory(void)	
{
	uint8_t addr = TM1637_MEMORY_REG_START_ADDRESS;
	
	while (addr < TM1637_MEMORY_REG_STOP_ADDRESS)	{
		TM1637_write_to_display_memory_fixed_addr(TM1637_MEMORY_REG_START_ADDRESS, TM1637_EMPTY_DIGIT);
		addr++;
	}
}

static void TM1637_tx_all_digits(void)	
{
	TM1637_write_to_display_memory_fixed_addr(DIGIT1_ADDRESS, Digit1);
	TM1637_write_to_display_memory_fixed_addr(DIGIT2_ADDRESS, Digit2);
	TM1637_write_to_display_memory_fixed_addr(DIGIT3_ADDRESS, Digit3);
	TM1637_write_to_display_memory_fixed_addr(DIGIT4_ADDRESS, Digit4);
}

void TM1637_set_dp_or_colon(uint8_t dpPosition)
{
	if (dpPosition == 1) Dp1Flag = 1;
	if (dpPosition == 2) Dp2Flag = 1;
	if (dpPosition == 3) Dp3Flag = 1;
	if (dpPosition == 4) Dp4Flag = 1;
	TM1637_tx_all_digits();
}

void TM1637_reset_dp_or_colon(uint8_t dpPosition)
{
	if (dpPosition == 1) Dp1Flag = 0;
	if (dpPosition == 2) Dp2Flag = 0;
	if (dpPosition == 3) Dp3Flag = 0;
	if (dpPosition == 4) Dp4Flag = 0;
	TM1637_tx_all_digits();
}

/*
void TM1637_display_temp_int_oC (uint8_t temp_int)	
{
	uint8_t dziesiatki = temp_int/10;
	uint8_t jednostki = temp_int%10;
	TM1637_display_digit(1, dziesiatki);
	TM1637_display_digit(2, jednostki);
	TM1637_display_digit(3, 12);
	TM1637_display_digit(4, 13);
}
*/

void TM1637_display_u16(uint16_t value)	
{
	if (value > 9999)	
	{
		TM1637_display_digit(1, TM1637_MINUS_SIGN);
		TM1637_display_digit(2, TM1637_MINUS_SIGN);
		TM1637_display_digit(3, TM1637_MINUS_SIGN);
		TM1637_display_digit(4, TM1637_MINUS_SIGN);
	}
	else {
		uint8_t tempFlag = 0;

		value %= 10000;
		uint8_t temp = value / 1000;

		if (temp) 
		{
			TM1637_display_digit(1, temp);
			tempFlag = 1;
		}
		else 
		{
			TM1637_display_digit(1, TM1637_EMPTY_DIGIT);
		}

		value %= 1000;
		temp = value / 100;

		if (temp) 
		{
			TM1637_display_digit(2, temp);
			tempFlag = 1;
		}
		else 
		{
			if (tempFlag > 0) TM1637_display_digit(2, 0);
			else TM1637_display_digit(2, TM1637_EMPTY_DIGIT);
		}

		value %= 100;
		temp = value / 10;

		if (temp) 
		{
			TM1637_display_digit(3, temp);
			tempFlag = 1;
		}
		else 
		{
			if (tempFlag > 0) TM1637_display_digit(3, 0);
			else TM1637_display_digit(3, TM1637_EMPTY_DIGIT);
		}

		value %= 10;
		TM1637_display_digit(4, value);
	}
}

void TM1637_display_u8_as_2_digits(uint8_t value)
{
	if (value > 99)	
	{
		TM1637_display_digit(3, 11);
		TM1637_display_digit(4, 11);
	}
	else 
	{
		uint8_t temp_flag = 0;
		uint8_t temp = value / 10;

		if (temp) 
		{
			TM1637_display_digit(3, temp);
			temp_flag = 1;
		}
		else 
		{
			if (temp_flag > 0) TM1637_display_digit(3, 0);
			else TM1637_display_digit(3, TM1637_EMPTY_DIGIT);
		}

		value %= 10;
		TM1637_display_digit(4, value);
	}
}

void TM1637_display_int16(int16_t value)	
{
	if (value > 9999 || value < -999) {

		TM1637_display_digit (1, 11);
		TM1637_display_digit (2, 11);
		TM1637_display_digit (3, 11);
		TM1637_display_digit (4, 11);
	}
	else
	{
		uint8_t tempFlag = 0;
		uint8_t minusFlag = 0;
		uint8_t firstPosition = 1;

		uint8_t temp = value / 1000;

		if (value < 0) 
		{
			minusFlag = 1;
			value *= -1;
			firstPosition = 2;
		}

		if (temp) 
		{
			TM1637_display_digit(firstPosition, temp);
			tempFlag = 1;

			if (minusFlag == 1)	
			{
				TM1637_display_digit((firstPosition - 1), TM1637_MINUS_SIGN);
				minusFlag = 0;
			}
		}
		else
		{
			TM1637_display_digit(firstPosition, TM1637_EMPTY_DIGIT);
			firstPosition = 1;
		}

		firstPosition++;
		value %= 1000;
		temp = value / 100;

		if (temp) 
		{
			TM1637_display_digit(firstPosition, temp);
			tempFlag = 1;

			if (minusFlag == 1)	
			{
				TM1637_display_digit((firstPosition - 1), TM1637_MINUS_SIGN);
				minusFlag = 0;
			}
		}
		else 
		{
			if (tempFlag > 0) TM1637_display_digit(firstPosition, 0);
			else TM1637_display_digit(firstPosition, TM1637_EMPTY_DIGIT);
		}

		firstPosition++;
		value %= 100;
		temp = value / 10;

		if (temp) 
		{
			TM1637_display_digit (firstPosition, temp);
			tempFlag = 1;

			if (minusFlag == 1)	
			{
				TM1637_display_digit((firstPosition - 1), TM1637_MINUS_SIGN);
				minusFlag = 0;
			}
		}
		else 
		{
			if (tempFlag > 0) TM1637_display_digit(firstPosition, 0);
			else TM1637_display_digit(firstPosition, TM1637_EMPTY_DIGIT);
		}

		firstPosition++;
		value %= 10;

		if (minusFlag == 1)	TM1637_display_digit((firstPosition - 1), TM1637_MINUS_SIGN); 
		TM1637_display_digit(firstPosition, value);
	}
}

void TM1637_display_float(float value, uint8_t precision)	
{
	TM1637_reset_dp_or_colon(1);
	TM1637_reset_dp_or_colon(2);
	TM1637_reset_dp_or_colon(3);
	TM1637_reset_dp_or_colon(4);

	if (precision == 1 || precision == 10)	
	{
		value *= 10;
		TM1637_set_dp_or_colon(3);
	}
	if (precision == 2 || precision == 100)	
	{
		value *= 100;
		TM1637_set_dp_or_colon(2);
	}
	if (precision == 3 || precision == 1000)	
	{
		value *= 1000;
		TM1637_set_dp_or_colon(1);
	}

	TM1637_display_int16(value);
}

void TM1637_display_hours(uint8_t hours)	
{
	if (hours / 10) TM1637_display_digit(1, (hours / 10));
	else TM1637_display_digit(1, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(2, (hours % 10));
}

void TM1637_display_minutes(uint8_t minutes)	
{
	TM1637_display_digit(3, (minutes / 10));
	TM1637_display_digit(4, (minutes % 10));
}

void TM1637_displaySECONDS(uint8_t seconds)	
{
	TM1637_display_digit(1, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(2, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(3, (seconds / 10));
	TM1637_display_digit(4, (seconds % 10));
}

void TM1637_display_clear(void)
{
	TM1637_display_digit(1, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(2, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(3, TM1637_EMPTY_DIGIT);
	TM1637_display_digit(4, TM1637_EMPTY_DIGIT);
}

