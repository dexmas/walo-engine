#include "FileSystem/Stream.hpp"
#include "Core/Math.hpp"

CStream::CStream()
{
	m_Buffer = 0;
	m_Size = 0;
	m_Cursor = 0;
}

CStream::CStream(void* _data, u32 _size)
{
	m_Buffer = _data;
	m_Size = _size;
	m_Cursor = 0;
}

CStream::~CStream()
{
	if(m_Buffer)
	{
		delete[] m_Buffer;
	}
}

u32 CStream::Read(void* _buffer,u32 _size)
{
	s32 sz = _size;

	if((m_Cursor + _size) > m_Size)
		sz = m_Size - m_Cursor;

	if(sz <= 0)
		return 0;

	memcpy(_buffer, (char*)m_Buffer + m_Cursor, sz);

	m_Cursor += sz;

	return sz;
}

u32 CStream::Write(void* _data, u32 _size)
{
	s32 sz = _size;

	if((m_Cursor + _size) > m_Size)
		sz = m_Size - m_Cursor;

	if(sz <= 0)
		return 0;

	memcpy((char*)m_Buffer + m_Cursor, _data, sz);

	m_Cursor += sz;

	return sz;
}

void CStream::Seek(s32 _pos, bool _relative)
{
	s32 pos = _relative ? (m_Cursor + _pos) : _pos;
	
	if(pos > m_Size || pos < 0)
		pos = 0;

	m_Cursor = pos;
}