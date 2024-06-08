/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        disp7segLed.c                                                                                                                  */
/* Created:     18.04.2024                                                                                                                     */
/* Description: Interface for 7seg display module                                                                                              */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/ 
#include "disp7segLed.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 

/* HW init */
void DISP7SEGLED_init(uint8_t brightness)
{
    TM1637_init(brightness);
}

/* Turn On Led driver */
void DISP7SEGLED_on(void)
{
    TM1637_display_on();
}

/* Turn Off Led driver */
void DISP7SEGLED_off(void)
{
    TM1637_display_off();
}

/* Brightness setting. Min: 0, Max: 7 */
void DISP7SEGLED_set_brightness(uint8_t brightness)
{
    TM1637_set_brightness(brightness);
}

void DISP7SEGLED_display_dev_point(ProfilConfig_t* defaultProfil, uint8_t devPoint)
{
	if (SET == defaultProfil->config_LEDdisplayDevPointValue)
	{
		TM1637_display_u8_as_2_digits(devPoint);
	}
}

void DISP7SEGLED_display_alarm_temp(ProfilConfig_t* defaultProfil)
{
	if (SET == defaultProfil->config_LEDdisplayTempAlarmValue)
	{
		TM1637_display_u8_as_2_digits(defaultProfil->config_alarmTempValue);
	}
}

void DISP7SEGLED_display_gps_speed(GPSstate_t* gpsData)
{
	if (gpsData->satelites_number > 0U)
	{
		TM1637_display_u16((uint16_t)gpsData->speed_kilometers);
	}
	else
	{
		/* display: -- */
		TM1637_display_digit(1, 11);
		TM1637_display_digit(2, 11);
	}
}

void DISP7SEGLED_display_time(Time_t* time)
{
	TM1637_display_hours(time->hours);
	TM1637_set_dp_or_colon(2);
	TM1637_display_minutes(time->minutes);
}

void DISP7SEGLED_display_adc(uint16_t value)
{
	TM1637_display_u16(value);
}

void DISP7SEGLED_display_loHi_info(uint8_t humidityValue)
{
	if (1 == pProfilDefault->config_informLimitLoHighOverflow)
	{
		// info that humidity is too high or too low
		if (humidityValue > pProfilDefault->config_humidityToHighValue)
			{
				TM1637_display_digit(1, 14);   //display H
				TM1637_display_digit(2, 11);   //display i
				//beeper on
			}
		else if  (humidityValue < pProfilDefault->config_humidityToLowValue)
			{
				TM1637_display_digit(1, 15);   //display L
				TM1637_display_digit(2, 12);   //display o
			}
	}
}

void DISP7SEGLED_set_first_stripe_before_raad_from_sensor(void)
{
	if (1 == pProfilDefault->config_LEDdisplayStripesForDebug)
	{
		//for debug - 2 lines on TMdisp
#if (1 == USE_DEBUG_STRIPS)
		TM1637_display_digit(1, 11); //line
#endif
#if (1 == USE_DEBUG_DOTS)
		TM1637_setDPorClockDoublepoints(3); //dot
#endif
	}
}

void DISP7SEGLED_reset_first_stripe_after_raad_from_sensor(void)
{
	if (1 == pProfilDefault->config_LEDdisplayStripesForDebug)
	{
#if (1 == USE_DEBUG_STRIPS)
		TM1637_display_digit(1, 10); //nothing
#endif
#if (1 == USE_DEBUG_DOTS)
		TM1637_resetDPorClockDoublepoints(3); //dot off
#endif
	}
}

void DISP7SEGLED_set_second_stripe_after_raad_from_sensor(void)
{
	if (1 == pProfilDefault->config_LEDdisplayStripesForDebug)
	{
#if (1 == USE_DEBUG_STRIPS)
		TM1637_display_digit(2, 11);
		PERIPHERALS_delay_ms(100);
#endif
#if (1 == USE_DEBUG_DOTS)
		TM1637_set_dp_or_colon(4); //dot
		COMMON_delay_ms(100);
#endif
	}
}

void DISP7SEGLED_reset_second_stripe_after_raad_from_sensor(void)
{
	if (1 == pProfilDefault->config_LEDdisplayStripesForDebug)
	{
	#if (1 == USE_DEBUG_STRIPS)
			TM1637_display_digit(2, 10); //nothing
	#endif
	#if (1 == USE_DEBUG_DOTS)
		TM1637_reset_dp_or_colon(4); //dot off
	#endif
	}
}

void DISP7SEGLED_display_temp_value_for_alarm(void)
{
	#if(1 == USE_PROFIL_BADROOM)
		TM1637_display_int16(S_ProfilBadroom.config_alarmTempValue);
	#endif
}

void DISP7SEGLED_clear(void)
{
	TM1637_display_clear();
}

void DISP7SEGLED_display_digit(uint8_t position, uint8_t digit)
{
	TM1637_display_digit(position, digit);
}

void DISP7SEGLED_display_int16(int16_t value)
{
	TM1637_display_int16(value);
}
