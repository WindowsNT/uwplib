#include "cstdafx.h"
#include <functional>
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
#include ".\\xml\\xml3all.h"
using namespace XML3;
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

		case WM_SIZE:
		{
			RECT rc;
			GetClientRect(hh, &rc);
			SetWindowPos(GetDlgItem(hh, 901), 0, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);

			UWPLIB::UWPCONTROL* u = (UWPLIB::UWPCONTROL*)SendDlgItemMessage(hh, 901, UWPM_GET_CONTROL, 0, 0);
			if (u)
				SetWindowPos(u->hwndDetailXamlIsland, 0, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);

			return 0;
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

				auto sz = yy.length() * 4 + 1000;
				vector<char> d(sz);
				WideCharToMultiByte(CP_UTF8, 0, yy.c_str(), yy.length(), d.data(), sz, 0, 0);

				// Make sure the namespaces are there
				XML3::XML x;
				x.Parse(d.data(),strlen(d.data()));
				x.GetRootElement().vv("xmlns") = "http://schemas.microsoft.com/winfx/2006/xaml/presentation";
				x.GetRootElement().vv("xmlns:x") = "http://schemas.microsoft.com/winfx/2006/xaml";
				auto sr = x.Serialize();

				auto sz2 = sr.length() * 2 + 100;
				vector<wchar_t> d2(sz2);
				MultiByteToWideChar(CP_UTF8, 0, sr.c_str(), sr.length(), d2.data(), sz2);

				SetWindowText(GetDlgItem(hh, 901), d2.data());
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