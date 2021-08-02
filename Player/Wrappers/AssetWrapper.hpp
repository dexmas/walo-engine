#pragma once

#include <Assets/Asset.hpp>
#include <sqrat.h>

class CAssetWrapper: public CAsset
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CAsset, Sqrat::NoConstructor<CAsset> > cl(_vm, "CAsset");

		cl.Func("Load", &CAssetWrapper::Load);
		cl.Func("Unload", &CAssetWrapper::Unload);

		Sqrat::RootTable(_vm).Bind("CAsset", cl);
	}
};