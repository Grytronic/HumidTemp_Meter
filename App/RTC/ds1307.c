/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        ds1307.c                                                                                                                       */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DS1307 RTC driver                                                                                                              */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "ds1307.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define DS1307_I2C_ADDRESS 0x68 // address 

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
uint8_t pm_flag;
uint8_t mode_24_on;

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void DS1307_write_memory (uint8_t addr, uint8_t TM_register)	{
	TWI_write_byte(DS1307_I2C_ADDRESS, addr, TM_register);
}


uint8_t DS1307_read_memory (uint8_t addr)	{
	return TWI_read_byte(DS1307_I2C_ADDRESS, addr);
}


uint8_t decTObcd_conv (uint8_t dec)	{
	 uint8_t bcd = (0b11110000 & ((dec/10)<<4)) | (0b00001111 & (dec%10) );
	return bcd;
}


uint8_t bcdTOdec_conv (uint8_t bcd)	{
	 uint8_t dec = (10*(bcd>>4))+(0b00001111 & bcd);
	return dec;
}


void clock_halt_on (void)	{
	uint8_t temp = 0b01111111 & DS1307_read_memory (0x00);
	DS1307_write_memory(0x00, temp);
}


void clock_halt_off (void)	{
	uint8_t temp = 0b10000000 | DS1307_read_memory (0x00);
	DS1307_write_memory(0x00, temp);
}


void DS1307_init(void)	{
	//i2cSetBitrate(100);
	clock_halt_on();
	DS1307_set_mode24h();
	DS1307_set_control_register_1hz ();
}

void DS1307_set_control_register_1hz (void)	{
	DS1307_write_memory(0x07, 0b00010000);
}

void DS1307_write_seconds (uint8_t seconds)	{
	if (seconds > 59) seconds = 0;
	uint8_t temp = decTObcd_conv(seconds);
	temp = (0b10000000 & DS1307_read_memory (0x00)) | (0b01111111 & temp);
	DS1307_write_memory(0x00, temp);
}


void DS1307_write_minutes (uint8_t minutes)	{
	if (minutes > 59) minutes = 0;
	uint8_t temp = decTObcd_conv(minutes);
	DS1307_write_memory(0x01, (0b01111111 & temp));
}


void DS1307_write_am_pm_hour (uint8_t hour, uint8_t pm)	{
	if (hour >= 13) {
		if (hour >= 24) hour = 24;

			hour -= 12;
			pm = 1;
	}
	hour = decTObcd_conv(hour);
	if (pm == 1) {
		hour |= 0b00100000;  //set PM
	}
	if (pm == 0) {
		hour &= ~0b00100000; //set AM
	}
	hour |= 0b01000000; 	 //mode 12h
	DS1307_write_memory(0x02, (0b01111111 & hour));
}


void DS1307_write_24h_hour (uint8_t hour)	{
	if (hour > 24) hour = 24;
	if (hour > 12) pm_flag = 1;
	hour = decTObcd_conv(hour);
	DS1307_write_memory(0x02, (0b00111111 & hour));
}


void DS1307_write_day_name (uint8_t day_name)	{
	DS1307_write_memory(0x03, (0b00000111 & day_name));
}


void DS1307_write_day (uint8_t day)	{
	day = decTObcd_conv(day);
	DS1307_write_memory(0x04, (0b00111111 & day));
}


void DS1307_write_month (uint8_t month)	{
	month = decTObcd_conv(month);
	DS1307_write_memory(0x05, (0b00011111 & month)) ;
}


void DS1307_write_year (uint8_t year)	{
	year = decTObcd_conv(year);
	DS1307_write_memory(0x06, year);
}


uint8_t DS1307_read_hour (void)	{
	uint8_t hours_reg_state = 0b01111111 & DS1307_read_memory (0x02);
	uint8_t hour = 0;
	//if mode 24h
	if (!(hours_reg_state & 0b01000000)) 	{
		hour = (hours_reg_state & 0b00111111);
		mode_24_on = 1;
	}
	//if mode am/pm
	else 	{
		hour = (hours_reg_state & 0b00111111);
		//if pm
		if ((hour & 0b00100000))	pm_flag = 1;
		else pm_flag = 0;
		hour = (hour & 0b00011111);
		if (hour == 0b00010010 && pm_flag == 1 ) hour = 0;
		mode_24_on = 0;
	}
	return bcdTOdec_conv(hour);
}


uint8_t DS1307_read_minutes (void)	{
	uint8_t minuts = 0b01111111 & DS1307_read_memory (0x01);
	return bcdTOdec_conv(minuts);
}


uint8_t DS1307_read_seconds (void)	{
	uint8_t seconds = 0b01111111 & DS1307_read_memory (0x00);
	return bcdTOdec_conv(seconds);
}


uint8_t DS1307_read_day_name (void)	{
	/*  1 - Sunday
	 * 	2 - Monday
	 * 	3 - Tuesday
	 * 	4 - Wensday
	 * 	5 - Thursday
	 * 	6 - Friday
	 * 	7 - Sutarday
	 */
	uint8_t days_name = 0b00000111 & DS1307_read_memory (0x03);
	return days_name;
}


uint8_t DS1307_read_day (void)	{
	uint8_t day = 0b00111111 & DS1307_read_memory (0x04);
	return bcdTOdec_conv(day);
}

uint8_t DS1307_read_month (void)	{
	uint8_t month = 0b00011111 & DS1307_read_memory (0x05);
	return bcdTOdec_conv(month);
}


void DS1307_reset_control_reg (void)	{
	DS1307_write_memory(0x07, 0);
}


uint8_t DS1307_read_control_reg (void)	{
	uint8_t control_reg = DS1307_read_memory (0x07);
	return control_reg;
}


uint8_t DS1307_read_year (void)	{
	uint8_t year = DS1307_read_memory (0x06);
	return bcdTOdec_conv(year);
}


void DS1307_set_mode24h (void)	{

	uint8_t hours_reg_state = DS1307_read_memory (0x02);
	uint8_t hour = 0;
	//jesli juz jest tryb 24h to nic nie rob
	if (!(hours_reg_state & 0b01000000)) return;
	else	{

		hour = hours_reg_state & 0b00011111;
		hour = bcdTOdec_conv(hour);
		//jesli godzina jest PM to dodaj 12
		if(hours_reg_state & 0b00100000) {
			hour += 12;
			pm_flag = 1;
		}
		hour = decTObcd_conv(hour);
		DS1307_write_memory(0x02, (0b00111111 & hour));
	}
}


void DS1307_set_modeAM_PM (void)	{

	uint8_t hours_reg_state = DS1307_read_memory (0x02);
	//when am-pm mod
	if ((hours_reg_state & 0b01000000)) return;
	//when 24h mod
	else	{
		uint8_t hour = (hours_reg_state & 0b00111111) ;
		hour = bcdTOdec_conv(hour);
		//jesli godzina powyzej 12 to odejmij 12 i ustaw bit pm
		if (hour > 12) 	{
			hour = hour - 12;
			pm_flag = 1;
		}
		else pm_flag = 0;
		hour = decTObcd_conv(hour);
		//set pm
		if (pm_flag == 1) hour |= 0b00100000;
		//set mode 12h
		hour |= 0b01000000;
		DS1307_write_memory(0x02, hour);
	}
}


/*
uint8_t DS1307_read_all (void)	{
	uint8_t read_bufor=0;

		//odczyt aktualnej godziny,minut i sekund
		TWI_start();
		TWI_write(0b11010000);
		TWI_write(0);
		TWI_start();
		TWI_write(0b11010001);
		sekundy_temp = TWI_read(1);
		minuty_temp = TWI_read(1);
		---tu kolejne bajty
		godziny_temp = TWI_read(0);
		TWI_stop();
		}
*/


