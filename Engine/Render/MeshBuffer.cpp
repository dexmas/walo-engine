#include "Render/MeshBuffer.hpp"

#include "Game.hpp"

CMeshBuffer::CMeshBuffer()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
	m_Material = 0;

	m_PrimitiveType = EPT_TRIANGLES;

	m_VertexStart = 0;
	m_VertexCount = 0;
	m_IndexStart = 0;
	m_IndexCount = 0;

	m_Render = CGame::Instance()->GetRender();
}

CMeshBuffer::~CMeshBuffer()
{
}

void CMeshBuffer::SetVertexBuffer(EPrimitiveType _ptype, CVertexBuffer* _vbuffer, u32 _vstart, u32 _vcount)
{
	m_VertexBuffer = _vbuffer;
	m_VertexStart = _vstart;
	m_VertexCount = _vcount;
}

void CMeshBuffer::SetIndexBuffer(CIndexBuffer* _ibuffer, u32 _istart, u32 _icount)
{
	m_IndexBuffer = _ibuffer;
	m_IndexStart = _istart;
	m_IndexCount = _icount;
}

void CMeshBuffer::SetMaterial(CMaterial* _material)
{
	m_Material = _material;
}

void CMeshBuffer::Render()
{
	m_Render->SetVertexBuffer(m_VertexBuffer);

	if(m_IndexBuffer)
	{
		m_Render->SetIndexBuffer(m_IndexBuffer);
		m_Render->SetMaterial(m_Material);
		m_Render->RenderIndexed(m_PrimitiveType, m_IndexStart, m_IndexCount);
	}
	else
	{
		m_Render->SetMaterial(m_Material);
		m_Render->Render(m_PrimitiveType, m_VertexStart, m_VertexCount);
	}
}