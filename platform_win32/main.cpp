#include "ressources/resource.h"
#include <windows.h>
#include "Skinned Window/SkinnedWindow.h"
#include <stdio.h>
#include <stdint.h>
#include "guicon.h"
#include "../common/config.h"
#include "../common/common_main.h"
#include "main.h"

SkinnedWindow sk;

// Global variables for C-code.
extern "C" {
  void common_main();
  HDC g_hDisplayDC;
  HWND g_hEmuWnd;
}

DWORD WINAPI system_thread(LPVOID lpParam) {
  common_main();

  return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  if (!sk.makeSkinnedWindow("Floppyorgel Emulator", "Coonami Entertainment", 640, 480, mainDlgProc,
      IDB_SKIN, IDR_SKINREGION, "BINARY", 17, 13, DISPLAY_RESOLUTION_X, DISPLAY_RESOLUTION_Y)) {

    MessageBoxA(0, "Error Making Window. Exiting", "Fatal Error", 0);
    exit(0);
  }

  g_hDisplayDC = sk.getUserContentHDC();
  g_hEmuWnd = sk.getHWnd();

  HANDLE hSystemThread = CreateThread(NULL, 0, system_thread, NULL, 0, NULL);
  if (hSystemThread == NULL)
    ExitProcess(0);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

LRESULT CALLBACK mainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;

  switch (message) {
    case WM_CREATE:
      redirectIOToConsole();
      return (0);

  case WM_LBUTTONDOWN:{
    // ---------------------------------------------------------
    // this is a common trick for easy dragging of the window.
    // this message fools windows telling that the user is
    // actually dragging the application caption bar.
    // ---------------------------------------------------------
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
    break;
  }

  case WM_KEYDOWN: {
    switch (wParam) {
      case VK_ESCAPE:
        SendMessage(hWnd, WM_DESTROY, 0, 0);
        return 0;
      }
  }

  case WM_PAINT: {
    hdc = BeginPaint(hWnd, &ps);
    sk.draw();
    EndPaint(hWnd, &ps);
    return (0);
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    return (0);
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}
