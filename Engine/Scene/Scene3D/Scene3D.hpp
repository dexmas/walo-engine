#pragma once

#include "Scene/System.hpp"
#include "Scene/Scene3D/Octree.hpp"

#include "Scene/DebugRenderer.hpp"

class CRender;
class CObject3D;
class CCamera3D;

class CScene3D: public CSystem
{
public:
	CScene3D();

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);
	void TransformComponent(CComponent* _comp);

	void SetCamera(CCamera3D* _camera);

	void Render();

private:
	CRender* m_Render;

	COctree m_Octree;
	CCamera3D* m_Camera;

	CList<CObject3D*> m_RenderList;

	CDebugRenderer m_DebugRender;
};