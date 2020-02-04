# A simple UWP Wrapper for Win32

CodeProject article: https://www.codeproject.com/Articles/1279856/UwpAX-A-small-library-for-including-Xaml-controls

# UWPLib

It's a library that allows you to use an UWP control as a HWND.

1. Create an app with the attached manifest 
2. Call winrt::init_apartment(apartment_type::single_threaded); 
3. Call WindowsXamlManager::InitializeForCurrentThread();
4. Call Register()
5. Create windows with UWP_Custom class and use WM_SETTEXT to set the Xaml.
6. The control returns an UWPCONTROL pointer which you can use to manipulate the IInspectable

```C++

	struct UWPCONTROL
	{
		DesktopWindowXamlSource xs;
		HWND hParent = 0;
		HWND hwnd = 0;
		HWND hwndDetailXamlIsland = 0;
		winrt::Windows::Foundation::IInspectable ins;
	};

 auto pv = LR"(<Pivot xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml" >
    <PivotItem Header="Items">
            <ListView Name="GridItems"/>
    </PivotItem>
    <PivotItem Header="Transfers">
            <ListView Name="TransferItems"/>
    </PivotItem>
    <PivotItem Header="Add">
        <StackPanel>
            <Button x:Name="btn1" Content="Add Files" Margin="5" Width="150" />
            <Button x:Name="btn2" Content="Add Directory" Margin="5" Width="150" />
        </StackPanel>
    </PivotItem>
    <PivotItem Header="Upload">
        <StackPanel>
        </StackPanel>
    </PivotItem>
    <PivotItem Header="Configuration">
        <StackPanel>
            <TextBox Name="portnum" Margin="5" Header="Port Number" Text="7001"/>
            <CheckBox Name="CB_RightClick" Content="Enable right click" />
            <CheckBox Name="CB_ForceOctetStream" Content="Force MIME application/octet-stream" />
            <TextBox x:Name="ip" Margin="5" Header="IP or Hostname (Empty = default) " />
        </StackPanel>
    </PivotItem>
</Pivot>)";

SetWindowText(GetDlgItem(hh, 901), pv);
UWPLIB::UWPCONTROL* u = (UWPLIB::UWPCONTROL*)SendDlgItemMessage(hh, 901, UWPM_GET_CONTROL, 0, 0);
auto pivot = u->ins.as<Pivot>();
...
```

An example of a DLL that can be called from an ordinary Win32 app is also there in UWP directory. This dll exports a function that can transform a HMENU to an UWP menu and the Win32 can display it.


![TT](https://www.codeproject.com/KB/miscctrl/1279856/1.jpg)

![Test](https://www.codeproject.com/KB/miscctrl/1279856/2.jpg)


