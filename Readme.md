# A simple UWP Wrapper for Win32

CodeProject article: https://www.codeproject.com/Articles/1279856/UwpAX-A-small-library-for-including-Xaml-controls

# UWPLib

It's a DLL that allows you to use an UWP control as a HWND.

1. Create an app with the attached manifest 
2. Call winrt::init_apartment(apartment_type::single_threaded); 
3. Call WindowsXamlManager::InitializeForCurrentThread();
4. Call Register()
5. Create UWP windows using the class names below and a UNIQUE title.

All Controls send WM_NOTIFY notifications, with a structure passed:

```C++
struct UWPNOTIFICATION
{
	NMHDR n = { 0 };
	UWPCONTROL* s = 0;
};

struct UWPCONTROL
{
	DesktopWindowXamlSource xs;
	HWND hParent = 0;
	HWND hwnd = 0;
	HWND hwndDetailXamlIsland = 0;
	winrt::Windows::Foundation::IInspectable ins;
};

```
All classes reply to UWPM_GET_CONTROL to return a pointer to the above UWPCONTROL structure. 
Use the included "ins" parameter to manipulate the control directly.

The classes available at the moment are:

## Custom Control
Create using class name: UWP_CheckCustom, then use WM_SETTEXT to set the XML for it and UWPM_GET_CONTROL to get the UWPCONTROL.

## Rating Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.ratingcontrol

Create using class name: UWP_Rating

Messages | Notifications
------------- | -------------
UWPM_RATING_GET (returns Rating) | UWPN_RATING_CHANGE
UWPM_RATING_SET (wParam: New Rating) | 


## ProgressRing Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.progressring

Create using class name: UWP_ProgressRing

Messages | Notifications
------------- | -------------
UWPM_PROGRESSRING_ACTIVATE (wParam: 0 or 1) | 


## HyperlinkButton Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.hyperlinkbutton

Create using class name: UWP_HyperlinkButton

Messages | Notifications
------------- | -------------
WM_SETTEXT | UWPN_HYPERLINKBUTTON_CLICKED



## TimePicker Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.timepicker

Create using class name: UWP_Timepicker

Messages | Notifications
------------- | -------------
UWPM_TIMEPICK_SET (lParam = seconds) | UWPN_TIMEPICK_CHANGED
UWPM_TIMEPICK_GET  | 


## ToggleSwitch Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.toggleswitch

Create using class name: UWP_ToggleSwitch

Messages | Notifications
------------- | -------------
UWPM_TOGGLESWITCH_SET (wParam = 0 or 1) | UWPN_TOGGLESWITCH_CHANGE
UWPM_TOGGLESWITCH_GET  | 


## CalendarView Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.calendarview

Create using class name: UWP_CalendarView

Messages | Notifications
------------- | -------------
UWPM_CALENDARVIEW_SETSM (wParam = 0 or 1 or 2) | UWPN_CALENDARVIEW_CHANGED
UWPM_TOGGLESWITCH_GETSM  | 
UWPM_CALENDARVIEW_SET (wParam = count, lParam = FILETIME* | 
UWPM_TOGGLESWITCH_GET (lParam = FILETIME* , use null to get the count in return value) | 



## CalendarDatePicker Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.calendardatepicker

Create using class name: UWP_CalendarDatePicker

Messages | Notifications
------------- | -------------
UWPM_CALENDARPICK_GET (lParam = FILETIME* | UWPN_CALENDARPICK_CHANGED
UWPM_CALENDARPICK_GET (lParam = FILETIME*) | 


## CheckBox Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.checkbox

Create using class name: UWP_CheckBox

Messages | Notifications
------------- | -------------
UWPM_CHECKBOX_SET (wParam = 0 or 1, 2 for three state on) | UWPN_CHECKBOX_CHANGE
UWPM_CHECKBOX_GET  | 
WM_SETTEXT | 

## ProgressBar Control
https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.controls.progressbar

Create using class name: UWP_ProgressBar

Messages | Notifications
------------- | -------------
UWPM_PROGRESSBAR_SETPARAMS Sets Params | UWPN_CHECKBOX_CHANGE

For the UWPM_PROGRESSBAR_SETPARAMS message, wParam = width or 0 indeterminate, lParam = State (0-2 =Running/Paused/Error)