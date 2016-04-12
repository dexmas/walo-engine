#pragma once

#include "Core/Types.hpp"
#include "Scene/System.hpp"
#include "Scene/DebugRenderer.hpp"

#include "Scene/Node.hpp"

class b2Vec2;
class b2Contact;
class b2World;
class CPhysics2DProxy;

class CPhysics2D: public CSystem
{
public:
	CPhysics2D();
	~CPhysics2D();

	void OnAssigned(CNode* _node);
	void Update(f32 _dt);
	void Render();

	b2World* GetWorld() {return m_World;}

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	b2World* m_World;
	CPhysics2DProxy* m_Proxy;
};