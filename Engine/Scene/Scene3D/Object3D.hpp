#pragma once

#include "Core/BBox.hpp"
#include "Scene/Component.hpp"
#include "Scene/Node.hpp"
#include "Scene/Scene3D/Scene3D.hpp"

#include "Game.hpp"
#include "Render/Render.hpp"

class COctreeNode;

class CObject3D: public CComponentImpl<CScene3D>
{
	friend class CScene3D;
	friend class COctreeNode;
public:
	CObject3D(EComponentType _type = ECT_OBJECT3D):CComponentImpl(_type)
	{
		m_Render = CGame::Instance()->GetRender();
		m_OctreeNode = 0;
	}

	virtual ~CObject3D(){}

	virtual void OnTransformed()
	{
		m_Transform = m_Node->GetTransform();

		_UpdateBound();

		m_System->TransformComponent(this);
	}

	CBBox GetBound() const {return m_Bound;}
	CBBox GetLocalBound() const {return m_LocalBound;}

	virtual void Render() {}

#ifdef _DEBUG
	virtual void DebugRender()
	{
		CDebugRenderer::Instance()->AddBBox(m_Bound, CColor::GREEN);
	}
#endif

protected:
	void _UpdateBound()
	{
		m_Bound = m_LocalBound;
		m_Transform.TransformAABBox(m_Bound);
	}

	CBBox    m_LocalBound;
	CBBox    m_Bound;
	CRender* m_Render;
	CMatrix4 m_Transform;

	COctreeNode* m_OctreeNode;
};