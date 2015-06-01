#ifndef __FLOPPYORGEL_MENU_H
#define __FLOPPYORGEL_MENU_H

#include <stdbool.h>

FsmState mainMenu(StackBasedFsm_t* fsm);
FsmState buttonTest(StackBasedFsm_t* fsm);
FsmState liveMode(StackBasedFsm_t* fsm);
FsmState liveReceiving(StackBasedFsm_t* fsm);
FsmState playlist(StackBasedFsm_t* fsm);
FsmState startPlayBack(StackBasedFsm_t* fsm);
FsmState playing(StackBasedFsm_t* fsm);
FsmState playbackAborted(StackBasedFsm_t* fsm);
FsmState playbackFinished(StackBasedFsm_t* fsm);
FsmState floppyTest(StackBasedFsm_t* fsm);

void drawCursor(uint32_t cursorPos);
void drawMenu(char* path, int16_t cursorPos);
int drawTracks(char* path);
void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos);

#endif
