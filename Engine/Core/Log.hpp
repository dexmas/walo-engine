#pragma once

#include "Config.hpp"

#ifdef _DEBUG
     #if defined(WALO_PLATFORM_ANDROID)
			#include <android/log.h>
			#define LOG_TAG "WaloEngine"
            #define LOG(format, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##__VA_ARGS__)
			#define VLOG(format, valist) __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, format, valist)
    #elif defined(WALO_PLATFORM_LINUX) || defined(WALO_PLATFORM_IOS) || defined(WALO_PLATFORM_OSX)
            #include <stdio.h>
            #define LOG(format, ...) printf(format, ##__VA_ARGS__)
            #define VLOG(format, valist) vprintf(format, valist)
     #elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
			#include "agile.h"
			#include <debugapi.h>
			void OutputVLOG(const char* format, va_list vl);
			void OutputLOG(const char* format, ...);
			#define LOG(format, ...) OutputLOG(format, ##__VA_ARGS__)
			#define VLOG(format, valist) OutputVLOG(format, valist)
	#else
            #include <stdio.h>
            #define LOG(format, ...) printf(format, ##__VA_ARGS__)
            #define VLOG(format, valist) vprintf(format, valist)
     #endif

	 #define LOG_RELEASE(format, ...) LOG(format, ##__VA_ARGS__)
	 #define VLOG_RELEASE(format, valist) VLOG(format, valist)
#else
	#if defined(WALO_PLATFORM_ANDROID)
			#include <android/log.h>
			#define LOG_TAG "nEngine"
		    #define LOG_RELEASE(format, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##args)
			#define VLOG_RELEASE(format, valist) __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, format, valist)
	#elif defined(WALO_PLATFORM_LINUX) || defined(WALO_PLATFORM_IOS) || defined(WALO_PLATFORM_OSX)
		   #include <stdio.h>
		   #define LOG_RELEASE(format, ...) printf(format, ##__VA_ARGS__)
		   #define VLOG_RELEASE(format, valist) vprintf(format, valist)
	#elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
			#include "agile.h"
			#include <debugapi.h>
			void OutputVLOG(const char* format, va_list vl);
			void OutputLOG(const char* format, ...);
			#define VLOG_RELEASE(format, valist) OutputVLOG(format, valist)
			#define LOG_RELEASE(format, ...) OutputLOG(format, ##__VA_ARGS__)
	#else
		   #include <stdio.h>
		   #define LOG_RELEASE(format, ...) printf(format, __VA_ARGS__)
		   #define VLOG_RELEASE(format, valist) vprintf(format, valist)
	#endif
	#define LOG(format, ...)
	#define VLOG(format, valist)
#endif

