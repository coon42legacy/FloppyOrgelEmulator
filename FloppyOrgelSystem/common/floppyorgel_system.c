#include <stdio.h>
#include <stdint.h>
#include "../hal/hal_misc.h"
#include "floppyorgel_system.h"
#include "StackBasedFsm.h"
#include "menu.h"

static StackBasedFsm_t fsm;

void system_main() {
  hal_printf("\n\r");
  hal_printf("################################");
  hal_printf("Floppy Orgel v3.0 initialisiert.");
  hal_printf("################################");

  fsmInit(&fsm);
  fsmPush(&fsm, fsmStateMainMenu);

  while (1) {
    fsmTick(&fsm);
  }
}
