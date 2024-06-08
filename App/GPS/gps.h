/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        gps.h                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: GPS modul interface                                                                                                            */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef GPS_H_
#define GPS_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/

/* External types -----------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	uint8_t line_buffer[100];
	uint8_t writer_position;
	uint8_t reader_position;
	uint8_t field_buffer[30];
	uint8_t field_position;

	uint8_t date_day;
	uint8_t date_mounth;
	uint8_t date_year;
	uint8_t time_hour;
	uint8_t time_min;
	uint8_t time_sec;

	double latitude;
	char latitude_direction;
	double longitude;
	char longitude_direction;
	double altitude;

	double speed_knots;
	double speed_kilometers;

	uint8_t satelites_number;
	uint8_t quality;
	double dop;
	double hdop;
	double vdop;
} GPSstate_t;

/* External consts ----------------------------------------------------------------------------------------------------------------------------*/
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void GPS_init(void);
void GPS_recv_char(uint8_t recv_char);

#endif /* GPS_H_ */
