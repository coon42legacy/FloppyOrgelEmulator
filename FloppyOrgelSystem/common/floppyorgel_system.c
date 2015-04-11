#include <stdio.h>
#include <stdint.h>
#include "../hal/hal_inputdevice.h"
#include "floppyorgel_system.h"
#include "menu.h"

void debugPrintln(char* text) {
  printf("%s\n\r", text);
}

void system_main() {  
  debugPrintln("\n\r");
  debugPrintln("################################");
  debugPrintln("Floppy Orgel v3.0 initialisiert.");
  debugPrintln("################################"); 
   
  fsmInit();
  fsmPush(fsmStateMainMenu);

  while (1) {
    fsmTick();
  }
}
