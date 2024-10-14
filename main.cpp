int windowWidth		= 1000;
int windowHeight	=  50;
int fontSize		= 40;

#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
int CenterWindowOnScreen_Y(int windowHeight);
int CenterWindowOnScreen_X(int windowWidth);
void DrawLineString(HWND hWnd, TCHAR* string, Color boderColor, Color fillColor, int flag); LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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
		CenterWindowOnScreen_X(windowWidth), // initial x position
		CenterWindowOnScreen_Y(windowHeight), // initial y position
		windowWidth, // initial x size
		windowHeight, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL); // creation parameters
	ShowWindow(hWnd, iCmdShow);
	TCHAR string[] = { 0 };
	wcscpy(string, L"这是一个中文长文");
	DrawLineString(hWnd, string, Color(255, 0, 255, 0), Color(255, 0, 255, 0),0);

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
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.lpfnWndProc = wndProc;
	wnd.lpszClassName = lpClassName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW;
	wnd.hInstance = hInstance;
	return RegisterClass(&wnd);

}

void DrawLineString(HWND hWnd, TCHAR* string, Color boderColor, Color fillColor,int flag)
{
	int					string_length = lstrlen(string);
	int					newWindowWidth = ((fontSize + 10) * string_length) * 2;
	int					newWindowHeight = fontSize + 10;
	int					x = 0;
	int					y = 0;
	HDC					hdc = GetDC(hWnd);
	HDC					memDC = CreateCompatibleDC(hdc);
	HBITMAP				bmp = CreateCompatibleBitmap(hdc, newWindowWidth, newWindowHeight);
	FontFamily			fontFamily(L"黑体");
	RectF				rectF(0, 0, newWindowWidth, newWindowHeight);
	StringFormat		stringFormat;
	GraphicsPath		strPath;
	Pen					pen(boderColor, 2);
	POINT				p2 = { 0,0 };
	SIZE				size = { newWindowWidth,newWindowHeight };
	BLENDFUNCTION		_blend;
	LinearGradientBrush LGBrush(Point(0, 10), Point(0, 0), fillColor, fillColor);
	

	SelectObject(memDC, bmp);
	Graphics			g(memDC);
	if (flag == 0) 
	{
		stringFormat.SetAlignment(StringAlignmentCenter);
		strPath.AddString(string, string_length, &fontFamily, FontStyleBold, fontSize, rectF, &stringFormat);
		x = CenterWindowOnScreen_X(newWindowWidth);
		y = CenterWindowOnScreen_Y(newWindowHeight);
	}

	

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.DrawPath(&pen, &strPath);
	g.FillPath(&LGBrush, &strPath);


	_blend.BlendFlags = 0;
	_blend.BlendOp = AC_SRC_OVER;
	_blend.SourceConstantAlpha = 255;
	_blend.AlphaFormat = AC_SRC_ALPHA;


	UpdateLayeredWindow(hWnd, hdc, NULL, &size, memDC, &p2, NULL, &_blend, ULW_ALPHA);
	SetWindowPos(hWnd, NULL, x, y, newWindowWidth, newWindowHeight, SWP_NOZORDER);
}

int CenterWindowOnScreen_X(int windowWidth)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	return (screenWidth - windowWidth) / 2;
}

int CenterWindowOnScreen_Y(int windowHeight)
{
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	return (screenHeight - windowHeight) / 2;
}