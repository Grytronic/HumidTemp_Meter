/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        ssd1306.c                                                                                                                      */
/* Created:     21.04.2024                                                                                                                     */
/* Description: SSD1306 OLED Display Driver                                                                                                    */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "ssd1306.h" 
#include "characters.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 

#define SSD1306_WIDTH 128U

#if (SELECTED == SSD1306_128_32)
	#define SSD1306_HEIGHT 32U
#endif

#if (SELECTED == SSD1306_128_64)
	#define SSD1306_HEIGHT 64U
#endif

#define SSD1306_MAX_CONTRAST 0xFFU   

#define SSD1306_SETCONTRAST 0x81U
#define SSD1306_DISPLAYALLON_RESUME 0xA4U
#define SSD1306_DISPLAYALLON 0xA5U
#define SSD1306_NORMALDISPLAY 0xA6U
#define SSD1306_INVERTDISPLAY 0xA7U
#define SSD1306_DISPLAYOFF 0xAEU
#define SSD1306_DISPLAYON 0xAFU
#define SSD1306_SETDISPLAYOFFSET 0xD3U
#define SSD1306_SETCOMPINS 0xDAU
#define SSD1306_SETVCOMDETECT 0xDBU
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5U
#define SSD1306_SETPRECHARGE 0xD9U
#define SSD1306_SETMULTIPLEX 0xA8U
#define SSD1306_SETLOWCOLUMN 0x00U
#define SSD1306_SETHIGHCOLUMN 0x10U
#define SSD1306_SETSTARTLINE 0x40U
#define SSD1306_MEMORYMODE 0x20U
#define SSD1306_COLUMNADDR 0x21U
#define SSD1306_PAGEADDR   0x22U
#define SSD1306_COMSCANINC 0xC0U
#define SSD1306_COMSCANDEC 0xC8U
#define SSD1306_SEGREMAP 0xA0U
#define SSD1306_CHARGEPUMP 0x8DU
#define SSD1306_EXTERNALVCC 0x1U
#define SSD1306_SWITCHCAPVCC 0x2U
#define SSD1306_PAGE_ADDR_IN_PAGEADDRESSING_MODE 0xB0U
#define SSD1306_REFRESH_RATIO 0x80U

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2FU
#define SSD1306_DEACTIVATE_SCROLL 0x2EU
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3U
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26U
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27U
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29U
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2AU

#define SSD1306_COMMAND_WRITE_CONTROL_REG 0x00U
#define SSD1306_COMMAND_WRITE_DATA_REG 0x40U

/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/ 

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
uint8_t contrast_value = SSD1306_MAX_CONTRAST;
volatile uint8_t display_buffer[4][128];

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/ 
static void SSD1306_command(uint8_t cmd);
static void SSD1306_data(uint8_t dat);
static void SSD1306_display_u8_value_12x8(uint8_t value0to9, uint8_t x, uint8_t y);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
void SSD1306_init(uint8_t refresh)
{
	//#define  szerokosc_znaku ((18/8)+1)
	//#define  wysokosc_znaku  wysokosc_wyswietlacza/8
	//SSD1306_command(SSD1306_SETLOWCOLUMN | 0x00);
	//SSD1306_command(SSD1306_SETHIGHCOLUMN | 0x00);
	//SSD1306_command(SSD1306_SETSTARTLINE | 0x00);
	/*
	//ONLY FOR HORIZONTAL OR VERTICAL ADDRESSING MODE
	// set pages address - rows range
	SSD1306_command(0x22);  //set rows range
	SSD1306_command(0);     //rows start address
	SSD1306_command(3);     //rows end address
	// set column address
	SSD1306_command(0x21);  //set column range
	SSD1306_command(1);     //kolumn start address
	SSD1306_command(127);   //kolumn end address
	*/
	//SSD1306_command(0x08);  //ustaw nr kolumny dla page adressing mode
	//SSD1306_command(0xB0);  //ustaw na wiersz 1 - Page0
	//SSD1306_command(0xB1);  //ustaw na wiersz 2 - Page1
	//SSD1306_command(0xB2);  //ustaw na wiersz 3 - Page2
	//SSD1306_command(0xB3);  //ustaw na wiersz 3 - Page3  -ustaw nr wiersza

    SSD1306_command(SSD1306_DISPLAYOFF);
    //page addressing:
    SSD1306_command(SSD1306_SETLOWCOLUMN); 
    SSD1306_command(SSD1306_SETHIGHCOLUMN);
    SSD1306_command(SSD1306_SETSTARTLINE);
    SSD1306_command(SSD1306_PAGE_ADDR_IN_PAGEADDRESSING_MODE);
    // horizontal/vertical addressing:
    SSD1306_command(0x21U);   //set Column address
    SSD1306_command(0x00U);	 //set Column address - poczatku
    SSD1306_command(0x7FU);   // set Column address - konca
    SSD1306_command(0x22U);	 // set Page address
    SSD1306_command(0x00U);	 // set Page address - poczatku
    SSD1306_command(0x03U);	 // set Page address - konca
    SSD1306_command(SSD1306_MEMORYMODE);         
    SSD1306_command(0x00U); //00 - horizontal, 01-vertical, 02 - page addressing
    #if defined SSD1306_128_32
    SSD1306_command(SSD1306_SETCOMPINS); 
    SSD1306_command(0x02U);
    #elif defined SSD1306_128_64
    SSD1306_command(SSD1306_SETCOMPINS);                  
    SSD1306_command(0x12U);
    SSD1306_command(SSD1306_SETCONTRAST);    // 0x81
    if (vccstate == SSD1306_EXTERNALVCC)
    { SSD1306_command(0x9FU); }
    else
    { SSD1306_command(0xCFU); }
    #elif defined SSD1306_96_16
    SSD1306_command(SSD1306_SETCOMPINS);
    SSD1306_command(0x2U);   //ada x12
    SSD1306_command(SSD1306_SETCONTRAST);   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC)
    { SSD1306_command(0x10U); }
    else
    { SSD1306_command(0xAFU); }
    #endif
    //oscylator
    SSD1306_command(SSD1306_SETDISPLAYCLOCKDIV);      
    SSD1306_command(0x80U);                            //ratio 100 ramek/sek
    SSD1306_command(SSD1306_SETMULTIPLEX);        
    SSD1306_command(SSD1306_HEIGHT - 1U);
    SSD1306_set_contrast(contrast_value); 		        //range 0 - FF, default 0x8F
    SSD1306_command(SSD1306_NORMALDISPLAY);  			//negative/normal
    SSD1306_command(0x2EU); //scrollowanie wy��czone
	SSD1306_command(SSD1306_DEACTIVATE_SCROLL);
    SSD1306_command(SSD1306_SETDISPLAYOFFSET);        // 0xD3 przesuniecie w pionie  0..31
    SSD1306_command(0x00U);
    SSD1306_command(SSD1306_SEGREMAP | 0x01);         // 0x01 - widok normalny , 0x00 - negatyw tresci - nie koloru
    SSD1306_command(0xC0U);                            // ekran do gory nogami 0xC0 / 0xC8
    SSD1306_command(0xA1U);							  // do gory nogami A0 / A1
	SSD1306_command(SSD1306_SETVCOMDETECT);           // 0xDB
	SSD1306_command(0x40U);
	SSD1306_command(SSD1306_DEACTIVATE_SCROLL);
    SSD1306_command(0x8DU); //pompa ladunkowa ON
    SSD1306_command(0x14U);
    SSD1306_command(0xA4U); //dane z RAM sa wyswietlane A5, nie sa A4
    PERIPHERALS_delay_ms(150);
    SSD1306_cls(); 
	SSD1306_command(SSD1306_DISPLAYON); //turn on oled panel
	PERIPHERALS_delay_ms(150);
}

void SSD1306_set_contrast(uint8_t contrast_value)
{
	  SSD1306_command(SSD1306_SETCONTRAST);
	  SSD1306_command(contrast_value);
}

void SSD1306_scroll_horizontal(uint8_t LeftOrRight, uint8_t startPageAdr, uint8_t stopPageAdr, uint8_t offset)
{
		if (LeftOrRight == 0)
		{
			SSD1306_command(0x27U); //scroll left
		}
		if (LeftOrRight == 1)
				{
					SSD1306_command(0x26); //scroll right
				}
		SSD1306_command(0x00U);
        SSD1306_command(startPageAdr);     // range 0x00 - 0x07
        SSD1306_command(offset);           // time delay between shifts in range 0x00 - 0x07
        SSD1306_command(stopPageAdr);      // addr 0x00 - 0x07
        SSD1306_command(0x00U);
        SSD1306_command(0xFFU);
        SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

//needs to be fixed
void SSD1306_scroll_vertical(uint8_t horVert, uint8_t startPageAdr, uint8_t stopPageAdr,  uint8_t interwal, uint8_t offset)
{
	SSD1306_command(SSD1306_MEMORYMODE);          // 0x20
	SSD1306_command(0x00U);                        // 0x00 - horizontal, 0x01-vertical, 0x02 - page addressing

	if (horVert == 0U)
	{
	    SSD1306_command(0x29U); // przesuw do gory i w prawo
	}
	if (horVert == 1)
	{
	    SSD1306_command(0x2AU); // przesuw do gory i w lewo
	}
	SSD1306_command(0x00);
    SSD1306_command(startPageAdr); // adres strony startowej 0x00 - 0x07
    SSD1306_command(interwal);       // interwal pomiedzy przesunieciami 0x00 - 0x07
    SSD1306_command(stopPageAdr);  // adres strony koncowej 0x00 - 0x07
    SSD1306_command(offset);         // offset przesuniecia w liniach 0x00 - 0x3F
    SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_vertical_offset(void)
{  //range  0..31
	SSD1306_command(SSD1306_SETDISPLAYOFFSET);
	SSD1306_command(0x00U);
}

void SSD1306_negativ_off(void)  		 
{
    SSD1306_command(SSD1306_NORMALDISPLAY);
}

void SSD1306_negativ_on(void)         
{
    SSD1306_command(SSD1306_NORMALDISPLAY | 0x01U);
}

void SSD1306_scrolling_on(void)  	        
{
	SSD1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_scrolling_off(void) 
{
	SSD1306_command(SSD1306_DEACTIVATE_SCROLL);
}

//wyswietla pojedyncze pixele - wspolrzedne  x do 128, y do 32
void SSD1306_set_pixel(int x, int y, uint8_t bw, uint8_t *buf)
{
    SSD1306_command(SSD1306_MEMORYMODE); // 0x20
    SSD1306_command(0x00U); // 0x00 - horizontal, 0x01-vertical, 0x02 - page addressing

	if ((x<0)||(x >= SSD1306_WIDTH) || (y < 0) || (y >= SSD1306_HEIGHT))
		{
			return;
		}
	if (bw)
		{
			buf[x +((y/8) *SSD1306_WIDTH)] |= (1 << (y % 8U)); //zapal pixel
		}
	else
		{
			buf[x +((y/8) *SSD1306_WIDTH)] &= ~(1 << (y % 8U)); //zgas pixel
		}

	   for (uint16_t i = 0; i < (SSD1306_WIDTH*SSD1306_HEIGHT/8); i++)
	   {
		   SSD1306_data(buf[i]);
	   }
}

void SSD1306_display_u8_12x8(uint8_t valueuint, uint8_t x, uint8_t y)
{
	if (valueuint > 9)
	{
		SSD1306_display_u8_value_12x8(valueuint/10, x, y);
		SSD1306_display_u8_value_12x8(valueuint%10, x + 5, y);
	}
	else
	{
		SSD1306_display_u8_value_12x8(valueuint, x+5, y);
	}
}

//write bufor to RAM
void SSD1306_refresh_ram(void)
{
    	//	ssd1306_command(0xAE);				//Display_OFF

    	SSD1306_command(0x20);              // addressing mode
    	SSD1306_command(0x02);   			// 0x00 - horizontal, 0x01-vertical, 0x02 - page addressing
    	/*
    	    SSD1306_command(0xA4); //display ON - reading from RAM
    	    //SSD1306_command(0xA5); //display ON - ignoring RAM

    	    SSD1306_command(0xAF); //display ON - normal Mode
    	    //SSD1306_command(0xAE); //display OFF
    	*/
    	uint8_t w, k;
        SSD1306_command(0x22U); // set rows range
        SSD1306_command(0U);   //  rows start address
        SSD1306_command(3U);   // rows end address

    	for (w = 0; w < 4; w++) //adresuje wiersze
    	{
    		SSD1306_command(0xB0U + w); //ustaw na wiersz 1 i kolejne wiersze


    		for (k = 0; k < 128; k++) //adresuje kolumny
    		{
    			SSD1306_data(display_buffer[w][k]);  //kolumna, wiersz
    			//SSD1306_data(0xFF);
    		}
    	}
}

//wyswietlanie tekstu - do opracowania, czcionka 12x8
void SSD1306_display_digit_18x26(uint8_t pos_x, uint8_t cyfra)
{
	//pos_x = 0-127
	//pos_x *=16; //w ten sposob mozna numerowac kolejno po sobie miejsca do wyswietlenia znakow 0,1,2,3
	#define szerokosc_wyswietlacza 128U
    #define wysokosc_wyswietlacza 32U
	#define szerokosc_znaku 16U

	if (pos_x > 128U - szerokosc_znaku) pos_x = 128  -szerokosc_znaku;

		uint8_t nr_znaku_do_wyswietl = cyfra;
		uint16_t setpos_y = 0U;  //podnosi do gory

		for (uint16_t k =pos_x; k <pos_x + szerokosc_znaku; k++)
		{
			for (int8_t w = 2; w >= 0; w--)
			{
				//if(w==6)  display_buffer[w][k]=0xFF; else
				display_buffer[w][k]= pgm_read_byte(&MicrosoftTaiLe28[setpos_y + (nr_znaku_do_wyswietl *szerokosc_znaku *3)]); //3 oznacza szerokosc znaku na 3 bajty
				setpos_y++;
			}
		}
		SSD1306_refresh_ram();
}

void SSD1306_clear_display_buffer(void)
{
	for (uint16_t k = 0U; k < 127; k++)
	{
		for (int8_t w = 3U; w >= 0; w--)
		{
			//if(w==6)  display_buffer[w][k]=0xFF; else
			display_buffer[w][k]= 0; //3 oznacza szerokosc znaku na 3 bajty
		}
	}
	SSD1306_refresh_ram();
}

void SSD1306_cls(void)
{
	SSD1306_command(SSD1306_MEMORYMODE); // 0x20
	SSD1306_command(0x00U); // 0x00 - horizontal, 0x01-vertical, 0x02 - page addressing
	SSD1306_command(SSD1306_SETLOWCOLUMN | 0x00U);
	SSD1306_command(SSD1306_SETHIGHCOLUMN | 0x00U);
	SSD1306_command(SSD1306_SETSTARTLINE | 0x00U);

	for (uint16_t i=0; i<(SSD1306_WIDTH*SSD1306_HEIGHT/8);i++) SSD1306_data(0x00);
}

void SSD1306_display_temperature(float temperature)
{
	uint8_t t100 = (uint8_t)(temperature/100);
	uint8_t t10 = (uint8_t)(temperature -(t100*100)) /10;
	uint8_t t1 = (uint8_t)(temperature -(t100*100) -(t10*10));
	uint8_t t0_1 = (uint8_t)((temperature -(t100*100) -(t10*10) - t1) *10.0000);

	if (temperature > 9.9)
	{
		SSD1306_display_digit_18x26(0, t10);
	}
	else
	{
		SSD1306_display_digit_18x26(0, SSD1306_SIGN_EMPTY_DIGIT);
	}
	SSD1306_display_digit_18x26(16, t1);
	SSD1306_display_digit_18x26(38, t0_1);
}

void SSD1306_display_humidity(float humidity)
{
	uint8_t h10 = (uint8_t)(humidity/10);
	uint8_t h1 = (uint8_t)(humidity -(h10*10));

	if (h10 > 0U)
	{
		SSD1306_display_digit_18x26(77, h10);
	}
	else
	{
		SSD1306_display_digit_18x26(77, SSD1306_SIGN_EMPTY_DIGIT);
	}
	SSD1306_display_digit_18x26(94, h1);
}

void SSD1306_init_for_temp(void)
{
   	SSD1306_init(REFRESH_MID);
	SSD1306_set_contrast(SSD1306_DEFAULT_CONTRAST);
	SSD1306_display_digit_18x26(54, SSD1306_SIGN_DEG);
	SSD1306_display_digit_18x26(61, SSD1306_SIGN_C);
	SSD1306_display_digit_18x26(115, SSD1306_SIGN_PERCENT);
	SSD1306_display_digit_18x26(34, SSD1306_SIGN_DOT);
}

static void SSD1306_command(uint8_t cmd)
{
	DIO_twi_write_byte(SSD1306_I2C_ADDRESS, SSD1306_COMMAND_WRITE_CONTROL_REG, cmd);
}

static void SSD1306_data(uint8_t data)
{
	DIO_twi_write_byte(SSD1306_I2C_ADDRESS, SSD1306_COMMAND_WRITE_DATA_REG, data);
}

//wyswietlanie tekstu - do opracowania, czcionka 12x8
static void SSD1306_display_u8_value_12x8(uint8_t value0to9, uint8_t x, uint8_t y)
{
	#define szerokosc_znak 7U
	#define wysokosc_w_bytach 2U
	#define odstep_miedzy_znakami_w_liniach 2U

	if ((y > 1) || (x > 127 - szerokosc_znak)) return;

	uint16_t setpos_y = 0U;  //podnosi do gory

	for (uint16_t k = x; k < (x + szerokosc_znak); k++)
	{
		for (int8_t w = (3U -(y*wysokosc_w_bytach)); w >= (2U -(y*wysokosc_w_bytach)); w--)   //-y*2 ustawia poziomo y
		{
			//if(w==6)  bufor_wyswietlacza[w][k]=0xFF; else
			display_buffer[w][k+x]= pgm_read_byte(&MicrosoftSansSerif12ptBitmaps[setpos_y + (value0to9 * szerokosc_znak *wysokosc_w_bytach)]);
			setpos_y++;
		}
	}
	SSD1306_refresh_ram();
}

