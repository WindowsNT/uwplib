#include "teststdafx.h"

HWND MainWindow = 0;
HINSTANCE hAppInstance = 0;
HICON hIcon1 = 0;

// ----
const TCHAR* ttitle = _T("App");
// ----
extern "C" void UWPInit();
extern "C" void CreateUWPMenu(HWND hParent, short ID, HMENU m, HWND * h1, HWND * h2);


#pragma comment(lib,"uwpdll.lib")
HWND hX = 0;
HWND hXaml = 0;
LRESULT CALLBACK Main_DP(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
	{
	switch (mm)
		{
		case WM_CREATE:
			{
			CreateUWPMenu(hh,910,LoadMenu(GetModuleHandle(0),L"MENU_1"),&hX,&hXaml);
			SendMessage(hh, WM_SIZE, 0, 0);
			break;
			}

		case WM_COMMAND:
		{
			int lw = LOWORD(ww); 
			if (lw == 101) MessageBox(hh, L"New", L"Command", 0);
			if (lw == 102) MessageBox(hh, L"Save", L"Command", 0);
			if (lw == 103) MessageBox(hh, L"Load", L"Command", 0);
			if (lw == 104) MessageBox(hh, L"About", L"Command", 0);
			if (lw == 199) SendMessage(hh, WM_CLOSE, 0, 0);;
		}
		case WM_SIZE:
		{
			RECT rc;
			GetClientRect(hh, &rc);
			SetWindowPos(hXaml, 0, 5, 0, rc.right, 55,SWP_SHOWWINDOW);
			return 0;
		}

		case WM_CLOSE:
			{
			DestroyWindow(hX);
			DestroyWindow(hh);
			return 0;
			}

		case WM_DESTROY:
			{
			PostQuitMessage(0);
			return 0;
			}
		}
	return DefWindowProc(hh, mm, ww, ll);
	}



int __stdcall WinMain(HINSTANCE h, HINSTANCE, LPSTR, int)
	{
	WSADATA wData;
	WSAStartup(MAKEWORD(2, 2), &wData);
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	INITCOMMONCONTROLSEX icex = { 0 };
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_DATE_CLASSES | ICC_WIN95_CLASSES;
	icex.dwSize = sizeof(icex);
	InitCommonControlsEx(&icex);
	InitCommonControls();
	hIcon1 = LoadIcon(h, _T("ICON_1"));

	hAppInstance = h;

	WNDCLASSEX wClass = { 0 };
	wClass.cbSize = sizeof(wClass);

	wClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	wClass.lpfnWndProc = (WNDPROC)Main_DP;
	wClass.hInstance = h;
	wClass.hIcon = hIcon1;
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wClass.lpszClassName = _T("CLASS");
	wClass.hIconSm = hIcon1;
	RegisterClassEx(&wClass);

	// DLL
	UWPInit();
	MainWindow = CreateWindowEx(0,
		_T("CLASS"),
		ttitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, h, 0);

	ShowWindow(MainWindow, SW_SHOW);


	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return 0;
}