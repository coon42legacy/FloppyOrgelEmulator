#include <stdio.h>
#include <stdint.h>
#include "../hal/hal_misc.h"
#include "common_main.h"
#include "states/mainMenu.h"

static StackBasedFsm_t fsm;

void common_main() {
  hal_printf("\n\r");
  hal_printf("################################");
  hal_printf("Floppy Orgel v3.0 initialisiert.");
  hal_printf("################################");

  fsmInit(&fsm);
  fsmPush(&fsm, mainMenu, &fsm);

  while (1) {
    fsmTick(&fsm);
  }
}
