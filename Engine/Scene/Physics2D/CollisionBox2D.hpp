#pragma once

#include "Scene/Physics2D/CollisionShape2D.hpp"

class CCollisionBox2D: public CCollisionShape2D
{
public:
	CCollisionBox2D():CCollisionShape2D(ECT_COLLISIONBOX2D)
	{
		m_Width = 1.0f;
		m_Height = 1.0f;
		m_CenterX = 0;
		m_CenterY = 0;
		m_Rotation = 0.0f;
	}

	void SetSize(f32 _w, f32 _h)
	{
		_ReleaseFixture();
		m_Width = _w;
		m_Height = _h;
		_CreateFixture();
	}

	void SetCenter(f32 _x, f32 _y)
	{
		_ReleaseFixture();
		m_CenterX = _x;
		m_CenterY = _y;
		_CreateFixture();
	}

	void SetRotation(f32 _rot)
	{
		_ReleaseFixture();
		m_Rotation = _rot;
		_CreateFixture();
	}

private:
	void OnAssigned(CNode* _node)
	{
		m_Node = _node;

		CRect rc = _GetNodeBound();
		m_Width = CMath::Max(rc.Width, 1);
		m_Height = CMath::Max(rc.Height, 1);

		CCollisionShape2D::OnAssigned(_node);
	}

	void _CreateFixture();

	f32 m_Width;
	f32 m_Height;
	f32 m_CenterX;
	f32 m_CenterY;
	f32 m_Rotation;
};