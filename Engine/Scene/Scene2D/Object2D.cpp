#include "Scene/Scene2D/Object2D.hpp"
#include "Game.hpp"

#include "Scene/Physics2D/CollisionShape2D.hpp"

CObject2D::CObject2D(EComponentType _type):CComponentImpl(_type)
{
	m_VertexesDirty = true;

	m_Color = CColor::WHITE;

	m_QuadtreeNode = 0;
	m_CurrentBatch = 0;
	m_TransformDirty = true;

	m_Material = 0;
	m_Texture = 0;
	m_Order = 0.0f;
	m_Clipped = false;
	m_ClipChild = false;

	m_Render = CGame::Instance()->GetRender();
	SetMaterial(CAssetsDB::Instance()->GetMaterial("render2d_vc.mat"));
}

CObject2D::~CObject2D()
{
	
}

void CObject2D::SetOrder(f32 _order) 
{
	m_Order = _order;

	if(m_Node)
	{
		CVector3 pos = m_Node->GetPosition();
		pos.Z = _order;

		m_Node->SetPosition(pos);
	}
}

f32 CObject2D::GetOrder() const 
{
	return m_Order;
}

bool CObject2D::HitTest(const CVector2& _point)
{
	bool haveCollider = false;

	if(_HitTestPhysics(_point, haveCollider))
		return true;

	if(haveCollider)
		return false;

	/*CVector2 p3(_point.X, _point.Y);
	CVector2 p1(-m_Center.X, -m_Center.Y);
	CVector2 p2(m_Size.X - m_Center.X, m_Size.Y - m_Center.Y);

	m_Transform.TransformVector(p1);
	m_Transform.TransformVector(p2);

	if(p3.X > p1.X && p3.X < p2.X && p3.Y > p1.Y && p3.Y < p2.Y)
	{
		return true;
	}*/

	if(_point.X > m_Bound.X && _point.X < (m_Bound.X + m_Bound.Width) && _point.Y > m_Bound.Y && _point.Y < (m_Bound.Y + m_Bound.Height))
	{
		return true;
	}

	return false;
}

bool CObject2D::_HitTestPhysics(const CVector2& _point, bool& _processed)
{
	CList<CComponent*> clist;
	m_Node->GetComponents(ECT_COLLISIONBOX2D, clist);

	if (!clist.Size())
	{
		return false;
	}

	bool hited = false;

	CList<CComponent*>::CIterator it = clist.Begin();
	while(it != clist.End())
	{
		CComponent* cm = *it;

		CCollisionShape2D* shape = (CCollisionShape2D*)cm;
		hited |= shape->HitTest(_point);

		++it;
	}

	return hited;
}