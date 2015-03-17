#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "Msimg32.lib") // For TransparentBlt()

class SkinnedWindow {
public:
	SkinnedWindow();
	~SkinnedWindow();
	bool MakeWindow(int iWidth, int iHeight, WNDPROC DlgProc);
	bool SkinnedWindow::MakeSkinnedWindow(int iWidth, int iHeight, WNDPROC DlgProc, int BITMAP_ID, int SKINREGION_ID);
	int LoadSkin(int BITMAP_RESSOURCE, int SKINREGION_RESSOURCE);
	void Draw();
	void Clear_hDC();
	HWND gethWnd();
	HDC get_hDC();

private:
	HINSTANCE hInstance;
	HWND hWnd;
	HDC hOutDC;
	HBITMAP hSkinBitmap;
	HDC hSkinDC;
	HBITMAP hFrontbufferBitmap;
	HDC hFrontBufferDC;
	HBITMAP hBackBufferBitmap;
	HDC hBackBufferDC;
};
