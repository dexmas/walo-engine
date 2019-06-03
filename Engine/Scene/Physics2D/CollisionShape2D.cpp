#include "CollisionShape2D.hpp"
#include <Box2D.h>

CCollisionShape2D::CCollisionShape2D(EComponentType _type):CComponentImpl(_type)
{
	m_Fixture = 0;
	m_Body = 0;

	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
}

CCollisionShape2D::~CCollisionShape2D()
{
	_ReleaseFixture();
}

bool CCollisionShape2D::HitTest(const CVector2& _pos)
{
	if(m_Fixture && m_Fixture->TestPoint(b2Vec2(_pos.X, _pos.Y)))
		return true;
	return false;
}

CRect CCollisionShape2D::_GetNodeBound()
{
	if(!m_Node)
		return CRect();

	CList<CComponent*> clist;
	m_Node->GetComponents(clist);

	CRect rect;

	CList<CComponent*>::CIterator it = clist.Begin();
	while(it != clist.End())
	{
		CComponent* cm = *it;
		EComponentType type = cm->GetType();
		if(type == ECT_SPRITE2D || type == ECT_TEXT2D || type == ECT_TILEMAP2D || type == ECT_PROGRESS2D)
		{
			CObject2D* obj2d = (CObject2D*)cm;
				
			CRect subrc;

			subrc.X = -(obj2d->GetCenter().X);
			subrc.Y = -(obj2d->GetCenter().Y);
			subrc.Width = obj2d->GetSize().X;
			subrc.Height = obj2d->GetSize().Y;

			rect.Merge(subrc);
		}
		++it;
	}

	return rect;
}

void CCollisionShape2D::_CreateDefaultBody()
{
	b2BodyDef bodyDef;

	bodyDef.active = false;
	bodyDef.allowSleep = true;
	bodyDef.angle = 0.0f;
	bodyDef.angularDamping = 0.0f;
	bodyDef.angularVelocity = 0.0f;
	bodyDef.awake = false;
	bodyDef.bullet = false;
	bodyDef.fixedRotation = false;
	bodyDef.gravityScale = 1.0f;
	bodyDef.linearDamping = 0.0f;
	bodyDef.linearVelocity = b2Vec2(0.0f,0.0f);
	bodyDef.position = b2Vec2(0.0f,0.0f);
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;

	m_Body = m_System->GetWorld()->CreateBody(&bodyDef);
		
	_CreateFixture();
}

void CCollisionShape2D::_DestroyDefaultBody()
{
	if(m_Body)
	{
		_ReleaseFixture();

		m_System->GetWorld()->DestroyBody(m_Body);
		m_Body = 0;
	}
}

void CCollisionShape2D::OnAssigned(CNode* _node)
{
	CComponentImpl::OnAssigned(_node);

	CList<CComponent*> clist;
	m_Node->GetComponents(ECT_RIGIDBODY2D, clist);

	if(!clist.Size())
	{
		SetupRigidbody(0);
	}
}

void CCollisionShape2D::OnAttached(CComponent* _component)
{
	if(_component->GetType() == ECT_RIGIDBODY2D)
	{
		SetupRigidbody((CRigidBody2D*)_component);
	}
}

void CCollisionShape2D::OnDetached(CComponent* _component)
{
	if(_component->GetType() == ECT_RIGIDBODY2D)
	{
		SetupRigidbody(0);
	}
}

void CCollisionShape2D::SetupRigidbody(CRigidBody2D* _rbody)
{
	if(_rbody)
	{
		_DestroyDefaultBody();

		m_Body = _rbody->GetBody();

		_CreateFixture();
	}
	else
	{
		_ReleaseFixture();

		m_Fixture = 0;
		m_Body = 0;

		_CreateDefaultBody();
	}
}

void CCollisionShape2D::_CreateFixture()
{
	if(m_Body && !m_Fixture)
	{
		b2FixtureDef fixtureDef;

		fixtureDef.density = 1.0f;
		fixtureDef.filter = b2Filter();
		fixtureDef.friction = 1.0f;
		fixtureDef.isSensor = true;
		fixtureDef.restitution = 0.0f;
		fixtureDef.shape = 0;
		fixtureDef.userData = this;

		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}
}

void CCollisionShape2D::_ReleaseFixture()
{
	if(m_Body && m_Fixture)
	{
		m_Body->DestroyFixture(m_Fixture);
		m_Fixture = 0;
	}
}

void CCollisionShape2D::SetEnabled(bool _enabled)
{
	if(m_Enabled == _enabled)
	{
		return;
	}

	CComponentImpl::SetEnabled(_enabled);

	if(m_Body && m_Body->GetType() == b2_staticBody)
	{
		m_Body->SetActive(m_Enabled);
		m_Body->SetAwake(m_Enabled);
	}
}

void CCollisionShape2D::OnTransformed()
{
	const CMatrix4& tr = m_Node->GetTransform();

	if(CMath::Abs(tr.Scale().X - m_ScaleX) > CMath::EPS || CMath::Abs(tr.Scale().Y - m_ScaleY) > CMath::EPS)
	{
		m_ScaleX = tr.Scale().X;
		m_ScaleY = tr.Scale().Y;

		if(m_Fixture)
		{
			_ReleaseFixture();
			_CreateFixture();
		}
	}

	if(m_Body && m_Body->GetType() == b2_staticBody)
	{
		CVector3 pos;
		tr.TransformVector(pos);

		f32 rot = m_Node->GetRotation().Z;

		m_Body->SetTransform(b2Vec2(pos.X, pos.Y), rot);
	}
}