#include "Scene/Physics2D/RigidBody2D.hpp"
#include "Scene/Physics2D/CollisionBox2D.hpp"

#include <Box2D.h>

CRigidBody2D::CRigidBody2D():CComponentImpl(ECT_RIGIDBODY2D)
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
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = this;
    
    m_Body = m_System->GetWorld()->CreateBody(&bodyDef);
    
    m_CollisionShape = 0;
}

CRigidBody2D::~CRigidBody2D()
{
    if(m_CollisionShape)
        delete m_CollisionShape;
    
    m_System->GetWorld()->DestroyBody(m_Body);
}

void CRigidBody2D::OnEnabled(bool _enabled)
{
	m_Body->SetActive(_enabled);
	m_Body->SetAwake(_enabled);
}

void CRigidBody2D::OnTransformed()
{
	const CMatrix4& tr = m_Node->GetTransform();
	CVector3 pos;
	tr.TransformVector(pos);

	f32 rot = m_Node->GetRotation().Z;

	m_Body->SetTransform(b2Vec2(pos.X, pos.Y), rot);
}

void CRigidBody2D::OnAttached(CComponent* _component)
{
	if(_component->GetType() == ECT_COLLISIONBOX2D)
	{
		((CCollisionBox2D*)_component)->SetupRigidbody(this);
	}
}

void CRigidBody2D::OnDetached(CComponent* _component)
{
	if(_component->GetType() == ECT_COLLISIONBOX2D)
	{
		((CCollisionBox2D*)_component)->SetupRigidbody(0);
	}
}