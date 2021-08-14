#pragma once

#include <Core/Rect.hpp>
#include <sqrat.h>

class CRectWrapper: public CRect
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CRect> cl(_vm, "CRect");

		cl.Ctor<s32, s32, u32, u32>();

		cl.Var<s32>("X", &CRectWrapper::X);
		cl.Var<s32>("Y", &CRectWrapper::Y);
		cl.Var<s32>("Width", &CRectWrapper::Width);
		cl.Var<s32>("Height", &CRectWrapper::Height);

		cl.Func("Intersect", &CRectWrapper::Intersect);

		cl.GlobalFunc("_tostring", &CRectWrapper::sqToString);

		Sqrat::RootTable(_vm).Bind("CRect", cl);
	}

	static const Sqrat::string sqToString(const CRect* _rect)
	{
		char str[64];

		sprintf(str, "CRect(%i,%i,%i,%i)", _rect->X, _rect->Y, _rect->Width, _rect->Height);

		return Sqrat::string(str);
	}
};
