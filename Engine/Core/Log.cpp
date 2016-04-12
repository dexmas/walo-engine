#include "Core/Log.hpp"

#ifdef _DEBUG

	#include "Game.hpp"

	#if defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)

		#include "Filesystem/FileSystem.hpp"

		void OutputVLOG(const char* format, va_list vl)
		{
			char szBuffer[4096];
			vsprintf (szBuffer, format, vl);
			OutputDebugString(szBuffer);
			u32 nBufLength = strlen(szBuffer);
			szBuffer[nBufLength] = 1;
			char szPath[MAX_PATH];
			sprintf (szPath, "%s%s", CFileSystem::GetSavePath().CStr(), "log");
			FILE* pFile = fopen(szPath, "ab");
			fwrite(szBuffer, sizeof(char), nBufLength, pFile);
			fflush(pFile);
			fclose(pFile);
		}

		void OutputLOG(const char* format, ...)
		{
			va_list vl;
			va_start(vl, format);
			OutputVLOG(format, vl);
			va_end(vl);
		}
	#endif
#endif