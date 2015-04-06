#ifndef __FLOPPYORGEL_MENU_H
#define __FLOPPYORGEL_MENU_H

void fsmStateMainMenu();
void fsmStateButtonTest();
void fsmStatePlaylist();
void fsmStartPlayBack();
void fsmStatePlaying();
void fsmStatePlaybackAborted();
void fsmStatePlaybackFinished();

void fsmInit();
BOOL fsmPush(void* state);
BOOL fsmPop();
void* fsmGetCurrentState();
void fsmTick();

void drawCursor(uint32_t cursorPos);
void drawMenu(char* path, int16_t cursorPos);
int drawTracks(char* path);
void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos);

#endif