#pragma once

#include <Scene/Scene2D/Font2D.hpp>
#include <sqrat.h>

#include "Wrappers/AssetWrapper.hpp"

class CFontWrapper: public CFont2D
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CFont2D, CAsset, Sqrat::NoConstructor<CFont2D> > cl(_vm, "CFont2D");

		cl.Prop("Texture", &CFontWrapper::GetTexture);
		cl.Prop("Size", &CFontWrapper::GetSize);

		Sqrat::RootTable(_vm).Bind("CFont2D", cl);
	}
};