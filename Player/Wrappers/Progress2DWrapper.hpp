#pragma once

#include "Node2DWrapper.hpp"
#include <Scene/Scene2D/Progress2D.hpp>

class CProgress2DWrapper: public CNode2DWrapper
{
public:
	CProgress2DWrapper()
	{
		m_Object2D = new CProgress2D();
		AddComponent(m_Object2D);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CProgress2DWrapper, CNode2DWrapper, Sqrat::NoCopy<CProgress2DWrapper> > cl(_vm);

		cl.Func("SetFrame", &CProgress2DWrapper::sqSetFrame);
		cl.Func("SetTexture", &CProgress2DWrapper::sqSetTexture);
		cl.Func("SetType", &CProgress2DWrapper::sqSetType);
		cl.Func("SetValue", &CProgress2DWrapper::sqSetValue);
		
		Sqrat::RootTable(_vm).Bind("CProgress2D", cl);
	}

	void sqSetFrame(s32 _x, s32 _y, s32 _w, s32 _h)
	{
		GetObject2D<CProgress2D>()->SetFrame(CRect(_x, _y, _w, _h));
	}

	void sqSetTexture(CTexture* _tex)
	{
		GetObject2D<CProgress2D>()->SetTexture(_tex);
	}

	void sqSetType(u32 _type)
	{
		CProgress2D::EProgressType type = CProgress2D::EPT_LINEAR;

		if(_type != 0)
			type = CProgress2D::EPT_RADIAL;

		GetObject2D<CProgress2D>()->SetType(type);
	}

	void sqSetValue(f32 _val)
	{
		GetObject2D<CProgress2D>()->SetValue(_val);
	}
};