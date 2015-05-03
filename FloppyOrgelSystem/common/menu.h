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

void fsmInit();
bool fsmPush(void* state);
bool fsmPop();
void* fsmGetCurrentState();
void fsmTick();

void drawCursor(uint32_t cursorPos);
void drawMenu(char* path, int16_t cursorPos);
int drawTracks(char* path);
void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos);

#endif
