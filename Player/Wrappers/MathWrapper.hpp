#pragma once

#include "Core/Types.hpp"
#include "Core/Math.hpp"

#include <sqrat.h>

class CMathWrapper: public CMath
{
public:
	static void Register(HSQUIRRELVM vm)
	{
		Sqrat::Class<CMathWrapper, Sqrat::NoConstructor> cl(vm);

		cl.SetStaticValue<f32>("PI", CMathWrapper::PI);
		cl.SetStaticValue<f32>("TWO_PI", CMathWrapper::TWO_PI);
		cl.SetStaticValue<f32>("PI_DIV_TWO", CMathWrapper::PI_DIV_TWO);
		cl.SetStaticValue<f32>("RADTODEG", CMathWrapper::RADTODEG);
		cl.SetStaticValue<f32>("DEGTORAD", CMathWrapper::DEGTORAD);

		cl.StaticFunc<s32(*)(s32,s32)>("MinI", &CMathWrapper::Min);
		cl.StaticFunc<s32(*)(s32,s32)>("MaxI", &CMathWrapper::Max);
		cl.StaticFunc<s32(*)(s32)>("AbsI", &CMathWrapper::Abs);
		cl.StaticFunc<s32(*)(s32,s32,f32)>("LerpI", &CMathWrapper::Lerp);
		cl.StaticFunc<s32(*)(s32,s32,s32)>("ClampI", &CMathWrapper::Clamp);
		cl.StaticFunc<s32(*)(s32,s32)>("Rand", &CMathWrapper::Rand);
		cl.StaticFunc<u32(*)(u32)>("NextPower2", &CMathWrapper::NextPower2);

		cl.StaticFunc<f32(*)(f32,f32)>("Min", &CMathWrapper::Min);
		cl.StaticFunc<f32(*)(f32,f32)>("Max", &CMathWrapper::Max);
		cl.StaticFunc<f32(*)(f32)>("Abs", &CMathWrapper::Abs);
		cl.StaticFunc<f32(*)(f32,f32,f32)>("Lerp", &CMathWrapper::Lerp);
		cl.StaticFunc<f32(*)(f32,f32,f32)>("Clamp", &CMathWrapper::Clamp);
		cl.StaticFunc("Ceil", &CMathWrapper::Ceil);
		cl.StaticFunc("Floor", &CMathWrapper::Floor);
		cl.StaticFunc("Sqrt", &CMathWrapper::Sqrt);
		cl.StaticFunc("QRSqrt", &CMathWrapper::QRSqrt);
		cl.StaticFunc("Sin", &CMathWrapper::Sin);
		cl.StaticFunc("Cos", &CMathWrapper::Cos);
		cl.StaticFunc("Tan", &CMathWrapper::Tan);
		cl.StaticFunc("Asin", &CMathWrapper::Asin);
		cl.StaticFunc("Acos", &CMathWrapper::Acos);
		cl.StaticFunc("Atan", &CMathWrapper::Atan);
		cl.StaticFunc("Atan2", &CMathWrapper::Atan2);

		Sqrat::RootTable(vm).Bind("Math", cl);
	}
};