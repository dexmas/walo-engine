#pragma once

#include "Scene/Component.hpp"
#include "Scene/Physics2D/RigidBody2D.hpp"
#include "Scene/Scene2D/Object2D.hpp"

class b2Body;
class b2Fixture;

class CCollisionShape2D:public CComponentImpl<CPhysics2D>
{
	friend class CRigidBody2D;
public:
	CCollisionShape2D(EComponentType _type);

	virtual ~CCollisionShape2D();

	bool HitTest(const CVector2& _pos);

protected:

	CRect _GetNodeBound();

	void _CreateDefaultBody();
	void _DestroyDefaultBody();

	void OnAssigned(CNode* _node);
	void OnAttached(CComponent* _component);
	void OnDetached(CComponent* _component);

	virtual void SetupRigidbody(CRigidBody2D* _rbody);
	virtual void _CreateFixture();
	virtual void _ReleaseFixture();

	void SetEnabled(bool _enabled);
	void OnTransformed();

	f32 m_ScaleX;
	f32 m_ScaleY;

	b2Body* m_Body;
	b2Fixture* m_Fixture;
};