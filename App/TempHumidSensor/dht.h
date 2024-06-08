/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dht.h                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DHT11 and DHT22/AM2302 temp and humidity sensor driver                                                                         */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef DHT_H_
#define DHT_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/

/* External variables -------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	uint8_t RH_integral;	 /* store first eight bit in I_RH */
	uint8_t RH_decimal;	 	 /* store next eight bit in D_RH */
	uint8_t Temp_sign;
	uint8_t Temp_integral;	 /* store next eight bit in I_Temp */
	uint8_t Temp_decimal;	 /* store next eight bit in D_Temp */
	uint8_t NewValueReceived;
} sDTH;

//typedef struct DTH11 sDTH11;

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void DHT_init(void);
void DHT11_read_temp_and_humidity(sDTH* dth_sensor_data);
void DHT22_AM2302_read_temp_and_humidity(sDTH* dth_sensor_data);

#endif /* DHT_H_ */
