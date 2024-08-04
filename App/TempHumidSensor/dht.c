/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dht.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: DHT11 and DHT22/AM2302 temp and humidity sensor driver                                                                         */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "dht.h"
#include "../Peripherals/avr/dio.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define MAX_READ_TIM0_TIMEOUT  125

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static void DHT_response(void);
static void DHT11_request(void);
static uint8_t DHT_receive_data8(void);

static void DHT22_AM2302_request(void);
static uint16_t DHT_receive_data16(void);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void DHT_init(void)
{
	PERIPHERALS_delay_ms(2000);
}

static void DHT_response(void)		/* receive response from DHT11 */
{
	TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT);   // 20-40us
	TIM0_wait_for_high_level(MAX_READ_TIM0_TIMEOUT);  // min 80us
}

static void DHT11_request(void)			/* Microcontroller send start pulse/request */
{
	DIO_dht_setPIN_low();
	PERIPHERALS_delay_ms(20);		// min 18ms
	DIO_dht_setPIN_high();
	PERIPHERALS_delay_us(5);
}

static uint8_t DHT_receive_data8(void)			/* receive data */
{
	uint8_t dth_ret_value = 0;

	TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT);

	 /* dth starts to transmitt a bit, each 50us*/
	for (uint8_t i = 0; i < 8; i++)
	{
		TIM0_wait_for_high_level(MAX_READ_TIM0_TIMEOUT);
		TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT);

		if ( TIM0_time_calculated_us() < 40)
		{
			dth_ret_value = (dth_ret_value << 1);       		/* transmitted 0: 26 -28us is 0*/
		}
		else
		{
			dth_ret_value = (dth_ret_value << 1) | 0x01;	    /* transmitted 1: 70us */
		}
	}
	return dth_ret_value;
}

void DHT11_read_temp_and_humidity(sDTH* pSensor_data)
{
	uint8_t checksum = 0;

	DHT11_request();			/* send start pulse */
	DHT_response();				/* receive response */

	pSensor_data -> RH_integral   = DHT_receive_data8();	  /* store first eight bits in I_RH */
	pSensor_data -> RH_decimal    = DHT_receive_data8();	  /* store next eight bits in D_RH */
	pSensor_data -> Temp_integral = DHT_receive_data8();	  /* store next eight bits in I_Temp */
	pSensor_data -> Temp_decimal  = DHT_receive_data8();	  /* store next eight bits in D_Temp */
	checksum                      = DHT_receive_data8();     /* store last eight bits as checksum */

	uint16_t sum = (pSensor_data -> Temp_integral + pSensor_data -> Temp_decimal + pSensor_data -> RH_integral + pSensor_data -> RH_decimal);

	if ( (uint8_t)sum != checksum)
	{
		pSensor_data -> NewValueReceived = 0;
		PORTB ^= (1<<PB5);
		PERIPHERALS_delay_ms(50);
		PORTB ^= (1<<PB5);
	}
	else pSensor_data -> NewValueReceived = 1;
}

// -----------------------------------------------------------------------------------------------------------------------

static void DHT22_AM2302_request(void)			/* Microcontroller sends start pulse/request */
{
	DIO_dht_setPIN_low();   // Tbe 1-20ms
	PERIPHERALS_delay_ms(5);
	DIO_dht_setPIN_high();  // Tgo 20-200us
	PERIPHERALS_delay_us(5);
}

/*static void DHT_Response(void)
{
	TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT);
	TIM0_wait_for_high_level(MAX_READ_TIM0_TIMEOUT);
}
*/

static uint16_t DHT_receive_data16(void)			/* receive data */
{
	uint16_t dth_ret_value = 0;

	TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT);

	for (uint8_t i = 0; i < 16; i++)
	{
		TIM0_wait_for_high_level(MAX_READ_TIM0_TIMEOUT);
		TIM0_wait_for_low_level(MAX_READ_TIM0_TIMEOUT); 	 /* if high pulse is greater than 30ms */

		if ( TIM0_time_calculated_us() < 35)
		{
			dth_ret_value = (dth_ret_value << 1);       		/* transmitted 0: 22-28us */
		}
		else
		{
			dth_ret_value = (dth_ret_value << 1) | 0x01;	    /* transmitted 1: 68-75us */
		}
	}
	// wait on Tend: low 45-55us (not needed)
	return dth_ret_value;
}

void DHT22_AM2302_read_temp_and_humidity(sDTH* pSensor_data)
{
	uint16_t temp = 0;
	uint16_t humidity = 0;
	uint8_t checksum = 0;

	DHT22_AM2302_request();				 /* send start pulse */
	DHT_response();			 			 /* receive response */
	humidity = DHT_receive_data16();
	temp = DHT_receive_data16();
	checksum = DHT_receive_data8();     /* store next eight bits in CheckSum */

	pSensor_data -> RH_integral = humidity / 10; /* store first eight bits in I_RH */
	pSensor_data -> RH_decimal = humidity % 10; /* store next eight bits in D_RH */

	pSensor_data -> Temp_integral = temp / 10; /* store first eight bits in I_Temp */
	pSensor_data -> Temp_decimal  = temp % 10; /* store next eight bits in D_Temp */

	uint16_t sum = (humidity >> 8) + (uint8_t)humidity + (temp >> 8) + (uint8_t)temp;
	if ( (uint8_t)sum != checksum)
	{
		pSensor_data -> NewValueReceived = 0;  //ERROR
		/* Signalise error */
		PORTB ^= (1<<PB5);
		PERIPHERALS_delay_ms(50);
		PORTB ^= (1<<PB5);
	}
	else
	pSensor_data -> NewValueReceived = 1;

	// recognition of negative temperature
	if ((pSensor_data -> Temp_integral) & 0x1000000)
	{
		pSensor_data -> Temp_sign = 1;
		(pSensor_data -> Temp_integral) &= 0x01111111;
	}
}




