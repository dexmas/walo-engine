#pragma once

#include "Core/Types.hpp"
#include "Core/List.hpp"
#include "Core/BBox.hpp"

#include "Scene/DebugRenderer.hpp"

class COctree;
class CViewFrustum;
class CObject3D;

class COctreeNode
{
public:
	COctreeNode(const CBBox& _box, u32 _level, COctreeNode* _parent, COctree* _root, u32 _index);
	virtual ~COctreeNode();

	COctreeNode* GetChild(u32 _id);
	void Clear();
	void DeleteChild(u32 _id);
	void InsertObject(CObject3D* _drw);
	void RemoveObject(CObject3D* _drw);
	void GetVisibleList(const CViewFrustum& _frustum, CList<CObject3D*>& _list);

	CBBox& GetBBox(){return m_BBox;}

#ifdef _DEBUG
	void DebugRender(const CViewFrustum& _frustum);
#endif

protected:
	bool _CheckFit(const CBBox& _box);
	void _IncDrawables();
	void _DecDrawables();

	COctreeNode* m_Parent;
	COctree*	 m_Root;
	CBBox		 m_BBox;
	u32			 m_Level;
	u32			 m_Index;
	COctreeNode* m_Childs[8];
	u32			 m_NumDrawables;

	CList<CObject3D*> m_Drawables;
};

class COctree: public COctreeNode
{
	friend class COctreeNode;
public:
	COctree():COctreeNode(CBBox(CVector3(-600, -600, -600), CVector3(600, 600, 600)), 0, 0, this, 0xFF)
	{
		m_MaxLevels = 8;
	}

	u32 MaxLevels() const
	{
		return m_MaxLevels;
	}

private:
	u32 m_MaxLevels;
};