#pragma once

#include <Core/Map.hpp>
#include <Core/LinkedList.hpp>
#include <Render/VertexBuffer.hpp>
#include <Render/IndexBuffer.hpp>
#include <Game.hpp>

class CObject2D;

class CBatch2D: public CLinkedListNode<CBatch2D>
{
public:
	CBatch2D(CMaterial* _mat, CTexture* _tex, bool _clip, const CRect& _rect)
	{
		m_Material = _mat;
		m_Texture = _tex;
		m_BatchDirty = false;
		m_IndexCount = 0;
		m_Clip = _clip;
		m_ClipRect = _rect;

		m_Render = CGame::Instance()->GetRender();

		m_VertexBuffer = m_Render->CreateVertexBuffer();
		m_IndexBuffer = m_Render->CreateIndexBuffer();
	}

	~CBatch2D()
	{
		if(m_VertexBuffer)
		{
			delete m_VertexBuffer;
		}

		if(m_IndexBuffer)
		{
			delete m_IndexBuffer;
		}
	}

	bool Dirty()
	{
		return m_BatchDirty;
	}

	void MarkDirty()
	{
		m_BatchDirty = true;
	}

	bool Compare(CObject2D* _drw);

	void Add(CObject2D* _drw);

	void Remove(CObject2D* _drw);

	u32 Count()
	{
		return m_List.Size();
	}

	void Update(CObject2D* _drw);

	void Render();

#ifdef _DEBUG
	void DebugRender();
#endif

private:
	CMap<CObject2D*, u32> m_VertexMap;
	CList<CObject2D*> m_List;

	bool  m_Clip;
	CRect m_ClipRect;

	s32 m_IndexCount;

	CVertexBuffer* m_VertexBuffer;
	CIndexBuffer*  m_IndexBuffer;

	CRender* m_Render;

	CMaterial* m_Material;
	CTexture* m_Texture;

	bool m_BatchDirty;
};