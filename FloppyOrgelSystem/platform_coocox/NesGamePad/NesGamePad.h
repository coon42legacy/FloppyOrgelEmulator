#include <stdint.h>

typedef struct {
  uint8_t
  East : 1,
  West : 1,
  South : 1,
  North : 1,
  Start : 1,
  Select : 1,
  B : 1,
  A : 1;
} NesGamePadButtonStates_t;

union NesGamePadStates_t {
  NesGamePadButtonStates_t states;
  uint8_t code;
};

void setupNesGamePad();
union NesGamePadStates_t getNesGamepadState();
void latchShiftRegister_();
