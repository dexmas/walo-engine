#pragma once

#include "Config.hpp"
#include "Core/Types.hpp"
#include "Core/Log.hpp"

#ifdef WALO_PLATFORM_WIN32

#define WALO_THREADS_WIN

#include <Windows.h>

typedef HANDLE				ThreadHandle;
typedef CRITICAL_SECTION	ThreadMutex;
typedef void* (*TThreadFunction)(void*);

#elif defined(WALO_PLATFORM_LINUX) || defined(WALO_PLATFORM_IOS) || defined(WALO_PLATFORM_ANDROID)

#define WALO_THREADS_POSIX

#include <pthread.h>

typedef pthread_t			ThreadHandle;
typedef pthread_mutex_t 	ThreadMutex;
typedef void* (*TThreadFunction)(void*);

#endif

class CThread
{
public:
	CThread()
	{
		m_ThreadFunc = 0;
		m_ThreadFuncArg = 0;
		m_Handle = 0;

#if defined(WALO_THREADS_WIN)
		InitializeCriticalSection(&m_Mutex);
#elif defined(WALO_THREADS_POSIX)
		pthread_mutex_init(&m_Mutex, NULL);
#endif
	}

	virtual ~CThread()
	{
#if defined(WALO_THREADS_WIN)
		DeleteCriticalSection(&m_Mutex);
#elif defined(WALO_THREADS_POSIX)
		pthread_mutex_destroy(&m_Mutex);
#endif
		Stop();
	}

	void Start(TThreadFunction _threadFunc, void* _threadFuncArg)
	{
		if(m_Handle)
			return;

		m_ThreadFunc = _threadFunc;
		m_ThreadFuncArg = _threadFuncArg;

#if defined(WALO_THREADS_WIN)
		m_Handle = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)m_ThreadFunc, m_ThreadFuncArg, 0, 0);
#elif defined(WALO_THREADS_POSIX)
		pthread_create(&m_Handle, NULL, m_ThreadFunc, m_ThreadFuncArg);
#endif
	}

	void Stop()
	{
		if(!m_Handle)
			return;

#if defined(WALO_THREADS_WIN)
        CThread::Lock(&m_Mutex);
		TerminateThread(m_Handle, 0);
        CThread::Unlock(&m_Mutex);
#elif defined(WALO_THREADS_POSIX)
    #ifndef WALOENGINE_PLATFORM_ANDROID
        pthread_cancel(m_Handle);
    #endif
        pthread_join(m_Handle, NULL);
#endif

		m_Handle = 0;
	}

	void Join()
	{
#if defined(WALO_THREADS_WIN)
		WaitForSingleObject(m_Handle, INFINITE);
#elif defined(WALO_THREADS_POSIX)
		pthread_join(m_Handle, NULL);
#endif
	}

    static void Lock(ThreadMutex* _mutex)
	{
#if defined(THREADS_WIN)
		EnterCriticalSection(_mutex);
#elif defined(THREADS_POSIX)
		pthread_mutex_lock(_mutex);
#endif
	}

    static void Unlock(ThreadMutex* _mutex)
	{
#if defined(THREADS_WIN)
		LeaveCriticalSection(_mutex);
#elif defined(THREADS_POSIX)
		pthread_mutex_unlock(_mutex);
#endif
	}

	ThreadMutex& GetMutex(){return m_Mutex;}

private:
	TThreadFunction	m_ThreadFunc;
	void*			m_ThreadFuncArg;
	ThreadHandle	m_Handle;
	ThreadMutex		m_Mutex;
};