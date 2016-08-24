#include "delay.h"

void delayUs(uint32_t us) {
  TIM2->CNT = 0;
  while(TIM2->CNT < us);
}

void delayMs(uint32_t ms) {
  for(int i = 0; i < ms; i++)
    delayUs(1000);
}
