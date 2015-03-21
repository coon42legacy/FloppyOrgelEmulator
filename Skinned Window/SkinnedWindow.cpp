#include "SkinnedWindow.h"

SkinnedWindow::SkinnedWindow() {
  hWnd			         = 0;
  hInstance		       = 0;
  hSkinDC            = 0;
  hUserContentDC     = 0;
  hBackBufferDC      = 0;
  hFrontBufferDC     = 0;
  hOutDC			       = 0;
  hSkinBitmap        = 0;
  hUserContentBitmap = 0;
  hBackBufferBitmap  = 0;
  hFrontBufferBitmap = 0;
}

SkinnedWindow::~SkinnedWindow() {
  if(hOutDC)         ReleaseDC(hWnd, hOutDC);
  if(hSkinDC)        DeleteDC(hSkinDC);
  if(hBackBufferDC)  DeleteDC(hBackBufferDC);
  if(hFrontBufferDC) DeleteDC(hFrontBufferDC);
}

void SkinnedWindow::createCompatibleBuffer(HDC hSrcBitmapDC, int width, int height, HBITMAP* hDstBitmap, HDC* hDstBufferDC) {
  *hDstBitmap = CreateCompatibleBitmap(hSrcBitmapDC, width, height);
  *hDstBufferDC = CreateCompatibleDC(hSrcBitmapDC);
  HBITMAP hOnePixelBmp = SelectBitmap(*hDstBufferDC, *hDstBitmap);
  DeleteObject(hOnePixelBmp);
}

SkinnedWindowError_t SkinnedWindow::loadRegion(int skinRegionRessourceId, const char* ressourceType) {
  if (MAKEINTRESOURCE(skinRegionRessourceId) == 0) 
    return SKW_INVALID_REGION_RESSOURCE_ID;
    
  HRSRC hrSkin = FindResource(hInstance, MAKEINTRESOURCE(skinRegionRessourceId), ressourceType);
  if (!hrSkin)
    return SKW_ERROR_GETTING_REGION_FILE_FROM_RESSOURCE;
    
  LPRGNDATA pRegionData = (LPRGNDATA)LoadResource(hInstance, hrSkin);
  if (!pRegionData) 
    return SKW_ERROR_LOADING_REGION_RESSOURCE;

  HRGN rgnSkin = ExtCreateRegion(NULL, SizeofResource(NULL, hrSkin), pRegionData);
  if (!rgnSkin) 
    return SKW_ERROR_CREATING_REGION;

  SetWindowRgn(hWnd, rgnSkin, true);
  FreeResource(pRegionData);

  return SKW_OK;
}

SkinnedWindowError_t SkinnedWindow::loadSkin(int bitmapRessourceId) {
  hSkinBitmap = LoadBitmap(this->hInstance, MAKEINTRESOURCE(bitmapRessourceId));
  if (!hSkinBitmap) 
    return SKW_ERROR_LOADING_SKIN;
  
  hSkinDC = CreateCompatibleDC(NULL);
  HBITMAP hOldBmp = SelectBitmap(hSkinDC, hSkinBitmap);
  DeleteObject(hOldBmp);

  BITMAP bmp;
  GetObject(hSkinBitmap, sizeof(bmp), &bmp);

  // Adjust window size to skin dimensions and move the window to screen center
  int screenWidth  = (WORD)GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = (WORD)GetSystemMetrics(SM_CYSCREEN);
  MoveWindow(hWnd, 
    (screenWidth - bmp.bmWidth) / 2, 
    (screenHeight - bmp.bmHeight) / 2, 
    bmp.bmWidth, bmp.bmHeight, true);

  DeleteObject(hFrontBufferBitmap);
  DeleteDC(hFrontBufferDC);
  DeleteObject(hBackBufferBitmap);
  DeleteDC(hBackBufferDC);

  createCompatibleBuffer(hSkinDC, rcUserContent.right, rcUserContent.bottom, &hUserContentBitmap, &hUserContentDC);
  createCompatibleBuffer(hSkinDC, bmp.bmWidth, bmp.bmHeight, &hBackBufferBitmap, &hBackBufferDC);
  createCompatibleBuffer(hSkinDC, bmp.bmWidth, bmp.bmHeight, &hFrontBufferBitmap, &hFrontBufferDC);
     
  return SKW_OK;
}

void SkinnedWindow::draw() {
  BITMAP fb_bmp;
  BITMAP bb_bmp;
  GetObject(hFrontBufferBitmap, sizeof(fb_bmp), &fb_bmp);
  GetObject(hBackBufferBitmap, sizeof(bb_bmp), &bb_bmp);
  
  if(hSkinDC) // Only blit, if skin has been loaded successfully
    BitBlt(hBackBufferDC, 0, 0, bb_bmp.bmWidth, bb_bmp.bmHeight, hSkinDC, 0, 0, SRCCOPY);
    
  TransparentBlt(hFrontBufferDC, 0, 0, bb_bmp.bmWidth, bb_bmp.bmHeight, hBackBufferDC, 
                                 0, 0, fb_bmp.bmWidth, fb_bmp.bmHeight, RGB(0x00, 0x00, 0x00));
  BitBlt(
    hFrontBufferDC, rcUserContent.left, rcUserContent.top, rcUserContent.right, rcUserContent.bottom, 
    hUserContentDC, 0, 0, SRCCOPY);
  BitBlt(hOutDC, 0, 0, bb_bmp.bmWidth, bb_bmp.bmHeight, hFrontBufferDC, 0, 0, SRCCOPY);
}

bool SkinnedWindow::makeWindow(
  const char* windowTitle, const char* windowClassName, int width, int height, WNDPROC dlgProc) {
  WNDCLASS wndWc;
  
  wndWc.style = CS_OWNDC;
  wndWc.lpfnWndProc = dlgProc;
  wndWc.cbClsExtra = 0;
  wndWc.cbWndExtra = 0;
  wndWc.hInstance = GetModuleHandle(NULL);
  wndWc.hIcon = NULL;
  wndWc.hCursor = LoadCursor(0, IDC_ARROW);
  wndWc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
  wndWc.lpszMenuName = NULL;
  wndWc.lpszClassName = windowClassName;
  
  if (!RegisterClass(&wndWc)) 
    return false;
  
  // get actual screen resolution
  int screenWidth  = (WORD)GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = (WORD)GetSystemMetrics(SM_CYSCREEN);
  
  // make a rectangle on the center of the screen
  RECT rc = { (screenWidth - width) / 2, (screenHeight - height) / 2, width, height };
  
  // Create the window without caption, borders etc...
  this->hWnd = CreateWindow(windowClassName, windowTitle,
            WS_POPUP,
            rc.left, rc.top, width, height,
            NULL, NULL, GetModuleHandle(NULL), NULL);	
  if(this->hWnd) {
    this->hInstance = wndWc.hInstance;
    this->hOutDC = GetDC(this->hWnd);

    ShowWindow(this->hWnd, SW_SHOW);
    return true;
  }
  else
    return false;
}

bool SkinnedWindow::makeSkinnedWindow(
  const char* windowTitle, const char* windowClassName,
  int width, int height, WNDPROC dlgProc,  int bitmapRessourceId, 
  int skinRegionRessourceId, const char* regionFileType,
  int xPosUserContent, int yPosUserContent, int widthUserContent, int heightUserContent
) {
  rcUserContent.left = xPosUserContent;
  rcUserContent.top = yPosUserContent;
  rcUserContent.right = widthUserContent;
  rcUserContent.bottom = heightUserContent;

  if (
    makeWindow(windowTitle, windowClassName, width, height, dlgProc) &&
    loadSkin(bitmapRessourceId) == SKW_OK &&
    loadRegion(skinRegionRessourceId, regionFileType) == SKW_OK
    )
    return true;
  else
    return false;
}

HWND SkinnedWindow::getHWnd() {
  return this->hWnd;
}

HDC SkinnedWindow::getUserContentHDC() {
  return this->hUserContentDC;
}
