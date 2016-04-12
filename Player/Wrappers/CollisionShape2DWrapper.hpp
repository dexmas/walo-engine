#pragma once

#include <Scene/Physics2D/CollisionShape2D.hpp>
#include <sqrat.h>

class CCollisionShape2DWrapper: public CCollisionShape2D
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CCollisionShape2D, CComponent, Sqrat::NoConstructor > cl(_vm);

		Sqrat::RootTable(_vm).Bind("CCollisionShape2D", cl);
	}
};