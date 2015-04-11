#ifndef __HAL_FILESYSTEM_H
#define __HAL_FILESYSTEM_H

#include <stdbool.h>

typedef struct {
  char fileName[256];
} FO_FIND_DATA;

bool fo_findInit(char* path, FO_FIND_DATA* findData);
bool fo_findNext(FO_FIND_DATA* findData);
void fo_findFree();

#endif