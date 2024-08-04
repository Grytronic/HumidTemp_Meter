/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        main.c                                                                                                                         */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Main entry point                                                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

/* Private includes ---------------------------------------------------------------------------------------------------------------------------*/
#include "main.h"

#if (ACTIVE == USE_HW_PLATFORM_AVR)
	#include "../Peripherals/avr/peripherals.h"
#elif (ACTIVE == USE_HW_PLATFORM_STM32)
	#include "../Peripherals/stm32/peripherals.h"
#else
    #error "HW-platform not defined"
#endif

#include "../GPS/gps.h"
#include "../GPS/neo6m.h"
#include "../DispOLED/dispOLED.h"
#include "../TempHumidSensor/tempHumid.h"
#include "../Disp7SegLED/disp7segLed.h"
#include "../RTC/rtc.h"

/* Private defines ----------------------------------------------------------------------------------------------------------------------------*/ 
/* Private types ------------------------------------------------------------------------------------------------------------------------------*/ 
/* Private consts -----------------------------------------------------------------------------------------------------------------------------*/

/* Private variables --------------------------------------------------------------------------------------------------------------------------*/
ProfilConfig_t S_ProfilCar;
ProfilConfig_t S_ProfilHome; 
ProfilConfig_t S_ProfilBadroom;
ProfilCollection_t SProfilCollection = {&S_ProfilCar, &S_ProfilHome, &S_ProfilBadroom};

float Temperature;
float Humidity;
float DevPoint;
float DevPoint_TempDiff;

/* choose default mode */
mode_t E_activeMode;

sDTH S_SensorData;

volatile uint8_t TimePeriodLeft_FLAG        = FLAG_IS_RESET;
volatile uint8_t Button1_MENUpressed_FLAG   = FLAG_IS_RESET;
volatile uint8_t Button2_CHANGEpressed_FLAG = FLAG_IS_RESET;
volatile uint8_t OneSecondLeftFLAG          = FLAG_IS_RESET;

uint8_t SecCounter_u8 = 60U;

uint8_t SecondsTimerONflag              = FLAG_IS_RESET;
uint8_t SecondsTimerLeftReachedZeroFlag = FLAG_IS_RESET;
uint8_t FirstRunFLAG                    = FLAG_IS_SET;
uint8_t DisplayUpdateFlag               = FLAG_IS_RESET;
uint8_t BuzzerAlarmActive               = FLAG_IS_SET;
uint8_t IndexOfCircleBuffer             = 0U;

uint8_t Button1_MENUpressedFLAG         = FLAG_IS_RESET;
uint8_t Button2_CHANGEpressedFLAG       = FLAG_IS_RESET;

/* Extern variables ---------------------------------------------------------------------------------------------------------------------------*/
ProfilConfig_t* pProfilDefault = NULL;


float TempSampleTab[COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION];

/* Private function declarations --------------------------------------------------------------------------------------------------------------*/
static void choose_default_profil_and_set_active_mode(void);
static void handle_circle_buffor_with_temp_values(void);
static void update_buzer_alarm_notification(void);
static void execute_if_devPoint_is_too_low(void);
static void execute_if_any_button_is_pressed(void);

static bool time_1sec_is_elapsed(void);
static bool time_4sec_is_elapsed(void);
static void show_temp_value_for_alarm(void);
static void refresh_buzzer_status_if_badroom_profil_is_choosen(void);

static void init_profiles_with_default_values(ProfilCollection_t* pSProfilCollection);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Function definitions -----------------------------------------------------------------------------------------------------------------------*/ 
int main(void)
{
/* INITIALISATION */
	init_profiles_with_default_values(&SProfilCollection);
	choose_default_profil_and_set_active_mode();

	// HW-INIT
	PERIPHERALS_init();
	TEMPHUMID_init();
	DISPOLED_HW_init();
	DHT_init();
	DISP7SEGLED_init(pProfilDefault->config_LEDdispBrightness);
	  while(1);
	DISPOLED_init_for_temp();
#if ((ACTIVE == USE_GPS) && (ACTIVE == USE_FAKE_GPS))
	GPS_init();
#endif
#if (ACTIVE == USE_RTC)
	RTC_init();
#endif 
	// IO-behavior Init
    show_temp_value_for_alarm();
    refresh_buzzer_status_if_badroom_profil_is_choosen();
	PERIPHERALS_enable_global_interrupts();

while(1) 
{
	if (time_1sec_is_elapsed())
	{
		if ((FLAG_IS_SET == SecondsTimerONflag) && (SecondsTimerLeftReachedZeroFlag == FLAG_IS_RESET) && (ZERO_U != SecCounter_u8))
		{
			SecCounter_u8--;

			if (ZERO_U == SecCounter_u8)
			{

				SecondsTimerLeftReachedZeroFlag = FLAG_IS_SET;
				SecondsTimerONflag = FLAG_IS_RESET;
			}
		}
		DisplayUpdateFlag = FLAG_IS_SET;
		DISP7SEGLED_display_gps_speed(&S_GPSstate);
	}

	if (time_4sec_is_elapsed())
	{
		if (FLAG_IS_SET == TimePeriodLeft_FLAG) //time left flag: about 4s
		{
			TimePeriodLeft_FLAG = FLAG_IS_RESET;
			DISP7SEGLED_set_first_stripe_before_raad_from_sensor();
			TEMPHUMIDSENSOR_get_measurements(&S_SensorData);
			DISP7SEGLED_reset_first_stripe_after_raad_from_sensor();

			// checksum not fit or data error
			if (FLAG_IS_SET == S_SensorData.NewValueReceived)
			{
				S_SensorData.NewValueReceived = FLAG_IS_RESET;
				DISP7SEGLED_set_second_stripe_after_raad_from_sensor();

				Temperature = (float)(S_SensorData.Temp_integral + S_SensorData.Temp_decimal*0.1 - pProfilDefault->config_tempOffset);
				Humidity = S_SensorData.RH_integral + S_SensorData.RH_decimal*0.1 - pProfilDefault->config_humidityOffset;

				/* bufor kolowy zbierajacy wartosci temperatury */
				handle_circle_buffor_with_temp_values();

				DevPoint = roundf(TEMPHUMID_calculate_dewPoint_value(Temperature, Humidity));
				DevPoint_TempDiff = Temperature - DevPoint;

				DISP7SEGLED_reset_second_stripe_after_raad_from_sensor();
				DisplayUpdateFlag = FLAG_IS_SET;
			}
			execute_if_devPoint_is_too_low();
		}

		if (FLAG_IS_SET == DisplayUpdateFlag)
		{
			if ((Temperature > 0) && (Humidity > 0))
			{
				DISPOLED_display_data_from_tempHumid_sensor(Temperature, Humidity);
				DISP7SEGLED_display_dev_point(pProfilDefault, (uint8_t)DevPoint);
				DISP7SEGLED_display_alarm_temp(pProfilDefault);
				DISP7SEGLED_display_loHi_info(Humidity);
			}
			DISP7SEGLED_display_time(&Time);
			DISP7SEGLED_display_adc(ADC_get_measurement());

			DisplayUpdateFlag = FLAG_IS_RESET;
		}
	}
	execute_if_any_button_is_pressed();
  }

  return 0;
}


/* ---------------------------------------------------------------------------------------------------------------------------------------------- */

ISR(TIMER1_OVF_vect)
{
	static uint8_t CountOfPeriods262ms_u8 = ZERO_U;

	// next measurement period for DTHxx
	#define PERIOD_FACTOR_FOR_2s 9U
    #define PERIOD_FACTOR_FOR_4s 18U

	CountOfPeriods262ms_u8++;

	if (ZERO_U == (CountOfPeriods262ms_u8 % 4U))
	{
		OneSecondLeftFLAG = FLAG_IS_SET;
	}

	if (PERIOD_FACTOR_FOR_4s == CountOfPeriods262ms_u8)
		{
			TimePeriodLeft_FLAG = FLAG_IS_SET;
			CountOfPeriods262ms_u8 = 0;
		}

	//check_taufpunkt();
	//led_arduino_toggle();
}

ISR(USART_RX_vect)
{
	GPS_recv_char(UDR0); //register it as callback function
}

ISR(PCINT1_vect)
{
	if (DIO_check_if_Button_MENU_is_pressed())
	{
		Button1_MENUpressedFLAG = FLAG_IS_SET;
	}

	if(DIO_check_if_Button_CHANGE_is_pressed())
	{
		Button2_CHANGEpressedFLAG = FLAG_IS_SET;
	}
}

static void execute_if_any_button_is_pressed(void)
{
	if (FLAG_IS_SET == Button1_MENUpressedFLAG) //black button
	{

		if (home == E_activeMode)
		{
			if (1 <= pProfilDefault->config_LEDdispBrightness) pProfilDefault->config_LEDdispBrightness -= 1;
			DISP7SEGLED_set_brightness(pProfilDefault->config_LEDdispBrightness);
			DISP7SEGLED_display_digit(1, pProfilDefault->config_LEDdispBrightness);
			DISP7SEGLED_clear();
		}

		else if (badroom == E_activeMode)
		{
			/* deactivate buzzer when alaram is active , second press activates it back*/
			if (FLAG_IS_SET == BuzzerAlarmActive)
			{
				if (ZERO_U < pProfilDefault->config_alarmBeeperON)
				{
					pProfilDefault->config_alarmBeeperON = NOT_ACTIVE;
				}
				else
				{
					pProfilDefault->config_alarmBeeperON = ACTIVE;
				}
			}
			update_buzer_alarm_notification();
			DISP7SEGLED_clear();
			DISP7SEGLED_display_int16(S_ProfilBadroom.config_alarmTempValue);
			PERIPHERALS_delay_ms(600);
			for (uint64_t i = 0; i < 80000; i++)
			{
					if (DIO_check_if_Button_MENU_is_pressed())
					{
						S_ProfilBadroom.config_alarmTempValue -= 1;
						if (18 ==  S_ProfilBadroom.config_alarmTempValue) S_ProfilBadroom.config_alarmTempValue = 25;
						DISP7SEGLED_display_int16(S_ProfilBadroom.config_alarmTempValue);
						PERIPHERALS_delay_ms(600);
						i = 0;
						//break;
					}
			}

			DISP7SEGLED_clear();

		}

		else if (car == E_activeMode)
		{
			S_ProfilCar.config_LEDdisplayDevPointValue    = 0;
			S_ProfilCar.config_LEDdisplayGPSdata          = 0;
			S_ProfilCar.config_LEDdisplayTime         	  = 1;
			S_ProfilCar.config_LEDdisplayADCvalue         = 0;
			pProfilDefault->config_LEDdisplayStripesForDebug  = 0;
			DISP7SEGLED_clear();
		}

		DisplayUpdateFlag = FLAG_IS_SET;
		PERIPHERALS_delay_ms(50); // debouncing
		Button1_MENUpressedFLAG = FLAG_IS_RESET;
	}

	if (FLAG_IS_SET == Button2_CHANGEpressedFLAG) //blue
	{
		if (home == E_activeMode)
		{
			E_activeMode = badroom;
			pProfilDefault = &S_ProfilBadroom;
			DISP7SEGLED_clear();
		}

		else if (badroom == E_activeMode)
		{
			// if (sProfil_badroom.temp_alarm_level <30) sProfil_badroom.temp_alarm_level += 1;

			LOTTO_generate_numbers();
			while (ZERO_U == DIO_check_if_Button_CHANGE_is_pressed()); //wait until pressed second time
			PERIPHERALS_delay_ms(250);
			DISPOLED_clear();
			DISPOLED_display_digit_18x26(54, 12);  //znak stopnia - kolko
			DISPOLED_display_digit_18x26(61, 13); 	//znak stopnia - litera C
			DISPOLED_display_digit_18x26(115, 11);	//RH %
			DISPOLED_display_digit_18x26(34, 10);	//temp kropka
			DISP7SEGLED_clear();
		}

		else if (car == E_activeMode)
		{
			S_ProfilCar.config_LEDdisplayDevPointValue     = 0;
			S_ProfilCar.config_LEDdisplayGPSdata           = 1;
			S_ProfilCar.config_LEDdisplayTime         	   = 0;
			S_ProfilCar.config_LEDdisplayADCvalue          = 0;
			pProfilDefault->config_LEDdisplayStripesForDebug   = 0;
			DISP7SEGLED_clear();
		}

		DisplayUpdateFlag = FLAG_IS_SET;
		PERIPHERALS_delay_ms(50); // debouncing
		Button2_CHANGEpressedFLAG = FLAG_IS_RESET;
	}
}

static void choose_default_profil_and_set_active_mode(void)
{
	/* settings related to choosen work profil */
	#if(SET == USE_PROFIL_HOME)
			pProfilDefault = &S_ProfilHome;
			E_activeMode = home;
	#endif
	#if(SET == USE_PROFIL_CAR)
			pProfilDefault = &S_ProfilCar;
			E_activeMode = car;
	#endif
	#if(SET == USE_PROFIL_BADROOM)
			pProfilDefault = &S_ProfilBadroom;
			E_activeMode = badroom;
	#endif
}

static void handle_circle_buffor_with_temp_values(void)
{
#if (0)
	/* bufor kolowy zbierajacy wartosci temperatury */
	static uint8_t IndexOfCircleBuffer = 0;

	TempSampleTab[IndexOfCircleBuffer] = Temperature;
	/* wpisanie wartosci poczatkowej temperatury po inicjalizacji czujnika */
	if (FLAG_IS_SET == FirstRunFLAG)
	{
		FirstRunFLAG = FLAG_IS_RESET;
		for (uint8_t i = 0; i <= pProfilDefault->config_countOftempMeanValSamples; i++) TempSampleTab[i] = Temperature;
	}

	/* obliczenie wartosci sredniej temperatury */
	if (0U < pProfilDefault->config_countOftempMeanValSamples)
	{
		float temperature_mean = 0;
		for (uint8_t i = 0; i <= pProfilDefault->config_countOftempMeanValSamples; i++) temperature_mean += TempSampleTab[i];
		Temperature = temperature_mean/(pProfilDefault->config_countOftempMeanValSamples + 1U);
		IndexOfCircleBuffer++;
		if (IndexOfCircleBuffer >= pProfilDefault->config_countOftempMeanValSamples) IndexOfCircleBuffer = 0;
	}
#endif
}

static void update_buzer_alarm_notification(void)
{
	if (1U == pProfilDefault->config_alarmBeeperON)
	{
	    DIO_led_orange_on();
	}
	else
	{
		DIO_led_orange_off();
	}
}

static void execute_if_devPoint_is_too_low(void)
{
	if ((( S_SensorData.Temp_integral < S_ProfilBadroom.config_alarmTempValue) && (S_SensorData.RH_integral < S_ProfilBadroom.config_alarmHumidValue))
					||  ((S_SensorData.RH_integral < S_ProfilBadroom.config_alarmHumidValue) && ((Temperature - DevPoint) >= S_ProfilBadroom.config_alarmDevTempDifference)))
	{
		DIO_led_red_off();
		DIO_led_red_on();

		SecondsTimerONflag = FLAG_IS_SET;
		pProfilDefault->config_LEDdisplayStripesForDebug = 0U;
		pProfilDefault->config_LEDdisplayDevPointValue = 0U;

		if ((1U == pProfilDefault->config_alarmBeeperON) && (FLAG_IS_SET == BuzzerAlarmActive) && (FLAG_IS_SET == SecondsTimerLeftReachedZeroFlag))
		{
			DIO_buzzer_BEEP(); // air good, close a window
		}
	}
	else /* If too high humidity */
	{
		DIO_led_green_off();
		DIO_led_red_on();
		pProfilDefault->config_LEDdisplayStripesForDebug = 1U;
	}
}

static bool time_1sec_is_elapsed(void)
{
	if (FLAG_IS_SET == OneSecondLeftFLAG)
	{
		OneSecondLeftFLAG = FLAG_IS_RESET;
		return true;
	}
	else
	{
		return false;
	}
}

static bool time_4sec_is_elapsed(void)
{
	if (FLAG_IS_SET == TimePeriodLeft_FLAG)
	{
		TimePeriodLeft_FLAG = FLAG_IS_RESET;
		return true;
	}
	else
	{
		return false;
	}
}

static void show_temp_value_for_alarm(void)
{
    DISP7SEGLED_display_int16(S_ProfilBadroom.config_alarmTempValue);
}

void refresh_buzzer_status_if_badroom_profil_is_choosen(void)
{
	#if(1 == USE_PROFIL_BADROOM)
		update_buzer_alarm_notification();
	#endif
}

static void init_profiles_with_default_values(ProfilCollection_t* pSProfilCollection)
{
	/* PROFIL CAR INIT*/
	pSProfilCollection->pProfilCar->config_humidityOffset 	  	       = CAR_HUMIDITY_OFFSET;
	pSProfilCollection->pProfilCar->config_tempOffset 		  		   = CAR_TEMP_OFFSET;
	pSProfilCollection->pProfilCar->config_countOftempMeanValSamples   = COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION;
	pSProfilCollection->pProfilCar->config_LEDdispBrightness 		   = 0;
	pSProfilCollection->pProfilCar->config_OLEDbrightness 			   = 0xFF;
	pSProfilCollection->pProfilCar->config_informLimitLoHighOverflow   = 0;
	pSProfilCollection->pProfilCar->config_humidityToLowValue 		   = 0;
	pSProfilCollection->pProfilCar->config_humidityToHighValue 		   = 75;
	pSProfilCollection->pProfilCar->config_alarmBeeperON 			   = 0;
	pSProfilCollection->pProfilCar->config_alarmTempValue 	           = 0;
	pSProfilCollection->pProfilCar->config_alarmHumidValue 		       = 0;
	pSProfilCollection->pProfilCar->config_alarmDevTempDifference      = 9.0F;
	pSProfilCollection->pProfilCar->config_LEDdisplayTempAlarmValue    = 0;
	pSProfilCollection->pProfilCar->config_LEDdisplayDevPointValue     = 0;
	pSProfilCollection->pProfilCar->config_LEDdisplayGPSdata           = 1;
	pSProfilCollection->pProfilCar->config_LEDdisplayTime              = 0;
	pSProfilCollection->pProfilCar->config_LEDdisplayStripesForDebug   = 1;
	pSProfilCollection->pProfilCar->config_LEDdisplayADCvalue    	   = 0;

	/* PROFIL HOME INIT*/
	pSProfilCollection->pProfilHome->config_humidityOffset 			   = HOME_HUMIDITY_OFFSET;
	pSProfilCollection->pProfilHome->config_tempOffset 				   = HOME_TEMP_OFFSET;
	pSProfilCollection->pProfilHome->config_countOftempMeanValSamples  = COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION;
	pSProfilCollection->pProfilHome->config_LEDdispBrightness 		   = 0;
	pSProfilCollection->pProfilHome->config_OLEDbrightness 			   = 0x50;//0x4F;
	pSProfilCollection->pProfilHome->config_informLimitLoHighOverflow  = 1;
	pSProfilCollection->pProfilHome->config_humidityToLowValue 		   = 40;
	pSProfilCollection->pProfilHome->config_humidityToHighValue 	   = 70;
	pSProfilCollection->pProfilHome->config_alarmBeeperON 			   = 0;
	pSProfilCollection->pProfilHome->config_alarmTempValue 	           = 0;
	pSProfilCollection->pProfilHome->config_alarmHumidValue 		   = 0;
	pSProfilCollection->pProfilHome->config_alarmDevTempDifference     = 9.0F;
	pSProfilCollection->pProfilHome->config_LEDdisplayTempAlarmValue   = 0;
	pSProfilCollection->pProfilHome->config_LEDdisplayDevPointValue    = 1;
	pSProfilCollection->pProfilHome->config_LEDdisplayGPSdata          = 0;
	pSProfilCollection->pProfilHome->config_LEDdisplayTime       	   = 0;
	pSProfilCollection->pProfilHome->config_LEDdisplayStripesForDebug  = 1;
	pSProfilCollection->pProfilHome->config_LEDdisplayADCvalue         = 0;

	/* PROFIL BATHROOM INIT*/
	pSProfilCollection->pProfilBadroom->config_humidityOffset 		       = BADROOM_HUMIDITY_OFFSET;
	pSProfilCollection->pProfilBadroom->config_tempOffset 			       = BADROOM_TEMP_OFFSET;
	pSProfilCollection->pProfilBadroom->config_countOftempMeanValSamples   = COUNT_OF_SAMPLES_FOR_MEANVALUE_CALCULATION;
	pSProfilCollection->pProfilBadroom->config_LEDdispBrightness 		   = 0;   //min 0
	pSProfilCollection->pProfilBadroom->config_OLEDbrightness 		       = 0x4F;//0x4F;
	pSProfilCollection->pProfilBadroom->config_informLimitLoHighOverflow   = 0;
	pSProfilCollection->pProfilBadroom->config_humidityToLowValue 		   = 40;
	pSProfilCollection->pProfilBadroom->config_humidityToHighValue 		   = 70;
	pSProfilCollection->pProfilBadroom->config_alarmBeeperON 			   = 0;  // 0 -silent alarm - only LED
	pSProfilCollection->pProfilBadroom->config_alarmTempValue 	           = 22;
	pSProfilCollection->pProfilBadroom->config_alarmHumidValue 		       = 65;
	pSProfilCollection->pProfilBadroom->config_alarmDevTempDifference      = 9.0F;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayTempAlarmValue    = 0;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayDevPointValue     = 1;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayGPSdata           = 0;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayTime              = 0;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayStripesForDebug   = 1;
	pSProfilCollection->pProfilBadroom->config_LEDdisplayADCvalue          = 0;
}

