#pragma once

#include <Scene/Component.hpp>
#include <sqrat.h>

class CComponentImplWrapper: public CComponent
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CComponent, Sqrat::NoConstructor> cl(_vm);

		cl.Func("SetEnabled", &CComponent::SetEnabled);

		Sqrat::RootTable(_vm).Bind("CComponent", cl);
	}
};