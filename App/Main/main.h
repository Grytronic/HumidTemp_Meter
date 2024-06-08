/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        main.h                                                                                                                         */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Main entry point                                                                                                               */
/* Comments:    Car/home/badroom thermo-higro meter.                                                                                           */
/*                                                                                                                                             */
/*              Monitoring of:                                                                                                                 */
/*              - humidity and speed in a car                                                                                                  */
/*				- humidity in a bathroom with alarm of opened window                                                                           */
/*              Measurements and calculations:                                                                                                 */
/*                                                                                                                                             */
/*              - temp                                                                                                                         */
/*              - humidity																												       */
/*              - dev point																													   */
/*              - speed in km/h and time from gps                                                                                              */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef __MAIN_H_
#define __MAIN_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/ 
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Peripherals/avr/peripherals.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
/* ---------------------------- */
/* --- GLOBAL CONFIGURATION --- */
/* ---------------------------- */

/* Choose an active HW platform for HW config*/
#define USE_HW_PLATFORM_AVR    1U
#define USE_HW_PLATFORM_STM32  0U

#define USE_DEBUG_STRIPS      1U
#define USE_DEBUG_DOTS        0U

#define DEBUG_MODE_ACTIVE     0U

/* choose used temp/humid sensor */
#define USE_DTH11		      1U
#define USE_DTH22_AM2302	  0U
#define USE_SHT20       	  0U

/* choose additional modules     */
#define USE_RTC               0U
#define USE_GPS               0U
#define USE_FAKE_GPS          1U

/* choose default working profil */
#define USE_PROFIL_HOME		  0U
#define USE_PROFIL_CAR		  0U
#define USE_PROFIL_BADROOM    1U

/* some calibration parameters   */
#define COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION 0U

#define CAR_TEMP_OFFSET            0U
#define HOME_TEMP_OFFSET           0U
#define BADROOM_TEMP_OFFSET        0U
#define CAR_HUMIDITY_OFFSET        0U
#define HOME_HUMIDITY_OFFSET       0U
#define BADROOM_HUMIDITY_OFFSET    0U

#define FLAG_IS_SET   1U
#define FLAG_IS_RESET 0U
#define SET   1U
#define RESET 0U
#define ZERO  0U

/* External types -----------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	/* set temp and humidity offset */
	uint8_t config_humidityOffset;
	uint8_t config_tempOffset;
	uint8_t config_countOftempMeanValSamples;
	uint8_t config_LEDdispBrightness;
	uint8_t config_OLEDbrightness;
	uint8_t config_informLimitLoHighOverflow;
	uint8_t config_humidityToLowValue;
	uint8_t config_humidityToHighValue;
	uint8_t config_alarmBeeperON;
	uint8_t config_alarmTempValue;
	uint8_t config_alarmHumidValue;
	float config_alarmDevTempDifference;
	uint8_t config_LEDdisplayTempAlarmValue;
	uint8_t config_LEDdisplayDevPointValue;
	uint8_t config_LEDdisplayGPSdata;
	uint8_t config_LEDdisplayTime;
	uint8_t config_LEDdisplayStripesForDebug;
	uint8_t config_LEDdisplayADCvalue;
	void (*Func_Button_1_pressed)(void);
}  ProfilConfig_t;

typedef struct {
	ProfilConfig_t* pProfilCar;
	ProfilConfig_t* pProfilHome;
	ProfilConfig_t* pProfilBadroom;
} ProfilCollection_t;

typedef enum {
	home,
	car,
	badroom
} mode_t;

/* External consts ----------------------------------------------------------------------------------------------------------------------------*/

/* External variables -------------------------------------------------------------------------------------------------------------------------*/
extern ProfilConfig_t* pProfilDefault;
extern ProfilCollection_t SProfilCollection;

extern ProfilConfig_t S_ProfilCar;
extern ProfilConfig_t S_ProfilHome;
extern ProfilConfig_t S_ProfilBadroom;


/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 



#endif /* __MAIN_H_ */
