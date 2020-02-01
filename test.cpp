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
LRESULT CALLBACK DP(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
{
	switch (mm)
	{
	case WM_KEYDOWN:
	case WM_CHAR:
		DebugBreak();

	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hh, &rc);
		SetWindowPos(GetDlgItem(hh, 910), 0, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
		return 0;
	}
	case WM_NOTIFY:
	{
		NMHDR* n = (NMHDR*)ll;
		return 0;
	}
	case WM_CREATE:
	{
		CreateWindowEx(0, L"UWP_Custom", L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hh, (HMENU)910, 0, 0);
		MainWindow = hh;
		ShowWindow(hh, SW_SHOWMAXIMIZED);
		SetWindowText(GetDlgItem(hh, 910), LR"(<Pivot xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml" Title="UWP Library">
    <PivotItem Header="Basic Input">
		<StackPanel Orientation="Vertical" Margin="20">

		<Button Content="Button"  Margin="0,0,0,10"/>
		<CheckBox Content="Three-state CheckBox" IsThreeState="True"  Margin="0,0,0,10" />
		<ComboBox PlaceholderText="Pick a color" Width="200"  Margin="0,0,0,10">
			<x:String>Blue</x:String>
			<x:String>Green</x:String>
			<x:String>Red</x:String>
			<x:String>Yellow</x:String>
		</ComboBox>

		<DropDownButton Content="Email"  Margin="0,0,0,10">
			<DropDownButton.Flyout>
				<MenuFlyout Placement="Bottom">
					<MenuFlyoutItem Text="Send"/>
					<MenuFlyoutItem Text="Reply"/>
					<MenuFlyoutItem Text="Reply All"/>
				</MenuFlyout>
			</DropDownButton.Flyout>
		</DropDownButton>

		<HyperlinkButton Content="Microsoft home page" NavigateUri="http://www.microsoft.com" Margin="0,0,0,10" />

		<HyperlinkButton Content="Event on click" Click="HyperlinkButton_Click" Margin="0,0,0,10" x:Name="ClickURL"/>

		<RadioButton x:Name="Option1RadioButton" Content="Option 1" Checked="Option1RadioButton_Checked" Margin="0,0,0,10"/>
		<RadioButton x:Name="Option2RadioButton" Content="Option 2" Checked="Option2RadioButton_Checked" Margin="0,0,0,10"/>

		<RatingControl AutomationProperties.Name="Simple RatingControl" IsClearEnabled="False" IsReadOnly="False" Margin="0,0,0,10"/>
		
		<Slider Width="200" Minimum="500" Maximum="1000" StepFrequency="10" SmallChange="10" LargeChange="100" Value="800" Margin="0,0,0,10"/>
		
		<ToggleSwitch AutomationProperties.Name="simple ToggleSwitch" Margin="0,0,0,10"/>

<StackPanel Orientation="Horizontal">
    <AppBarButton Icon="AttachCamera" Label="Attach Camera"/>
    <AppBarSeparator />
    <AppBarButton Icon="Like" Label="Like"/>
    <AppBarButton Icon="Dislike" Label="Dislike"/>
    <AppBarSeparator />
    <AppBarButton Icon="Orientation" Label="Orientation"/>
</StackPanel>

		</StackPanel>
    </PivotItem>

    <PivotItem Header="Color">
		<ColorPicker
			  IsMoreButtonVisible="False"
			  IsColorSliderVisible="True"
			  IsColorChannelTextInputVisible="True"
			  IsHexInputVisible="True"
			  IsAlphaEnabled="False"
			  IsAlphaSliderVisible="True"
			  IsAlphaTextInputVisible="True" />

    </PivotItem>

    <PivotItem Header="Text">
		<StackPanel Orientation="Vertical" Margin="20">
			<AutoSuggestBox PlaceholderText="Type"  Margin="0,0,0,10"/>
			<RichEditBox x:Name="REBCustom" 
				AutomationProperties.Name="editor with custom menu"
				Width="800" Height="200" 
				Loaded="REBCustom_Loaded" 
				Unloaded="REBCustom_Unloaded"/>

		</StackPanel>
    </PivotItem>

    <PivotItem Header="Calendar">
		<StackPanel Orientation="Vertical" Margin="20">

			<CalendarDatePicker PlaceholderText="Pick a date" Header="Calendar"  Margin="0,0,0,10" />
				<CalendarView 
					SelectionMode="Single" 
					IsGroupLabelVisible="True"
					IsOutOfScopeEnabled="True"
					Language="en"
					CalendarIdentifier="GregorianCalendar" Margin="0,0,0,10"  />
				<DatePicker Header="Pick a date" Margin="0,0,0,10" />
				<TimePicker ClockIdentifier="24HourClock" Header="24 hour clock" Margin="0,0,0,10"  />
		</StackPanel>
    </PivotItem>

    <PivotItem Header="Dialog">
		<StackPanel Orientation="Vertical" Margin="20">
		<Button Content="Empty cart">
			<Button.Flyout>
				<Flyout>
					<StackPanel>
						<TextBlock Text="All items will be removed. Do you want to continue?" Margin="0,0,0,12" />
						<Button Click="DeleteConfirmation_Click" Content="Yes, empty my cart" />
					</StackPanel>
				</Flyout>
			</Button.Flyout>
		</Button>

		</StackPanel>
    </PivotItem>

    <PivotItem Header="Canvas">
	<StackPanel Orientation="Vertical" Margin="20">
		<InkToolbar x:Name="inkToolbar" />
		<InkCanvas x:Name="inkCanvas"/>
	</StackPanel>
    </PivotItem>


    <PivotItem Header="Menu">
	<StackPanel Orientation="Vertical" Margin="20">
		<AppBarButton Icon="Sort" IsCompact="True" ToolTipService.ToolTip="Sort" AutomationProperties.Name="Sort">
		<AppBarButton.Flyout>
			<MenuFlyout>
				<MenuFlyoutItem Text="By rating" Click="MenuFlyoutItem_Click" Tag="rating"/>
				<MenuFlyoutItem Text="By match" Click="MenuFlyoutItem_Click" Tag="match"/>
				<MenuFlyoutItem Text="By distance" Click="MenuFlyoutItem_Click" Tag="distance"/>
			</MenuFlyout>
		</AppBarButton.Flyout>
	</AppBarButton>


	<MenuBar>
		<MenuBarItem Title="File">
			<MenuFlyoutItem Text="New"/>
			<MenuFlyoutItem Text="Open..."/>
			<MenuFlyoutItem Text="Save"/>
			<MenuFlyoutItem Text="Exit"/>
		</MenuBarItem>

		<MenuBarItem Title="Edit">
			<MenuFlyoutItem Text="Undo"/>
			<MenuFlyoutItem Text="Cut"/>
			<MenuFlyoutItem Text="Copy"/>
			<MenuFlyoutItem Text="Paste"/>
		</MenuBarItem>

		<MenuBarItem Title="Help">
			<MenuFlyoutItem Text="About"/>
		</MenuBarItem>
	</MenuBar>

	</StackPanel>
    </PivotItem>

    <PivotItem Header="Layout">
	<StackPanel Orientation="Vertical" Margin="20">

		<FlipView MaxWidth="300" Height="70">
				<Button Content="Button 1" />
				<Button Content="Button 2" />
				<Button Content="Button 3" />
		</FlipView>

		<GridView
			x:Name="BasicGridView"
			IsItemClickEnabled="True"
			SelectionMode="Single">

				<GridView.ItemsPanel>
					<ItemsPanelTemplate>
						<ItemsWrapGrid x:Name="MaxItemsWrapGrid" 
									   MaximumRowsOrColumns="3" 
									   Orientation="Horizontal"/>
					</ItemsPanelTemplate>
				</GridView.ItemsPanel>    

				<Button Content="Button 1" />
				<Button Content="Button 2" />
				<Button Content="Button 3" />
				<Button Content="Button 4" />
				<Button Content="Button 5" />
				<Button Content="Button 6" />
				<Button Content="Button 7" />
				<Button Content="Button 8" />
				<Button Content="Button 9" />
		</GridView>
                    
		<ListBox Width="200">
			<x:String>Blue</x:String>
			<x:String>Green</x:String>
			<x:String>Red</x:String>
			<x:String>Yellow</x:String>
		</ListBox>

		<ListView
			x:Name="BaseExample"
			BorderThickness="1"
			Width="350" 
			Height="400"
			HorizontalAlignment="Left">
				<Button Content="Button 1" />
				<Button Content="Button 2" />
				<Button Content="Button 3" />
				<Button Content="Button 4" />

		</ListView>

		<TreeView SelectionMode="Multiple" >
		</TreeView>


	</StackPanel>
    </PivotItem>

    <PivotItem Header="Other">
		<StackPanel Orientation="Vertical" Margin="20">
			<PersonPicture  Margin="0,0,0,10" ProfilePicture="https://docs.microsoft.com/windows/uwp/contacts-and-calendar/images/shoulder-tap-static-payload.png" />

			<ProgressBar  Margin="0,0,0,10" Width="130" IsIndeterminate="True" ShowPaused="False" ShowError="False" />

			<ProgressRing  Margin="0,0,0,10" IsActive="True" />

		</StackPanel>
    </PivotItem>



</Pivot>)");
		UWPCONTROL* cust = (UWPCONTROL*)SendDlgItemMessage(hh, 910, UWPM_GET_CONTROL, 0, 0);
		cust->ins.as<Pivot>().FindName(L"ClickURL").as<HyperlinkButton>().Click([](const IInspectable& sender, const RoutedEventArgs&)
			{
				MessageBox(MainWindow, L"URL Clicked", L"", MB_OK);
			});
		break;
	}
	case WM_CLOSE:
	{
		for (int i = 901; i <= 940; i++)
			DestroyWindow(GetDlgItem(hh, i));
		DestroyWindow(hh);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hh,mm,ww,ll);
}


int __stdcall WinMain(HINSTANCE h, HINSTANCE, LPSTR, int)
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


	WNDCLASSEX wClass = { 0 };
	wClass.cbSize = sizeof(wClass);

	wClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	wClass.lpfnWndProc = (WNDPROC)DP;
	wClass.hInstance = h;
	wClass.hIcon = 0;
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wClass.lpszClassName = _T("W_CLASS");
	wClass.hIconSm = 0;
	RegisterClassEx(&wClass);

	auto MainWindow = CreateWindowEx(0,
		L"W_CLASS",
		L"UWP Library",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, GetModuleHandle(0), (void*)0);
	ShowWindow(MainWindow, SW_SHOWMAXIMIZED);
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	
	return 0;
}

