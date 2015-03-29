#include <stdint.h>
#include <Windows.h>
#include <stdio.h>
#include "hal_filesystem.h"

static HANDLE fo_findHandle;

BOOL fo_findInit(char* path, FO_FIND_DATA* findData) {
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  fo_findHandle = FindFirstFile(path, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fo_findHandle != INVALID_HANDLE_VALUE ? TRUE : FALSE;
}

BOOL fo_findNext(FO_FIND_DATA* findData) {
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  BOOL fileHasFound = FindNextFile(fo_findHandle, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fileHasFound;
}

void fo_findFree() {
  FindClose(fo_findHandle);
}
