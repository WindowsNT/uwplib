#include "cstdafx.h"
#include "uwplib.hpp"
#pragma comment(lib,"Comctl32.lib")
using namespace std;



wstring FromClip()
{
	vector<wchar_t> tt(10000000);
	wstring r;
	if (!OpenClipboard(GetDesktopWindow()))
		return r;

	HANDLE hG = GetClipboardData(CF_UNICODETEXT);
	void*p = GlobalLock(hG);
	if (p)
		wcscpy_s(tt.data(), 10000000, (wchar_t*)p);
	r = tt.data();
	GlobalUnlock(hG);
	CloseClipboard();
	return r;
}

INT_PTR  CALLBACK D_DP(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
{
	switch (mm)
	{
		case WM_CLOSE:
		{
			DestroyWindow(GetDlgItem(hh, 901));
			EndDialog(hh, 0);
			break;
		}

		case WM_COMMAND:
		{
			if (LOWORD(ww) == 101)
			{
				wstring yy = FromClip();
				if (OpenClipboard(hh))
				{
					CloseClipboard();
				}
				SetWindowText(GetDlgItem(hh, 901), yy.c_str());
			}
			return 0;
		}

		case WM_INITDIALOG:
		{
			const wchar_t* r = LR"(<Grid xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
		        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
<TextBlock Text="Please copy Xaml to clipboard, then select the Load Xaml from Clipboard menu" />
				</Grid>)";
			SetWindowText(GetDlgItem(hh, 901), r);
			return 0;
		}
	}

	return 0;
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
	
	winrt::init_apartment(apartment_type::single_threaded);
	WindowsXamlManager windowsXamlManager = WindowsXamlManager::InitializeForCurrentThread();
	UWPLIB::Register_Custom();

	DialogBox(0, L"DIALOG_TEST", 0, D_DP);


	return 0;
}