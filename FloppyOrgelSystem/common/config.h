#ifndef __FO_CONFIG_H
#define __FO_CONFIG_H

#ifdef _WIN32
  #define DONT_DEFINE_SYSTEM_MAIN
  #define MIDI_PATH "_sdcard"
#else
  #define MIDI_PATH ""
#endif

#define DISPLAY_RESOLUTION_X 320
#define DISPLAY_RESOLUTION_Y 240

#endif
