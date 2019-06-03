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
	m_ViewRect = CRect(0, 0, CGame::Instance()->GetDevice()->GetScreenSize().X, CGame::Instance()->GetDevice()->GetScreenSize().Y);
	m_Projection.Ortho(m_ViewRect.Width, m_ViewRect.Height, -1000, 1000);
}

CScene2D::~CScene2D()
{
}

static u32 s_batch_count = 0;

void CScene2D::AddComponent(CComponent* _comp)
{
	CObject2D* drw = (CObject2D*)_comp;

	if(drw->GetType() == ECT_SPRITE2D || drw->GetType() == ECT_TEXT2D || drw->GetType() == ECT_PROGRESS2D || drw->GetType() == ECT_TILEMAP2D)
	{
		m_Quadtree.InsertObject(drw);
	}
}

void CScene2D::RemoveComponent(CComponent* _comp)
{
	CObject2D* drw = (CObject2D*)_comp;

	if(drw->GetType() == ECT_SPRITE2D || drw->GetType() == ECT_TEXT2D || drw->GetType() == ECT_PROGRESS2D || drw->GetType() == ECT_TILEMAP2D)
	{
		CQuadtreeNode* node = drw->m_QuadtreeNode;

		if(node)
		{
			node->RemoveObject(drw);
		}

		CBatch2D* batch = drw->m_CurrentBatch;

		if(batch)
		{
			batch->Remove(drw);

			if (batch->Count() == 0)
			{
				m_Batches.Erase(batch);
				delete batch;

				LOG("Removed empty batch. Current batch count: %d\n", --s_batch_count);
			}
		}
	}
}

void CScene2D::SetCamera(CRect& _camera)
{
	m_ViewRect = _camera;
	m_Projection.Ortho(m_ViewRect.Width, m_ViewRect.Height, -1000, 1000);
}

CBatch2D* CScene2D::_FindBatch(CObject2D* _drw)
{
	CList<CBatch2D*>::CIterator it = m_Batches.Begin();

	while (it != m_Batches.End())
	{
		CBatch2D* batch = *it;

		if(batch->Compare(_drw))
		{
			return batch;
		}

		++it;
	}

	return 0;
}

void CScene2D::Render()
{
	m_Render->SetTransform(ETT_VIEWPROJ, m_Projection);

	m_ShowList.Clear();
	m_HideList.Clear();

	m_Quadtree.QueryVisibility(m_ViewRect, m_ShowList, m_HideList);

	CList<CObject2D*>::CIterator it = m_HideList.Begin();

	while(it != m_HideList.End())
	{
		CBatch2D* batch = (*it)->m_CurrentBatch;

		if(batch)
		{
			batch->Remove(*it);
		
			if(batch->Count() == 0)
			{
				m_Batches.Erase(batch);
				delete batch;

				LOG("Removed empty batch. Current batch count: %d\n", --s_batch_count);
			}
		}

		++it;
	}

	it = m_ShowList.Begin();

	while(it != m_ShowList.End())
	{
		CObject2D* drw = *it;
		CBatch2D* batch = _FindBatch(drw);

		if(!batch)
		{
			LOG("Created new batch. Current batch count: %d\n", ++s_batch_count);

			batch = new CBatch2D(drw->GetMaterial(), drw->GetTexture(), drw->m_Clipped, drw->m_ClipRect);
			m_Batches.PushBack(batch);
		}

		batch->Add(drw);

		++it;
	}

	CList<CBatch2D*>::CIterator jt = m_Batches.Begin();

	while (jt != m_Batches.End())
	{
		CBatch2D* batch = *jt;

		batch->Render();

#ifdef _DEBUG
		batch->DebugRender();
#endif
		++jt;
	}

#ifdef _DEBUG
	m_Render->SetTransform(ETT_MODEL, CMatrix4::IDENTY);

	//m_Quadtree.DebugRender(m_Camera);

	CDebugRenderer::Instance()->Render();
#endif
}