#pragma once

#include "Core/Types.hpp"
#include <memory.h>

class CStream
{
public:
	CStream();
	CStream(void* _data, u32 _size);
	~CStream();

	virtual bool Open(){return true;}
	virtual void Close(){}

	virtual u32 Read(void* _buffer,u32 _size);
	virtual u32 Write(void* _data, u32 _size);

	virtual u32 Pos(){return m_Cursor;}
	virtual u32 Size(){return m_Size;}
	virtual void Seek(s32 _pos, bool _relative);

protected:
	void* m_Buffer;
	u32   m_Size;
	u32   m_Cursor;
};