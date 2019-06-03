#pragma once

#include "Scene/System.hpp"
#include "Scene/Node.hpp"
#include "Core/LinkedList.hpp"
#include "Scene/Scene2D/Quadtree.hpp"
#include "Scene/Scene2D/Batch2D.hpp"

#include "Scene/DebugRenderer.hpp"

class CRender;
class CMaterial;
class CTexture;
class CObject2D;
class CVertexBuffer;
class CIndexBuffer;

class CScene2D: public CSystem
{
public:
	CScene2D();
	~CScene2D();

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _comp);

	void SetCamera(CRect& _camera);

	void Render();

private:

	CBatch2D* _FindBatch(CObject2D* _drw);

	CRender* m_Render;

	CMatrix4 m_Projection;
	CRect	 m_ViewRect;

	CQuadtree m_Quadtree;

	CList<CBatch2D*>  m_Batches;

	CList<CObject2D*> m_ShowList;
	CList<CObject2D*> m_HideList;

	CDebugRenderer m_DebugRender;
};