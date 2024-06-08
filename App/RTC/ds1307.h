/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        ds1307.h                                                                                                                     */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DS1307 RTC driver                                                                                                              */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef DS1307_H_
#define DS1307_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/

/* External variables -------------------------------------------------------------------------------------------------------------------------*/
extern uint8_t pm_flag;
extern uint8_t mode_24_on; 

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void DS1307_init(void);
void DS1307_set_control_register_1hz (void);
void DS1307_write_memory (uint8_t addr, uint8_t TM_register);
uint8_t DS1307_read_memory (uint8_t addr);

void DS1307_write_24h_hour (uint8_t hour);
void DS1307_write_am_pm_hour (uint8_t hour, uint8_t pm);
void DS1307_write_minutes (uint8_t minutes);
void DS1307_write_seconds (uint8_t seconds);

void DS1307_write_day_name (uint8_t day);
void DS1307_write_day (uint8_t day);
void DS1307_write_month (uint8_t month);
void DS1307_write_year (uint8_t year);

void DS1307_set_mode24h (void);
void DS1307_set_modeAM_PM (void);

uint8_t DS1307_read_hour (void);
uint8_t DS1307_read_minutes (void);
uint8_t DS1307_read_seconds (void);

uint8_t DS1307_read_day_name (void);
uint8_t DS1307_read_day (void);
uint8_t DS1307_read_month (void);
uint8_t DS1307_read_year (void);

void DS1307_reset_control_reg (void);

//clok halt after power off
// 0-oscillator enABLED; 1 - oscillator disabled
void clock_halt_on (void);
void clock_halt_off (void);

uint8_t DS1307_read_control_reg (void);
//uint8_t DS1307_read_all (void);


#endif /* DS1307_H_ */
