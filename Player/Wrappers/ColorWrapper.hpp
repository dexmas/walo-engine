#pragma once

#include <Core/Color.hpp>
#include <sqrat.h>

class CColor3Wrapper: public CColor
{
public:
	CColor3Wrapper(){}
	CColor3Wrapper(u32 _rgba):CColor(_rgba){}
	CColor3Wrapper(f32 _r, f32 _g, f32 _b, f32 _a):CColor(_r,_g,_b,_a){}
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::ImprovedClass<CColor3Wrapper> cl(_vm);

		cl.Ctor<u32>();
		cl.Ctor<f32, f32, f32, f32>();

		cl.Prop<f32>("R", &CColor3Wrapper::sqGetR, &CColor3Wrapper::sqSetR);
		cl.Prop<f32>("G", &CColor3Wrapper::sqGetG, &CColor3Wrapper::sqSetG);
		cl.Prop<f32>("B", &CColor3Wrapper::sqGetB, &CColor3Wrapper::sqSetB);
		cl.Prop<f32>("A", &CColor3Wrapper::sqGetA, &CColor3Wrapper::sqSetA);

		cl.GlobalFunc("_tostring", &CColor3Wrapper::sqToString);

		Sqrat::RootTable(_vm).Bind("CColor", cl);
	}

	inline f32 sqGetR() {return R/255.0f;}
	inline void sqSetR(const f32 _r) {R = u8(255.0f * CMath::Clamp(_r, 0.0f, 1.0f));}

	inline f32 sqGetG() {return G/255.0f;}
	inline void sqSetG(const f32 _g) {G = u8(255.0f * CMath::Clamp(_g, 0.0f, 1.0f));}

	inline f32 sqGetB() {return B/255.0f;}
	inline void sqSetB(const f32 _b) {B = u8(255.0f * CMath::Clamp(_b, 0.0f, 1.0f));}

	inline f32 sqGetA() {return A/255.0f;}
	inline void sqSetA(const f32 _a) {A = u8(255.0f * CMath::Clamp(_a, 0.0f, 1.0f));}

	static const Sqrat::string sqToString(const CColor* _clr)
	{
		char str[64];

		sprintf(str, "RGBA:%d", (u32)(*_clr));

		return Sqrat::string(str);
	}
};