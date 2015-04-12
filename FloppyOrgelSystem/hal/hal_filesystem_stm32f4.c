#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "Filesystem/fatfs/ff.h"
#include "hal_filesystem.h"

// static HANDLE fo_findHandle;

bool hal_findInit(char* path, FO_FIND_DATA* findData) {
  /*
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  fo_findHandle = FindFirstFile(path, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fo_findHandle != INVALID_HANDLE_VALUE ? TRUE : FALSE;
  */

  return false;
}

bool hal_findNext(FO_FIND_DATA* findData) {
  /*
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  BOOL fileHasFound = FindNextFile(fo_findHandle, &search_data);
  strcpy_s(findData->fileName, sizeof(findData->fileName), search_data.cFileName);
  return fileHasFound;
  */

  return false;
}

void hal_findFree() {
  // TODO
}

// Returns 1, if file was opened successfully or 0 on error.
// TODO: check pointer dereferencing here!!!
int32_t hal_fopen(FILE** pFile, const char* pFileName) {
  return f_open((FIL*)pFile, pFileName, FA_READ);
}

int32_t hal_fclose(FILE* pFile) {
  return f_close((FIL*)pFile);
}

int32_t hal_fseek(FILE* pFile, int startPos) {
  return f_lseek((FIL*)pFile, startPos);
}

size_t hal_fread(FILE* pFile, void* dst, size_t numBytes) {
  UINT bytesRead;
  f_read((FIL*)pFile, dst, numBytes, &bytesRead); // TODO: word access? Does it increase performance?
  return bytesRead;
}

int32_t hal_ftell(FILE* pFile) {
  return f_tell((FIL*)pFile);
}
