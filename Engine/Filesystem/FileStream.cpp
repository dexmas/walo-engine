#include "FileSystem/FileStream.hpp"

CFileStream::CFileStream(CString _path, bool _write)
{
	m_Path = _path;
	m_Write = _write;
	m_File = 0;
	m_Opened = false;

	Open();
}

CFileStream::~CFileStream()
{
	Close();
}

bool CFileStream::Open()
{
	if(m_Opened)
		return true;

    m_File = fopen(m_Path.CStr(), m_Write ? "wb" : "rb");
	
    if(m_File == 0)
        return false;
	
	fseek(m_File, 0, SEEK_END);
	m_Size = ftell(m_File);
    
	fseek(m_File, 0, SEEK_SET);

	m_Cursor = 0;
	m_Opened = true;

	return true;
}

void CFileStream::Close()
{
	if(!m_Opened)
		return;

	fclose(m_File);
	m_File = 0;

	m_Size = 0;
	m_Cursor = 0;
	m_Opened = false;
}

u32 CFileStream::Read(void* _buffer,u32 _size)
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

	sz = fread(_buffer, 1, sz, m_File);

	m_Cursor += sz;

	return sz;
}

u32 CFileStream::Write(void* _data, u32 _size)
{
	if(!m_Write || !Open())
	{
		return 0;
	}

	s32 sz = fwrite(_data, 1, _size, m_File);

	m_Cursor += sz;
	m_Size += sz;

	return sz;
}

void CFileStream::Seek(s32 _pos, bool _relative)
{
	if(!Open())
	{
		return;
	}

	s32 pos = _relative ? (m_Cursor + _pos) : _pos;
	
	if(pos > m_Size || pos < 0)
		pos = 0;

	fseek(m_File, pos, _relative ? SEEK_CUR : SEEK_SET);

	m_Cursor = pos;
}