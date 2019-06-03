#include "Scene/Scene3D/Camera3D.hpp"
#include "Scene/Node.hpp"
#include "Scene/System.hpp"
#include "Scene/Scene3D/Scene3D.hpp"
#include "Game.hpp"

CCamera3D::CCamera3D():CComponentImpl<CScene3D>(ECT_CAMERA3D)
{
	m_Projection = CMatrix4::IDENTY;
	m_Projection.Perspective(CMath::PI/2.5f, 4.0f/3.0f, 0.10f, 1000.0f);
}

void CCamera3D::OnEnabled(bool _enabled)
{
	if(_enabled)
	{
		m_System->SetCamera(this);
	}
	else
	{
		m_System->SetCamera(0);
	}
}

void CCamera3D::OnTransformed()
{
	m_View = CMatrix4::IDENTY;
		
	CMatrix4 rotate = CMatrix4::IDENTY;

	CNode* node = m_Node;
	node->GetTransform();

	//TODO: Optimize rotation
	CVector3 rot = CVector3(0.0f, 0.0f, node->GetRotation().Z);
	rotate.Rotate(rot);

	m_View *= rotate;

	rot = CVector3(node->GetRotation().X, 0.0f, 0.0f);
	rotate.Rotate(rot);

	m_View *= rotate;

	rot = CVector3(0.0f, node->GetRotation().Y, 0.0f);
	rotate.Rotate(rot);

	m_View *= rotate;

	CMatrix4 translate = CMatrix4::IDENTY;
	translate.Translate(node->GetPosition());

	m_View *= translate;

	CMatrix4 frustum = CMatrix4::IDENTY;

	frustum *= m_Projection;
	frustum *= m_View;

	m_Frustum.Setup(frustum);
}