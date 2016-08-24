//--------------------------------------------------------------
// File     : stm32_ub_rng.c
// Datum    : 23.02.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : RNG
// Funktion : Zufallszahlen Generator
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_rng.h"


//--------------------------------------------------------------
// Init vom Zufallszahlen Generator
//--------------------------------------------------------------
void UB_Rng_Init(void)
{
  // Clock Enable
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

  // RNG enable
  RNG_Cmd(ENABLE);
}


//--------------------------------------------------------------
// Erzeugt eine Zufallszahl zwischen min- und maxwert
// min und maxwerte sind enthalten
// also RNG(3,8) ergibt als mögliche Zahlen [3,4,5,6,7,8]
//--------------------------------------------------------------
int32_t UB_Rng_makeZufall(int32_t minwert, int32_t maxwert)
{
  int32_t ret_wert=0;
  int32_t maske=0xFFFFFFFF;
  int32_t rng_wert=0;

  int32_t temp_minwert = 0;
  int32_t temp_maxwert = maxwert - minwert;

  if(minwert < maxwert) {
	// maske setzen
    if(temp_maxwert<=0x0FFFFFFF) maske=0x0FFFFFFF;
    if(temp_maxwert<=0x00FFFFFF) maske=0x00FFFFFF;
    if(temp_maxwert<=0x000FFFFF) maske=0x000FFFFF;
    if(temp_maxwert<=0x0000FFFF) maske=0x0000FFFF;
    if(temp_maxwert<=0x00000FFF) maske=0x00000FFF;
    if(temp_maxwert<=0x000000FF) maske=0x000000FF;
    if(temp_maxwert<=0x0000000F) maske=0x0000000F;

    // Zufallszahl suchen
    do {
    	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
    	rng_wert = RNG_GetRandomNumber();
    	// maske benutzen (damit wird Zahl schneller gefunden)
    	rng_wert = (rng_wert&maske);
    }while((rng_wert > temp_maxwert) || (rng_wert < temp_minwert));
    // Zufallszahl zurückgeben
    ret_wert = rng_wert + minwert;
  }
  else if(temp_minwert == temp_maxwert) {
	// min und maxwert sind gleich
    ret_wert = minwert;
  }
  else {
    // minwert ist groesser als maxwert
	ret_wert = minwert;
  }

  return(ret_wert);
}




