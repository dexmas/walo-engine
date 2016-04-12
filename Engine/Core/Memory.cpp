#ifdef _DEBUG

#include "Core/Memory.hpp"
#include "Core/Types.hpp"
#include "Core/Log.hpp"
#include "Game.hpp"

#include <stdlib.h>

#if defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
#include <exception>
#endif

void* operator new(size_t blocksize) throw(std::bad_alloc)
{
	u32* ret = (u32*)malloc(blocksize+4);
	ret[0] = blocksize;
	return ret+1;
}

void* operator new[](size_t blocksize) throw(std::bad_alloc)
{
	u32* ret = (u32*)malloc(blocksize+4);
	ret[0] = blocksize;
	return ret+1;
}
void  operator delete(void* block) throw()
{
	u32 sz = 0;
	if(block)
	{
		u32* ptr = (u32*)block - 1;
		sz = ptr[0];
		free(ptr);
	}
}
void  operator delete[](void* block) throw()
{
	u32 sz = 0;
	if(block)
	{
		u32* ptr = (u32*)block - 1;
		sz = ptr[0];
		free(ptr);
	}
}

#endif