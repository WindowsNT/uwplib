#include "stdafx.h"

#pragma comment(lib,"windowsapp.lib")

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

extern "C" void UWPInit()
{
	UWPLIB::Register();
}

#include "..\\xml\\xml3all.h"
#include "..\\mt\\rw.hpp"

void MenuLoop(HMENU hm,XML3::XMLElement& e,std::vector<short>& ids)
{
	int nc = GetMenuItemCount(hm);
	for (int i = 0; i < nc; i++)
	{
		wchar_t a[100] = {};
		MENUITEMINFO mi = {  };
		mi.cbSize = sizeof(mi);
		mi.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_SUBMENU;
		mi.dwTypeData = a;
		mi.cch = 100;
		GetMenuItemInfo(hm, i, true, &mi);
		if (mi.hSubMenu)
		{
			// 		<MenuBarItem Title="File">
			auto& e2 = e.AddElement("MenuBarItem");
			e2.vv("Title").SetWideValue(a);
			MenuLoop(mi.hSubMenu, e2,ids);
			continue;
		}
		if (mi.fType == MFT_SEPARATOR)
		{
			e.AddElement("MenuFlyoutSeparator");
			continue;
		}
		if (mi.fType == MFT_STRING)
		{
			auto& e2 = e.AddElement("MenuFlyoutItem");
			e2.vv("Text").SetWideValue(a);
			swprintf_s(a, 100, L"Id%u", mi.wID);
			e2.vv("Name").SetWideValue(a);
			ids.push_back(mi.wID);
		}
	}
}

std::string Menu2Xaml(HMENU hm, std::vector<short>& ids)
{
	XML3::XML x;
	x.GetRootElement().SetElementName("MenuBar");
	x.GetRootElement().vv("xmlns") = "http://schemas.microsoft.com/winfx/2006/xaml/presentation";
	x.GetRootElement().vv("xmlns:x") = "http://schemas.microsoft.com/winfx/2006/xaml";

	MenuLoop(hm,x.GetRootElement(),ids);

	return x.Serialize();
}

extern "C" void CreateUWPMenu(HWND hParent,short ID,HMENU hm,HWND* h1,HWND* h2)
{
	std::vector<short> ids;
	auto xml = Menu2Xaml(hm,ids);
	auto hX = CreateWindowEx(0, L"UWP_Custom", L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, (HMENU)ID, 0, 0);
	SetWindowTextA(hX, xml.c_str());

	UWPLIB::UWPCONTROL* cust = (UWPLIB::UWPCONTROL*)SendMessage(hX, UWPM_GET_CONTROL, 0, 0);

	for (auto& id : ids)
	{
		wchar_t a[100];
		swprintf_s(a, 100, L"Id%u", id);
		auto& mfi = cust->ins.as<MenuBar>().FindName(a).as<MenuFlyoutItem>();
		mfi.Tag(box_value((unsigned long long)hParent));

		mfi.Click([](const IInspectable& sender, const RoutedEventArgs&)
			{
				auto name = sender.as<MenuFlyoutItem>().Name();
				auto n2 = name.c_str();
				n2 += 2;
				int id = _wtoi(n2);
				auto u = unbox_value<unsigned long long>(sender.as<MenuFlyoutItem>().Tag());
				PostMessage((HWND)u, WM_COMMAND, id, 0);

			});
	}
	*h1 = hX;
	*h2 = cust->hwndDetailXamlIsland;
}