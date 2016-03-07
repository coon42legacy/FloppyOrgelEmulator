#ifndef __FO_CONFIG_H
#define __FO_CONFIG_H

#define VERSION "0.8.2"

#ifdef _WIN32
  #define DONT_DEFINE_COMMON_MAIN
  #define MIDI_PATH "_sdcard"
#else
  #define MIDI_PATH ""
#endif

#define FSM_STACK_SIZE 16
#define RING_BUFFER_SIZE 256
#define DISPLAY_RESOLUTION_X 320
#define DISPLAY_RESOLUTION_Y 240

#define CURSOR_SPEED_ITEMS_PER_SECOND 32
#define CURSOR_DELAY_MS_BEFORE_REPEAT 500
#define INPUT_DEVICE_DEBOUNCE_MS 0 // TODO: Set to about 100 - 50 ms later (should be lower than CURSOR_DELAY_MS_BEFORE_REPEAT)

#endif
