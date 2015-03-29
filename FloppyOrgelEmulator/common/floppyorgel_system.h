#ifndef __FLOPPYORGEL_SYSTEM_H
#define __FLOPPYORGEL_SYSTEM_H

#ifndef DONT_DEFINE_SYSTEM_MAIN
  void system_main();
#endif

void debugPrintNesGamePadState();
int drawTracks(char* path);

#endif // __FLOPPYORGEL_SYSTEM_H