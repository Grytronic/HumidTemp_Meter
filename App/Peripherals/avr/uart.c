/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        uart.c                                                                                                                         */
/* Created:     21.04.2024                                                                                                                     */
/* Description: UART driver                                                                                                                    */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "uart.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/
#if (1 == use_m16m32_init_settings)
	#define usartIOreg                   UDR
	#define transmit_buffer_not_empty    !(UCSRA&(1<<UDRE))
    #define transmit_buffer_empty        (UCSRA&(1<<UDRE))
	#define receiving_complete_flag		 (UCSRA & (1<<RXC))
#else
	#define usartIOreg                   UDR0
	#define transmit_buffer_not_empty    !(UCSR0A&(1<<UDRE0))
	#define transmit_buffer_empty        (UCSR0A&(1<<UDRE0))
	#define receiving_complete_flag		 (UCSR0A & (1<<RXC0))
#endif

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private variables --------------------------------------------------------------------------------------------------------------------------*/ 
/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void UART_init( uint16_t baudrate )  {
// Patrz--> Tabelka z bledami (BLAD < 2%)
	uint16_t ubrr = ((F_CPU+baudrate*8UL) / (16UL*baudrate)-1);

	//PORTD |= (1<<PD0);

	#if use_m16m32_init_settings
		//Set baud rate
		UBRRH = ( char )( ubrr >> 8 );
		UBRRL = ( char )ubrr;
		//Enable receiver and transmitter
		UCSRB = ( 1 << RXEN ) | ( 1 << TXEN );
		// Set frame format: 8data, 1stop bit
		UCSRC = (1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL);
	#else
		//Set baud rate
		UBRR0H = (char)(ubrr >> 8);
		UBRR0L = (char)ubrr;
		//Enable receiver and transmitter
		UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 );
		// Set frame format: 8data, 1stop bit
		UCSR0C = (1<<UCSZ00)|(1<<UCSZ01)|(1<<USBS0);
	#endif
		UCSR0B |= (1<<RXCIE0); // Rx INT enable
}


// wyslij znak char
void UART_send_char( char data ) {
		// Wait for empty transmit buffer
		while (transmit_buffer_not_empty);
		// Put data into buffer, sends the data
		usartIOreg = data;
}


// wyslij string
void UART_send_string ( char *s ) {
	while ( *s) UART_send_char (*s++ );
}


// wyslij string zapisany jako const
void UART_send_string_progmem ( const char *s ) {
	register char c;
    while ((c = pgm_read_byte(s++) )) UART_send_char( c );
}


// wyslij INT
void UART_send_int ( int val ) {
    char buf[9];
    itoa(val,buf, 10);
    UART_send_string( buf );
}



void UART_set_new_line (void)  {
	UART_send_char('\n');	//13
	UART_send_char('\r');	//10
}





/*
//wyslij 9 bitow

void UART_Transmit(unsigned int data)
{
	// Wait for empty transmit buffer
	while (!(UCSRnA & (1<<UDREn))));

	// Copy 9th bit to TXB8
	UCSRnB &= ~(1<<TXB8);
	if (data & 0x0100) UCSRnB |= (1<<TXB8);

	// Put data into buffer, sends the data
	UDRn = data;
}
*/


//odbierz 5-8 bitow
char UART_Receive_one_char(void)  {
	// Wait for data to be received
	while (!receiving_complete_flag);
	// Get and return received data from buffer
	return usartIOreg;
}


//	---receiving command from pc (code dec: 0-3), recceiving as char
void UART_receive_string (unsigned char *serial_received_data)	{
	if (receiving_complete_flag)	{
		for (uint8_t i=0; i<10; i++)	{
			serial_received_data[i] = UART_Receive_one_char();		//if uart bufor is not empty - receive char
			serial_received_data[i++] = '\0';
		}
	//received_command = atoi(serial_received_data); //command from pc as integer
	}
}
/*
void UART_receive_string (char *serial_received_data)	{
	if (((UCSRA & (1<<RXC))))	{

		static uint8_t i = 0;

			serial_received_data[i] = UART_Receive();		//if uart bufor is not empty - receive char

			if (serial_received_data[i] == '\0')	i=0;
			else i++;
	}
}
*/





/*
//odbierz 9 bitow
unsigned int USART_Receive( void )

{
	unsigned char status, resh, resl;

	// Wait for data to be received
	while (!(UCSRnA & (1<<RXCn)));

	// Get status and 9th bit, then data
	// from buffer
	status = UCSRnA;
	resh = UCSRnB;
	resl = UDRn;

	// If error, return -1
	if (status & (1<<FEn)|(1<<DORn)|(1<<UPEn))
	return -1;

	// Filter the 9th bit, then return
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | resl);
}
*/

/*
void UART_Flush(void)
{
	unsigned char dummy;
	while (UCSRnA & (1<<RXCn)) dummy = UDRn;
}
*/

//_____________________________________________________________________

/*




	void send_serial_string (char zmienna)
	{
		if(UCSR0A&(1<<UDRE0))
			{
				UDR0 = (int)zmienna;
			}
	}

	void send_serial_Uintiger16 (uint16_t zmienna)
	{
		if(UCSR0A&(1<<UDRE0))
			{
				//UDR0 = utoa(zmienna);

			}
	}



*/








#if VT100_command_for_teminal_use

//aby miec polskie ogonki w Putty to w opcjach; Windows-Translation - Win1250


static const char VT100_TERMINAL_cls[] PROGMEM = {"\x1b[2J"};


static const char VT100_TERMINAL_cursor_off[] PROGMEM = {"\x1b[?25l"};
static const char VT100_TERMINAL_cursor_on[] PROGMEM = {"\x1b[?25h"};
static const char VT100_TERMINAL_cursor_home[] PROGMEM = {"\x1b[;H"}; //ustawi kursor na pozycji 1,1

static const char VT100_TERMINAL_ATTR_off[] PROGMEM = {"\x1b[m"};

//print "\\033[s Save cursor position"
//print "\\033[u Restore cursor position"


void VT100_terminal_cursor_hide (uint8_t hide)	{
	if (hide) UART_send_string_progmem( VT100_TERMINAL_cursor_off );
	else UART_send_string_progmem( VT100_TERMINAL_cursor_on );
}


void VT100_terminal_cls (uint8_t cursor_off) {
	UART_send_string_progmem( VT100_TERMINAL_ATTR_off );
	VT100_terminal_cursor_hide (cursor_off);
	UART_send_string_progmem( VT100_TERMINAL_cls );
	VT100_terminal_locate(1,1);
}


void VT100_terminal_cursor_home_1_1( uint8_t cur_onoff ) 	{	//ustawia kursor na pozycji 1,1 na gorze
	VT100_terminal_cursor_hide (cur_onoff);
	UART_send_string_progmem( VT100_TERMINAL_cursor_home );
}


void VT100_terminal_locate (uint8_t y, uint8_t x) {
	UART_send_char ( 0x1b );
	UART_send_char ( '[' );
	//UART_send_int ( y,10 ); // 10 -oznacza ze w systemie dziesietnym
	UART_send_int ( y );
	UART_send_char ( ';' );
	//UART_send_int ( x,10 );
	UART_send_int ( x );
	UART_send_char ( 'f' );  //lub H
}



void VT100_terminal_background_colour (uint8_t colour) {
	UART_send_char ( 0x1b );
	UART_send_char ( '[' );
	UART_send_char ( '4' );
	UART_send_int ( colour);
	UART_send_char ( 'm' );
}


void VT100_terminal_font_colour_basic (uint8_t colour) {
	UART_send_char ( 0x1b );
	UART_send_char ( '[' );
	UART_send_char ( '3' );
	UART_send_int ( colour);
	UART_send_char ( 'm' );
}


void VT100_terminal_font_colour_brigt (uint8_t colour) {
	UART_send_char ( 0x1b );  //  \e  lub 33
	UART_send_char ( '[' );
	UART_send_char ( '9' );
	UART_send_int ( colour);
	UART_send_char ( 'm' );
}


void VT100_terminal_font_attr (uint8_t atr, uint8_t fg, uint8_t bg) {
	UART_send_char ( 0x1b );
	UART_send_char ( '[' );
	UART_send_int ( atr);
	UART_send_char ( ';' );
	UART_send_char ( '3' );
	UART_send_int ( fg);
	UART_send_char ( ';' );
	UART_send_char ( '4' );
	UART_send_int (bg);
	UART_send_char ( 'm' );
}


void VT100_terminal_print_line (char ascii, uint8_t cnt) {
	for(uint8_t i=0; i<cnt; i++)UART_send_char ( ascii );
}
#endif


/*
//+++++++++++++++++++++++++++++++++++++++++++++
//	---receiving command from pc (code dec: 0-3), recceiving as char
if (((UCSR0A & (1<<RXC0))))	{
	uint8_t temp = 1;
	uint8_t i = 0;
	while (temp ==1)
		{
		serial_received_data[i] = USART_Receive();		//if uart bufor is not empty - receive char
		if (((UCSR0A & (1<<RXC0)))) temp =1;
		else temp = 0;
		i++;
		}
	serial_received_data[i] = '\0';

	received_command = atoi(serial_received_data); //command from pc as integer
}

*/
