#pragma once

#include "Node2DWrapper.hpp"
#include <Scene/Scene2D/Text2D.hpp>

class CText2DWrapper: public CNode2DWrapper
{
public:
	CText2DWrapper()
	{
		m_Object2D = new CText2D();
		AddComponent(m_Object2D);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CText2DWrapper, CNode2DWrapper, Sqrat::NoCopy<CText2DWrapper> > cl(_vm, "CText2D");

		cl.Func("SetText", &CText2DWrapper::sqSetText);
		cl.Func("SetFont", &CText2DWrapper::sqSetFont);
		cl.Func("SetAlignment", &CText2DWrapper::sqSetAlignment);
		
		Sqrat::RootTable(_vm).Bind("CText2D", cl);
	}

	void sqSetAlignment(u32 _hal, u32 _val)
	{
		GetObject2D<CText2D>()->SetAlignment((EHAlignment)_hal, (EVAlignment)_val);
	}

	void sqSetText(const char* _text)
	{
		GetObject2D<CText2D>()->SetText(_text);
	}

	void sqSetFont(CFont2D* _font)
	{
		GetObject2D<CText2D>()->SetFont(_font);
	}
};