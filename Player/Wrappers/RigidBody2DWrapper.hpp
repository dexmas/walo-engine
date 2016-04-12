#pragma once

#include <Scene/Physics2D/RigidBody2D.hpp>
#include <sqrat.h>

class CRigidBody2DWrapper: public CRigidBody2D
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CRigidBody2D, CComponent, Sqrat::NoCopy<CRigidBody2D> > cl(_vm);

		Sqrat::RootTable(_vm).Bind("CRigidBody2D", cl);
	}
};