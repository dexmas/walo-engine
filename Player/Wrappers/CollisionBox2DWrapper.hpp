#pragma once

#include <Scene/Physics2D/CollisionBox2D.hpp>
#include <sqrat.h>

class CCollisionBox2DWrapper: public CCollisionBox2D
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CCollisionBox2D, CCollisionShape2D, Sqrat::NoCopy<CCollisionBox2D> > cl(_vm, "CCollisionBox2D");

		cl.Func("SetSize", &CCollisionBox2D::SetSize);
		cl.Func("SetCenter", &CCollisionBox2D::SetCenter);
		cl.Func("SetRotation", &CCollisionBox2D::SetRotation);

		Sqrat::RootTable(_vm).Bind("CCollisionBox2D", cl);
	}
};