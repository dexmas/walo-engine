#include "Scene/Scene3D/Octree.hpp"

#include "Scene/Scene3D/Camera3D.hpp"
#include "Scene/Scene3D/Object3D.hpp"

COctreeNode::COctreeNode(const CBBox& _box, u32 _level, COctreeNode* _parent, COctree* _root, u32 _index)
{
	m_Parent = _parent;
	m_Root = _root;
	m_Level = _level;
	m_BBox = _box;
	m_Index = _index;
	m_NumDrawables = 0;

	for(u32 i=0;i<8;i++)
	{
		m_Childs[i] = 0;
	}
}

COctreeNode::~COctreeNode()
{
	Clear();
}

COctreeNode* COctreeNode::GetChild(u32 _id)
{
	if(_id > 7)
		return 0;

	if(!m_Childs[_id])
	{
		CVector3 newMin = m_BBox.Min();
		CVector3 newMax = m_BBox.Max();
		CVector3 oldCenter = m_BBox.Center();

		if(_id & 1)
			newMin.X = oldCenter.X;
		else
			newMax.X = oldCenter.X;

		if(_id & 2)
			newMin.Y = oldCenter.Y;
		else
			newMax.Y = oldCenter.Y;

		if(_id & 4)
			newMin.Z = oldCenter.Z;
		else
			newMax.Z = oldCenter.Z;

		m_Childs[_id] = new COctreeNode(CBBox(newMin, newMax), m_Level + 1, this, m_Root, _id);
	}

	return m_Childs[_id];
}

void COctreeNode::DeleteChild(u32 _id)
{
	if(_id > 7 || !m_Childs[_id])
		return;

	delete m_Childs[_id];
	m_Childs[_id] = 0;
}

void COctreeNode::Clear()
{
	CList<CObject3D*>::CIterator it = m_Drawables.Begin();

	while(it != m_Drawables.End())
	{
		CObject3D* drw = *it;

		drw->m_OctreeNode = 0;
	}

	m_Drawables.Clear();

	for(u32 i=0;i<8;i++)
	{
		if(m_Childs[i])
		{
			DeleteChild(i);
			m_Childs[i] = 0;
		}
	}
}

void COctreeNode::InsertObject(CObject3D* _drw)
{
	const CBBox box = _drw->GetBound();

	if(_CheckFit(box))
	{
		m_Drawables.PushBack(_drw);
		_IncDrawables();

		_drw->m_OctreeNode = this;
	}
	else
	{
		CVector3 boxCenter = box.Center();
		CVector3 center    = m_BBox.Center();

		u32 x = (boxCenter.X < center.X) ? 0 : 1;
		u32 y = (boxCenter.Y < center.Y) ? 0 : 2;
		u32 z = (boxCenter.Z < center.Z) ? 0 : 4;

		GetChild(x + y + z)->InsertObject(_drw);
	}
}

void COctreeNode::RemoveObject(CObject3D* _drw)
{
	m_Drawables.Erase(_drw);
	_DecDrawables();

	_drw->m_OctreeNode = 0;
}

bool COctreeNode::_CheckFit(const CBBox& _box)
{
	CVector3 boxSize = _box.Size();
	CVector3 halfSize = m_BBox.Size() * 0.5f;

	// If max split level, size always OK, otherwise check that box is at least half size of octant
	if(m_Level >= m_Root->MaxLevels() || boxSize.X >= halfSize.X || boxSize.Y >= halfSize.Y || boxSize.Z >= halfSize.Z)
	{
		return true;
	}
	// Also check if the box can not fit a child octant's culling box, in that case size OK (must insert here)
	else
	{
		const CVector3& boxMin = _box.Min();
		const CVector3& boxMax = _box.Max();
		const CVector3& min = m_BBox.Min();
		const CVector3& max = m_BBox.Max();

		if(boxMin.X <= min.X - 0.5f * halfSize.X ||
			boxMax.X >= max.X + 0.5f * halfSize.X ||
			boxMin.Y <= min.Y - 0.5f * halfSize.Y ||
			boxMax.Y >= max.Y + 0.5f * halfSize.Y ||
			boxMin.Z <= min.Z - 0.5f * halfSize.Z ||
			boxMax.Z >= max.Z + 0.5f * halfSize.Z)
			return true;
	}

	// Bounding box too small, should create a child octant
	return false;
}

#ifdef _DEBUG
void COctreeNode::DebugRender(const CViewFrustum& _frustum)
{
	CDebugRenderer::Instance()->AddBBox(m_BBox, CColor::GRAY);

	for(u32 i=0;i<8;i++)
	{
		if(m_Childs[i])
		{
			if(_frustum.Intersect(m_BBox) != EI_OUTSIDE)
				m_Childs[i]->DebugRender(_frustum);
		}
	}
}
#endif

void COctreeNode::_IncDrawables()
{
	m_NumDrawables++;

	if(m_Parent)
		m_Parent->_IncDrawables();
}

void COctreeNode::_DecDrawables()
{
	 COctreeNode* parent = m_Parent;
        
    m_NumDrawables--;

    if(m_NumDrawables <= 0)
    {
        if (parent)
            parent->DeleteChild(m_Index);
    }
        
    if(parent)
        parent->_DecDrawables();
}

void COctreeNode::GetVisibleList(const CViewFrustum& _frustum, CList<CObject3D*>& _list)
{
	CList<CObject3D*>::CIterator it = m_Drawables.Begin();

	while(it != m_Drawables.End())
	{
		CObject3D* drw = *it;
		const CBBox& box = drw->GetBound();

		if(_frustum.Intersect(box) != EI_OUTSIDE)
			_list.PushBack(drw);

		++it;
	}

	for(u32 i=0;i<8;i++)
	{
		if(m_Childs[i])
		{
			if(_frustum.Intersect(m_Childs[i]->GetBBox()) != EI_OUTSIDE)
			{
				m_Childs[i]->GetVisibleList(_frustum, _list);
			}
		}
	}
}