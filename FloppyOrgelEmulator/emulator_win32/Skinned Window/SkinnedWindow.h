#ifndef __SKINNED_WINDOW_H
#define __SKINNED_WINDOW_H

#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "Msimg32.lib") // For TransparentBlt()

enum SkinnedWindowError_t {
  SKW_OK,
  SKW_ERROR_LOADING_SKIN,
  SKW_INVALID_REGION_RESSOURCE_ID,
  SKW_ERROR_GETTING_REGION_FILE_FROM_RESSOURCE,
  SKW_ERROR_LOADING_REGION_RESSOURCE,
  SKW_ERROR_CREATING_REGION,
};

class SkinnedWindow {
public:
  SkinnedWindow();
  ~SkinnedWindow();
  bool makeSkinnedWindow(
    const char* windowTitle, const char* windowClassName,
    int width, int height, WNDPROC dlgProc,
    int bitmapRessourceId, int skinRegionRessourceId, const char* regionFileType = "BINARY",
    int xPosUserContent = 0, int yPosUserContent = 0,
    int widthUserContent = 0, int heightUserContent = 0
  );
  void draw();
  HWND getHWnd();
  HDC getUserContentHDC();

private:
  void createCompatibleBuffer(HDC hSrcBitmapDC, int width, int height, HBITMAP* hDstBitmap, HDC* hDstBufferDC);
  SkinnedWindowError_t loadSkin(int bitmapRessourceId);
  SkinnedWindowError_t loadRegion(int skinRegionRessourceId, const char* ressourceType);
  bool makeWindow(const char* windowTitle, const char* windowClassName, int width, int height, WNDPROC dlgProc);

  HWND hWnd;
  HINSTANCE hInstance;
  HDC hSkinDC;
  HDC hUserContentDC;
  HDC hBackBufferDC;
  HDC hFrontBufferDC;
  HDC hOutDC;
  HBITMAP hSkinBitmap;
  HBITMAP hUserContentBitmap;
  HBITMAP hFrontBufferBitmap;
  HBITMAP hBackBufferBitmap;
  RECT rcUserContent;
};

#endif