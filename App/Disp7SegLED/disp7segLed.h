/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        disp7segLed.h                                                                                                                  */
/* Created:     18.04.2024                                                                                                                     */
/* Description: Interface for 7seg display module                                                                                              */
/* Comments:    Allow to use one interface for LED displays based on different HW drivers                                                      */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef DISP7SEGLED_H_
#define DISP7SEGLED_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "tm1637.h"
#include "../RTC/rtc.h"
#include "../GPS/gps.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/ 

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
/* HW init */
void DISP7SEGLED_init(uint8_t brightness);
void DISP7SEGLED_on(void);
void DISP7SEGLED_off(void);
void DISP7SEGLED_set_brightness(uint8_t brightness); //0-7
void DISP7SEGLED_clear(void);

void DISP7SEGLED_display_loHi_info(uint8_t humidityValue);
void DISP7SEGLED_display_dev_point(ProfilConfig_t* defaultProfil, uint8_t devPoint);
void DISP7SEGLED_display_alarm_temp(ProfilConfig_t* defaultProfil);
void DISP7SEGLED_display_time(Time_t* time);
void DISP7SEGLED_display_adc(uint16_t value);
void DISP7SEGLED_display_digit(uint8_t position, uint8_t digit);
void DISP7SEGLED_display_int16(int16_t value);

void DISP7SEGLED_set_first_stripe_before_raad_from_sensor(void);
void DISP7SEGLED_reset_first_stripe_after_raad_from_sensor(void);
void DISP7SEGLED_set_second_stripe_after_raad_from_sensor(void);
void DISP7SEGLED_reset_second_stripe_after_raad_from_sensor(void);

void DISP7SEGLED_display_gps_speed(GPSstate_t* gpsData);

#endif /* DISP7SEGLED_H_ */
