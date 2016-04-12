#include "CollisionBox2D.hpp"
#include <Box2D.h>

void CCollisionBox2D::_CreateFixture()
{
	if(m_Body && !m_Fixture)
	{
		b2FixtureDef fixtureDef;
		b2PolygonShape shape;

		shape.SetAsBox((m_ScaleX * m_Width)/2.0f, (m_ScaleY * m_Height)/2.0f, b2Vec2(m_CenterX, m_CenterY), m_Rotation);

		fixtureDef.density = 1.0f;
		fixtureDef.filter = b2Filter();
		fixtureDef.friction = 1.0f;
		fixtureDef.isSensor = true;
		fixtureDef.restitution = 0.0f;
		fixtureDef.shape = &shape;
		fixtureDef.userData = this;

		m_Fixture = m_Body->CreateFixture(&fixtureDef);
	}
}