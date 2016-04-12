#pragma once

#include "Core/Types.hpp"
#include "Core/List.hpp"
#include "Core/Rect.hpp"

#include "Scene/DebugRenderer.hpp"

class CQuadtree;
class CObject2D;

class CQuadtreeNode
{
public:
	CQuadtreeNode(const CRect& _bound, u32 _level, CQuadtreeNode* _parent, CQuadtree* _root, u32 _index);
	virtual ~CQuadtreeNode();

	CQuadtreeNode* GetChild(u32 _id);
	void Clear();
	void DeleteChild(u32 _id);
	void InsertObject(CObject2D* _drw);
	void RemoveObject(CObject2D* _drw);
	void QueryVisibility(const CRect& _camera, CList<CObject2D*>& _show, CList<CObject2D*>& _hide);

	CRect& GetBound() { return m_Bound; }

#ifdef _DEBUG
	void DebugRender(const CRect& _camera);
#endif

protected:
	bool _CheckFit(const CRect& _bound);
	void _IncDrawables();
	void _DecDrawables();

	CQuadtreeNode* m_Parent;
	CQuadtree*	 m_Root;
	CRect		 m_Bound;
	u32			 m_Level;
	u32			 m_Index;
	CQuadtreeNode* m_Childs[4];
	u32			 m_NumDrawables;

	CList<CObject2D*> m_Drawables;
};

class CQuadtree : public CQuadtreeNode
{
	friend class CQuadtreeNode;
public:
	CQuadtree() :CQuadtreeNode(CRect(0, 0, 960, 640), 0, 0, this, 0xFF)
	{
		m_MaxLevels = 4;
	}

	u32 MaxLevels() const
	{
		return m_MaxLevels;
	}

private:
	u32 m_MaxLevels;
};