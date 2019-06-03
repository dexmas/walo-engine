#pragma once

#include "Filesystem/FileStream.hpp"
#include "Core/String.hpp"

#include <android/asset_manager.h>

class CFileStreamAndroid: public CFileStream
{
	friend class CFileSystem;

public:
	CFileStreamAndroid(CString _path, bool _write, AAssetManager* _mgr);
	~CFileStreamAndroid();

	bool Open();
	void Close();

	u32 Read(void* _buffer,u32 _size);
	u32 Write(void* _data, u32 _size);

	void Seek(s32 _pos, bool _relative);

protected:
	AAsset* m_Asset;
	AAssetManager* m_AssetManager;

private:
	CFileStreamAndroid(CString _path, bool _write);
};