#include "FileSystem/FileSystem.hpp"

#if defined(WALO_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#import <CoreFoundation/CoreFoundation.h>
#endif

CFileSystem* CFileSystem::m_Instance = 0;

CFileSystem::CFileSystem()
{
	_GetSystemFolders();
}

CFileSystem::~CFileSystem()
{
}

CFileSystem* CFileSystem::Instance()
{
	if(!m_Instance)
	{
		m_Instance = new CFileSystem();
	}

	return m_Instance;
}

CFileStream* CFileSystem::OpenFile(const CString& _name)
{
	CString path = m_RootPath + _name;
	return new CFileStream(path, false);
}

CFileStream* CFileSystem::OpenCasheFile(const CString& _name, bool _write)
{
	CString path = m_CashePath + _name;
	return new CFileStream(path, _write);
}

CFileStream* CFileSystem::OpenSaveFile(const CString& _name, bool _write)
{
	CString path = m_SavePath + _name;
	return new CFileStream(path, _write);
}

void CFileSystem::_GetSystemFolders()
{
	m_RootPath = "";
	m_SavePath = "";
	m_CashePath = "";

#if defined(WALO_PLATFORM_IOS)
	CFBundleRef mainBundle  = CFBundleGetMainBundle();
	CFURLRef	mainURL		= CFBundleCopyBundleURL( mainBundle );
    
	CFStringRef mainPath	= CFURLCopyFileSystemPath( mainURL, kCFURLPOSIXPathStyle );
    
	long		pathLength  = CFStringGetLength( mainPath ) + 1;
	char		cStrPath[pathLength];
	CFStringGetCString( mainPath, cStrPath, pathLength, kCFStringEncodingMacRoman );
	m_RootPath = CString(cStrPath) + "/Assets/";
    
	CFRelease( mainURL );
	CFRelease( mainPath );

	NSString* docDir = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	docDir = [NSString stringWithFormat:@"%@/", docDir];
	m_SavePath = CString([docDir UTF8String]);

	m_CashePath = m_SavePath  + "cashe/";

#elif defined(WALO_PLATFORM_WIN32)
	char cCurrentPath[FILENAME_MAX];

	if(GetModuleFileNameA(0, cCurrentPath, sizeof(cCurrentPath)))
	{
		for(u32 i=strlen(cCurrentPath)-1;i>=0;i--)
		{
			if(cCurrentPath[i] == '/' || cCurrentPath[i] == '\\')
			{
				cCurrentPath[i+1] = 0;
				break;
			}
		}

		m_RootPath = CString(cCurrentPath);
	}
	m_SavePath = m_RootPath + "save/";
	m_CashePath = m_RootPath + "cashe/";

	DWORD dwAttrib = GetFileAttributesA(m_SavePath.CStr());

	if((dwAttrib == INVALID_FILE_ATTRIBUTES) || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		CreateDirectoryA(m_SavePath.CStr(), NULL);
	}

#elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
	Windows::Storage::StorageFolder^ folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
	Platform::String^ sPath = folder->Path + "/bin/";
	TWString wpath(sPath->Data());
	u32 nPathLength = wpath.Size() + 1;

	char pBuffer[_MAX_PATH];
	wcstombs (pBuffer, wpath.CStr(), nPathLength);

	m_RootPath = CString(pBuffer);

	Platform::String^ ssSavePath = Windows::Storage::ApplicationData::Current->LocalFolder->Path + L"\\";
	wcstombs(pBuffer, ssSavePath->Data(), ssSavePath->Length());
	pBuffer[ssSavePath->Length()] = 0;

	m_SavePath = CString(pBuffer);
	m_CashePath = m_SavePath + "cashe/";
#else
	m_SavePath = m_RootPath + "save/";
	m_CashePath = m_RootPath + "cashe/";
#endif
}