#ifndef __HAL_FILESYSTEM_H
#define __HAL_FILESYSTEM_H

#include <Windows.h>

typedef struct {
  char fileName[MAX_PATH];
} FO_FIND_DATA;

BOOL fo_findInit(char* path, FO_FIND_DATA* findData);
BOOL fo_findNext(FO_FIND_DATA* findData);
void fo_findFree();

#endif