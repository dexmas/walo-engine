#pragma once

#include "NodeWrapper.hpp"
#include <Scene/Scene2D/Object2D.hpp>

class CNode2DWrapper: public CNodeWrapper
{
public:
	CNode2DWrapper()
	{
		m_Object2D = 0;
	}

	template<class T>
	T* GetObject2D() 
	{
		if(!m_Object2D)
		{
			m_Object2D = new CObject2D();
			AddComponent(m_Object2D);
		}

		return (T*)m_Object2D;
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CNode2DWrapper, CNodeWrapper, Sqrat::NoCopy<CNode2DWrapper> > cl(_vm);

		cl.Func("SetPosition", &CNode2DWrapper::sqSetPosition);
		cl.Func("SetRotation", &CNode2DWrapper::sqSetRotation);
		cl.Func("SetScale", &CNode2DWrapper::sqSetScale);
		cl.Func("SetCenter", &CNode2DWrapper::sqSetCenter);
		cl.Func("SetSize", &CNode2DWrapper::sqSetSize);
		cl.Func("SetFlip", &CNode2DWrapper::sqSetFlip);
		cl.Func("SetColor", &CNode2DWrapper::sqSetColor);
		cl.Func("SetOrder", &CNode2DWrapper::sqSetOrder);
		cl.Func("SetClip", &CNode2DWrapper::sqSetClip);

		cl.Func("GetSize", &CNode2DWrapper::sqGetSize);
		cl.Func("GetPosition", &CNode2DWrapper::sqGetPosition);

		cl.Func("HitTest", &CNode2DWrapper::sqHitTest);

		Sqrat::RootTable(_vm).Bind("CNode2D", cl);
	}

	void sqSetPosition(f32 _x, f32 _y)
	{
		SetPosition(CVector3(_x, _y, 0.0f));
	}

	CVector2 sqGetPosition()
	{
		return CVector2(GetPosition().X, GetPosition().Y);
	}

	void sqSetRotation(f32 _angle)
	{
		SetRotation(CVector3(0.0f, 0.0f, _angle));
	}

	void sqSetScale(f32 _x, f32 _y)
	{
		SetScale(CVector3(_x, _y, 1.0f));
	}

	void sqSetCenter(f32 _x, f32 _y)
	{
		GetObject2D<CObject2D>()->SetCenter(CVector2(_x, _y));
	}

	void sqSetSize(f32 _x, f32 _y)
	{
		GetObject2D<CObject2D>()->SetSize(CVector2(_x, _y));
	}

	CVector2 sqGetSize()
	{
		return GetObject2D<CObject2D>()->GetSize();
	}

	void sqSetFlip(bool _x, bool _y)
	{
		GetObject2D<CObject2D>()->SetFlip(CVector2(_x?1.0f:0.0f, _y?1.0f:0.0f));
	}

	void sqSetColor(f32 _r, f32 _g, f32 _b, f32 _a)
	{
		GetObject2D<CObject2D>()->SetColor(CColor(_r, _g, _b, _a));
	}

	f32 sqGetOrder()
	{
		return GetObject2D<CObject2D>()->GetOrder();
	}

	void sqSetOrder(f32 _order)
	{
		GetObject2D<CObject2D>()->SetOrder(_order);
	}

	void sqSetClip(bool _clip)
	{
		GetObject2D<CObject2D>()->SetClipChild(_clip);
	}

	bool sqHitTest(f32 _x, f32 _y)
	{
		return GetObject2D<CObject2D>()->HitTest(CVector2(_x, _y));
	}

protected:
	CObject2D* m_Object2D;
};