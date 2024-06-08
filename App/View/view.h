/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        view.h                                                                                                                         */
/* Created:     18.04.2024                                                                                                                     */
/* Description: View                                                                                                                           */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef VIEW_H_
#define VIEW_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/

#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/
 
/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
/* values stored in global structures of component interface variable */
void VIEW_display_devPoint(ProfilConfig_t* defaultProfil, uint8_t devPoint);
void VIEW_display_alarmTemp(ProfilConfig_t* defaultProfil);
//void VIEW_display_gpsSpeed(S_GPSstate* gpsData);
void VIEW_display_adc(uint16_t value);
void VIEW_display_time(Time_t* time);
//void VIEW_set_oled_contrast(S_ProfilDefault->config_OLEDbrightness);
void VIEW_display_lotto_numbers(void);

#endif /* VIEW_H_ */
