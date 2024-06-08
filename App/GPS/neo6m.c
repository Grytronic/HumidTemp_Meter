/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        neo6m.c                                                                                                                        */
/* Created:     21.04.2024                                                                                                                     */
/* Description: NEO6M GPS Driver                                                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "neo6m.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
GPSstate_t S_GPSstate;

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void NEO6M_init(void)
{
	//state.uart = uart;
	for(uint8_t i=0; i<100; i++) S_GPSstate.line_buffer[i] = '\0';
	S_GPSstate.writer_position = 0;
	S_GPSstate.reader_position = 0;
	for(uint8_t i=0; i<30; i++) S_GPSstate.field_buffer[i] = '\0';
	S_GPSstate.field_position = 0;

	S_GPSstate.date_day = 0;
	S_GPSstate.date_mounth = 0;
	S_GPSstate.date_year = 0;
	S_GPSstate.time_hour = 0;
	S_GPSstate.time_min = 0;
	S_GPSstate.time_sec = 0;

	S_GPSstate.latitude = 0.0;
	S_GPSstate.latitude_direction = '?';
	S_GPSstate.longitude = 0.0;
	S_GPSstate.longitude_direction = '?';
	S_GPSstate.altitude = 0.0;

	S_GPSstate.speed_knots = 0.0;
	S_GPSstate.speed_kilometers = 0.0;

	S_GPSstate.satelites_number = 0;
	S_GPSstate.quality = '?';
	S_GPSstate.dop = 0.0;
	S_GPSstate.hdop = 0.0;
	S_GPSstate.vdop = 0.0;
}

void NEO6M_recv_char(uint8_t recv_char) {
	if (S_GPSstate.writer_position == 0 && recv_char == '$') {
		S_GPSstate.writer_position++;
	} else if (S_GPSstate.writer_position >= 1 && S_GPSstate.writer_position < 99) {
		if (recv_char == '\r' || recv_char == '\n') {
			S_GPSstate.line_buffer[S_GPSstate.writer_position - 1] = '\0';
			S_GPSstate.writer_position = 0;
			NEO6M_process_line();
		} else {
			S_GPSstate.line_buffer[S_GPSstate.writer_position - 1] = recv_char;
			S_GPSstate.writer_position++;
		}
	} else {
		S_GPSstate.writer_position = 0;
	}
}

void NEO6M_read_field(void) {
	S_GPSstate.field_position = 0;
	while(S_GPSstate.line_buffer[S_GPSstate.reader_position] != ',' && S_GPSstate.line_buffer[S_GPSstate.reader_position] != '\0'
			&& S_GPSstate.field_position < 29) {
		S_GPSstate.field_buffer[S_GPSstate.field_position] = S_GPSstate.line_buffer[S_GPSstate.reader_position];
		S_GPSstate.reader_position++;
		S_GPSstate.field_position++;
	}
	S_GPSstate.field_buffer[S_GPSstate.field_position] = '\0';
	S_GPSstate.reader_position++;
}

void NEO6M_process_line(void) {
	S_GPSstate.reader_position = 0;
	NEO6M_read_field();


#if (1 ==  USE_FAKE_GPS)
	if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GNZDA") == 0) NEO6M_process_gprmc();
	else if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GPGSV") == 0) NEO6M_process_gpgsa();
	else if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GNRMC") == 0) NEO6M_process_gpgga();
#else if
	if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GPRMC") == 0) NEO6M_process_gprmc();
	else if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GPVTG") == 0) NEO6M_process_gpvtg();
	else if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GPGSA") == 0) NEO6M_process_gpgsa();
	else if(strcmp((uint8_t*)&(S_GPSstate.field_buffer), "GPGGA") == 0) NEO6M_process_gpgga();
#endif
}

void NEO6M_process_gprmc(void)
{
	//$GPRMC,212846.00,A,5025.81511,N,01639.92090,E,0.196,,140417,,,A*73



#if (1 ==  USE_FAKE_GPS)

	NEO6M_read_field();

	if(strlen((uint8_t*)&(S_GPSstate.field_buffer)) > 0) {


		int8_t tmp = 0;
		uint8_t temp_buf[3] = {'\0', '\0', '\0'};

		//hours
		memcpy((uint8_t*)&temp_buf[0], (uint8_t*)&(S_GPSstate.field_buffer)+0, 2 );
		tmp = atoi(temp_buf);
		S_GPSstate.time_hour = tmp + 1;

		//minutes
		memcpy((uint8_t*)&temp_buf[0], (uint8_t*)&(S_GPSstate.field_buffer)+2, 2 );
		tmp = atoi(temp_buf);
		S_GPSstate.time_min = tmp;

#if(0)
		//seconds
		memcpy((uint8_t*)&temp_buf[0], (uint8_t*)(S_GPSstate.field_buffer)+4, 2 );
		tmp = atoi(temp_buf);
		S_GPSstate.time_sec = tmp;
#endif


#if(0)
		UART_send_int(tmp);
		UART_send_string(" ");
		UART_send_string(S_GPSstate.field_buffer);
		UART_set_new_line();
#endif

	}
#else
	NEO6M_read_field();
	if(strlen(state->field_buffer) > 0) {
		uint32_t tmp;
		sscanf(state->field_buffer, "%d", &tmp);
		S_GPSstate.time_sec = tmp % 100;
		S_GPSstate.time_min = (tmp / 100) % 100;
		S_GPSstate.time_hour = (tmp / 10000) % 100;
	}

	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen(S_GPSstate.field_buffer) > 0) sscanf(S_GPSstate.field_buffer, "%lf", &(S_GPSstate.latitude));

	NEO6M_read_field(state);
	if(strlen(S_GPSstate.field_buffer) > 0) sscanf(S_GPSstate.field_buffer, "%c", &(S_GPSstate.latitude_direction));

	NEO6M_read_field(state);
	if(strlen(S_GPSstate.field_buffer) > 0) sscanf(S_GPSstate.field_buffer, "%lf", &(S_GPSstate.longitude));

	NEO6M_read_field(state);
	if(strlen(S_GPSstate.field_buffer) > 0) sscanf(S_GPSstate.field_buffer, "%c", &(S_GPSstate.longitude_direction));

	NEO6M_read_field();
	NEO6M_read_field();

	NEO6M_read_field(state);
	if(strlen(S_GPSstate.field_buffer) > 0) {
		uint32_t tmp;
		sscanf(S_GPSstate.field_buffer, "%d", &tmp);
		S_GPSstate.date_year = tmp % 100;
		S_GPSstate.date_mounth = (tmp / 100) % 100;
		S_GPSstate.date_day = (tmp / 10000) % 100;
	}

#endif
}

void NEO6M_process_gpvtg(void)
{
	//$GPVTG,,T,,M,0.196,N,0.363,K,A*2B
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen((uint8_t*)&(S_GPSstate.field_buffer)) > 0) sscanf((uint8_t*)&(S_GPSstate.field_buffer), "%lf", &(S_GPSstate.speed_knots));

	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen((uint8_t*)&(S_GPSstate.field_buffer)) > 0) sscanf((uint8_t*)&(S_GPSstate.field_buffer), "%lf", &(S_GPSstate.speed_kilometers));
}

void NEO6M_process_gpgga(void)
{
	float speed_knot = 0;
	uint8_t temp_integral[10] = {0};
	uint8_t temp_decimal[10] = {0};

#if (1 ==  USE_FAKE_GPS)

	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();

	if(strlen((uint8_t*)&(S_GPSstate.field_buffer)) > 0) {

		//calosci
		memcpy((uint8_t*)&temp_integral[0], (uint8_t*)&S_GPSstate.field_buffer[0], strlen(S_GPSstate.field_buffer)-4);
		speed_knot = atof((uint8_t*)&temp_integral[0]);
		//dziesietne
		uint8_t size = strlen((uint8_t*)&S_GPSstate.field_buffer) - 3u;
		memcpy((uint8_t*)&temp_decimal[0], (uint8_t*)&S_GPSstate.field_buffer[0] + size, 3);
		speed_knot = speed_knot + (atof((uint8_t*)&temp_decimal[0])) *0.001;

		S_GPSstate.speed_knots = speed_knot;
		S_GPSstate.speed_kilometers = S_GPSstate.speed_knots*1.852;

#if (0)
		UART_send_int(speed_knot);
		UART_send_string(" ");
		UART_send_string(test_str);
		UART_send_string(state->field_buffer);
		UART_set_new_line();
#endif

	}
	else S_GPSstate.speed_knots = 0;

#else

	//$GPGGA,212846.00,5025.81511,N,01639.92090,E,1,04,4.72,281.1,M,42.0,M,,*5F
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen((uint8_t*)&S_GPSstate.field_buffer)) > 0) sscanf((uint8_t*)&S_GPSstate.field_buffer), "%d", &(state->quality));

	NEO6M_read_field();
	if(strlen((uint8_t*)&S_GPSstate.field_buffer)) > 0) sscanf((uint8_t*)&S_GPSstate.field_buffer), "%d", &(state->satelites_number));

	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen((uint8_t*)&S_GPSstate.field_buffer)) > 0) sscanf((uint8_t*)&S_GPSstate.field_buffer), "%lf", &(state->altitude));
#endif
}

void NEO6M_process_gpgsa(void)
{

#if (1 ==  USE_FAKE_GPS)
	NEO6M_read_field();
	NEO6M_read_field();

	NEO6M_read_field();
	if(strlen((uint8_t*)&S_GPSstate.field_buffer) > 0) {

		int8_t tmp = 0;
		uint8_t temp_buf[3] = {'\0', '\0', '\0'};

		//satelite number
		memcpy((uint8_t*)&temp_buf[0], (uint8_t*)&S_GPSstate.field_buffer + 0, 2 );
		tmp = atoi(temp_buf);
		S_GPSstate.satelites_number = tmp;
#if(0) //debug
		UART_send_int(tmp);
		UART_send_string(" ");
		UART_send_string(S_GPSstate.field_buffer);
		UART_set_new_line();
#endif
	}
	else S_GPSstate.satelites_number = 0;

#else
	10,18,21,15,,,,,,,,,6.79,4.72,4.89*01
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	NEO6M_read_field();
	if(strlen(state->field_buffer) > 0) sscanf(state->field_buffer, "%lf", &(state->dop));

	NEO6M_read_field();
	if(strlen(state->field_buffer) > 0) sscanf(state->field_buffer, "%lf", &(state->hdop));

	NEO6M_read_field();
	if(strlen(state->field_buffer) > 0) sscanf(state->field_buffer, "%lf", &(state->vdop));
#endif

}
