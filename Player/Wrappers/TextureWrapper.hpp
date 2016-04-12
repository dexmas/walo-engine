#pragma once

#include <Render/Texture.hpp>
#include <sqrat.h>

#include "Wrappers/AssetWrapper.hpp"

class CTextureWrapper: public CTexture
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CTexture, CAsset, Sqrat::NoConstructor> cl(_vm);

		cl.Prop<u32>("Width", &CTextureWrapper::GetWidth);
		cl.Prop<u32>("Height", &CTextureWrapper::GetHeight);

		cl.Prop<u32>("ImageWidth", &CTextureWrapper::GetImageWidth);
		cl.Prop<u32>("ImageHeight", &CTextureWrapper::GetImageHeigh);

		Sqrat::RootTable(_vm).Bind("CTexture", cl);
	}
};