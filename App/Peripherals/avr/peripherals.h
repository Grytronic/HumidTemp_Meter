/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        peripherals.h                                                                                                                  */
/* Created:     04.06.2024                                                                                                                     */
/* Description: Peripherals interface                                                                                                          */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "../../Main/main.h"
#include "m328p.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void PERIPHERALS_init(void);
void PERIPHERALS_enable_global_interrupts(void);
int PERIPHERALS_get_generated_random_value(void);
void PERIPHERALS_update_buzer_alarm_notification(void);
void PERIPHERALS_check_any_button_presed(void);
void PERIPHERALS_delay_us(uint16_t delay);
void PERIPHERALS_delay_ms(uint16_t delay);

#endif /* PERIPHERALS_H_ */



