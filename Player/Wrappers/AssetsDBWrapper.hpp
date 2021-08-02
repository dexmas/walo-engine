#pragma once

#include <Assets/AssetsDB.hpp>
#include <sqrat.h>

class CAssetsDBWrapper: public CAssetsDB
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CAssetsDB, Sqrat::NoConstructor<CAssetsDB> > cl(_vm, "CAssetsDB");

		cl.Func("GetTexture", &CAssetsDBWrapper::GetTexture);
		cl.Func("GetFont", &CAssetsDBWrapper::GetFont2D);
		cl.Func("GetMesh", &CAssetsDBWrapper::GetMesh);
		cl.Func("GetMaterial", &CAssetsDBWrapper::GetMaterial);
		cl.Func("GetSound", &CAssetsDBWrapper::GetSound);

		Sqrat::RootTable(_vm).Bind("CAssetsDB", cl);
	}
};