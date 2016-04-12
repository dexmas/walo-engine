#pragma once

#include "Scene/System.hpp"
#include "Scene/Node.hpp"
#include "Core/LinkedList.hpp"
#include "Scene/Scene2D/Quadtree.hpp"
#include "Scene/Scene2D/Batch2d.hpp"

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

	void Render();

private:

	CBatch2D* _FindBatch(CObject2D* _drw);

	void _AddVisible(CObject2D* _drw);
	void _RemoveVisible(CObject2D* _drw);

	CRender* m_Render;

	CMatrix4 m_Projection;
	CRect    m_ScreenRect;

	CQuadtree m_Quadtree;
	CList<CObject2D*> m_Show;
	CList<CObject2D*> m_Hide;
	CLinkedList<CBatch2D> m_Batches;

	CDebugRenderer m_DebugRender;
};