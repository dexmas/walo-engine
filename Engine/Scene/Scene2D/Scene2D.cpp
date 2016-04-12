#include "Scene/Scene2D/Scene2D.hpp"
#include "Scene/Scene2D/Object2D.hpp"
#include "Scene/Scene2D/Sprite2D.hpp"
#include "Scene/Scene2D/Text2D.hpp"

#include "Render/Render.hpp"
#include "Game.hpp"

#include <Core/Log.hpp>

CScene2D::CScene2D()
{
	m_Render = CGame::Instance()->GetRender();

	CVector2 scrSize = CGame::Instance()->GetDevice()->GetScreenSize();
	m_ScreenRect = CRect(0, 0, scrSize.X, scrSize.Y);
	m_Projection.Ortho(scrSize.X, scrSize.Y, -1000.0f, 1000.0f);
}

CScene2D::~CScene2D()
{
}

static u32 s_batch_count = 0;

void CScene2D::AddComponent(CComponent* _comp)
{
	CObject2D* drw = (CObject2D*)_comp;

	m_Quadtree.InsertObject(drw);
}

void CScene2D::RemoveComponent(CComponent* _comp)
{
	CObject2D* drw = (CObject2D*)_comp;
	CQuadtreeNode* node = drw->m_QuadtreeNode;
	
	_RemoveVisible(drw);

	if (node)
	{
		node->RemoveObject(drw);
	}
}


void CScene2D::_AddVisible(CObject2D* _drw)
{
	CBatch2D* batch = _FindBatch(_drw);

	if(!batch)
	{
		LOG("Created new batch. Current batch count: %d\n", ++s_batch_count);

		batch = new CBatch2D(_drw->GetMaterial(), _drw->GetTexture(), _drw->m_Clipped, _drw->m_ClipRect);
		m_Batches.PushBack(batch);
	}

	batch->Add(_drw);
}

void CScene2D::_RemoveVisible(CObject2D* _drw)
{
	CBatch2D* batch = _drw->m_CurrentBatch;

	if(batch)
	{
		batch->Remove(_drw);
		
		if(batch->Count() == 0)
		{
			m_Batches.Remove(batch);
			delete batch;

			LOG("Removed empty batch. Current batch count: %d\n", --s_batch_count);
		}
	}
}

CBatch2D* CScene2D::_FindBatch(CObject2D* _drw)
{
	CBatch2D* batch = m_Batches.GetHead();

	while (batch)
	{
		if(batch->Compare(_drw))
		{
			return batch;
		}

		batch = batch->GetNext();
	}

	return 0;
}

void CScene2D::Render()
{
	m_Render->SetTransform(ETT_VIEWPROJ, m_Projection);

	m_Show.Clear();
	m_Hide.Clear();

	m_Quadtree.QueryVisibility(m_ScreenRect, m_Show, m_Hide);

	CList<CObject2D*>::CIterator oi = m_Hide.Begin();

	while (oi != m_Hide.End())
	{
		_RemoveVisible(*oi);

		++oi;
	}

	oi = m_Show.Begin();

	while (oi != m_Show.End())
	{
		_AddVisible(*oi);

		++oi;
	}

	CBatch2D* batch = m_Batches.GetHead();

	while (batch)
	{
		batch->Render();

#ifdef _DEBUG
		//batch->DebugRender();
#endif
		batch = batch->GetNext();
	}

#ifdef _DEBUG
	m_Render->SetTransform(ETT_MODEL, CMatrix4::IDENTY);

	//m_Quadtree.DebugRender(m_Camera);

	CDebugRenderer::Instance()->Render();
#endif
}