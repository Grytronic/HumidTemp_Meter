/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dispOLED.c                                                                                                                     */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Allow to use one interface for OLED displays based on different HW drivers                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/  

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "dispOLED.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
static void DISPOLED_ssd1306_init_for_temp(void);
static void DISPOLED_ssd1306_display_lotto_numbers(Lotto_t* pNumbers);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void DISPOLED_HW_init(void)
{
	SSD1306_init(SSD1306_REFRESH_RATIO_MID, SSD1306_MAX_CONTRAST);
}

void DISPOLED_init_for_temp(void)
{
	DISPOLED_ssd1306_init_for_temp();
}

void DISPOLED_set_contrast(uint8_t value)
{
    SSD1306_set_contrast(value);
}

void DISPOLED_clear(void)
{
	SSD1306_clear_display_buffer();
}

void DISPOLED_display_data_from_tempHumid_sensor(float32_t humidity, float32_t temperature)
{
	SSD1306_display_temperature(temperature);
	SSD1306_display_humidity(humidity);
}

void DISPOLED_display_lotto_numbers(Lotto_t* pNumbers)
{
	DISPOLED_ssd1306_display_lotto_numbers(pNumbers);
}

static void DISPOLED_ssd1306_init_for_temp(void)
{
	SSD1306_init_for_temp();
}

static void DISPOLED_ssd1306_display_lotto_numbers(Lotto_t* pNumbers)
{
	SSD1306_cls();
	SSD1306_display_u8_12x8(pNumbers->lottoTab5from50[0], 0, 0);
	SSD1306_display_u8_12x8(pNumbers->lottoTab5from50[1], 13, 0);
	SSD1306_display_u8_12x8(pNumbers->lottoTab5from50[2], 26, 0);
	SSD1306_display_u8_12x8(pNumbers->lottoTab5from50[3], 39, 0);
	SSD1306_display_u8_12x8(pNumbers->lottoTab5from50[4], 52, 0);

	SSD1306_display_u8_12x8(pNumbers->lottoTab2from12[0], 0, 1);
	SSD1306_display_u8_12x8(pNumbers->lottoTab2from12[1], 13, 1);
}

void DISPOLED_display_digit_18x26(uint8_t pos_x, uint8_t value)
{
	SSD1306_display_digit_18x26(pos_x, value);
}
