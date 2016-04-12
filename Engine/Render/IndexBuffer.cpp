#include "Render/IndexBuffer.hpp"
#include "Game.hpp"
#include "Render/Render.hpp"

CIndexBuffer::CIndexBuffer()
{
	m_IndexCount = 0;
	m_IndexSize = 0;
}

bool CIndexBuffer::SetSize(u32 _indexCount, bool _largeIndexes, bool _dynamic)
{
	m_IndexCount = _indexCount;
	m_IndexSize = _largeIndexes?sizeof(u32):sizeof(u16);
	m_Dynamic = _dynamic;

	return _UpdateSize();
}

bool CIndexBuffer::SetData(void* _data, u32 _start, u32 _count, bool _discard)
{
	bool ret = _Upload(_data, _start, _count);

	return ret;
}

void CIndexBuffer::Release()
{
	CRender* render = CGame::Instance()->GetRender();

	if(render->Ready())
    {
		if(render->GetIndexBuffer() == this)
			render->SetIndexBuffer(0);
    }
}