#pragma once

#include "Core/Types.hpp"

#if defined(WALO_PLATFORM_WIN)
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

class CTimer
{
public:
	CTimer()
	{
		m_HighResFrequency = 1000;
		m_HighResSuported = false;

#ifdef WALO_PLATFORM_WIN
		LARGE_INTEGER frequency;
		if (QueryPerformanceFrequency(&frequency))
		{
			m_HighResFrequency = frequency.QuadPart;
			m_HighResSuported = true;
		}
#else
		m_HighResFrequency = 1000000;
		m_HighResSuported = true;
#endif
	}

	void Sleep(u32 _ms)
	{
#if defined(WALO_PLATFORM_WIN)
		::Sleep(_ms);
#else
		usleep(_ms * 1000);
#endif
	}

	u64 GetCurentTime()
	{
		s64 ct = 0;
#ifdef WALO_PLATFORM_WIN
		if(m_HighResSuported)
		{
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);
			ct = counter.QuadPart;
		}
		else
			ct = GetTickCount();
#else
		struct timeval time;
		gettimeofday(&time, NULL);
		ct = time.tv_sec * 1000000LL + time.tv_usec;
#endif

		if(ct < 0)
			ct = 0;

		return ct * 1000000LL / m_HighResFrequency;
	}

private:
	bool m_HighResSuported;
	u64  m_HighResFrequency;
};
