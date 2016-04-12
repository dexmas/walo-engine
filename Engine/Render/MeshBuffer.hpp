#pragma once

#include "Render/Render.hpp"
#include "Render/IndexBuffer.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/Material.hpp"

class CMeshBuffer
{
public:
	CMeshBuffer();
	virtual ~CMeshBuffer();

	void SetVertexBuffer(EPrimitiveType _ptype, CVertexBuffer* _vbuffer, u32 _vstart, u32 _vcount);
	void SetIndexBuffer(CIndexBuffer* _ibuffer, u32 _istart, u32 _icount);
	void SetMaterial(CMaterial* _material);

	void Render();

private:
	CVertexBuffer* m_VertexBuffer;
	CIndexBuffer*  m_IndexBuffer;
	CMaterial*	   m_Material;

	EPrimitiveType m_PrimitiveType;

	u32 m_VertexStart;
	u32 m_VertexCount;
	u32 m_IndexStart;
	u32 m_IndexCount;

	CRender* m_Render;
};