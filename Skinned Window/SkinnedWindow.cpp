#include "SkinnedWindow.h"

SkinnedWindow::SkinnedWindow() {
	hWnd			          = 0;
	hInstance		        = 0;
	hOutDC			        = 0;
	hSkinBitmap		      = 0;
	hSkinDC			        = 0;
	hFrontbufferBitmap	= 0;
	hFrontBufferDC		  = 0;
	hBackBufferBitmap	  = 0;
	hBackBufferDC		    = 0;
}

SkinnedWindow::~SkinnedWindow() {
	if(hOutDC)
		ReleaseDC(hWnd, hOutDC);

	if(hSkinDC)
		DeleteDC(hSkinDC);

	if(hFrontBufferDC)
		DeleteDC(hFrontBufferDC);
}

int SkinnedWindow::LoadSkin(int BITMAP_ID, int SKINREGION_ID) {
	hSkinBitmap = LoadBitmap(this->hInstance, MAKEINTRESOURCE(BITMAP_ID));
	if (!hSkinBitmap)
		return 1; // Error loading skin
	
	hSkinDC = CreateCompatibleDC(NULL);
	HBITMAP hOldBmp = SelectBitmap(hSkinDC, hSkinBitmap);
	DeleteObject(hOldBmp);

	BITMAP bmp;
	GetObject(hSkinBitmap, sizeof(bmp), &bmp);

	// Adjust window size to skin dimensions
	int iSw = (WORD)GetSystemMetrics(SM_CXSCREEN);  // width
	int iSh = (WORD)GetSystemMetrics(SM_CYSCREEN);  // height
	MoveWindow(hWnd, (iSw - bmp.bmWidth)/2, (iSh - bmp.bmHeight) / 2, bmp.bmWidth, bmp.bmHeight, true);

	DeleteObject(hFrontbufferBitmap);
	DeleteDC(hFrontBufferDC);
	DeleteObject(hBackBufferBitmap);
	DeleteDC(hBackBufferDC);

	hFrontbufferBitmap = CreateCompatibleBitmap(hSkinDC, bmp.bmWidth, bmp.bmHeight);
	hFrontBufferDC = CreateCompatibleDC(NULL);
	hOldBmp = SelectBitmap(hFrontBufferDC, hFrontbufferBitmap);
	DeleteObject(hOldBmp);

	hBackBufferBitmap = CreateCompatibleBitmap(hSkinDC, bmp.bmWidth, bmp.bmHeight);
	hBackBufferDC = CreateCompatibleDC(NULL);
	hOldBmp = SelectBitmap(hBackBufferDC, hBackBufferBitmap);
	DeleteObject(hOldBmp);

	// Load region
	if(MAKEINTRESOURCE(SKINREGION_ID) == 0)
		return 0;

	// Ask resource for our skin.
	HRSRC hrSkin = FindResource(hInstance, MAKEINTRESOURCE(SKINREGION_ID), "BINARY");
	if (!hrSkin) 
		return 3;

	// This is standard "BINARY" retrieval.
	LPRGNDATA pSkinData = (LPRGNDATA)LoadResource(hInstance, hrSkin);
	if (!pSkinData)
		return 4;

	// Create the region using the binary data.
	HRGN rgnSkin = ExtCreateRegion(NULL, SizeofResource(NULL,hrSkin), pSkinData);

	// Free the allocated resource
	FreeResource(pSkinData);

	// Check if we have the skin at hand.
	if (!rgnSkin) 
		return 5; // Error reading region file

	SetWindowRgn(hWnd, rgnSkin, true);

	return 0;
}

void SkinnedWindow::Draw() {
	BITMAP fb_bmp;
	BITMAP bb_bmp;
	GetObject(hFrontbufferBitmap, sizeof(fb_bmp), &fb_bmp);
	GetObject(hBackBufferBitmap, sizeof(bb_bmp), &bb_bmp);

	// If skin has been loaded successfully
	if(hSkinDC)
		BitBlt(hBackBufferDC,0,0,bb_bmp.bmWidth, bb_bmp.bmHeight, hSkinDC,0,0,SRCCOPY);

	TransparentBlt(hBackBufferDC,0,0,bb_bmp.bmWidth,bb_bmp.bmHeight, hFrontBufferDC,0,0,fb_bmp.bmWidth, fb_bmp.bmHeight, RGB(0x00, 0x00, 0x00));
	BitBlt(hOutDC,0,0,bb_bmp.bmWidth,bb_bmp.bmHeight,hBackBufferDC,0,0,SRCCOPY);
}

// ------------------------------------------------------------------------
// A Basic, still smart window creation function.
// ------------------------------------------------------------------------
bool SkinnedWindow::MakeWindow(int iWidth, int iHeight, WNDPROC DlgProc) {
	// our window class
	WNDCLASS wndWc;
	
	// ---------------------------------------------------------
	// fill window class members
	// ---------------------------------------------------------
	wndWc.style = CS_OWNDC;
	wndWc.lpfnWndProc = DlgProc;
	wndWc.cbClsExtra = 0;
	wndWc.cbWndExtra = 0;
	wndWc.hInstance = GetModuleHandle(NULL);
	wndWc.hIcon = NULL;
	wndWc.hCursor = LoadCursor(0, IDC_ARROW);
	wndWc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndWc.lpszMenuName = NULL;
	wndWc.lpszClassName = "Coonami Entertainment";
	
	// register class
	if (!RegisterClass(&wndWc)) return false;
	// ---------------------------------------------------------
	
	// get actual screen resolution
	int iSw = (WORD)GetSystemMetrics(SM_CXSCREEN);       // width
	int iSh = (WORD)GetSystemMetrics(SM_CYSCREEN);       // height
	
	// make a rectangle on the center of the screen
	RECT rc = { (iSw - iWidth)/2, (iSh - iHeight)/2, iWidth, iHeight };
	
	// create the window.
	// note the WS_POPUP flag, no caption, no borders, no nothing.
		
  this->hWnd = CreateWindow(wndWc.lpszClassName, "Floppy Orgel Emulator",
						WS_POPUP,
						rc.left,rc.top, iWidth,iHeight,
						NULL, NULL, GetModuleHandle(NULL), NULL);	
	if(this->hWnd) {
		this->hInstance = wndWc.hInstance;
		this->hOutDC = GetDC(this->hWnd);

		//Backbuffer initialisieren
		this->hFrontbufferBitmap = CreateCompatibleBitmap(hOutDC, iWidth, iHeight);
		this->hFrontBufferDC = CreateCompatibleDC(hOutDC);
		HBITMAP hOldBmp = SelectBitmap(hFrontBufferDC, hFrontbufferBitmap);
		DeleteObject(hOldBmp);
			
		ShowWindow(this->hWnd, SW_SHOW);

		return true;
	}
	else
		return false;
}

bool SkinnedWindow::MakeSkinnedWindow(int iWidth, int iHeight, WNDPROC DlgProc, int BITMAP_ID, int SKINREGION_ID) {
	if(MakeWindow(iWidth, iHeight, DlgProc) && !LoadSkin(BITMAP_ID, SKINREGION_ID))
		return true;
	else
		return false;
}

// TODO: Test this!
void SkinnedWindow::Clear_hDC() {
	BITMAP bmp;
	GetObject(hFrontbufferBitmap, sizeof(bmp), &bmp);

	HBRUSH MyBrush = CreateSolidBrush( RGB(0,0,0) );
	HGDIOBJ original = SelectObject(hFrontBufferDC, MyBrush );
	Rectangle(hFrontBufferDC, 0, 0, bmp.bmWidth, bmp.bmHeight);

	SelectObject(hFrontBufferDC, original);
	DeleteObject(MyBrush);
}

HWND SkinnedWindow::gethWnd() {
	return this->hWnd;
}


HDC SkinnedWindow::get_hDC() {
	return this->hFrontBufferDC;
}
