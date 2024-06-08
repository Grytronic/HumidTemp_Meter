/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        i2c.h                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: I2C driver                                                                                                                     */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef I2C_H_
#define I2C_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/
#include "peripherals.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/ 
/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
/* External consts ----------------------------------------------------------------------------------------------------------------------------*/ 
/* External variables -------------------------------------------------------------------------------------------------------------------------*/ 
/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void TWI_set_bitrate(uint16_t bitrateKHz);  //100 or 400

uint8_t TWI_read_byte(uint8_t Slave_Addr, uint8_t reg_adr);
void TWI_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data);
//void TWI_write_buf(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t *buf);
void TWI_read_buf(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t len, uint8_t *buf) ;

extern uint8_t i2c_addr_dec [];  //wartosci adresow zapisane w formacie dziesietnym
uint8_t TWI_read_device_address (void);  /* Funkcja ta zwraca liczbe znalezionych na magistrali urzadzen
i wpisuje adresy do tablicy extern uint8_t i2c_addr_dec[];  */

//	---funkcja umieszcza pojedynczy wynik konwersji
//  w stringu char dec_to_hex_str [] = {'0', 'x', '-','-'};
extern char dec_to_hex_str [];
void dec_to_asci_conv (uint8_t dec);


#endif /* I2C_H_ */
