#pragma once

#include <new>
#include <sys/types.h>

template<typename T>
class CAllocator
{
public:
	T* Allocate(size_t _cnt)
	{
		return (T*)operator new(_cnt*sizeof(T));
	}

	void Deallocate(T* _ptr)
	{
		operator delete(_ptr);
	}

	void Construct(T* _ptr, const T&_e)
	{
		new ((void*)_ptr) T(_e);
	}

	void Destruct(T* ptr)
	{
		ptr->~T();
	}
};