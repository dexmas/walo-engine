#include "Device/Device.hpp"

#if defined(WALO_PLATFORM_WIN32)
#include "Device/Win32/DeviceWin32.hpp"
#elif defined(WALO_PLATFORM_ANDROID)
#include "Device/Android/DeviceAndroid.hpp"
#elif defined(WALO_PLATFORM_LINUX)
#include "Device/Linux/DeviceLinux.hpp"
#elif defined(WALO_PLATFORM_IOS)
#include "Device/iOS/DeviceIOS.hpp"
#elif defined(WALO_PLATFORM_OSX)
#include "Device/OSX/DeviceOSX.hpp"
#elif defined(WALO_PLATFORM_WINPH) || defined(WALOENGINE_PLATFORM_WINRT)
#include "Device/WinRT/DeviceWinRT.hpp"
#include "Device/WinRT/CurlThreadWrapper.hpp"
#endif

CDevice::CDevice()
{
	m_Type = EDT_NULL;
	m_Active = false;
	m_Close = false;
	m_Multisamples = 0;
	m_Landscape = true;
}

CDevice::~CDevice()
{

}

CDevice* CDevice::CreateDevice()
{
#if defined(WALO_PLATFORM_WIN32)
	return new CDeviceWin32();
#elif defined(WALO_PLATFORM_ANDROID)
	return new CDeviceAndroid();
#elif defined(WALO_PLATFORM_IOS)
	return new CDeviceIOS();
#elif defined(WALO_PLATFORM_OSX)
    return new CDeviceOSX();
#elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
	return new CDeviceWinRT();
#elif defined(WALO_PLATFORM_LINUX)
	return new CDeviceLinux();
#else
	return 0;
#endif
}
