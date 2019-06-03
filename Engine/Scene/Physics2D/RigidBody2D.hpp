#pragma once

#include "Game.hpp"
#include "Scene/Component.hpp"

#include "Scene/Physics2D/Physics2D.hpp"

class CCollisionShape2D;
class b2Body;

class CRigidBody2D: public CComponentImpl<CPhysics2D>
{
public:
    CRigidBody2D();
    ~CRigidBody2D();

	b2Body* GetBody() {return m_Body;}

protected:
	void SetEnabled(bool _enabled);

	void OnTransformed();

	void OnAttached(CComponent* _component);

	void OnDetached(CComponent* _component);

private:
	b2Body* m_Body;
};