#ifndef __FLOPPYORGEL_MENU_H
#define __FLOPPYORGEL_MENU_H

void drawCursor(uint32_t cursorPos);
void drawMenu(char* path, int16_t cursorPos);
int drawTracks(char* path);
void getFileNameFromCursorPos(char* srcPath, char* dstFilePath, int cursorPos);

#endif