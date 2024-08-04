/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        config.h                                                                                                                     */
/* Created:     04.08.2024                                                                                                                     */
/* Description: Global project configuration                                                                                   */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef APP_MAIN_CONFIG_CONFIG_H_
#define APP_MAIN_CONFIG_CONFIG_H_

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
#define ACTIVE       1U
#define NOT_ACTIVE   0U
#define ZERO_U       0U

/* Choose an active HW platform for HW config*/
#define USE_HW_PLATFORM_AVR    ACTIVE
#define USE_HW_PLATFORM_STM32  NOT_ACTIVE

#define USE_DEBUG_STRIPS       ACTIVE
#define USE_DEBUG_DOTS         NOT_ACTIVE

#define DEBUG_MODE_ACTIVE      NOT_ACTIVE

/* choose used temp/humid sensor */
#define USE_DTH11		       ACTIVE
#define USE_DTH22_AM2302	   NOT_ACTIVE
#define USE_SHT20       	   NOT_ACTIVE
#define USE_INTERNAL_PULLUP_FOR_DHT_SENSOR  ACTIVE

/* choose additional modules     */
#define USE_RTC                NOT_ACTIVE
#define USE_GPS                NOT_ACTIVE
#define USE_FAKE_GPS           ACTIVE

/* choose default working profil */
#define USE_PROFIL_HOME		   NOT_ACTIVE
#define USE_PROFIL_CAR		   NOT_ACTIVE
#define USE_PROFIL_BADROOM     ACTIVE

/* some calibration parameters   */
#define COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION   NOT_ACTIVE

#define CAR_TEMP_OFFSET            NOT_ACTIVE
#define HOME_TEMP_OFFSET           NOT_ACTIVE
#define BADROOM_TEMP_OFFSET        NOT_ACTIVE
#define CAR_HUMIDITY_OFFSET        NOT_ACTIVE
#define HOME_HUMIDITY_OFFSET       NOT_ACTIVE
#define BADROOM_HUMIDITY_OFFSET    NOT_ACTIVE


#endif /* APP_MAIN_CONFIG_CONFIG_H_ */
