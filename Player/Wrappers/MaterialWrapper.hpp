#pragma once

#include <Render/Material.hpp>
#include <sqrat.h>

#include "AssetWrapper.hpp"

class CMaterialWrapper: public CMaterial
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CMaterial, CAsset, Sqrat::NoConstructor> cl(_vm);

		Sqrat::RootTable(_vm).Bind("CMaterial", cl);
	}
};