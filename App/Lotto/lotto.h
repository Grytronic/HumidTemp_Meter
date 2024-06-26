/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 
/* Author:      Lukasz Gryc                                                                                                                    */
/* File:        lotto.h                                                                                                                        */
/* Created:     21.04.2024                                                                                                                     */
/* Description: Lotto Jack-Pot numbers generator                                                                                               */
/* Comments:                                                                                                                                   */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/ 

#ifndef LOTTO_H_
#define LOTTO_H_

/* External includes --------------------------------------------------------------------------------------------------------------------------*/ 
#include "../Main/main.h"

/* External defines ---------------------------------------------------------------------------------------------------------------------------*/

/* External types -----------------------------------------------------------------------------------------------------------------------------*/ 
typedef struct S_Lotto {
	uint8_t lottoTab5from50[5];
	uint8_t lottoTab2from12[2];
} Lotto_t;

/* External consts ----------------------------------------------------------------------------------------------------------------------------*/

/* External variables -------------------------------------------------------------------------------------------------------------------------*/
extern Lotto_t LOTTO_results;

/* External functions -------------------------------------------------------------------------------------------------------------------------*/ 
void LOTTO_generate_numbers(void);


#endif /* LOTTO_H_ */
