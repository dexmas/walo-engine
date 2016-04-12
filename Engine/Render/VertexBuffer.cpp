
#include "Render/VertexBuffer.hpp"
#include "Game.hpp"
#include "Render/Render.hpp"

const unsigned g_ElementSize[] =
{
    3 * sizeof(float),			// EVE_POSITION
    3 * sizeof(float),			// EVE_NORMAL
    4 * sizeof(unsigned char),  // EVE_COLOR
    2 * sizeof(float),			// EVE_TEXCOORD1
    2 * sizeof(float),			// EVE_TEXCOORD2
	2 * sizeof(float),			// EVE_TEXCOORD3
    2 * sizeof(float),			// EVE_TEXCOORD4
	4 * sizeof(float)			// EVE_TANGENT
};

CVertexBuffer::CVertexBuffer()
{
	m_VertexCount = 0;
	m_VertexSize = 0;
	m_ElementMask = 0;

	_UpdateOffsets();
}

CVertexBuffer::~CVertexBuffer()
{
	Release();
}

u32 CVertexBuffer::GetElementOffset(EVertexElement _element)
{
    return m_ElementOffset[_element];
}

bool CVertexBuffer::SetSize(u32 _vertexCount, u32 _elementMask, bool _dynamic)
{
	m_VertexCount = _vertexCount;
	m_ElementMask = _elementMask;
	m_Dynamic = _dynamic;

	_UpdateOffsets();

	return _UpdateSize();
}

bool CVertexBuffer::SetData(void* _data, u32 _start, u32 _count, bool _discard)
{
	bool ret = _Upload(_data, _start, _count);

	return true;
}

void CVertexBuffer::Release()
{
	CRender* render = CGame::Instance()->GetRender();

	if(render->Ready())
    {
		if(render->GetVertexBuffer() == this)
			render->SetVertexBuffer(0);
    }
}

void CVertexBuffer::_UpdateOffsets()
{
	u32 offset = 0;
	for(u32 i = 0; i < EVE_COUNT; ++i)
	{
		if(m_ElementMask & (1 << i))
		{
			m_ElementOffset[i] = offset;
			offset += g_ElementSize[i];
		}
		else
			m_ElementOffset[i] = EEM_DEFAULT;
	}
	m_VertexSize = offset;
}