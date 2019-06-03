#include "Filesystem/Android/FileStreamAndroid.hpp"
#include <fcntl.h>

CFileStreamAndroid::CFileStreamAndroid(CString _path, bool _write, AAssetManager* _mgr):CFileStream(_path, _write)
{
	m_Asset = 0;
	m_AssetManager = _mgr;
}

CFileStreamAndroid::~CFileStreamAndroid()
{
	Close();
}

bool CFileStreamAndroid::Open()
{
	if(m_Opened)
		return true;

    m_Asset = AAssetManager_open(m_AssetManager, m_Path.CStr(), AASSET_MODE_UNKNOWN);
	
    if(m_Asset == 0)
        return false;

	m_Size = AAsset_getLength(m_Asset);

	m_Cursor = 0;
	m_Opened = true;

	return true;
}

void CFileStreamAndroid::Close()
{
	if(!m_Opened)
		return;

	AAsset_close(m_Asset);

	m_Asset = 0;
	m_Size = 0;
	m_Cursor = 0;

	m_Opened = false;
}

u32 CFileStreamAndroid::Read(void* _buffer,u32 _size)
{
	if(!Open())
	{
		return 0;
	}

	s32 sz = _size;

	if((m_Cursor + _size) > m_Size)
		sz = m_Size - m_Cursor;

	if(sz <= 0)
		return 0;

	sz = AAsset_read(m_Asset, _buffer, sz);

	m_Cursor += sz;

	return sz;
}

u32 CFileStreamAndroid::Write(void* _data, u32 _size)
{
	return 0;
}

void CFileStreamAndroid::Seek(s32 _pos, bool _relative)
{
	if(!Open())
	{
		return;
	}

	s32 pos = _relative ? (m_Cursor + _pos) : _pos;
	
	if(pos > m_Size || pos < 0)
		pos = 0;

	AAsset_seek(m_Asset, pos, _relative ? SEEK_CUR : SEEK_SET);

	m_Cursor = pos;
}