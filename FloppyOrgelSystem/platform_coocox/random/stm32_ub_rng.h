//--------------------------------------------------------------
// File     : stm32_ub_rng.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_RNG_H
#define __STM32F4_UB_RNG_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_Rng_Init(void);
int32_t UB_Rng_makeZufall(int32_t minwert, int32_t maxwert);


//--------------------------------------------------------------
#endif // __STM32F4_UB_RNG_H
