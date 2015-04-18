#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Filesystem/fatfs/ff.h"
#include "hal_filesystem.h"

static FATFS SD_Fs;
static DIR dirs;

static bool mountSdCard() {
  /* Try to mount SD card */
  /* SD card is at 0: */
  hal_printf("Mounting SD card... ");
  if (f_mount(&SD_Fs, "0:", 1) == FR_OK) {
    hal_printf("success!\n\r"); /* Mounted ok */

    /* Get total and free space on SD card */
    uint32_t free, total;
    TM_FATFS_DriveSize(&total, &free);

    /* Total space */
    hal_printf("Total: %8u kB; %5u MB; %2u GB\n\r", total, total / 1024, total / 1048576);
    hal_printf("Free:  %8u kB; %5u MB; %2u GB\n\r", free, free / 1024, free / 1048576);
    return true;
  }
  else {
    hal_printf("Failed! No SD-Card?\n\r");
    return false;
  }
}

void hal_fileSystemInit() {
  mountSdCard();
}

bool hal_findInit(char* path, FO_FIND_DATA* findData) {
  FRESULT fres = f_opendir(&dirs, "/");
  return fres == FR_OK && hal_findNext(findData);
}

bool hal_findNext(FO_FIND_DATA* findData) {
  static FILINFO finfo;
  finfo.lfname = findData->fileName;
  finfo.lfsize = 256;
  FRESULT fres = f_readdir(&dirs, &finfo);
  return fres == FR_OK && finfo.fname[0];
}

void hal_findFree() {
  // Not needed in this implementation.
}

// Returns 1, if file was opened successfully or 0 on error.
// TODO: check pointer dereferencing here!!!
int32_t hal_fopen(FILE** pFile, const char* pFileName) {
  static FIL file;
  bool res = f_open(&file, pFileName, FA_READ);
  *pFile = &file;

  return res;
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
