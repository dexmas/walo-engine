#include "Scene/Scene2D/Quadtree.hpp"
#include "Scene/Scene2D/Object2D.hpp"

CQuadtreeNode::CQuadtreeNode(const CRect& _bound, u32 _level, CQuadtreeNode* _parent, CQuadtree* _root, u32 _index)
{
	m_Parent = _parent;
	m_Root = _root;
	m_Level = _level;
	m_Bound = _bound;
	m_Index = _index;
	m_NumDrawables = 0;

	for (u32 i = 0; i < 4; i++)
	{
		m_Childs[i] = 0;
	}
}

CQuadtreeNode::~CQuadtreeNode()
{
	Clear();
}

CQuadtreeNode* CQuadtreeNode::GetChild(u32 _id)
{
	if (_id > 3)
		return 0;

	if (!m_Childs[_id])
	{
		CVector2 newMin = CVector2(m_Bound.X, m_Bound.Y);
		CVector2 newMax = CVector2(m_Bound.X + m_Bound.Width, m_Bound.Y + m_Bound.Height);
		CVector2 oldCenter = CVector2(m_Bound.X + m_Bound.Width / 2.0f, m_Bound.Y + m_Bound.Height / 2.0f);

		if (_id & 1)
			newMin.X = oldCenter.X;
		else
			newMax.X = oldCenter.X;

		if (_id & 2)
			newMin.Y = oldCenter.Y;
		else
			newMax.Y = oldCenter.Y;

		m_Childs[_id] = new CQuadtreeNode(CRect(newMin.X, newMin.Y, newMax.X - newMin.X, newMax.Y - newMin.Y), m_Level + 1, this, m_Root, _id);
	}

	return m_Childs[_id];
}

void CQuadtreeNode::DeleteChild(u32 _id)
{
	if (_id > 3 || !m_Childs[_id])
		return;

	delete m_Childs[_id];
	m_Childs[_id] = 0;
}

void CQuadtreeNode::Clear()
{
	CList<CObject2D*>::CIterator it = m_Drawables.Begin();

	while (it != m_Drawables.End())
	{
		CObject2D* drw = *it;

		drw->m_QuadtreeNode = 0;
	}

	m_Drawables.Clear();

	for (u32 i = 0; i<4; i++)
	{
		if (m_Childs[i])
		{
			DeleteChild(i);
			m_Childs[i] = 0;
		}
	}
}

void CQuadtreeNode::InsertObject(CObject2D* _drw)
{
	const CRect bound = _drw->GetBound();

	if (_CheckFit(bound))
	{
		m_Drawables.PushBack(_drw);
		_IncDrawables();

		_drw->m_QuadtreeNode = this;
	}
	else
	{
		CVector2 boxCenter = CVector2(bound.X + bound.Width / 2.0f, bound.Y + bound.Height / 2.0f);
		CVector2 center = CVector2(m_Bound.X + m_Bound.Width / 2.0f, m_Bound.Y + m_Bound.Height / 2.0f);

		u32 x = (boxCenter.X < center.X) ? 0 : 1;
		u32 y = (boxCenter.Y < center.Y) ? 0 : 2;

		GetChild(x + y)->InsertObject(_drw);
	}
}

void CQuadtreeNode::RemoveObject(CObject2D* _drw)
{
	m_Drawables.Erase(_drw);
	_DecDrawables();

	_drw->m_QuadtreeNode = 0;
}

bool CQuadtreeNode::_CheckFit(const CRect& _bound)
{
	CVector2 size = CVector2(_bound.Width, _bound.Height);
	CVector2 halfSize = CVector2(m_Bound.Width * 0.5f, m_Bound.Height * 0.5f);

	// If max split level, size always OK, otherwise check that box is at least half size of octant
	if (m_Level >= m_Root->MaxLevels() || size.X >= halfSize.X || size.Y >= halfSize.Y)
	{
		return true;
	}
	// Also check if the box can not fit a child octant's culling box, in that case size OK (must insert here)
	else
	{
		const CVector2& boxMin = CVector2(_bound.X, _bound.Y);
		const CVector2& boxMax = CVector2(_bound.X + _bound.Width, _bound.Y + _bound.Height);
		const CVector2& min = CVector2(m_Bound.X, m_Bound.Y);
		const CVector2& max = CVector2(m_Bound.X + m_Bound.Width, m_Bound.Y + m_Bound.Height);

		if (boxMin.X <= min.X - 0.5f * halfSize.X ||
			boxMax.X >= max.X + 0.5f * halfSize.X ||
			boxMin.Y <= min.Y - 0.5f * halfSize.Y ||
			boxMax.Y >= max.Y + 0.5f * halfSize.Y)
			return true;
	}

	// Bounding box too small, should create a child octant
	return false;
}

#ifdef _DEBUG
void CQuadtreeNode::DebugRender(const CRect& _camera)
{
	CDebugRenderer::Instance()->AddRect(m_Bound, CColor::GRAY);

	for (u32 i = 0; i<4; i++)
	{
		if (m_Childs[i])
		{
			if (_camera.Intersect(m_Bound) != EI_OUTSIDE)
				m_Childs[i]->DebugRender(_camera);
		}
	}
}
#endif

void CQuadtreeNode::_IncDrawables()
{
	m_NumDrawables++;

	if (m_Parent)
		m_Parent->_IncDrawables();
}

void CQuadtreeNode::_DecDrawables()
{
	CQuadtreeNode* parent = m_Parent;

	m_NumDrawables--;

	if (m_NumDrawables <= 0)
	{
		if (parent)
			parent->DeleteChild(m_Index);
	}

	if (parent)
		parent->_DecDrawables();
}

void CQuadtreeNode::QueryVisibility(const CRect& _camera, CList<CObject2D*>& _showList, CList<CObject2D*>& _hideList)
{
	CList<CObject2D*>::CIterator it = m_Drawables.Begin();

	while (it != m_Drawables.End())
	{
		CObject2D* drw = *it;
		const CRect& bound = drw->GetBound();

		if (_camera.Intersect(bound) != EI_OUTSIDE)
		{
			if (drw->m_CurrentBatch == 0)
			{
				_showList.PushBack(drw);
			}
		}
		else
		{
			if (drw->m_CurrentBatch != 0)
			{
				_hideList.PushBack(drw);
			}
		}

		++it;
	}

	for (u32 i = 0; i<4; i++)
	{
		if (m_Childs[i])
		{
			if (_camera.Intersect(m_Childs[i]->GetBound()) != EI_OUTSIDE)
			{
				m_Childs[i]->QueryVisibility(_camera, _showList, _hideList);
			}
		}
	}
}