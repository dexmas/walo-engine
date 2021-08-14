#pragma once

#include <Scene/Input/InputSystem.hpp>
#include <sqrat.h>

class CInputSystemWrapper: public CInputSystem
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CInputSystem, Sqrat::NoConstructor<CInputSystem> > cl(_vm, "CInputSystem");

		cl.Func("DropEvent", &CInputSystem::DropEvent);

		Sqrat::RootTable(_vm).Bind("CInputSystem", cl);
	}
};