#ifndef __FO_CONFIG_H
#define __FO_CONFIG_H

#define VERSION "0.8.0"

#ifdef _WIN32
  #define DONT_DEFINE_SYSTEM_MAIN
  #define MIDI_PATH "_sdcard"
#else
  #define MIDI_PATH ""
#endif

#define FSM_STACK_SIZE 16
#define RING_BUFFER_SIZE 256
#define DISPLAY_RESOLUTION_X 320
#define DISPLAY_RESOLUTION_Y 240

#endif
