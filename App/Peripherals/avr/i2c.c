/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        i2c.c                                                                                                                          */
/* Created:     21.04.2024                                                                                                                     */
/* Description: I2C driver                                                                                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "i2c.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#define ACK 1
#define NACK 0
 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
uint8_t i2c_addr_dec [10]; //liczba urzadzen podpietych do magistrali
char dec_to_hex_str [] = {'0', 'x', '-','-', '\0'};

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
//	---funkcja umieszcza pojedynczy wynik konwersji w stringu char dec_to_hex_str [];
void dec_to_asci_conv (uint8_t dec)	{
	uint8_t temp = 0;

	temp= dec>>4;
	if (temp>9) temp += 55;
	else temp += '0';
	dec_to_hex_str [2] = temp;

	temp= dec & 0b00001111;
	if (temp>9) temp += 55;
	else temp += '0';
	dec_to_hex_str [3] = temp;
}


void TWI_set_bitrate(uint16_t bitrateKHz) {
	uint8_t bitrate_div;

	bitrate_div = ((F_CPU/1000l)/bitrateKHz);
	if (bitrate_div >= 16) bitrate_div = (bitrate_div-16)/2;

	TWBR = bitrate_div;
}


static void TWI_start(void) {
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
		while (!(TWCR&(1<<TWINT)));
	}
}


static void TWI_stop(void) {
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
		while (!(TWCR&(1<<TWSTO)));
		//while (TWCR& (1<<TWSTO));
	}
}


static void TWI_write(uint8_t bajt) {
	{
		TWDR = bajt;
		TWCR = (1<<TWINT)|(1<<TWEN);
		while (!(TWCR&(1<<TWINT)));
	}
}


static uint8_t TWI_read(uint8_t ack) {
	{
		TWCR = (1<<TWINT)|(ack<<TWEA)|(1<<TWEN);
		while (!(TWCR & (1<<TWINT)));
	}
	return TWDR;
}


/*
void TWI_write_buf(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t *buf) {
	uint8_t len = sizeof buf;
	Slave_Addr<<=1;
	TWI_start();
	TWI_write(Slave_Addr);
	TWI_write(reg_adr);
	while (len--) TWI_write(*buf++);
	TWI_stop();
}
*/


void TWI_write_byte(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t data) {
	Slave_Addr<<=1;
	TWI_start();
	TWI_write(Slave_Addr);
	TWI_write(reg_adr);
	TWI_write(data);
	TWI_stop();
}

void TWI_read_buf(uint8_t Slave_Addr, uint8_t reg_adr, uint8_t len, uint8_t *buf) {
	Slave_Addr<<=1;
	TWI_start();
	TWI_write(Slave_Addr);
	TWI_write(reg_adr);
	TWI_start();
	TWI_write(Slave_Addr + 1);
	while (len--) *buf++ = TWI_read( len ? ACK : NACK );
	TWI_stop();
}

uint8_t TWI_read_byte(uint8_t Slave_Addr, uint8_t reg_adr) {
	uint8_t data;
	Slave_Addr<<=1;
	TWI_start();
	TWI_write(Slave_Addr);
	TWI_write(reg_adr);
	TWI_start();
	TWI_write(Slave_Addr +1);
	data =TWI_read(NACK);
	TWI_stop();
	return data;
}

uint8_t TWI_read_device_address (void)	{
	uint8_t addr_count = 0;

	for (uint8_t addr=0; addr<128; addr++)	{
		TWI_start();
		TWI_write(addr<<1); //slave addr+0 - write

		//when slave give ACK
		if (TWSR == 24)	{
			i2c_addr_dec[addr_count]= addr>>1;
			addr_count++;
		}
		TWI_stop();
	}
	return addr_count; //zmienna informujaca o ilosci adresow na magistrali
}
