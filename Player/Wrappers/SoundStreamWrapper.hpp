#pragma once

#include <Sound/SoundStream.hpp>
#include <sqrat.h>

#include "AssetWrapper.hpp"

class CSoundStreamWrapper: public CSoundStream
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CSoundStream, CAsset, Sqrat::NoConstructor<CSoundStream> > cl(_vm, "CSoundStream");

		Sqrat::RootTable(_vm).Bind("CSoundStream", cl);
	}
};