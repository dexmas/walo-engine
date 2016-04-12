#include "Physics2D.hpp"
#include <Box2d.h>

const b2Vec2 cDefaultGravi = b2Vec2(0, 0);

class CPhysics2DProxy: public b2ContactListener, public b2Draw
{
public:
	CPhysics2DProxy(CPhysics2D* _system)
	{
#ifdef _DEBUG
		m_DebugRender = CDebugRenderer::Instance();
		SetFlags(/*e_shapeBit|*/e_jointBit|e_aabbBit|e_pairBit|e_centerOfMassBit);
#endif
	}
	/// Called when two fixtures begin to touch.
	void BeginContact(b2Contact* contact)
	{
		m_System->BeginContact(contact);
	}

	/// Called when two fixtures cease to touch.
	void EndContact(b2Contact* contact) 
	{ 
		m_System->EndContact(contact);
	}

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* _vertices, s32 _vertexCount, const b2Color& _color)
	{
#ifdef _DEBUG
		CColor c = CColor(_color.r, _color.g, _color.b, 1.0f);
		for(u32 i = 0; i < _vertexCount - 1; ++i)
		{
			CVector3 v1(_vertices[i].x, _vertices[i].y, 1.0f);
			CVector3 v2(_vertices[i + 1].x, _vertices[i + 1].y, 1.0f);
			m_DebugRender->AddLine(v1, v2, c);
		}

		CVector3 v1(_vertices[_vertexCount - 1].x, _vertices[_vertexCount - 1].y, 0.0f);
		CVector3 v2(_vertices[0].x, _vertices[0].y, 0.0f);

		m_DebugRender->AddLine(v1, v2, c);
#endif
	}

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* _vertices, s32 _vertexCount, const b2Color& _color)
	{
#ifdef _DEBUG
		CColor c = CColor(_color.r, _color.g, _color.b, 0.5f);

		if(_vertexCount > 2)
		{
			CVector3 v0(_vertices[0].x, _vertices[0].y, 0.0f);

			for(int i = 1; i < _vertexCount - 1; ++i)
			{
				CVector3 v1(_vertices[i].x, _vertices[i].y, 0.0f);
				CVector3 v2(_vertices[i + 1].x, _vertices[i + 1].y, 0.0f);

				m_DebugRender->AddTriangle(v0, v1, v2, c);
			}
		}
#endif
	}

	/// Draw a circle.
	void DrawCircle(const b2Vec2& _center, f32 _radius, const b2Color& _color)
	{
#ifdef _DEBUG
		CVector3 p = CVector3(_center.x, _center.y, 0.0f);
		CColor c = CColor(_color.r, _color.g, _color.b, 1.0f);
		for(u32 i = 0; i < 360; i += 30)
		{
			u32 j = i + 30;
			f32 x1 = _radius * CMath::Cos((f32)i);
			f32 y1 = _radius * CMath::Sin((f32)i);
			f32 x2 = _radius * CMath::Cos((f32)j);
			f32 y2 = _radius * CMath::Sin((f32)j);

			m_DebugRender->AddLine(p + CVector3(x1, y1, 0.0f), p + CVector3(x2, y2, 0.0f), c);
		}
#endif
	}
	
	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& _center, f32 _radius, const b2Vec2& _axis, const b2Color& _color)
	{
#ifdef _DEBUG
		CVector3 p = CVector3(_center.x, _center.y, 0.0f);
		CColor c = CColor(_color.r, _color.g, _color.b, 0.5f);
		for(u32 i = 0; i < 360; i += 30)
		{
			u32 j = i + 30;
			f32 x1 = _radius * CMath::Cos((f32)i);
			f32 y1 = _radius * CMath::Sin((f32)i);
			f32 x2 = _radius * CMath::Cos((f32)j);
			f32 y2 = _radius * CMath::Sin((f32)j);

			m_DebugRender->AddTriangle(p, p + CVector3(x1, y1, 0.0f), p + CVector3(x2, y2, 0.0f), c);
		}
#endif
	}
	
	/// Draw a line segment.
	void DrawSegment(const b2Vec2& _p1, const b2Vec2& _p2, const b2Color& _color)
	{
#ifdef _DEBUG
		m_DebugRender->AddLine(CVector3(_p1.x, _p1.y, 0.0f), CVector3(_p2.x, _p2.y, 0.0f), CColor(_color.r, _color.g, _color.b, 1.0f));
#endif
	}

	/// Draw a transform. Choose your own length scale.
	void DrawTransform(const b2Transform& xf)
	{
#ifdef _DEBUG
		const f32 axisScale = 16.0f;

		b2Vec2 p1 = xf.p, p2;
		p2 = p1 + axisScale * xf.q.GetXAxis();
		m_DebugRender->AddLine(CVector3(p1.x, p1.y, 0.0f), CVector3(p2.x, p2.y, 0.0f), CColor::RED);

		p2 = p1 + axisScale * xf.q.GetYAxis();
		m_DebugRender->AddLine(CVector3(p1.x, p1.y, 0.0f), CVector3(p2.x, p2.y, 0.0f), CColor::BLUE);
#endif
	}

private:
#ifdef _DEBUG
	CDebugRenderer* m_DebugRender;
#endif
	CPhysics2D* m_System;
};

CPhysics2D::CPhysics2D()
{
	m_World = new b2World(cDefaultGravi);
	m_Proxy = new CPhysics2DProxy(this);
	m_World->SetAllowSleeping(false);
	m_World->SetContactListener(m_Proxy);
	m_World->SetDebugDraw(m_Proxy);
}

CPhysics2D::~CPhysics2D()
{
	delete m_World;
}

void CPhysics2D::OnAssigned(CNode* _node)
{
}

void CPhysics2D::Update(f32 _dt)
{
	m_World->Step(_dt, 0, 1);
}

void CPhysics2D::Render()
{
#ifdef _DEBUG
	m_World->DrawDebugData();
#endif
}

void CPhysics2D::BeginContact(b2Contact* contact)
{
		
}

void CPhysics2D::EndContact(b2Contact* contact) 
{ 
		
}