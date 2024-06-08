
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        adc.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: ADC driver                                                                                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "adc.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void ADC_init (void) 
{
		// ---KONFIGURACJA ADC:
		ACSR |= (1 << ACD); 									// turning off the Analog Comparator 	(for low current consumption)
		//PORTC |= (1<<PC1);  //pullup ADC1
		DIDR0 |= (1 << ADC1D); 								    // digital Input disable 			    (for low current consumption)

		//brak bitow to Vref z wejscia
		//ADMUX|=(1 << REFS1) | (1 << REFS0); 					// 1,1V Vref
		ADMUX |= (1 << REFS0); 								    // AVCC as Vref - napieciem odniesienia jest napiecie zasilania AVCC
		//ADMUX with no bits --ADC0 input(default)
		//ADMUX |= (1 << MUX2) | (1<<MUX1); 							  // ADC1 input
		//PORTC |= (1 << PC1);
		//ADMUX |= (1 << MUX3); 								          // TemperatureSensor input
		//ADMUX |= (1 << MUX3) | (1<<MUX2) | (1 << MUX1); 			      // Vref input
		//ADMUX |= (1 << MUX3) | (1<<MUX2) | (1 << MUX1) | (1 << MUX0);   // GND as input
		//ADMUX |= (1 << ADLAR); 								          // odwrocenie H i L - dla fokladnosic 8bitowej, odczyt tylko z ADCH

		ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 			  // preskaler na 128  max is 200kHz
		ADCSRA |= (1 << ADEN); 									          // ADC enable

		PORTC |= ( 1<< PC0); //ADC0 pull-up
}

uint16_t ADC_get_measurement(void) 
{
		ADCSRA |= (1 << ADSC);								// ADSC - uruchomienie pojedynczej konwersji
		while(ADCSRA & (1 << ADSC)); 					    // czeka na zakonczenie konwersji
		//adc_value= ADCL|(ADCH << 8);
		//adc_value=(uint64_t)((ADCH << 8)| ADCL);
		//send_serial_Uinteger32((ADCH << 8)| ADCL);
		return ADCW;
}

float ADC_calculate_voltage_value(uint16_t adcRAWvalue)
{
		return (((adcRAWvalue + 1U) *5U) /1024.0);
}

uint8_t ADC_calculate_percentage_from_RAW(uint32_t adcRAWvalue)
{
		return ((adcRAWvalue * 100) /1023.0); 	//range 0-100%
}




