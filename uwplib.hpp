
namespace UWPLIB
{
	using namespace std;
	using namespace winrt::Windows::UI::Xaml::Hosting;
	using namespace winrt::Windows::Foundation::Collections;
	using namespace winrt::Windows::UI::Xaml::Markup;

#pragma pack(push,1)
	struct UWPCONTROL
	{
		DesktopWindowXamlSource xs;
		HWND hParent = 0;
		HWND hwnd = 0;
		HWND hwndDetailXamlIsland = 0;
		winrt::Windows::Foundation::IInspectable ins;
	};

	struct UWPNOTIFICATION
	{
		NMHDR n = { 0 };
		UWPCONTROL* s = 0;
	};
#pragma pack(pop)

#define UWPM_GET_CONTROL (WM_USER + 100)


#ifndef UWPLIB_CUSTOMONLY

#define UWPM_RATING_SET (WM_USER + 101)
#define UWPM_RATING_GET (WM_USER + 102)
#define UWPN_RATING_CHANGE (1)

#define UWPM_PROGRESSRING_ACTIVATE (WM_USER + 101)

#define UWPN_HYPERLINKBUTTON_CLICKED (1)

#define UWPM_TIMEPICK_SET (WM_USER + 101)
#define UWPM_TIMEPICK_GET (WM_USER + 102)
#define UWPN_TIMEPICK_CHANGED (1)

#define UWPM_TOGGLESWITCH_SET (WM_USER + 101)
#define UWPM_TOGGLESWITCH_GET (WM_USER + 102)
#define UWPN_TOGGLESWITCH_CHANGE (1)

#define UWPM_IMAGE_SETSOURCE (WM_USER + 101)

#define UWPM_CALENDARVIEW_SET (WM_USER + 101)
#define UWPM_CALENDARVIEW_GET (WM_USER + 102)
#define UWPM_CALENDARVIEW_SETSM (WM_USER + 103)
#define UWPM_CALENDARVIEW_GETSM (WM_USER + 104)
#define UWPN_CALENDARVIEW_CHANGED (1)


#define UWPM_CALENDARPICK_SET (WM_USER + 101)
#define UWPM_CALENDARPICK_GET (WM_USER + 102)
#define UWPN_CALENDARPICK_CHANGED (1)

#define UWPM_CHECKBOX_SET (WM_USER + 101)
#define UWPM_CHECKBOX_GET (WM_USER + 102)
#define UWPN_CHECKBOX_CHANGED (1)

#define UWPM_PROGRESSBAR_SETPARAMS (WM_USER + 101)
#endif

	// ---

#include ".\\mt\\rw.hpp"
	inline tlock<map<wstring, UWPCONTROL*>> controls;

	inline void GenericDestroy(HWND hh)
	{
		UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
		if (s)
		{
			controls.writelock([&](map<wstring, UWPCONTROL*>& m) {
				auto str = s->ins.as<IFrameworkElement>().Name();
				m.erase(str.c_str());
			});
			RemoveProp(hh, L"s");
			delete s;
			s = 0;
		}
	}

	template <typename T>
	void GenericNotify(IInspectable const&  sender, int Code)
	{
		auto rt = sender.as<T>();
		auto na = rt.Name();
		UWPCONTROL* s = 0;
		controls.readlock([&](const map<wstring, UWPCONTROL*>& m) {
			s = m.at(na.c_str());
		});
		if (!s)
			return; // Duh
		UWPNOTIFICATION n;
		n.n.hwndFrom = s->hwnd;
		n.n.code = Code;
		n.n.idFrom = GetDlgCtrlID(s->hwnd);
		n.s = (UWPCONTROL*)s;
		SendMessage(s->hParent, WM_NOTIFY, 0, (LPARAM)&n);

	}

	inline bool GetControlByName(const wchar_t* n, UWPCONTROL** ptr)
	{
		if (!n || !ptr)
			return false;
		UWPCONTROL* s = 0;
		controls.readlock([&](const map<wstring, UWPCONTROL*>& m) {
			s = m.at(n);
		});
		if (!s)
			return false;
		*ptr = s;
		return true;
	}




	inline UWPCONTROL* GenericCreate(HWND hP, HWND hh, LPVOID cs)
	{
		auto s = new UWPCONTROL;
		SetProp(hh, L"s", (HANDLE)s);
		s->hwnd = hh;
		s->hParent = hP;
		auto interopDetail = s->xs.as<IDesktopWindowXamlSourceNative>();
		auto hr = E_FAIL;

		// Generate the string if not there
		const wchar_t* strx = (const wchar_t*)cs;

		vector<wchar_t> tstr(10000);
#ifndef UWPLIB_CUSTOMONLY
		if (!strx)
		{
			wchar_t cn[100] = { 0 };
			GetClassName(hh, cn, 100);
			wchar_t wn[100] = { 0 };
			GetWindowText(hh, wn, 100);
			wstring ccn = cn;

			if (wcslen(wn) == 0)
			{
				CLSID ci;
				UuidCreate(&ci);
				wchar_t cn2[100] = { 0 };
				StringFromGUID2(ci, cn2, 100);
				cn2[wcslen(cn2) - 1] = 0;
				wcscpy_s(wn, 100, cn2 + 1);
			}

			if (ccn == L"UWP_Rating")
			{
				swprintf_s(tstr.data(), 10000, LR"(<RatingControl	xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation" xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml" 
					Name="%s" 
					IsClearEnabled="False" 
					IsReadOnly="False" 
					 />)", wn);
				strx = tstr.data();
			}

			if (ccn == L"UWP_ProgressRing")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
    <ProgressRing xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  
/>
)", wn);
				strx = tstr.data();
			}

			if (ccn == L"UWP_HyperlinkButton")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
    <HyperlinkButton xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />
)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_CalendarView")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
    <CalendarView xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />
)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_CalendarDatePicker")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
    <CalendarDatePicker xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />
)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_InkCanvas")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
<Grid  xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
Name="GridTop_%s"
>
    <InkCanvas 
			Name="%s" />

</Grid>
)", wn, wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_Timepicker")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <TimePicker xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_Colorpicker")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <ColorPicker xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_ToggleSwitch")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <ToggleSwitch xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_CheckBox")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <CheckBox xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_ProgressBar")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <ProgressBar xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
			if (ccn == L"UWP_Image")
			{
				swprintf_s(tstr.data(), 10000,
					LR"(
  <Image xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
			Name="%s"  />


)", wn);
				strx = tstr.data();
			}
		}
#endif

		if (strx)
			hr = interopDetail->AttachToWindow(hh);
		if (!strx || FAILED(hr))
		{
			delete s;
			RemoveProp(hh, L"s");
			SetWindowLongPtr(hh, GWLP_USERDATA, 0);
			return 0;
		}
		try
		{
			interopDetail->get_WindowHandle(&s->hwndDetailXamlIsland);
			winrt::param::hstring str(strx);
			s->ins = XamlReader::Load(str);
			s->xs.Content(s->ins.as<UIElement>());
		}
		catch (...)
		{
			delete s;
			RemoveProp(hh, L"s");
			SetWindowLongPtr(hh, GWLP_USERDATA, 0);
			return 0;
		}

		controls.writelock([&](map<wstring, UWPCONTROL*>& m) {
			auto str = s->ins.as<IFrameworkElement>().Name();
			m[str.c_str()] = s;
		});

		RECT rc = { 0 };
		GetClientRect(hh, &rc);
		MapWindowPoints(hh, hP, (LPPOINT)&rc, 2);
		//	ShowWindow(hh, SW_HIDE);
		//	SetWindowPos(s->hwnd, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		SetWindowPos(s->hwndDetailXamlIsland, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);


		GetClientRect(hP, &rc);
		SetWindowPos(s->hwnd, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		//SetWindowPos(s->hwndDetailXamlIsland, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
		return s;
	}

	inline ATOM Register_Custom()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_SETTEXT:
			{
				GenericDestroy(hh);
				HWND hP = GetParent(hh);
				s = GenericCreate(hP, hh, (LPVOID)ll);
				if (!s)
					return 0;
				return 1;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_Custom";
		return RegisterClassExW(&wC);
	}

#ifndef UWPLIB_CUSTOMONLY
	inline ATOM Register_Rating()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case UWPM_RATING_SET:
			{
				auto ctrl = s->ins.as<RatingControl>();
				ctrl.Value((double)ww);
				return 0;
			}
			case UWPM_RATING_GET:
			{
				auto ctrl = s->ins.as<RatingControl>();
				return (LRESULT)ctrl.Value();
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto ctrl = s->ins.as<RatingControl>();
				TypedEventHandler<RatingControl, IInspectable> lu = [](const RatingControl& sender, const IInspectable&)
				{
					GenericNotify<RatingControl>(sender, UWPN_RATING_CHANGE);
				};
				ctrl.ValueChanged(lu);
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_Rating";
		return RegisterClassExW(&wC);
	}

	inline ATOM Register_ProgressRing()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_ERASEBKGND:
				return 1;
			case UWPM_PROGRESSRING_ACTIVATE:
			{
				// ww 0 no, 1 yes
				auto ctrl = s->ins.as<ProgressRing>();
				ctrl.IsActive((bool)ww);
				return 0;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_ProgressRing";
		return RegisterClassExW(&wC);
	}


	inline ATOM Register_HyperlinkButton()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_SETTEXT:
			{
				auto ctrl = s->ins.as<HyperlinkButton>();
				ctrl.Content(winrt::box_value((wchar_t*)ll));
				return 0;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto ctrl = s->ins.as<HyperlinkButton>();
				ctrl.Click([](const IInspectable&  sender, const RoutedEventArgs&)
				{
					GenericNotify<HyperlinkButton>(sender, UWPN_HYPERLINKBUTTON_CLICKED);
				});
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_HyperlinkButton";
		return RegisterClassExW(&wC);
	}

	inline ATOM Register_InkCanvas()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;

				auto grd = s->ins.as<Grid>();
				wstring nz = grd.Name().c_str();
				nz.erase(nz.begin(), nz.begin() + 8);// Remove "GridTop_"
				auto c2 = grd.FindName(nz.c_str());

				auto c = c2.as<InkCanvas>();
				auto pr = c.InkPresenter();
				pr.InputDeviceTypes(winrt::Windows::UI::Core::CoreInputDeviceTypes::Mouse | winrt::Windows::UI::Core::CoreInputDeviceTypes::Pen | winrt::Windows::UI::Core::CoreInputDeviceTypes::Touch);


				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_InkCanvas";
		return RegisterClassExW(&wC);
	}


	// 



	inline ATOM Register_Colorpicker()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}

			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}

			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				//		auto tpick = s->ins.as<ColorPicker>();
				//		tpick.onch
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_Colorpicker";
		return RegisterClassExW(&wC);
	}

	// 



	inline ATOM Register_Timepicker()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}

			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case UWPM_TIMEPICK_SET:
			{
				auto tpick = s->ins.as<TimePicker>();
				std::chrono::seconds s2(ll);
				TimeSpan sp(s2);
				tpick.SelectedTime(sp);
				return 0;
			}
			case UWPM_TIMEPICK_GET:
			{
				auto tpick = s->ins.as<TimePicker>();
				auto d = tpick.SelectedTime();
				auto int_ms = std::chrono::duration_cast<std::chrono::seconds>(d.GetTimeSpan());
				return int_ms.count();
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto tpick = s->ins.as<TimePicker>();

				tpick.TimeChanged([](IInspectable const&  sender, TimePickerValueChangedEventArgs const&)
				{
					GenericNotify<TimePicker>(sender, UWPN_TIMEPICK_CHANGED);
				});
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_Timepicker";
		return RegisterClassExW(&wC);
	}


	inline ATOM Register_ToggleSwitch()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case UWPM_TOGGLESWITCH_SET:
			{
				auto ctrl = s->ins.as<ToggleSwitch>();
				ctrl.IsOn((bool)ww);
				return 0;
			}
			case UWPM_TOGGLESWITCH_GET:
			{
				auto ctrl = s->ins.as<ToggleSwitch>();
				return (LRESULT)ctrl.IsOn();
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto ctrl = s->ins.as<ToggleSwitch>();
				ctrl.Toggled([](const IInspectable&  sender, const RoutedEventArgs&)
				{
					GenericNotify<ToggleSwitch>(sender, UWPN_TOGGLESWITCH_CHANGE);
				});
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_ToggleSwitch";
		return RegisterClassExW(&wC);
	}


	inline ATOM Register_CheckBox()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_SETTEXT:
			{
				auto ctrl = s->ins.as<CheckBox>();
				ctrl.Content(winrt::box_value((wchar_t*)ll));
				return 0;
			}
			case UWPM_CHECKBOX_SET:
			{
				auto ctrl = s->ins.as<CheckBox>();
				if (ww == 2)
				{
					ctrl.IsThreeState(true);
				}
				else
				{
					ctrl.IsThreeState(false);
					ctrl.IsChecked((bool)ww);
				}
				return 0;
			}
			case UWPM_CHECKBOX_GET:
			{
				auto ctrl = s->ins.as<CheckBox>();
				return (LRESULT)ctrl.IsChecked().GetBoolean();
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto ctrl = s->ins.as<CheckBox>();
				ctrl.Checked([](const IInspectable&  sender, const RoutedEventArgs&)
				{
					GenericNotify<CheckBox>(sender, UWPN_CHECKBOX_CHANGED);
				});
				ctrl.Indeterminate([](const IInspectable&  sender, const RoutedEventArgs&)
				{
					GenericNotify<CheckBox>(sender, UWPN_CHECKBOX_CHANGED);
				});
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_CheckBox";
		return RegisterClassExW(&wC);
	}



	inline ATOM Register_CalendarDatePicker()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}

			case UWPM_CALENDARPICK_GET:
			{
				//FILETIME* tpv = (FILETIME*)ll;
				auto tpick = s->ins.as<CalendarDatePicker>();
				auto dt = tpick.Date();
				std::chrono::time_point tp = dt.GetDateTime();
				FILETIME ft = winrt::clock::to_FILETIME(tp);
				return 0;
			}
			case UWPM_CALENDARPICK_SET:
			{
				// ww = count (1 if single set)
				// ll = FILETIME*
				auto tpick = s->ins.as<CalendarDatePicker>();
				FILETIME* ff = (FILETIME*)ll;
				auto cr = winrt::clock::from_FILETIME(*ff);
				tpick.Date(cr);
				break;
			}

			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;

				auto tpick = s->ins.as<CalendarDatePicker>();
				tpick.DateChanged([](IInspectable const&  sender, CalendarDatePickerDateChangedEventArgs const&)
				{
					GenericNotify<CalendarDatePicker>(sender, UWPN_CALENDARPICK_CHANGED);
				});

				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_CalendarDatePicker";
		return RegisterClassExW(&wC);
	}

	inline ATOM Register_ProgressBar()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case UWPM_PROGRESSBAR_SETPARAMS:
			{
				auto ctrl = s->ins.as<ProgressBar>();
				if (ww > 0)
				{
					ctrl.IsIndeterminate(false);
					ctrl.Value((double)ww);
				}
				else
					ctrl.IsIndeterminate(true);

				ctrl.ShowPaused(false);
				ctrl.ShowError(false);
				if (ll == 1)
					ctrl.ShowPaused(true);
				if (ll == 2)
					ctrl.ShowError(true);

				return 0;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				auto ctrl = s->ins.as<ProgressBar>();
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_ProgressBar";
		return RegisterClassExW(&wC);
	}


	inline ATOM Register_CalendarView()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}

			case UWPM_CALENDARVIEW_GET:
			{
				FILETIME* tpv = (FILETIME*)ll;
				auto tpick = s->ins.as<CalendarView>();
				IVector<DateTime> vdt = tpick.SelectedDates();
				auto sz = vdt.Size();
				if (!tpv)
					return sz;
				for (unsigned int i = 0; i < sz; i++)
				{
					DateTime dt = vdt.GetAt(i);
					std::chrono::time_point tp = dt;
					FILETIME ft = winrt::clock::to_FILETIME(tp);
					tpv[i] = ft;
				}
				return sz;
			}
			case UWPM_CALENDARVIEW_SET:
			{
				// ww = count (1 if single set)
				// ll = FILETIME*
				auto tpick = s->ins.as<CalendarView>();
				IVector<DateTime> vdt = tpick.SelectedDates();
				vdt.Clear();
				FILETIME* ff = (FILETIME*)ll;
				for (int i = 0; i < ww; i++)
				{
					auto cl = winrt::clock::from_FILETIME(ff[i]);
					vdt.Append(cl);
				}
				break;
			}
			case UWPM_CALENDARVIEW_SETSM:
			{
				auto tpick = s->ins.as<CalendarView>();
				tpick.SelectionMode((winrt::Windows::UI::Xaml::Controls::CalendarViewSelectionMode)ww);
				return 0;
			}
			case UWPM_CALENDARVIEW_GETSM:
			{
				auto tpick = s->ins.as<CalendarView>();
				return (LRESULT)tpick.SelectionMode();
				return 0;
			}

			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;

				auto tpick = s->ins.as<CalendarView>();
				tpick.SelectedDatesChanged([](IInspectable const&  sender, CalendarViewSelectedDatesChangedEventArgs const&)
				{
					GenericNotify<CalendarView>(sender, UWPN_CALENDARVIEW_CHANGED);
				});
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_CalendarView";
		return RegisterClassExW(&wC);
	}


	inline ATOM Register_Image()
	{
		auto WndProc = [](HWND hh, UINT mm, WPARAM ww, LPARAM ll) -> LRESULT
		{
			UWPCONTROL* s = (UWPCONTROL*)GetProp(hh, L"s");
			switch (mm)
			{
			case UWPM_GET_CONTROL:
			{
				return (LRESULT)s;
			}
			case WM_DESTROY:
			{
				GenericDestroy(hh);
				break;
			}
			case UWPM_IMAGE_SETSOURCE:
			{
				/*				auto ctrl = s->ins.as<Image>();
								auto h = (wchar_t*)ll;
								winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage bi;
								winrt::Windows::Foundation::Uri u(h);
								bi.UriSource(u);
								ctrl.Source(bi);
					*/			return 0;
			}
			case WM_CREATE:
			{
				HWND hP = GetParent(hh);
				CREATESTRUCT* cS = (CREATESTRUCT*)ll;
				s = GenericCreate(hP, hh, cS->lpCreateParams);
				if (!s)
					return -1;
				return 0;
			}
			}
			return DefWindowProc(hh, mm, ww, ll);
		};

		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = WndProc;
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"UWP_Image";
		return RegisterClassExW(&wC);
	}
#endif
	inline void Register()
	{
		Register_Custom();
#ifndef UWPLIB_CUSTOMONLY
		Register_Rating();
		Register_ProgressRing();
		Register_HyperlinkButton();
		Register_InkCanvas();
		Register_Timepicker();
		Register_Colorpicker();
		Register_ToggleSwitch();
		Register_Image();
		Register_CalendarView();
		Register_CalendarDatePicker();
		Register_CheckBox();
		Register_ProgressBar();
#endif
	}

}
