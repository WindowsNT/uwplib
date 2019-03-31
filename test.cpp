#include "stdafx.h"


#pragma comment(lib, "windowsapp")

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Core;
using namespace Windows::UI::Text;
using namespace Windows::UI::Input::Inking;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Media::Core;
using namespace Windows::UI::Xaml::Markup;






#include "uwplib.hpp"



HWND MainWindow = 0;
using namespace UWPLIB;
INT_PTR CALLBACK DP(HWND hh, UINT mm, WPARAM, LPARAM ll)
{
	switch (mm)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)GetStockObject(WHITE_BRUSH);

	case WM_NOTIFY:
	{
		NMHDR* n = (NMHDR*)ll;
		if (n->hwndFrom == GetDlgItem(hh, 901))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_RATING_CHANGE)
			{
				// Changed value
				int r = (int)SendDlgItemMessage(hh, 901, UWPM_RATING_GET, 0, 0);
				wchar_t txt[100] = { 0 };
				swprintf_s(txt, 100, L"Rating changed to %u", r);
				SetWindowText(hh, txt);
			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 903))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_HYPERLINKBUTTON_CLICKED)
			{
				// Changed value
				SetWindowText(hh, L"URL Clicked");
			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 904))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_TIMEPICK_CHANGED)
			{
				// Changed value
				int r = (int)SendDlgItemMessage(hh, 904, UWPM_TIMEPICK_GET, 0, 0);
				wchar_t txt[100] = { 0 };
				swprintf_s(txt, 100, L"Time changed to %u", r);
				SetWindowText(hh, txt);
			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 905))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_TOGGLESWITCH_CHANGE)
			{
				// Changed value
				int r = (int)SendDlgItemMessage(hh, 905, UWPM_TOGGLESWITCH_GET, 0, 0);
				wchar_t txt[100] = { 0 };
				swprintf_s(txt, 100, L"Switch changed to %u", r);
				SetWindowText(hh, txt);

			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 907))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_CALENDARVIEW_CHANGED)
			{
				// Changed value
				std::vector<FILETIME> ft;
				int szf = (int)SendDlgItemMessage(hh, 907, UWPM_CALENDARVIEW_GET, 0, (LPARAM)0);
				ft.resize(szf);
				SendDlgItemMessage(hh, 907, UWPM_CALENDARVIEW_GET, 0, (LPARAM)ft.data());
				wchar_t txt[100] = { 0 };
				swprintf_s(txt, 100, L"Calendar View changed (%u items selected)", szf);
				SetWindowText(hh, txt);
			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 908))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_CALENDARPICK_CHANGED)
			{
				// Changed value
				FILETIME ft;
				SendDlgItemMessage(hh, 907, UWPM_CALENDARVIEW_GET, 0, (LPARAM)&ft);
				wchar_t txt[100] = { 0 };
				swprintf_s(txt, 100, L"Calendar Date Picker changed");
				SetWindowText(hh, txt);
			}
		}
		if (n->hwndFrom == GetDlgItem(hh, 909))
		{
			UWPNOTIFICATION* nn = (UWPNOTIFICATION*)n;
			if (nn->n.code == UWPN_CHECKBOX_CHANGED)
			{
				SetWindowText(hh, L"Checkbox Changed");
			}
		}
		return 0;
	}
	case WM_INITDIALOG:
	{
		MainWindow = hh;
		SendMessage(hh, WM_SIZE, 0, 0);
		UWPCONTROL* s = (UWPCONTROL*)SendMessage(GetDlgItem(hh, 901), UWPM_GET_CONTROL, 0, 0);
		if (s)
		{
			auto ctrl = s->ins.as<RatingControl>();
			ctrl.MaxRating(10);
		}
		// Rating
		SendMessage(GetDlgItem(hh, 901), UWPM_RATING_SET, 2, 0);

		// Progress Ring
		SendMessage(GetDlgItem(hh, 902), UWPM_PROGRESSRING_ACTIVATE, 1, 0);

		// Hyperlink
		SetWindowText(GetDlgItem(hh, 903), L"Click me");

		// Toggle
		SendMessage(GetDlgItem(hh, 905), UWPM_TOGGLESWITCH_SET, 1, 0);

		// Calendar
		SendMessage(GetDlgItem(hh, 907), UWPM_CALENDARVIEW_SETSM, 1, 0);
		FILETIME fT;
		SYSTEMTIME sT;
		GetLocalTime(&sT);
		sT.wDay--;
		SystemTimeToFileTime(&sT, &fT);
		SendDlgItemMessage(hh, 907, UWPM_CALENDARVIEW_SET, 1, (LPARAM)&fT);

		// CheckBox
		SetWindowText(GetDlgItem(hh, 909), L"C_Test");

		// Progress
		SendDlgItemMessage(hh, 911, UWPM_PROGRESSBAR_SETPARAMS, 50, 0);
		SendDlgItemMessage(hh, 911, UWPM_PROGRESSBAR_SETPARAMS, 0, 0);

		// Custom
		SetWindowText(GetDlgItem(hh, 910), LR"(<Pivot xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml" Title="CUSTOM CONTROL - NOW USING PIVOT">
    <PivotItem Header="All">
        <TextBlock Text="all emails go here." />
    </PivotItem>
    <PivotItem Header="Unread">
        <TextBlock Text="unread emails go here." />
    </PivotItem>
    <PivotItem Header="Flagged">
        <TextBlock Text="flagged emails go here." />
    </PivotItem>
    <PivotItem Header="Urgent">
        <TextBlock Text="urgent emails go here." />
    </PivotItem>
</Pivot>)");
		UWPCONTROL* cust = (UWPCONTROL*)SendDlgItemMessage(hh, 910, UWPM_GET_CONTROL, 0, 0);
		cust->ins.as<Pivot>().SelectionChanged([](const IInspectable&  sender, const RoutedEventArgs&)
		{
			auto na = sender.as<Pivot>().Name();

			UWPCONTROL* u = 0;
			UWPLIB::GetControlByName(na.c_str(), &u);
			if (!u)
				return 0;

			SetWindowText(MainWindow, L"Pivot Changed");
			return 1;
		});

		break;
	}
	case WM_CLOSE:
	{
		for (int i = 901; i <= 940; i++)
			DestroyWindow(GetDlgItem(hh, i));
		EndDialog(hh, 0);
		break;
	}
	}
	return 0;
}


int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WSADATA wData;
	WSAStartup(MAKEWORD(2, 2), &wData);
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	winrt::init_apartment(apartment_type::single_threaded);
	WindowsXamlManager windowsXamlManager = WindowsXamlManager::InitializeForCurrentThread();

	INITCOMMONCONTROLSEX icex = { 0 };
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_DATE_CLASSES | ICC_WIN95_CLASSES;
	icex.dwSize = sizeof(icex);
	InitCommonControlsEx(&icex);
	InitCommonControls();

	UWPLIB::Register();
	DialogBox(GetModuleHandle(0), L"DIALOG_TEST2", 0, DP);
	return 0;
}

