/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        SSD1306.h                                                                                                                      */
/* Created:     21.04.2024                                                                                                                     */
/* Description: SSD1306 OLED Display Driver                                                                                                    */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef SSD1306_H_
#define SSD1306_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
#define SELECTED     1U
#define UNSELECTED   0U

#define I2C_128x32_ADDRESS 0x3CU
#define I2C_128x64_ADDRESS 0x3DU

/* Global setting - resolution */ 
#define SSD1306_128_32 SELECTED
#define SSD1306_128_64 UNSELECTED 
#define SSD1306_I2C_ADDRESS I2C_128x32_ADDRESS

#define SSD1306_DEFAULT_CONTRAST 0x49U

/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/ 

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void SSD1306_set_contrast(uint8_t contrast_value); 
void SSD1306_init(uint8_t refresh, uint8_t contrast_value);

void SSD1306_vertical_offset(void); // przesuniecie w pionie  0..31
void SSD1306_negativ_on(void);
void SSD1306_negativ_off(void);

void SSD1306_scrolling_on(void);
void SSD1306_scrolling_off(void);
void SSD1306_scroll_vertical(uint8_t horVert, uint8_t startPageAdr, uint8_t stopPageAdr,  uint8_t interwal, uint8_t offset);
void SSD1306_scroll_horizontal(uint8_t horVert, uint8_t startPageAdr, uint8_t stopPageAdr, uint8_t offset);

void SSD1306_display_u8_12x8(uint8_t valueuint, uint8_t x, uint8_t y);
void SSD1306_display_int8_12x8(uint8_t valueuint, uint8_t x, uint8_t y);
void SSD1306_display_one_digit_12x8(uint8_t value0to9, uint8_t x, uint8_t y);

void SSD1306_display_digit_18x26(uint8_t pos_x, uint8_t cyfra);

void SSD1306_display_graph(uint8_t *tab);
void SSD1306_set_pixel(int x, int y, uint8_t bw, uint8_t *buf);

void SSD1306_cls(void);
void SSD1306_clear_display_buffer(void);
void SSD1306_refresh_ram(void);

void SSD1306_init_for_temp(void);
void SSD1306_display_temperature(float temperature);
void SSD1306_display_humidity(float humidity);

#endif

