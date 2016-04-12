#pragma once

#include <Core/Vector.hpp>
#include <sqrat.h>

class CVector2Wrapper: public CVector2
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::ImprovedClass<CVector2> cl(_vm);

		cl.Ctor<f32, f32>();

		cl.Var<f32>("X", &CVector2::X);
		cl.Var<f32>("Y", &CVector2::Y);

		cl.Func("_add", &CVector2::operator +);
		cl.Func("_mul", &CVector2::operator *);
		cl.Func("_div", &CVector2::operator /);
		cl.Func<CVector2 (CVector2::*)(void) const>("_unm", &CVector2::operator -);
		cl.Func<CVector2 (CVector2::*)(const CVector2&) const>("_sub", &CVector2::operator -);

		cl.Func("Normalize", &CVector2::Normalize);
		cl.Func("Length", &CVector2::Length);
		cl.Func("Distance", &CVector2::Distance);
		cl.Func("Interpolate", &CVector2::Interpolate);

		cl.GlobalFunc("_tostring", &CVector2Wrapper::sqToString);

		Sqrat::RootTable(_vm).Bind("CVector2", cl);
	}

	static const Sqrat::string sqToString(const CVector2* _vec)
	{
		char str[64];

		sprintf(str, "CVector2(%f,%f)", _vec->X, _vec->Y);

		return Sqrat::string(str);
	}
};

class CVector3Wrapper: public CVector3
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::ImprovedClass<CVector3> cl(_vm);

		cl.Ctor<f32, f32, f32>();

		cl.Var<f32>("X", &CVector3::X);
		cl.Var<f32>("Y", &CVector3::Y);
		cl.Var<f32>("Z", &CVector3::Z);

		cl.Func("_add", &CVector3::operator +);
		cl.Func("_mul", &CVector3::operator *);
		cl.Func("_div", &CVector3::operator /);
		cl.Func<CVector3 (CVector3::*)(void) const>("_unm", &CVector3::operator -);
		cl.Func<CVector3 (CVector3::*)(const CVector3&) const>("_sub", &CVector3::operator -);

		cl.Func("Normalize", &CVector3::Normalize);
		cl.Func("Cross", &CVector3::Cross);
		cl.Func("Dot", &CVector3::Dot);
		cl.Func("Interpolate", &CVector3::Interpolate);

		cl.GlobalFunc("_tostring", &CVector3Wrapper::sqToString);

		Sqrat::RootTable(_vm).Bind("CVector3", cl);
	}

	static const Sqrat::string sqToString(const CVector3* _vec)
	{
		char str[64];

		sprintf(str, "CVector3(%f,%f,%f)", _vec->X, _vec->Y, _vec->Z);

		return Sqrat::string(str);
	}
};