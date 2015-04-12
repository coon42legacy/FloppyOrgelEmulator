#include <stdint.h>
#include <Windows.h>
#include <stdio.h>
#include "hal_filesystem.h"

static HANDLE fo_findHandle;

bool hal_findInit(char* path, FO_FIND_DATA* findData) {
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  fo_findHandle = FindFirstFile(path, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fo_findHandle != INVALID_HANDLE_VALUE ? TRUE : FALSE;
}

bool hal_findNext(FO_FIND_DATA* findData) {
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  BOOL fileHasFound = FindNextFile(fo_findHandle, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fileHasFound;
}

void hal_findFree() {
  FindClose(fo_findHandle);
}

int32_t hal_fopen(FILE** pFile, const char* pFileName) {
  return fopen_s(pFile, pFileName, "rb") == 0;
}

int32_t hal_fclose(FILE* pFile) {
  return fclose(pFile);
}

int32_t hal_fseek(FILE* pFile, int startPos) {
  return fseek(pFile, startPos, SEEK_SET);
}

size_t hal_fread(FILE* pFile, void* dst, size_t numBytes) {
  return fread_s(dst, numBytes, 1, numBytes, pFile); // TODO: word access? Does it increase performance?
}

int32_t hal_ftell(FILE* pFile) {
  return ftell(pFile);
}
