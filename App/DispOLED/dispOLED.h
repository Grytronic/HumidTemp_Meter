/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        dispOLED.h                                                                                                                     */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Allow to use one interface for OLED displays based on different HW drivers                                                     */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef DISPLOLED_H_
#define DISPLOLED_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "ssd1306.h"
#include "../Lotto/lotto.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/
#define SSD1306_MAX_CONTRAST 0xFFU
#define SSD1306_REFRESH_RATIO_MIN 0x80U
#define SSD1306_REFRESH_RATIO_MID 0xB0U
#define SSD1306_REFRESH_RATIO_MAX 0xF0U

/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
typedef float float32_t;

/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/
void DISPOLED_HW_init(void);
void DISPOLED_init_for_temp(void);
void DISPOLED_clear(void);
void DISPOLED_set_contrast(uint8_t value);
void DISPOLED_display_data_from_tempHumid_sensor(float32_t humidity, float32_t temperature);
void DISPOLED_display_lotto_numbers(Lotto_t* pNumbers);

void DISPOLED_display_digit_18x26(uint8_t pos_x, uint8_t value);

#endif /* DISPLOLED_H_ */
