#include <stdint.h>
#include <Windows.h>
#include <stdio.h>
#include "hal_filesystem.h"
#include "hal_gui.h"

int drawTracks(char* path) {
  WIN32_FIND_DATA search_data;
  memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
  HANDLE handle = FindFirstFile(path, &search_data);

  static const uint32_t X_OFFSET = 35;
  static const uint32_t Y_OFFSET = 40;
  int itemCount = 0;

  while (handle != INVALID_HANDLE_VALUE) {
    if (search_data.cFileName[0] != '.')
      if (search_data.cFileName[1] != '.') {
        gui_drawText(X_OFFSET, Y_OFFSET + 18 * itemCount++, search_data.cFileName,
          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
      }
    
    if (FindNextFile(handle, &search_data) == FALSE)
      break;
  }

  //Close the handle after use or memory/resource leak
  FindClose(handle);

  return 0;
}