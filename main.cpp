int windowWidth		= 1000;
int windowHeight	= 100;
int fontSize		= 50;


#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

void DrawString(HWND hWnd, TCHAR* string, Color boderColor, Color fillColor);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM registeWndClass(HINSTANCE hInstance, LPCWSTR lpClassName, WNDPROC wndProc, DWORD dwColor);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND hWnd;
	MSG msg;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	registeWndClass(hInstance, TEXT("GettingStarted"), WndProc, WHITE_BRUSH);

	hWnd = CreateWindowEx(
		WS_EX_LAYERED,
		TEXT("GettingStarted"), // window class name
		TEXT("Getting Started"), // window caption
		WS_OVERLAPPEDWINDOW, // window style
		CW_USEDEFAULT, // initial x position
		CW_USEDEFAULT, // initial y position
		windowWidth, // initial x size
		windowHeight, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL); // creation parameters
	ShowWindow(hWnd, iCmdShow);
	TCHAR string[] = { 0 };
	wcscpy(string, L"这是一个中文长文本长长长长长长长长长长长长长长长长长长长长");
	DrawString(hWnd, string, Color(255, 255, 0, 0), Color(255, 0, 255, 0));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
} // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int count = 0;

	TCHAR count_str[256];
	HDC hdc = GetDC(hWnd);
	Graphics g(hdc);

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_DESTROY:
		wsprintf(count_str, L"%d", count);
		MessageBox(0, count_str, 0, 0);
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

ATOM registeWndClass(HINSTANCE hInstance, LPCWSTR lpClassName, WNDPROC wndProc, DWORD dwColor)
{
	WNDCLASS wnd;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)(GetStockObject(dwColor));
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadCursor(NULL, IDI_APPLICATION);
	wnd.lpfnWndProc = wndProc;
	wnd.lpszClassName = lpClassName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW;
	wnd.hInstance = hInstance;
	return RegisterClass(&wnd);

}

void DrawString(HWND hWnd, TCHAR* string, Color boderColor, Color fillColor)
{
	HDC					hdc = GetDC(hWnd);
	HDC					memDC = CreateCompatibleDC(hdc);
	HBITMAP				bmp = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	FontFamily			fontFamily(L"黑体");
	RectF				rectF(REAL(0), REAL(0), REAL(windowWidth), REAL(windowHeight));
	StringFormat		stringFormat;
	GraphicsPath		strPath;
	Pen					pen(boderColor, 2);
	Rect				rect;
	POINT				p2 = { 0,0 };
	SIZE				size = { windowWidth,windowHeight };
	BLENDFUNCTION		_blend;
	LinearGradientBrush LGBrush(Point(rect.Width / 2, 6), Point(rect.Width / 2, rect.Height), fillColor, fillColor);
	int string_length	= lstrlen(string);

	SelectObject(memDC, bmp);
	Graphics			g(memDC);

	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetLineAlignment(StringAlignmentCenter);


	strPath.AddString(string, string_length, &fontFamily, FontStyleBold, fontSize, rectF, &stringFormat);


	
	SetWindowPos(hWnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, fontSize * string_length, fontSize, SWP_NOZORDER | SWP_NOMOVE);

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.DrawPath(&pen, &strPath);
	g.FillPath(&LGBrush, &strPath);
	

	_blend.BlendFlags = 0;
	_blend.BlendOp = AC_SRC_OVER;
	_blend.SourceConstantAlpha = 255;
	_blend.AlphaFormat = AC_SRC_ALPHA;
	

	UpdateLayeredWindow(hWnd, hdc, NULL, &size, memDC, &p2, NULL, &_blend, ULW_ALPHA);
}

