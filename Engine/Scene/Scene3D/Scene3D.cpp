#include "Scene/Scene3D/Scene3D.hpp"
#include "Scene/Scene3D/Camera3D.hpp"
#include "Scene/Scene3D/Object3D.hpp"
#include "Scene/Scene3D/StaticModel.hpp"

#include "Game.hpp"
#include "Render/Render.hpp"
#include "Render/Mesh.hpp"

CScene3D::CScene3D()
{
	m_Camera = 0;
	m_Render = CGame::Instance()->GetRender();
}

void CScene3D::AddComponent(CComponent* _comp)
{
	CObject3D* _drw = (CObject3D*)_comp;

	if(!_drw->m_OctreeNode)
	{
		m_Octree.InsertObject(_drw);
	}
}

void CScene3D::RemoveComponent(CComponent* _comp)
{
	CObject3D* _drw = (CObject3D*)_comp;

	if(_drw->m_OctreeNode)
	{
		_drw->m_OctreeNode->RemoveObject(_drw);
	}
}

void CScene3D::TransformComponent(CComponent* _comp)
{
	CObject3D* _drw = (CObject3D*)_comp;

	if(_drw->m_OctreeNode)
	{
		_drw->m_OctreeNode->RemoveObject(_drw);
		m_Octree.InsertObject(_drw);
	}
}

void CScene3D::SetCamera(CCamera3D* _camera)
{
	m_Camera = _camera;
}

void CScene3D::Render()
{
	if(m_Camera)
	{
		CMatrix4 viewproj = m_Camera->GetProjection();
		viewproj *= m_Camera->GetView();

		m_Render->SetTransform(ETT_VIEWPROJ, viewproj);
	}
	else
		return;

	const CViewFrustum& frustum = m_Camera->GetFrustum();

	m_RenderList.Clear();
	m_Octree.GetVisibleList(frustum, m_RenderList);

	CList<CObject3D*>::CIterator its = m_RenderList.Begin();

	while(its != m_RenderList.End())
	{
		CObject3D* drw = *its;
		
		drw->Render();

#ifdef _DEBUG
		if (CGame::Instance()->GetDebug())
		{
			drw->DebugRender();
		}
#endif
		++its;
	}

#ifdef _DEBUG
	if (CGame::Instance()->GetDebug())
	{
		m_Octree.DebugRender(frustum);
		m_Render->SetTransform(ETT_MODEL, CMatrix4::IDENTY);
		CDebugRenderer::Instance()->Render();
	}
#endif
}