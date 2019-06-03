#pragma once

#include "Core/Types.hpp"
#include "Core/String.hpp"

#if defined(WALO_PLATFORM_ANDROID)
#include "Filesystem/Android/FileStreamAndroid.hpp"
#else
#include "Filesystem/FileStream.hpp"
#endif

class CFileSystem
{
public:
	CFileSystem();
	~CFileSystem();

	static CFileSystem* Instance();

	CFileStream* OpenFile(const CString& _name);
	CFileStream* OpenCasheFile(const CString& _name, bool _write = false);
	CFileStream* OpenSaveFile(const CString& _name, bool _write = false);

private:
	void _GetSystemFolders();

	static CFileSystem* m_Instance;

	CString m_RootPath;
	CString m_SavePath;
	CString m_CashePath;

#if defined(WALO_PLATFORM_ANDROID)
	AAssetManager* m_AssetManager;
#endif
};