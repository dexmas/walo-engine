#pragma once

#include "Node2DWrapper.hpp"
#include <Scene/Scene2D/Sprite2D.hpp>

class CSprite2DWrapper: public CNode2DWrapper
{
public:
	CSprite2DWrapper()
	{
		m_Object2D = new CSprite2D();
		AddComponent(m_Object2D);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CSprite2DWrapper, CNode2DWrapper, Sqrat::NoCopy<CSprite2DWrapper> > cl(_vm, "CSprite2D");

		cl.Func("SetFrame", &CSprite2DWrapper::sqSetFrame);
		cl.Func("SetTexture", &CSprite2DWrapper::sqSetTexture);
		cl.Func("SetTiling", &CSprite2DWrapper::sqSetTiling);
		
		Sqrat::RootTable(_vm).Bind("CSprite2D", cl);
	}

	void sqSetFrame(s32 _x, s32 _y, s32 _w, s32 _h)
	{
		GetObject2D<CSprite2D>()->SetFrame(CRect(_x, _y, _w, _h));
	}

	void sqSetTexture(CTexture* _tex)
	{
		GetObject2D<CSprite2D>()->SetTexture(_tex);
	}

	void sqSetTiling(bool _tx, bool _ty)
	{
		GetObject2D<CSprite2D>()->SetTiling(_tx, _ty);
	}
};