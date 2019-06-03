#pragma once

#if defined(_WIN32) || defined(WINAPI_FAMILY)
	#define WALO_PLATFORM_WIN
	#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
		#define WALO_PLATFORM_WINPH
	#elif defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define WALO_PLATFORM_WINRT
	#else
		#define WALO_PLATFORM_WIN32
		#include <Windows.h>
	#endif
#elif defined(_IOS)
	#define WALO_PLATFORM_IOS
#elif defined(_OSX)
    #define WALO_PLATFORM_OSX
#elif defined(_ANDROID)
	#define WALO_PLATFORM_ANDROID
#elif defined(_LINUX)
	#define WALO_PLATFORM_LINUX
#else
	#error WaloEngine platform not defined.
#endif
