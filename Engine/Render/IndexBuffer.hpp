#pragma once

#include "Core/Types.hpp"
#include "Render/RenderTypes.hpp"

class CIndexBuffer
{
	friend class CRender;
public:
	virtual ~CIndexBuffer(){}

	u32 GetHandle() {return m_Handle;}

	u32 GetIndexSize() {return m_IndexSize;}
	u32 GetIndexCount() {return m_IndexCount;}
	 
	bool  SetSize(u32 _indexCount, bool _largeIndexes, bool _dynamic = false);
    bool  SetData(void* _data, u32 _start, u32 _count, bool _discard = false);

	virtual void Release();

protected:
	CIndexBuffer();

	virtual bool _UpdateSize(){return true;}
	virtual bool _Upload(void* _data, u32 _start, u32 _count){return true;}

	u32 m_IndexCount;
	u32 m_IndexSize;

	u32		   m_Handle;

	bool m_Dynamic;
};