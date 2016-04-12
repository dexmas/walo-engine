#pragma once

#include <Render/Mesh.hpp>
#include <sqrat.h>

#include "AssetWrapper.hpp"

class CMeshWrapper: public CMesh
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CMesh, CAsset, Sqrat::NoConstructor> cl(_vm);

		cl.Func("SetMaterial", &CMesh::SetMaterial);

		Sqrat::RootTable(_vm).Bind("CMesh", cl);
	}
};