#include "resource.h"
#include <windows.h>
#include "../Skinned Window/SkinnedWindow.h"
#include "main.h"

SkinnedWindow sk;

int APIENTRY WinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nCmdShow) {

  //Fenster erstellen
  if (!sk.MakeSkinnedWindow(640, 480, MainDlgProc, IDB_SKIN, IDR_SKINREGION)) {
    MessageBoxA(0, "Error Making Window. Exiting", "Fatal Error", 0);
    exit(0);
  }

  AllocConsole();

  // Event oriented message loop
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;

  switch (message) {
    case WM_CREATE:
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

  case WM_TIMER:
    return 0;
  
  case WM_KEYDOWN: {
    switch (wParam) {
      case VK_ESCAPE:
        SendMessage(hWnd, WM_DESTROY, 0, 0);
        return 0;

      case VK_INSERT:
        return 0;

      case VK_HOME:
        return 0;
      }
  }

  case WM_PAINT: {
    hdc = BeginPaint(hWnd, &ps);
    sk.Draw();
    EndPaint(hWnd, &ps);
    return (0);
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    return (0);
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}