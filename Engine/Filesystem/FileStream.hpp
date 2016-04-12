#pragma once

#include "FileSystem/Stream.hpp"
#include "Core/String.hpp"

#include <stdio.h>

class CFileStream: public CStream
{
	friend class CFileSystem;

public:
	~CFileStream();

	virtual bool Open();
	virtual void Close();

	virtual u32 Read(void* _buffer,u32 _size);
	virtual u32 Write(void* _data, u32 _size);

	virtual void Seek(s32 _pos, bool _relative);

protected:
	FILE* m_File;

	bool m_Opened;
	bool m_Write;
	CString m_Path;

private:
	CFileStream(CString _path, bool _write);
};