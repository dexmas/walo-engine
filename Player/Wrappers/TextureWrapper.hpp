#pragma once

#include <Render/Texture.hpp>
#include <sqrat.h>

#include "Wrappers/AssetWrapper.hpp"

class CTextureWrapper: public CTexture
{
public:
	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::DerivedClass<CTexture, CAsset, Sqrat::NoConstructor<CTexture> > cl(_vm, "CTexture");

		cl.Prop("Width", &CTextureWrapper::GetWidth);
		cl.Prop("Height", &CTextureWrapper::GetHeight);

		cl.Prop("ImageWidth", &CTextureWrapper::GetImageWidth);
		cl.Prop("ImageHeight", &CTextureWrapper::GetImageHeigh);

		Sqrat::RootTable(_vm).Bind("CTexture", cl);
	}
};