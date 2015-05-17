#ifndef __FLOPPYORGEL_MENU_H
#define __FLOPPYORGEL_MENU_H

#include <stdbool.h>

void fsmStateMainMenu();
void fsmStateButtonTest();
void fsmStateLiveMode();
void fsmStateLiveReceiving();
void fsmStatePlaylist();
void fsmStartPlayBack();
void fsmStatePlaying();
void fsmStatePlaybackAborted();
void fsmStatePlaybackFinished();

void drawCursor(uint32_t cursorPos);
void drawMenu(char* path, int16_t cursorPos);
int drawTracks(char* path);
void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos);

#endif
