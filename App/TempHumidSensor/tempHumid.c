/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        tempHumid.c                                                                                                                    */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Temperature and humidity sensor driver interface                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "tempHumid.h"
#include "dht.h"
 
/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
void (*temp_measure)(sDTH* pSensor_data);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void TEMPHUMID_register_temp_measure_callback(void(*func)(sDTH*))
{
	temp_measure = func;
}

void TEMPHUMID_init(void)
{
        /* register temp/humid sensor depends of configuration */
    #if (1 == USE_DTH11)
    #if (1 == DEBUG_MODE_ACTIVE)
            //TEMPHUMID_register_temp_measure_callback(debug_DHT11_readTempandHumid);
    #else
            TEMPHUMID_register_temp_measure_callback(DHT11_read_temp_and_humidity);
    #endif
    #endif

    #if (1 == USE_DTH22_AM2302)
    #if (1 == DEBUG_MODE_ACTIVE)
            //TEMPHUMID_register_temp_measure_callback(debug_DHT11_readTempandHumid);
    #else
            TEMPHUMID_register_temp_measure_callback(DHT22_AM2302_readTempandHumid);
    #endif
    #endif
}

float TEMPHUMID_calculate_dewPoint_value(float temperature, float humidity)
{
    float Td = (pow(humidity/100, 0.125)) * (float)(112 + (0.9*temperature)) + (float)(0.1*temperature) - 112;
    
    return Td;
}

void TEMPHUMIDSENSOR_get_measurements(sDTH* pSensorData)
{
	temp_measure(pSensorData);
}
