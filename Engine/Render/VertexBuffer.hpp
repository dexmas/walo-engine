#pragma once

#include "Core/Types.hpp"
#include "Render/RenderTypes.hpp"

enum EVertexElement
{
	EVE_POSITION = 0,
	EVE_NORMAL,
	EVE_COLOR,
	EVE_TEXCOORD1,
	EVE_TEXCOORD2,
	EVE_TEXCOORD3,
	EVE_TEXCOORD4,
	EVE_TANGENT,

	EVE_COUNT
};

enum EElementMask
{
	EEM_NONE = 0x0,
	EEM_POSITION = 0x1,
	EEM_NORMAL = 0x2,
	EEM_COLOR = 0x4,
	EEM_TEXCOORD1 = 0x8,
	EEM_TEXCOORD2 = 0x10,
	EEM_TEXCOORD3 = 0x20,
	EEM_TEXCOORD4 = 0x40,
	EEM_TANGENT = 0x80,

	EEM_DEFAULT = 0xffffffff,
};

class CVertexBuffer
{
	friend class CRender;
public:
	virtual ~CVertexBuffer();

	u32 GetHandle() {return m_Handle;}

	u32 GetVertexSize() {return m_VertexSize;}
	u32 GetVertexCount() {return m_VertexCount;}
	u32 GetElementMask() {return m_ElementMask;} 
	u32 GetElementOffset(EVertexElement _element);

	bool  SetSize(u32 _vertexCount, u32 _elementMask, bool _dynamic = false);
    bool  SetData(void* _data, u32 _start, u32 _count, bool _discard = false);

	virtual void Release();

protected:
	CVertexBuffer();

	void _UpdateOffsets();

	virtual bool _UpdateSize(){return true;}
	virtual bool _Upload(void* _data, u32 _start, u32 _count){return true;}

	u32 m_VertexCount;
	u32 m_VertexSize;
	u32 m_ElementMask;
	u32 m_ElementOffset[EVE_COUNT];

	u32		   m_Handle;

	bool m_Dynamic;
};